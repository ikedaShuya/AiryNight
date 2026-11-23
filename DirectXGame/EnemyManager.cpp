
#include "EnemyManager.h"

using namespace KamataEngine;

void EnemyManager::Initialize(Model* model, Camera* camera) {
	model_ = model;
	camera_ = camera;
}

void EnemyManager::Generate(const StagePlacementData& placementData, MapChipField* mapChipField) {

	// 既存の敵を削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	// 配置データから敵を生成
	for (const StagePlacementData::EnemyPosition& position : placementData.GetEnemyPositions()) {

		Vector3 enemyPosition = mapChipField->GetMapChipPositionByIndex(position.x, position.y);

		Enemy* enemy = new Enemy;

		enemy->Initialize(model_, camera_, enemyPosition);
		enemy->SetMapChipField(mapChipField);

		enemies_.push_back(enemy);
	}
}

void EnemyManager::Update(Player* player) {
	for (Enemy* enemy : enemies_) {
		enemy->Update(player);
	}
}

void EnemyManager::Draw() {
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
}

int EnemyManager::RemoveDeadEnemies() {

	int removedCount = 0;

	for (auto it = enemies_.begin(); it != enemies_.end();) {

		if ((*it)->IsDead()) {
			delete *it;
			it = enemies_.erase(it);
			removedCount++;
		} else {
			++it;
		}
	}

	return removedCount;
}

EnemyManager::~EnemyManager() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	delete model_;
}