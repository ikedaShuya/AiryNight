#pragma once
#include "Character.h"
#include "EnemyMovementStrategy.h"
#include "EnemyState.h"
#include "KamataEngine.h"
#include "Math.h"
#include <memory>

class Player;
class MapChipField;

/// <summary>
/// 敵
/// </summary>
class Enemy : public Character {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// マップチップフィールドを設定
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// 移動戦略を設定
	void SetMovementStrategy(std::unique_ptr<EnemyMovementStrategy> strategy) { movementStrategy_ = std::move(strategy); }

	// 移動速度を取得
	float GetVelocityX() const { return velocity_.x; }

	// 移動戦略から速度を取得
	float GetMoveVelocityX(const Player* player) const {
		if (movementStrategy_) {
			return movementStrategy_->GetMoveVelocityX(*this, player);
		}

		return velocity_.x;
	}

	// 壁にぶつかったときの移動処理
	void OnMovementWallHit() {
		if (movementStrategy_) {
			movementStrategy_->OnWallHit(*this);
		} else {
			ReverseVelocityX();
		}
	}

	// 歩行速度を取得
	float GetWalkSpeed() const { return kWalkSpeed; }

	// 進行方向を反転
	void ReverseVelocityX() { velocity_.x *= -1.0f; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Player* player);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	KamataEngine::Vector3 GetWorldPosition() const override;
	AABB GetAABB() const override;

	// 衝突応答
	void OnCollision(const Player* player);

	// 攻撃を受けたとき
	bool OnHit(const Player* player);

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// 移動状態の処理
	bool UpdateMovingState(const Player* player);

	// 待機状態の処理
	bool UpdateWaitingState();

	// 落下状態の処理
	bool UpdateFallingState();

	// 死亡状態の処理
	bool UpdateDeadState();

private:
	// 敵の剣との当たり判定
	void CheckHitByPlayerSword(const Player* player);

	// 状態変更
	void ChangeState(std::unique_ptr<EnemyState> newState);

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.02f;

	// 速度
	KamataEngine::Vector3 velocity_ = {};

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;

	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 30.0f;

	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 壁との当たり判定
	static inline const float kBlank = 0.04f;

	// 左右の壁との当たり判定
	void CheckMapCollisionX(float& moveX);

	// 進行方向の先に床があるか確認
	bool HasFloorAhead(float moveX);

	// 落下中かどうか
	bool isFalling_ = false;

	// 落下速度
	float fallSpeed_ = 0.0f;

	// 重力
	static inline const float kGravity = 0.02f;

	// ===== ダメージ関連 =====

	// HP
	int hp_ = 3;

	// デスフラグ
	bool isDead_ = false;

	// 無敵時間
	int invincibleTimer_ = 0;
	static constexpr int kInvincibleFrame = 20;

	// ノックバック速度
	KamataEngine::Vector3 knockbackVelocity_{0, 0, 0};

	// ノックバック減衰率
	static constexpr float kKnockbackAttenuation = 0.85f;

	// ノックバックの強さ
	static inline const float kKnockbackPower = 0.25f;

	// 現在の状態
	std::unique_ptr<EnemyState> state_;

	// 移動戦略
	std::unique_ptr<EnemyMovementStrategy> movementStrategy_;

	float stateTimer_ = 0.0f;

	const float moveDuration_ = 2.0f; // 2秒間移動
	const float waitDuration_ = 1.0f; // 1秒間停止
};