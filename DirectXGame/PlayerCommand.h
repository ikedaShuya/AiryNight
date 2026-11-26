#pragma once

class Player;

// プレイヤー操作コマンドの基底クラス
class PlayerCommand {
public:
	virtual void Execute(Player& player) = 0;
	virtual ~PlayerCommand() = default;
};

// 左移動コマンド
class MoveLeftCommand : public PlayerCommand {
public:
	void Execute(Player& player) override;
};

// 右移動コマンド
class MoveRightCommand : public PlayerCommand {
public:
	void Execute(Player& player) override;
};

// ジャンプコマンド
class JumpCommand : public PlayerCommand {
public:
	void Execute(Player& player) override;
};

// 攻撃コマンド
class AttackCommand : public PlayerCommand {
public:
	void Execute(Player& player) override;
};