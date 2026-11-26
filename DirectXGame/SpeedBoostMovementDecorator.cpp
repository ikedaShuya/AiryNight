#include "SpeedBoostMovementDecorator.h"

// コンストラクタ
SpeedBoostMovementDecorator::SpeedBoostMovementDecorator(std::unique_ptr<EnemyMovementStrategy> strategy) : strategy_(std::move(strategy)) {}

// 元の移動速度に倍率をかける
float SpeedBoostMovementDecorator::GetMoveVelocityX(const Enemy& enemy, const Player* player) { return strategy_->GetMoveVelocityX(enemy, player) * kSpeedMultiplier; }

// 壁にぶつかったときの処理は元の移動方法に任せる
void SpeedBoostMovementDecorator::OnWallHit(Enemy& enemy) { strategy_->OnWallHit(enemy); }