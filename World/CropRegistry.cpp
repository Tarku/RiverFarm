#include "CropRegistry.h"

Crop* CropRegistry::crops[] = {
	new BarleyCrop()
};

int CropRegistry::CropAmount()
{
	return sizeof(CropRegistry::crops) / sizeof(Crop*);
}

Crop* CropRegistry::GetInstance(CropID& cropId)
{
	switch (cropId)
	{
	case CropBarley:
		return new BarleyCrop();
		break;
	}
}