#pragma once

#include "KamataEngine.h"
#include "MapChipField.h"
#include <vector>

class BlockManager {
public:
	void Initialize(KamataEngine::Model* model);
	void Generate(MapChipField* mapChipField);
	void Update();
	void Draw(const KamataEngine::Camera& camera);
	~BlockManager();

private:
	KamataEngine::Model* model_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransforms_;
};