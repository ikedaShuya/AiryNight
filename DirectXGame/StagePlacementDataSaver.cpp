#include "StagePlacementDataSaver.h"
#include <fstream>
#include <string>

bool StagePlacementDataSaver::Save(int stageNumber, const StagePlacementData& placementData) {
	std::string enemyFilePath;
	if (stageNumber == 1) {
		enemyFilePath = "Resources/enemies1.csv";
	} else if (stageNumber == 2) {
		enemyFilePath = "Resources/enemies2.csv";
	} else {
		enemyFilePath = "Resources/enemies3.csv";
	}

	std::ofstream enemyFile(enemyFilePath);
	if (!enemyFile.is_open()) {
		return false;
	}

	for (const StagePlacementData::EnemyPosition& position : placementData.GetEnemyPositions()) {
		enemyFile << position.x << "," << position.y << "\n";
	}
	enemyFile.close();

	std::string playerFilePath;
	if (stageNumber == 1) {
		playerFilePath = "Resources/player1.csv";
	} else if (stageNumber == 2) {
		playerFilePath = "Resources/player2.csv";
	} else {
		playerFilePath = "Resources/player3.csv";
	}

	std::ofstream playerFile(playerFilePath);
	if (!playerFile.is_open()) {
		return false;
	}

	playerFile << placementData.GetPlayerX() << "," << placementData.GetPlayerY() << "\n";
	playerFile.close();

	std::string flagFilePath;
	if (stageNumber == 1) {
		flagFilePath = "Resources/flag1.csv";
	} else if (stageNumber == 2) {
		flagFilePath = "Resources/flag2.csv";
	} else {
		flagFilePath = "Resources/flag3.csv";
	}

	std::ofstream flagFile(flagFilePath);
	if (!flagFile.is_open()) {
		return false;
	}

	flagFile << placementData.GetFlagX() << "," << placementData.GetFlagY() << "\n";
	flagFile.close();

	return true;
}