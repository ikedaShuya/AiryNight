#pragma once

#include "KamataEngine.h"

class HpBar {
public:
	// 初期化
	void Initialize();

	// HPに応じて更新
	void Update(int hp, int maxHp);

	// 描画
	void Draw();

	~HpBar();

private:
	// HPバーの背景と前景
	KamataEngine::Sprite* background_ = nullptr;
	KamataEngine::Sprite* foreground_ = nullptr;

	// テクスチャ
	uint32_t backgroundTexture_ = 0;
	uint32_t foregroundTexture_ = 0;
};