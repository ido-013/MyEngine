#pragma once

#include <list>
#include <glm/glm.hpp>

class Pathfinder
{
private:
    Pathfinder();

    Pathfinder(const Pathfinder&) = delete;
    const Pathfinder& operator=(const Pathfinder&) = delete;

    ~Pathfinder();

    static constexpr int width = 100;
    static constexpr int blank = -1;

    int dx[8] = { -1, 0, 0, 1, -1, -1, 1, 1 };
    int dy[8] = { 0, -1, 1, 0, -1, 1, -1, 1 };

    int terrain[width][width];
    int wallLayer;

    float Manhattan(const float& _diffX, const float& _diffY);

    void Rubberbanding(std::list<std::pair<int, int>>& _coordList);

    struct Node
    {
        int x;
        int y;

        int parentX;
        int parentY;

        float given;
        float cost;

        bool operator<(const Node& _other) const
        {
            return cost > _other.cost;
        }
    };

public:
    static Pathfinder& GetInstance()
    {
        static Pathfinder instance;
        return instance;
    }

    const int& GetWidth() { return width; }

    void SetTerrain(const int& _x, const int& _y, const int& _layer = blank);

    bool IsWall(const int& _x, const int& _y);
    bool CheckStraightLine(const int& _curX, const int& _curY, const int& _dstX, const int& _dstY);

    std::list<glm::vec2> ComputePath(const int& _curX, const int& _curY, const int& _dstX, const int& _dstY);
};