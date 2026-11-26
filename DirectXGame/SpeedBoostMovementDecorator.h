#pragma once

#include "EnemyMovementStrategy.h"
#include <memory>

// 移動速度を上げるDecorator
class SpeedBoostMovementDecorator : public EnemyMovementStrategy {
public:
	explicit SpeedBoostMovementDecorator(std::unique_ptr<EnemyMovementStrategy> strategy);

	float GetMoveVelocityX(const Enemy& enemy, const Player* player) override;
	void OnWallHit(Enemy& enemy) override;

private:
	// 元の移動方法
	std::unique_ptr<EnemyMovementStrategy> strategy_;

	// 速度倍率
	static constexpr float kSpeedMultiplier = 1.5f;
};