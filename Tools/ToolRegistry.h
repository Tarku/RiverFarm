#pragma once

#include "ToolDeclarations.h"

class ToolRegistry
{
public:
	static AgriculturalTool* Tools[];

	static int ToolCount();
};