#pragma once
#include "BaseLevel.h"

namespace level
{
	class TestLevel : public BaseLevel
	{
	public:
		void Init() override;
		void Update() override;
		void Exit() override;
	};
}