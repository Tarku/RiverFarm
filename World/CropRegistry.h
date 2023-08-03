#pragma once

#include "../Shared.h"
#include "Crop.h"


class CropRegistry
{
public:
	static Crop* crops[];

	static int CropAmount();
};

enum CropID
{
	CropBarley
};