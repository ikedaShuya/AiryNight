#pragma once

class Enemy;
class Player;

// 敵の状態基底クラス
class EnemyState {
public:
	virtual ~EnemyState() = default;

	// 状態更新
	virtual bool Update(Enemy& enemy, const Player* player) = 0;

	// 無敵時間中かどうか
	virtual bool IsInvincible() const { return false; }

	// 頭の向きを変更するかどうか
	virtual bool CanTurnHead() const { return false; }
};

// 落下状態
class EnemyFallingState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;
};

// 死亡状態
class EnemyDeadState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;
};

class EnemyMovingState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;

	bool IsInvincible() const override { return true; }

	bool CanTurnHead() const override { return true; }
};

class EnemyWaitingState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;

	bool IsInvincible() const override { return true; }
};