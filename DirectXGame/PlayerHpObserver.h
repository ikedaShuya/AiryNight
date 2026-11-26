#pragma once

// HP変更通知を受け取る側のインターフェース
class PlayerHpObserver {
public:
	virtual void OnHpChanged(int hp, int maxHp) = 0;
	virtual ~PlayerHpObserver() = default;
};