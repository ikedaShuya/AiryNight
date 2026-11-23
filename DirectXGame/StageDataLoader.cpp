
#include "StageDataLoader.h"
#include <fstream>
#include <string>

void StageDataLoader::LoadPlayerPosition(int stageNumber, StagePlacementData& placementData) {

	// 初期位置
	placementData.SetPlayerPosition(1, 12);

	// ステージごとの配置ファイル
	std::string filePath;

	if (stageNumber == 1) {
		filePath = "Resources/player1.csv";
	} else if (stageNumber == 2) {
		filePath = "Resources/player2.csv";
	} else {
		filePath = "Resources/player3.csv";
	}

	std::ifstream file(filePath);

	if (file.is_open()) {
		int x;
		int y;
		char comma;
		file >> x >> comma >> y;

		if (file && comma == ',') {
			placementData.SetPlayerPosition(x, y);
		}
	}
}

void StageDataLoader::LoadFlagPosition(int stageNumber, StagePlacementData& placementData) {

	// 初期位置
	placementData.SetFlagPosition(97, 12);

	std::string filePath;

	if (stageNumber == 1) {
		filePath = "Resources/flag1.csv";
	} else if (stageNumber == 2) {
		filePath = "Resources/flag2.csv";
	} else {
		filePath = "Resources/flag3.csv";
	}

	std::ifstream file(filePath);

	if (file.is_open()) {
		int x;
		int y;
		char comma;

		file >> x >> comma >> y;

		if (file && comma == ',') {
			placementData.SetFlagPosition(x, y);
		}
	}
}

void StageDataLoader::LoadEnemyPositions(int stageNumber, StagePlacementData& placementData) {

	placementData.ClearEnemyPositions();

	// ステージごとの初期配置
	if (stageNumber == 1) {
		placementData.AddEnemyPosition(6, 12);
		placementData.AddEnemyPosition(20, 12);
		placementData.AddEnemyPosition(40, 12);
		placementData.AddEnemyPosition(65, 12);
		placementData.AddEnemyPosition(88, 12);
	} else if (stageNumber == 2) {
		placementData.AddEnemyPosition(5, 12);
		placementData.AddEnemyPosition(18, 12);
		placementData.AddEnemyPosition(30, 12);
		placementData.AddEnemyPosition(42, 12);
		placementData.AddEnemyPosition(55, 12);
		placementData.AddEnemyPosition(72, 12);
		placementData.AddEnemyPosition(88, 12);
	} else {
		placementData.AddEnemyPosition(5, 12);
		placementData.AddEnemyPosition(15, 12);
		placementData.AddEnemyPosition(25, 12);
		placementData.AddEnemyPosition(35, 12);
		placementData.AddEnemyPosition(45, 12);
		placementData.AddEnemyPosition(55, 12);
		placementData.AddEnemyPosition(65, 12);
		placementData.AddEnemyPosition(78, 12);
		placementData.AddEnemyPosition(90, 12);
	}

	std::string filePath;

	if (stageNumber == 1) {
		filePath = "Resources/enemies1.csv";
	} else if (stageNumber == 2) {
		filePath = "Resources/enemies2.csv";
	} else {
		filePath = "Resources/enemies3.csv";
	}

	std::ifstream file(filePath);

	if (file.is_open()) {
		placementData.ClearEnemyPositions();

		int x;
		int y;
		char comma;

		while (file >> x >> comma >> y) {
			if (comma == ',') {
				placementData.AddEnemyPosition(x, y);
			}
		}
	}
}