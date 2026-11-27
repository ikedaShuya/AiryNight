#pragma once
#include <memory>

class Player;

// 状態の基底クラス
class PlayerState {
public:
	virtual ~PlayerState() = default;

	virtual std::unique_ptr<PlayerState> Update(Player& player) = 0;

	virtual bool IsDead() const { return false; }
};

// 通常状態
class PlayerNormalState : public PlayerState {
public:
	std::unique_ptr<PlayerState> Update(Player& player) override;
};

// 攻撃状態
class PlayerAttackState : public PlayerState {
public:
	std::unique_ptr<PlayerState> Update(Player& player) override;
};

// 死亡状態
class PlayerDeadState : public PlayerState {
public:
	std::unique_ptr<PlayerState> Update(Player& player) override;

	bool IsDead() const override { return true; }
};