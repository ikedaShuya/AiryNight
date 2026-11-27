#pragma once

#include "FadeState.h"
#include "KamataEngine.h"
#include <memory>

class Fade {
public:
	void Initialize();
	void Update();
	void Draw();

	void StartFadeOut();
	void StartFadeIn();

	bool IsFadeOutFinished() const { return fadeOutFinished_; }
	bool IsFadeInFinished() const { return fadeInFinished_; }

	// フェード状態から呼び出す更新処理
	bool AdvanceFadeOut();
	bool AdvanceFadeIn();

private:
	static const int kStarCount = 12;

	std::unique_ptr<KamataEngine::Sprite> sprite_;
	std::unique_ptr<KamataEngine::Sprite> stars_[kStarCount];

	float starStartX_[kStarCount] = {};
	float starStartY_[kStarCount] = {};

	// 現在のフェード状態
	std::unique_ptr<FadeState> state_;

	float alpha_ = 0.0f;
	float progress_ = 0.0f;
	float speed_ = 0.035f;

	bool fadeOutFinished_ = false;
	bool fadeInFinished_ = true;
};