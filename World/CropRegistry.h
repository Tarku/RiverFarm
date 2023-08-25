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
	static std::vector<Crop*> Crops;

	static int CropAmount();

	static Crop* GetInstance(CropID& cropId);
};