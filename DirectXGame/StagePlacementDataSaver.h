#pragma once
#include "StagePlacementData.h"

class StagePlacementDataSaver {
public:
	bool Save(int stageNumber, const StagePlacementData& placementData);
};