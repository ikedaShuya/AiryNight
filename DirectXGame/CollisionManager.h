#pragma once

#include "Enemy.h"
#include "Player.h"
#include "ParticleManager.h"
#include <list>

class CollisionManager {
public:
	void CheckAllCollisions(Player* player, const std::list<Enemy*>& enemies, ParticleManager* particleManager, ParticleManager* deathParticleManager);

private:
	// キャラクター同士の当たり判定
	bool IsCharacterCollision(const Character& characterA, const Character& characterB);
};