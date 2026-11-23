#pragma once

#ifdef USE_IMGUI

#include "BlockManager.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "Goal.h"
#include "MapChipField.h"
#include "MapEditor.h"
#include "Player.h"
#include "StageDataLoader.h"
#include "StageMapLoader.h"
#include "StagePlacementData.h"
#include "StagePlacementDataSaver.h"

class MapEditorController {
public:
	void Update(
	    MapEditor& mapEditor, int& stageNumber, StagePlacementData& placementData, StageDataLoader& dataLoader, StageMapLoader& mapLoader, StagePlacementDataSaver& dataSaver,
	    BlockManager& blockManager, EnemyManager& enemyManager, Goal& goal, Player* player, MapChipField* mapChipField, CameraController* cameraController);
};

#endif