#include "StageMapLoader.h"

void StageMapLoader::LoadMap(int stageNumber, MapChipField& mapChipField) {

	if (stageNumber == 1) {
		mapChipField.LoadMapChipCsv("Resources/blocks.csv");
	} else if (stageNumber == 2) {
		mapChipField.LoadMapChipCsv("Resources/blocks2.csv");
	} else {
		mapChipField.LoadMapChipCsv("Resources/blocks3.csv");
	}
}