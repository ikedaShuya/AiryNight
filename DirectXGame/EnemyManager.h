#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "StagePlacementData.h"
#include <list>

class EnemyManager {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Generate(const StagePlacementData& placementData, MapChipField* mapChipField);
	void Update(Player* player);
	void Draw();
	int RemoveDeadEnemies();
	const std::list<Enemy*>& GetEnemies() const { return enemies_; }
	~EnemyManager();

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	std::list<Enemy*> enemies_;
};