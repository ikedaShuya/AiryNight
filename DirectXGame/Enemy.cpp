#include "Enemy.h"
#include "Player.h"
#include "MapChipField.h"
#include <numbers>
#include <cassert>

using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLポインタチェック
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update(const Player*) {

	if (isDead_) {
		return;
	}

	// 落下中の処理

	// Falling branch
	if (isFalling_) {
		// 重力で落下
		fallSpeed_ -= kGravity;

		float currentY = worldTransform_.translation_.y;
		float nextY = currentY + fallSpeed_;

		// 敵の足元の座標
		float currentBottom = currentY - kHeight / 2.0f;
		float nextBottom = nextY - kHeight / 2.0f;

		// 着地判定
		bool landed = false;

		if (mapChipField_) {
			for (uint32_t y = 0; y < mapChipField_->GetNumBlockVertical(); ++y) {
				for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); ++x) {

					// ブロック以外は無視
					if (mapChipField_->GetMapChipTypeByIndex(x, y) != MapChipType::kBlock) {
						continue;
					}

					// ブロックの範囲
					MapChipField::Rect rect = mapChipField_->GetRectByIndex(x, y);

					// 敵とブロックが左右に重なっているか
					float enemyLeft = worldTransform_.translation_.x - kWidth / 2.0f;
					float enemyRight = worldTransform_.translation_.x + kWidth / 2.0f;

					if (enemyRight <= rect.left || enemyLeft >= rect.right) {
						continue;
					}

					// ブロックの上面を通過したか
					if (currentBottom >= rect.top && nextBottom <= rect.top) {
						worldTransform_.translation_.y = rect.top + kHeight / 2.0f;
						fallSpeed_ = 0.0f;
						isFalling_ = false;
						landed = true;
						break;
					}
				}

				if (landed) {
					break;
				}
			}
		}

		// 着地していなければ落下を続ける
		if (!landed) {
			worldTransform_.translation_.y = nextY;
		}

		// マップの外まで落ちた場合
		if (worldTransform_.translation_.y < -10.0f) {
			isDead_ = true;
		}

		WorldTransformUpdate(worldTransform_);
		return;
	}

	// 無敵時間カウント
	if (invincibleTimer_ > 0) {
		invincibleTimer_--;
	}

	// 状態時間更新
	stateTimer_ += 1.0f / 60.0f;

	switch (state_) {
	case EnemyState::Moving:
		// 移動処理
		{
			Vector3 totalVelocity = velocity_ + knockbackVelocity_;

			// 壁との当たり判定
			float moveX = totalVelocity.x;
			CheckMapCollisionX(moveX);

			// 壁にぶつかった場合
			if (moveX != totalVelocity.x) {
				velocity_.x *= -1.0f;
				knockbackVelocity_.x = 0.0f;
			}

			// 床がなくなる場合
			if (!HasFloorAhead(moveX)) {

				// 落下状態にする
				isFalling_ = true;

				// 落下速度を初期化
				fallSpeed_ = 0.0f;

				// ノックバックを停止
				knockbackVelocity_.x = 0.0f;
			}

			// 移動
			worldTransform_.translation_.x += moveX;
			worldTransform_.translation_.y += totalVelocity.y;

			// ノックバック減衰
			knockbackVelocity_.x *= kKnockbackAttenuation;

			// 移動時間終了したら待機へ
			if (stateTimer_ >= moveDuration_) {
				state_ = EnemyState::Waiting;
				stateTimer_ = 0.0f;
			}
		}
		break;

	case EnemyState::Waiting:
		// 停止して動かない
		// ノックバックは残す場合ここで処理（省略可）

		// 待機時間終了したら移動へ戻る
		if (stateTimer_ >= waitDuration_) {
			state_ = EnemyState::Moving;
			stateTimer_ = 0.0f;
		}
		break;
	}

	// 首フリフリ等の処理（移動中だけ首フリフリしたい場合は条件を追加）
	if (state_ == EnemyState::Moving && std::abs(knockbackVelocity_.x) < 0.01f) {
		const float t = stateTimer_;
		const float baseAngle = -1.5f;
		const float kNeckRollAngle = 1.0f;
		const float kNeckRollSpeed = 4.0f;
		const float kNeckNodAngle = 0.3f;
		const float kNeckNodSpeed = 2.0f;
		const float kBodySwayAmount = 0.01f;
		const float kBodySwaySpeed = 0.8f;

		worldTransform_.rotation_.y = baseAngle + std::sin(t * kNeckRollSpeed) * kNeckRollAngle;
		worldTransform_.rotation_.x = std::sin(t * kNeckNodSpeed) * kNeckNodAngle;
		worldTransform_.translation_.x += std::sin(t * kBodySwaySpeed) * kBodySwayAmount;
	} else if (std::abs(knockbackVelocity_.x) >= 0.01f) {
		// ノックバック時は向き固定
		const float baseAngle = -1.5f;
		worldTransform_.rotation_.y = baseAngle + std::numbers::pi_v<float>;
		worldTransform_.rotation_.x = 0.0f;
	} else {
		// 停止中に首角度を0にスムーズに戻す処理
		const float returnSpeed = 0.12f; // 補間速度（0〜1で調整）
		worldTransform_.rotation_.y = Lerp(worldTransform_.rotation_.y, -1.5f, returnSpeed);
		worldTransform_.rotation_.x = Lerp(worldTransform_.rotation_.x, 0.0f, returnSpeed);
		// 体の揺れもリセットしたい場合は同様に補間してください
	}

	WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

KamataEngine::Vector3 Enemy::GetWorldPosition() const {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() const {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }

bool Enemy::OnHit(const Player* player) {

	if (isDead_ || invincibleTimer_ > 0) {
		return false;
	}

	hp_--;

	// Player::LRDirection を明示する
	float dir = (player->GetLRDirection() == Player::LRDirection::kRight) ? 1.0f : -1.0f;

	knockbackVelocity_.x = dir * kKnockbackPower;

	invincibleTimer_ = kInvincibleFrame;

	if (hp_ <= 0) {
		isDead_ = true;
	}

	return true;
}

// 左右の壁との当たり判定
void Enemy::CheckMapCollisionX(float& moveX) {

	// マップが設定されていなければ処理しない
	if (mapChipField_ == nullptr) {
		return;
	}

	// 移動方向によって判定する
	if (moveX > 0.0f) {

		// 右側の壁を調べる
		for (int i = 0; i < 2; i++) {

			float offsetY = (i == 0) ? -kHeight / 2.0f : kHeight / 2.0f;

			Vector3 checkPosition = worldTransform_.translation_ + Vector3(moveX + kWidth / 2.0f, offsetY, 0);

			MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(checkPosition);

			if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {

				MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

				moveX = rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank);

				break;
			}
		}

	} else if (moveX < 0.0f) {

		// 左側の壁を調べる
		for (int i = 0; i < 2; i++) {

			float offsetY = (i == 0) ? -kHeight / 2.0f : kHeight / 2.0f;

			Vector3 checkPosition = worldTransform_.translation_ + Vector3(moveX - kWidth / 2.0f, offsetY, 0);

			MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(checkPosition);

			if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {

				MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

				moveX = rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank);

				break;
			}
		}
	}
}

// 進行方向の先に床があるか確認
bool Enemy::HasFloorAhead(float moveX) {

	if (mapChipField_ == nullptr) {
		return true;
	}

	if (moveX == 0.0f) {
		return true;
	}

	// 移動方向
	float direction = 1.0f;

	if (moveX < 0.0f) {
		direction = -1.0f;
	}

	// 敵の進行方向の足元を調べる
	Vector3 checkPosition = worldTransform_.translation_;

	checkPosition.x += moveX + direction * (kWidth / 2.0f);
	checkPosition.y -= kHeight / 2.0f + 0.15f;

	// マップチップの番号を取得
	MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(checkPosition);

	// マップの範囲外なら床がない
	if (indexSet.xIndex >= mapChipField_->GetNumBlockHorizontal() || indexSet.yIndex >= mapChipField_->GetNumBlockVertical()) {
		return false;
	}

	// 足元にブロックがあるか確認
	MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	return mapChipType == MapChipType::kBlock;
}