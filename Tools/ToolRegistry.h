#pragma once

#include "ToolDeclarations.h"

class ToolRegistry
{
public:
	inline static AgriculturalTool* Tools[4] = {
		new AxeTool(),
		new HoeTool(),
		new ShovelTool(),
		new ScytheTool()
	};

	static int ToolCount();
};