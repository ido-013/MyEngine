#pragma once

#include <string>
#include <chrono>
#include <list>
#include <map>

namespace MyProfiler
{
	class Block
	{
	private:
		std::string name;
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;

		std::list<Block*> children;
		Block* parent;

	public:
		Block(const std::string& _name, Block* _parent = nullptr);
		~Block();

		void End();

		double GetSeconds() const;
		std::string GetName() const { return name; }
		Block* GetParent() const { return parent; }

		Block* AddChild(const std::string& _name);

		void Dump(int _n, std::map<std::string, float[300]>& _blockTime, std::map<std::string, float[300]>& _blockCount) const;
	};

	class Profiler
	{
	private:
		Profiler() = default;
		Profiler(const Profiler&) = delete;
		Profiler& operator=(const Profiler&) = delete;
		~Profiler();

		Block* current = nullptr;

		std::list<Block*> fullyFinishedBlocks;

	public:
		static Profiler& GetInstance()
		{
			static Profiler instance;
			return instance;
		}

		void StartBlock(std::string _name);
		void End();
		void Dump();

		void Clear();
	};
}

#ifndef __FUNCTION_NAME__
#ifdef WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif // Win32
#endif // !__FUNCTION_NAME__

#ifdef _DEBUG
#define DEBUG_PROFILER_START(x) MyProfiler::Profiler::GetInstance().StartBlock(x)
#define DEBUG_PROFILER_END MyProfiler::Profiler::GetInstance().End()
#define DEBUG_PROFILER_DUMP MyProfiler::Profiler::GetInstance().Dump()
#else
#define DEBUG_PROFILER_START(x) //
#define DEBUG_PROFILER_END		//
#define DEBUG_PROFILER_DUMP		//
#endif
