
#pragma once

#include "KamataEngine.h"

class Fade {
public:
	enum class Status {
		kNone,
		kFadeOut,
		kFadeIn,
	};

	void Initialize();
	void Update();
	void Draw();

	void StartFadeOut();
	void StartFadeIn();

	bool IsFadeOutFinished() const { return fadeOutFinished_; }
	bool IsFadeInFinished() const { return fadeInFinished_; }

private:
	static const int kStarCount = 12;

	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Sprite* stars_[kStarCount] = {};

	float starStartX_[kStarCount] = {};
	float starStartY_[kStarCount] = {};

	Status status_ = Status::kNone;

	float alpha_ = 0.0f;
	float progress_ = 0.0f;
	float speed_ = 0.035f;

	bool fadeOutFinished_ = false;
	bool fadeInFinished_ = true;
};