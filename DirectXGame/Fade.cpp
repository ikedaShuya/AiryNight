#include "Fade.h"
#include <cmath>

using namespace KamataEngine;

void Fade::Initialize() {
	uint32_t textureHandle = TextureManager::Load("white1x1.png");

	// 画面全体を覆う黒いスプライト
	sprite_.reset(Sprite::Create(textureHandle, {0.0f, 0.0f}));
	sprite_->SetSize({1280.0f, 720.0f});
	sprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});

	// 光の粒が集まる位置
	float startX[kStarCount] = {0.0f, 160.0f, 400.0f, 640.0f, 900.0f, 1200.0f, 1280.0f, 1100.0f, 850.0f, 600.0f, 300.0f, 50.0f};

	float startY[kStarCount] = {100.0f, 0.0f, 80.0f, 0.0f, 100.0f, 200.0f, 450.0f, 720.0f, 650.0f, 720.0f, 600.0f, 400.0f};

	for (int i = 0; i < kStarCount; i++) {
		starStartX_[i] = startX[i];
		starStartY_[i] = startY[i];

		stars_[i].reset(Sprite::Create(textureHandle, {startX[i], startY[i]}));
		stars_[i]->SetSize({6.0f, 6.0f});
		stars_[i]->SetColor({0.5f, 0.9f, 1.0f, 0.0f});
	}
}

void Fade::StartFadeOut() {
	state_ = std::make_unique<FadeOutState>();
	progress_ = 0.0f;
	alpha_ = 0.0f;

	fadeOutFinished_ = false;
	fadeInFinished_ = false;
}

void Fade::StartFadeIn() {
	state_ = std::make_unique<FadeInState>();
	progress_ = 0.0f;
	alpha_ = 1.0f;

	fadeInFinished_ = false;
}

// フェードアウトの進行処理
bool Fade::AdvanceFadeOut() {
	progress_ += speed_;

	if (progress_ >= 1.0f) {
		progress_ = 1.0f;
	}

	alpha_ = progress_;

	return progress_ >= 1.0f;
}

// フェードインの進行処理
bool Fade::AdvanceFadeIn() {
	progress_ += speed_;

	if (progress_ >= 1.0f) {
		progress_ = 1.0f;
	}

	alpha_ = 1.0f - progress_;

	return progress_ >= 1.0f;
}

void Fade::Update() {
	bool isFadeIn = false;

	if (state_) {
		isFadeIn = state_->IsFadeIn();

		bool finished = state_->Update(*this);

		if (finished) {
			if (isFadeIn) {
				fadeInFinished_ = true;
			} else {
				fadeOutFinished_ = true;
			}

			state_.reset();
		}
	}

	sprite_->SetColor({0.0f, 0.0f, 0.02f, alpha_});

	for (int i = 0; i < kStarCount; i++) {
		float x;
		float y;

		if (isFadeIn) {
			// 中央から外側へ光が広がる
			x = 640.0f + (starStartX_[i] - 640.0f) * progress_;
			y = 360.0f + (starStartY_[i] - 360.0f) * progress_;
		} else {
			// 外側から中央へ光が集まる
			x = starStartX_[i] * (1.0f - progress_) + 640.0f * progress_;
			y = starStartY_[i] * (1.0f - progress_) + 360.0f * progress_;
		}

		stars_[i]->SetPosition({x, y});

		float starAlpha = 0.0f;

		if (state_) {
			starAlpha = std::sin(progress_ * 3.14159265f);
		}

		stars_[i]->SetColor({0.5f, 0.9f, 1.0f, starAlpha});
	}
}

void Fade::Draw() {
	Sprite::PreDraw();

	// 背景を暗くする
	sprite_->Draw();

	// 光の粒を重ねる
	for (int i = 0; i < kStarCount; i++) {
		stars_[i]->Draw();
	}

	Sprite::PostDraw();
}