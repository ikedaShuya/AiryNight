#include "CollisionManager.h"
#include "Math.h"

using namespace KamataEngine;

// キャラクター同士の当たり判定
bool CollisionManager::IsCharacterCollision(const Character& characterA, const Character& characterB) {

	AABB aabbA = characterA.GetAABB();
	AABB aabbB = characterB.GetAABB();

	return IsCollision(aabbA, aabbB);
}

void CollisionManager::CheckAllCollisions(Player* player, const std::list<Enemy*>& enemies) {

	if (!player) {
		return;
	}

	for (Enemy* enemy : enemies) {
		if (!enemy) {
			continue;
		}

		if (IsCharacterCollision(*player, *enemy)) {
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
		if (!enemy) {
			continue;
		}
		AABB enemyAABB = enemy->GetAABB();

		if (IsCollision(attackAABB, enemyAABB)) {
			enemy->OnHit(player);
		}
	}
}