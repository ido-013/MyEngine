#include "Profiler.h"

#include <iostream>

#include "../Imgui/imgui_inc.h"
#include "../Imgui/implot.h"

namespace MyProfiler
{
	Block::Block(const std::string& _name, Block* _parent) : name(_name), parent(_parent)
	{
		start = std::chrono::steady_clock::now();
	}

	Block::~Block()
	{
		//delete all the children
		for (auto it : children)
			delete it;

		children.clear();
	}

	void Block::End()
	{
		end = std::chrono::steady_clock::now();
	}

	double Block::GetSeconds() const
	{
		return std::chrono::duration<double>(end - start).count();
	}

	Block* Block::AddChild(const std::string& _name)
	{
		Block* nBlock = new Block(_name, this);
		children.push_back(nBlock);
		return nBlock;
	}

	void Block::Dump(int _n, std::map<std::string, float[300]>& _blockTime, std::map<std::string, float[300]>& _blockCount) const
	{
		std::string s;

		_blockTime[name][_n] += GetSeconds();
		_blockCount[name][_n]++;

		for (const auto* c : children)
		{
			c->Dump(_n, _blockTime, _blockCount);
		}
	}

	Profiler::~Profiler()
	{
		Clear();
	}

	void Profiler::StartBlock(std::string _name)
	{
		if (!current)
		{
			current = new Block(_name); //I am root
		}
		else
		{
			current = current->AddChild(_name);
		}
	}

	void Profiler::End()
	{
		//stop counting time on current block
		current->End();

		//go 1 step back
		Block* parent = current->GetParent();

		//if no parent. Push current to fullyFinished
		if (!parent)
		{
			fullyFinishedBlocks.push_back(current);

			if (fullyFinishedBlocks.size() > 300)
			{
				delete fullyFinishedBlocks.front();
				fullyFinishedBlocks.pop_front();
			}
		}

		current = parent;
	}

	void Profiler::Dump()
	{
		int id = 0;
		float arr[300];
		
		std::map<std::string, float[300]> blockTime;
		std::map<std::string, float[300]> blockCount;

		for (const auto* b : fullyFinishedBlocks)
		{
			b->Dump(id, blockTime, blockCount);
			arr[id] = id;
			id++;
		}

		for (auto& it : blockTime)
		{
			for (int i = 0; i < 300; i++)
			{
				it.second[i] /= blockCount[it.first][i];
			}
		}

		if (ImPlot::BeginPlot("dump"))
		{
			ImPlot::SetupAxes("frame", "ms");
			ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
			ImPlot::SetupAxesLimits(0, 300, -0.02, 0.02);
			ImPlot::SetupAxis(ImAxis_Y2, "count", ImPlotAxisFlags_AuxDefault);
			ImPlot::SetupAxisLimits(ImAxis_Y2, 0, 20);

			for (auto& it : blockTime)
			{
				ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
				ImPlot::PlotLine(it.first.c_str(), arr, it.second, fullyFinishedBlocks.size());
			}

			for (auto& it : blockCount)
			{
				ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
				ImPlot::PlotBars(it.first.c_str(), arr, it.second, fullyFinishedBlocks.size(), 0.01);
			}

			ImPlot::EndPlot();
		}
	}

	void Profiler::Clear()
	{
		//iterate end() UNTIL current in nullptr
		while (current)
			End();

		//delete all the finished nodes
		for (auto it : fullyFinishedBlocks)
			delete it;

		fullyFinishedBlocks.clear();
	}
}