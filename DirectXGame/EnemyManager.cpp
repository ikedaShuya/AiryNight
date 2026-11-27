#include "EnemyManager.h"
#include "EnemyMovementStrategy.h"
#include "SpeedBoostMovementDecorator.h"

using namespace KamataEngine;

void EnemyManager::Initialize(Model* model, Camera* camera) {
	model_ = model;
	camera_ = camera;
}

void EnemyManager::Generate(const StagePlacementData& placementData, MapChipField* mapChipField) {

	// 既存の敵を削除
	enemies_.clear();

	
	// 配置データから敵を生成
	int enemyIndex = 0;

	for (const StagePlacementData::EnemyPosition& position : placementData.GetEnemyPositions()) {

		Vector3 enemyPosition = mapChipField->GetMapChipPositionByIndex(position.x, position.y);

		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();

		enemy->Initialize(model_, camera_, enemyPosition);
		enemy->SetMapChipField(mapChipField);

		// 敵ごとに移動方法を設定
		if (enemyIndex % 2 == 0) {
			// 偶数番目：高速追跡
			enemy->SetMovementStrategy(std::make_unique<SpeedBoostMovementDecorator>(std::make_unique<ChaseMovementStrategy>()));
		} else {
			// 奇数番目：通常巡回
			enemy->SetMovementStrategy(std::make_unique<PatrolMovementStrategy>());
		}

		enemies_.push_back(std::move(enemy));

		enemyIndex++;
	}
}

void EnemyManager::Update(Player* player) {
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update(player);
	}
}

void EnemyManager::Draw() {
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw();
	}
}

int EnemyManager::RemoveDeadEnemies() {

	int removedCount = 0;

	for (auto it = enemies_.begin(); it != enemies_.end();) {

		if ((*it)->IsDead()) {
			it = enemies_.erase(it);
			removedCount++;
		} else {
			++it;
		}
	}

	return removedCount;
}

EnemyManager::~EnemyManager() { enemies_.clear(); }