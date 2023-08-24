#pragma once

#include "../Shared.h"
#include "Crop.h"

enum CropID
{
	CropBarley
};

class CropRegistry
{
public:
	static Crop* crops[];

	static int CropAmount();

	static Crop* GetInstance(CropID& cropId);
};