#pragma once

#include "Enemy.h"
#include "Player.h"
#include <list>

class CollisionManager {
public:
	void CheckAllCollisions(Player* player, const std::list<Enemy*>& enemies);
};