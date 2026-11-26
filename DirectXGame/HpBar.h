#pragma once

#include "KamataEngine.h"
#include "PlayerHpObserver.h"

class HpBar : public PlayerHpObserver {
public:
	// 初期化
	void Initialize();

	// HPに応じて更新
	void Update(int hp, int maxHp);

	// 描画
	void Draw();

	~HpBar();

	// HP変更通知を受け取る
	void OnHpChanged(int hp, int maxHp) override;

private:
	// HPバーの背景と前景
	KamataEngine::Sprite* background_ = nullptr;
	KamataEngine::Sprite* foreground_ = nullptr;

	// テクスチャ
	uint32_t backgroundTexture_ = 0;
	uint32_t foregroundTexture_ = 0;
};