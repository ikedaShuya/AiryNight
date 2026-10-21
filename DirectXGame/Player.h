#pragma once
#include "KamataEngine.h"
#include "Math.h"

class MapChipField;

class Enemy;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Model* swordModel, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// プレイヤーの横移動入力を処理する
	void InputHorizontal();

	// 移動入力
	void InputMove();

	// ジャンプ処理を行う
	void ProcessJump();

	// 重力による落下処理を行う
	void ApplyGravity();

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move{};
	};

	// マップ衝突判定
	void CheckMapCollision(CollisionMapInfo& info);

	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	// 接地状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁に接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB() const;

	// 衝突応答
	void OnCollision(const Enemy* enemy);

	// 近接攻撃
	void ProcessAttack();

	// 攻撃判定AABBを取得
	AABB GetAttackAABB() const;

	// 攻撃中か
	bool IsAttacking() { return isAttacking_; }

	LRDirection GetLRDirection() const { return lrDirection_; }

	AABB GetSwordAABB() const;

	bool IsAttacking() const { return isAttacking_; }

	int GetHp() const { return hp_; }
	int GetMaxHp() const { return maxHp_; }

	// ダメージ処理
	void TakeDamage(int damage);

	KamataEngine::Vector3 GetHeadWorldPosition() const;

	bool IsDead() const { return hp_ <= 0; }

	// プレイヤー操作の許可
	void SetControllable(bool controllable) { isControllable_ = controllable; }

private:
	// ============================
	// モデル・カメラ関連
	// ============================

	// ワールド変換
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;      // プレイヤー
	KamataEngine::Model* modelSword_ = nullptr; // 剣

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// ============================
	// 移動・物理パラメータ
	// ============================

	// 移動速度
	KamataEngine::Vector3 velocity_ = {};

	// 加速・減衰
	static inline const float kAcceleration = 0.025f;
	static inline const float kAttenuation = 0.90f;
	static inline const float kLimitRunSpeed = 0.15f;

	// 重力・ジャンプ
	static inline const float kGravityAcceleration = 0.045f;
	static inline const float kLimitFallSpeed = 0.42f;
	static inline const float kJumpAcceleration = 0.52f;

	// 接地判定
	static inline const float kGroundHeight = 1.0f;
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float kBlank = 0.04f;

	bool onGround_ = false;

	// ============================
	// 旋回処理
	// ============================

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回制御
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;

	// ============================
	// 当たり判定・フィールド
	// ============================

	// キャラクターサイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// フィールド
	MapChipField* mapChipField_ = nullptr;

	// 接触時減衰
	static inline const float kAttenuationLanding = 0.03f;
	static inline const float kAttenuationWall = 0.2f;

	// ============================
	// ジャンプ関連
	// ============================

	// ジャンプバッファ
	int jumpBufferTimer_ = 0;
	static constexpr int kJumpBufferFrame = 5;

	// ジャンプ回数
	int jumpCount_ = 0;
	static constexpr int kMaxJumpCount = 2;

	// ============================
	// 近接攻撃関連
	// ============================

	// 攻撃状態
	bool isAttacking_ = false;

	// 攻撃タイマー
	int attackTimer_ = 0;
	static constexpr int kAttackFrame = 12;

	// 攻撃判定
	static inline const float kAttackWidth = 1.6f;
	static inline const float kAttackHeight = 1.6f;
	static inline const float kAttackOffset = 0.7f;

	// 攻撃開始時の姿勢保存
	float attackStartPosX_ = 0.0f;
	float attackStartRotationX_ = 0.0f;

	// ============================
	// 剣関連
	// ============================

	// ワールド変換
	KamataEngine::WorldTransform swordWorldTransform_;

	// 初期姿勢
	KamataEngine::Vector3 swordStartRotation_;
	KamataEngine::Vector3 swordStartTranslation_;

	// スケール
	KamataEngine::Vector3 swordScale_;

	// ============================
	// HP・ダメージ
	// ============================

	int hp_ = 100;
	int maxHp_ = 100;

	// ダメージ無敵時間
	int damageCoolTime_ = 0;
	static constexpr int kDamageCoolTime = 30;

	// 操作可能か
	bool isControllable_ = true;
};