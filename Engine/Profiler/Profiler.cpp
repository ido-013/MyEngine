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

	void Block::Dump(int _n) const
	{
		std::string s;

		//print correct ammount of tabs
		for (int i = 0; i < _n; i++)
		{
			//std::cout << "\t";
			s += "\t";
		}

		//print name and seconds
		//std::cout << name << " in " << GetSeconds() << " seconds" << std::endl;
		s += name + ": " + std::to_string(GetSeconds()) + " seconds";

		ImGui::Text(s.c_str());
		
		//print children
		for (const auto* c : children)
		{
			c->Dump(_n + 1);
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
			Clear();
			fullyFinishedBlocks = current;
		}

		current = parent;
	}

	void Profiler::Dump()
	{
		if (ImGui::TreeNode("Dump"))
		{
			if (fullyFinishedBlocks != nullptr)
			{
				fullyFinishedBlocks->Dump();
			}

			ImGui::TreePop();
		}
	}

	void Profiler::Clear()
	{
		//delete all the finished nodes
		if (fullyFinishedBlocks != nullptr)
		{
			delete fullyFinishedBlocks;
		}
	}
}