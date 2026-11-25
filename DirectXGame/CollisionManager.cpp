#include "CollisionManager.h"
#include "Math.h"
#include <cmath>
#include <cstdlib>

using namespace KamataEngine;

// キャラクター同士の当たり判定
bool CollisionManager::IsCharacterCollision(const Character& characterA, const Character& characterB) {

	AABB aabbA = characterA.GetAABB();
	AABB aabbB = characterB.GetAABB();

	return IsCollision(aabbA, aabbB);
}

void CollisionManager::CheckAllCollisions(Player* player, const std::list<Enemy*>& enemies, ParticleManager* particleManager, ParticleManager* deathParticleManager) {

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

			// 実際にダメージが入った場合だけ
			if (enemy->OnHit(player)) {

				Vector3 position = enemy->GetWorldPosition();

				if (!enemy->IsDead()) {

					// 火花を放射状に飛ばす
					for (int i = 0; i < 12; i++) {
						float angle = (2.0f * PI * i) / 12.0f;

						float speed = 0.08f + static_cast<float>(rand() % 100) / 1000.0f;

						Vector3 velocity = {std::cos(angle) * speed, std::sin(angle) * speed, (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 0.04f};

						Vector3 acceleration = {-velocity.x * 0.02f, -velocity.y * 0.02f, 0.0f};

						float particleScale = 0.12f + static_cast<float>(rand() % 100) / 800.0f;

						Vector3 scale = {particleScale * 4.0f, particleScale * 0.7f, particleScale * 0.7f};

						Vector3 rotationVelocity = {0.1f, 0.2f, 0.15f};

						particleManager->AddParticle(position, velocity, acceleration, scale, rotationVelocity, 30 + rand() % 16);
					}
				}

				// 敵を倒した場合
				if (enemy->IsDead()) {

					position = enemy->GetWorldPosition();

					// 45個の粒子を生成
					for (int i = 0; i < 45; i++) {

						// 粒子ごとに飛び散る方向を少し変える
						float angle = (2.0f * PI * i) / 45.0f;
						angle += (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 0.5f;

						// 粒子の移動速度をゆっくりにする
						float speed = 0.06f + static_cast<float>(rand() % 100) / 1000.0f;

						Vector3 velocity = {std::cos(angle) * speed, std::sin(angle) * speed + 0.025f, (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 0.12f};

						Vector3 acceleration = {-velocity.x * 0.015f, 0.0003f, -velocity.z * 0.015f};

						// 粒子ごとに大きさをランダムにする
						float particleScale = 0.10f + static_cast<float>(rand() % 100) / 350.0f;
						Vector3 scale = {particleScale, particleScale, particleScale};

						// 粒子ごとに回転速度をランダムにする
						Vector3 rotationVelocity = {static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f, static_cast<float>(rand() % 100) / 100.0f};

						deathParticleManager->AddParticle(position, velocity, acceleration, scale, rotationVelocity, 45 + (i % 3) * 10);
					}
				}
			}
		}
	}
}