#pragma once

#include "KamataEngine.h"
#include "MapChipField.h"
#include <vector>
#include <memory>

class BlockManager {
public:
	void Initialize(KamataEngine::Model* model);
	void Generate(MapChipField* mapChipField);
	void Update();
	void Draw(const KamataEngine::Camera& camera);
	~BlockManager() = default;

private:
	KamataEngine::Model* model_ = nullptr;
	std::vector<std::vector<std::unique_ptr<KamataEngine::WorldTransform>>> worldTransforms_;
};