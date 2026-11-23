#include "BlockManager.h"
#include "Math.h"

using namespace KamataEngine;

void BlockManager::Initialize(Model* model) { model_ = model; }

void BlockManager::Generate(MapChipField* mapChipField) {

	// 既存のブロックを削除
	for (std::vector<WorldTransform*>& line : worldTransforms_) {
		for (WorldTransform* block : line) {
			delete block;
		}
	}

	worldTransforms_.clear();

	// マップの大きさを取得
	uint32_t numBlockVertical = mapChipField->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField->GetNumBlockHorizontal();

	worldTransforms_.resize(numBlockVertical);

	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		worldTransforms_[i].resize(numBlockHorizontal);

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();

				worldTransforms_[i][j] = worldTransform;
				worldTransforms_[i][j]->translation_ = mapChipField->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void BlockManager::Update() {
	for (std::vector<WorldTransform*>& line : worldTransforms_) {
		for (WorldTransform* block : line) {
			if (!block) {
				continue;
			}

			WorldTransformUpdate(*block);
		}
	}
}

void BlockManager::Draw(const Camera& camera) {
	for (std::vector<WorldTransform*>& line : worldTransforms_) {
		for (WorldTransform* block : line) {
			if (!block) {
				continue;
			}

			model_->Draw(*block, camera);
		}
	}
}

BlockManager::~BlockManager() {
	for (std::vector<WorldTransform*>& line : worldTransforms_) {
		for (WorldTransform* block : line) {
			delete block;
		}
	}

	worldTransforms_.clear();
}