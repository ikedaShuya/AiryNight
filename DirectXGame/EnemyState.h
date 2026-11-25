#pragma once

class Enemy;
class Player;

// 敵の状態基底クラス
class EnemyState {
public:
	virtual ~EnemyState() = default;

	// 状態更新
	virtual bool Update(Enemy& enemy, const Player* player) = 0;
};

// 移動状態
class EnemyMovingState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;
};

// 待機状態
class EnemyWaitingState : public EnemyState {
public:
	bool Update(Enemy& enemy, const Player* player) override;
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