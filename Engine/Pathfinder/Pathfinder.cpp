#include "Pathfinder.h"

#include <queue>

#include "../LayerManager/LayerManager.h"
#include "../Editor/Grid.h"

Pathfinder::Pathfinder()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            terrain[i][j] = blank;
        }
    }

    wallLayer = LayerManager::GetInstance().GetLayerInd("Wall");
}

Pathfinder::~Pathfinder()
{
}

float Pathfinder::Manhattan(const float& _diffX, const float& _diffY)
{
    return _diffX + _diffY;
}

bool Pathfinder::CheckStraightLine(const int& _curX, const int& _curY, const int& _dstX, const int& _dstY)
{
	int minX = _curX < _dstX ? _curX : _dstX;
	int maxX = _curX > _dstX ? _curX : _dstX;

	int minY = _curY < _dstY ? _curY : _dstY;
	int maxY = _curY > _dstY ? _curY : _dstY;

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			if (IsWall(x, y))
			{
				return false;
			}
		}
	}

	return true;
}

void Pathfinder::Rubberbanding(std::list<std::pair<int, int>>& _coordList)
{
	if (_coordList.size() < 3)
		return;

	std::list<std::pair<int, int>>::iterator it = _coordList.begin();
	std::list<std::pair<int, int>>::iterator nodeIt[3];

	for (int i = 0; i < 3; ++i)
	{
		nodeIt[i] = it++;
	}

	while (true)
	{
		if (CheckStraightLine(nodeIt[0]->first, nodeIt[0]->second, nodeIt[2]->first, nodeIt[2]->second))
		{
			_coordList.erase(nodeIt[1]);
			nodeIt[1] = nodeIt[2];
			++nodeIt[2];
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				++nodeIt[i];
			}
		}

		if (nodeIt[2] == _coordList.end())
			break;
	}
}

void Pathfinder::SetTerrain(const int& _x, const int& _y, const int& _layer)
{
    terrain[_x][_y] = _layer;
}

bool Pathfinder::IsWall(const int& _x, const int& _y)
{
    return terrain[_x][_y] == wallLayer;
}

std::list<glm::vec2> Pathfinder::ComputePath(const int& _curX, const int& _curY, const int& _dstX, const int& _dstY)
{
	float weight = 2;

	std::list<glm::vec2> res;

	std::priority_queue<Node> openList;

	std::map<std::pair<int, int>, bool> isClosed;
	std::map<std::pair<int, int>, std::pair<int, int>> parentCoord;

	if (CheckStraightLine(_curX, _curY, _dstX, _dstY))
	{
		res.push_back(Grid::GetInstance().GetGridPos(_dstX, _dstY));
		return res;
	}

	openList.push({ _curX, _curY, -1, -1, 0, 0 });
	
	while (!openList.empty())
	{
		// Pop cheapest node off Open List(parent node)
		Node parent = openList.top();
		openList.pop();

		if (isClosed[{parent.x, parent.y}])
			continue;

		isClosed[{parent.x, parent.y}] = true;
		parentCoord[{parent.x, parent.y}] = { parent.parentX, parent.parentY };

		// If node is the Goal Node, then path found (RETURN ¡°found)
		if (parent.x == _dstX && parent.y == _dstY)
		{
			std::list<std::pair<int, int>> coordList;
			std::pair<int, int> coord = { parent.x, parent.y };

			while (true)
			{
				if (coord.first == -1)
					break;

				coordList.push_back(coord);
				coord = parentCoord[coord];
			}

			Rubberbanding(coordList);

			for (auto it = coordList.rbegin(); it != coordList.rend(); it++)
			{
				res.push_back(Grid::GetInstance().GetGridPos(it->first, it->second));
			}
			
			return res;
		}

		// Check all neighboring child nodes
		for (int i = 0; i < 8; i++)
		{
			int childX = parent.x + dx[i];
			int childY = parent.y + dy[i];

			if (childX < 0
				|| childY < 0
				|| childX >= width
				|| childY >= width
				|| IsWall(childX, childY)
				|| isClosed[{childX, childY}])
				continue;

			if (i > 3)
			{
				if (IsWall(childX, parent.y) || IsWall(parent.x, childY))
					continue;
			}

			// Compute its cost, f(x) = g(x) + h(x)
			float given = parent.given + (i > 3 ? 1.41 : 1);
			float cost = given + (Manhattan(std::abs(childX - _dstX), std::abs(childY - _dstY)) * weight);

			openList.push({ childX, childY, parent.x, parent.y, given, cost });
		}
	}

	return std::list<glm::vec2>();
}
