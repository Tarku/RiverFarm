#include "ToolRegistry.h"

int ToolRegistry::ToolCount()
{
	return sizeof(Tools) / sizeof(Tools[0]);
}
