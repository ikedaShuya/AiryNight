#pragma once

#include "KamataEngine.h"
#include "PlayerHpObserver.h"
#include <memory>

class HpBar : public PlayerHpObserver {
public:
	// 初期化
	void Initialize();

	// HPに応じて更新
	void Update(int hp, int maxHp);

	// 描画
	void Draw();

	// HP変更通知を受け取る
	void OnHpChanged(int hp, int maxHp) override;

private:
	// HPバーの背景と前景
	std::unique_ptr<KamataEngine::Sprite> background_;
	std::unique_ptr<KamataEngine::Sprite> foreground_;

	// テクスチャ
	uint32_t backgroundTexture_ = 0;
	uint32_t foregroundTexture_ = 0;
};