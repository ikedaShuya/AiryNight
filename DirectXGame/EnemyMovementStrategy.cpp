#include "EnemyMovementStrategy.h"
#include "Enemy.h"
#include "Player.h"

// 巡回移動
float PatrolMovementStrategy::GetMoveVelocityX(const Enemy& enemy, const Player* player) {
	(void)player;

	return enemy.GetVelocityX();
}

void PatrolMovementStrategy::OnWallHit(Enemy& enemy) { enemy.ReverseVelocityX(); }

// 追跡移動
float ChaseMovementStrategy::GetMoveVelocityX(const Enemy& enemy, const Player* player) {

	if (player == nullptr) {
		return enemy.GetVelocityX();
	}

	float enemyX = enemy.GetWorldPosition().x;
	float playerX = player->GetWorldPosition().x;

	if (playerX > enemyX) {
		return enemy.GetWalkSpeed();
	}

	if (playerX < enemyX) {
		return -enemy.GetWalkSpeed();
	}

	return 0.0f;
}

void ChaseMovementStrategy::OnWallHit(Enemy& enemy) {
	// 壁にぶつかったら反転
	enemy.ReverseVelocityX();
}