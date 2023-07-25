#include "ToolRegistry.h"

AgriculturalTool* ToolRegistry::Tools[] = {
	new AxeTool(),
	new HoeTool(),
	new ShovelTool(),
	new ScytheTool()
};

int ToolRegistry::ToolCount()
{
	return sizeof(Tools) / sizeof(Tools[0]);
}
