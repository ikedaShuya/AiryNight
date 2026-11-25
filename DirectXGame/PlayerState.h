#pragma once

class Player;

// プレイヤーの状態
enum class PlayerStateType { kNormal, kAttack, kDead };

// 状態の基底クラス
class PlayerState {
public:
	virtual ~PlayerState() = default;

	virtual PlayerStateType Update(Player& player) = 0;
};

// 通常状態
class PlayerNormalState : public PlayerState {
public:
	PlayerStateType Update(Player& player) override;
};

// 攻撃状態
class PlayerAttackState : public PlayerState {
public:
	PlayerStateType Update(Player& player) override;
};

// 死亡状態
class PlayerDeadState : public PlayerState {
public:
	PlayerStateType Update(Player& player) override;
};