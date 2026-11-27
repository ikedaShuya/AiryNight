#include "BlockManager.h"
#include "Math.h"

using namespace KamataEngine;

void BlockManager::Initialize(Model* model) { model_ = model; }

void BlockManager::Generate(MapChipField* mapChipField) {

	// 既存のブロックを削除
	worldTransforms_.clear();

	// マップの大きさを取得
	uint32_t numBlockVertical = mapChipField->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField->GetNumBlockHorizontal();

	worldTransforms_.resize(numBlockVertical);

	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		worldTransforms_[i].resize(numBlockHorizontal);

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				std::unique_ptr<WorldTransform> worldTransform = std::make_unique<WorldTransform>();
				worldTransform->Initialize();

				worldTransforms_[i][j] = std::move(worldTransform);
				worldTransforms_[i][j]->translation_ = mapChipField->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void BlockManager::Update() {
	for (std::vector<std::unique_ptr<WorldTransform>>& line : worldTransforms_) {
		for (std::unique_ptr<WorldTransform>& block : line) {
			if (!block) {
				continue;
			}

			WorldTransformUpdate(*block);
		}
	}
}

void BlockManager::Draw(const Camera& camera) {
	for (std::vector<std::unique_ptr<WorldTransform>>& line : worldTransforms_) {
		for (std::unique_ptr<WorldTransform>& block : line) {
			if (!block) {
				continue;
			}

			model_->Draw(*block, camera);
		}
	}
}