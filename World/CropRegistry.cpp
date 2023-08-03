#include "CropRegistry.h"

Crop* CropRegistry::crops[] = {
	new BarleyCrop()
};

int CropRegistry::CropAmount()
{
	return sizeof(CropRegistry::crops) / sizeof(Crop*);
}