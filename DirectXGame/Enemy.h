#pragma once
#include "KamataEngine.h"
#include "Math.h"

class Player;
class MapChipField;

enum class EnemyState { Moving, Waiting };

/// <summary>
/// 敵
/// </summary>
class Enemy {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// マップチップフィールドを設定
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Player* player);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Player* player);

	// 攻撃を受けたとき
	void OnHit(const Player* player);

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	void CheckHitByPlayerSword(const Player* player);

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル（スライド6
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

	KamataEngine::Vector3 knockbackVelocity_{0, 0, 0};
	static constexpr float kKnockbackAttenuation = 0.85f;

	static inline const float kKnockbackPower = 0.25f;

	 EnemyState state_ = EnemyState::Moving;
	float stateTimer_ = 0.0f; // 状態継続時間カウント

	const float moveDuration_ = 2.0f; // 2秒間移動
	const float waitDuration_ = 1.0f; // 1秒間停止
};