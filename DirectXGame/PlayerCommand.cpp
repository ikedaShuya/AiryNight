#include "PlayerCommand.h"
#include "Player.h"

// 左移動
void MoveLeftCommand::Execute(Player& player) { player.RequestMoveLeft(); }

// 右移動
void MoveRightCommand::Execute(Player& player) { player.RequestMoveRight(); }

// ジャンプ
void JumpCommand::Execute(Player& player) { player.RequestJump(); }

// 攻撃
void AttackCommand::Execute(Player& player) { player.RequestAttack(); }