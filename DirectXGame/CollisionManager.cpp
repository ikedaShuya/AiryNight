#include "CollisionManager.h"
#include "Math.h"

using namespace KamataEngine;

void CollisionManager::CheckAllCollisions(Player* player, const std::list<Enemy*>& enemies) {

	// 自キャラと敵キャラの当たり判定
	AABB playerAABB = player->GetAABB();

	for (Enemy* enemy : enemies) {
		AABB enemyAABB = enemy->GetAABB();

		if (IsCollision(playerAABB, enemyAABB)) {
			player->OnCollision(enemy);
			enemy->OnCollision(player);
		}
	}

	// 近接攻撃中のみ、剣と敵の当たり判定を行う
	if (!player->IsAttacking()) {
		return;
	}

	AABB attackAABB = player->GetSwordAABB();

	for (Enemy* enemy : enemies) {
		AABB enemyAABB = enemy->GetAABB();

		if (IsCollision(attackAABB, enemyAABB)) {
			enemy->OnHit(player);
		}
	}
}