#pragma once

class Enemy;
class Player;

// 敵の移動方法のインターフェース
class EnemyMovementStrategy {
public:
	virtual float GetMoveVelocityX(const Enemy& enemy, const Player* player) = 0;
	virtual void OnWallHit(Enemy& enemy) = 0;

	virtual ~EnemyMovementStrategy() = default;
};

// 巡回移動
class PatrolMovementStrategy : public EnemyMovementStrategy {
public:
	float GetMoveVelocityX(const Enemy& enemy, const Player* player) override;
	void OnWallHit(Enemy& enemy) override;
};

// 追跡移動
class ChaseMovementStrategy : public EnemyMovementStrategy {
public:
	float GetMoveVelocityX(const Enemy& enemy, const Player* player) override;
	void OnWallHit(Enemy& enemy) override;
};