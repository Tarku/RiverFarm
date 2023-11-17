#pragma once

#include <vector>
#include "ToolDeclarations.h"

class ToolRegistry
{
public:
	inline static std::vector<AgriculturalTool*> Tools = std::vector<AgriculturalTool*> {
		new HandTool(),
		new AxeTool(),
		new HoeTool(),
		new ShovelTool(),
		new ScytheTool(),
		new SeedBagTool(),
		new MilkPailTool()
	};

	inline static int ToolCount()
	{
		return static_cast<int>(Tools.size());
	}
};