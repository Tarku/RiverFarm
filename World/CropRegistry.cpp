#include "CropRegistry.h"

std::vector<Crop*> CropRegistry::Crops = {
	new BarleyCrop(),
	new GlasswortCrop()
};

int CropRegistry::CropAmount()
{
	return static_cast<int>(Crops.size());
}

Crop* CropRegistry::GetInstance(CropID& cropId)
{
	switch (cropId)
	{
	case CropBarley:
		return new BarleyCrop();
	case CropGlasswort:
		return new GlasswortCrop();
	}
}