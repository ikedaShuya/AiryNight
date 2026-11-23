#pragma once

#include "StagePlacementData.h"

class StageDataLoader {
public:
	void LoadPlayerPosition(int stageNumber, StagePlacementData& placementData);
	void LoadFlagPosition(int stageNumber, StagePlacementData& placementData);
	void LoadEnemyPositions(int stageNumber, StagePlacementData& placementData);
};