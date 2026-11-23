#include "GameOverEffect.h"

using namespace KamataEngine;

// 初期化
void GameOverEffect::Initialize() {
	fadeSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});

	fadeSprite_->SetSize({1280.0f, 720.0f});
	fadeSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

// 演出開始
void GameOverEffect::Start(const Camera& camera) {
	isPlaying_ = true;
	timer_ = 0;
	startFovY_ = camera.fovAngleY;
}

// 更新
bool GameOverEffect::Update(Camera& camera) {
	if (!isPlaying_) {
		return false;
	}

	timer_++;

	// カメラを30フレームかけてズームイン
	if (timer_ <= kZoomFrame) {
		float t = static_cast<float>(timer_) / static_cast<float>(kZoomFrame);
		float ease = t * t;

		camera.fovAngleY = startFovY_ * (1.0f - 0.35f * ease);
	}

	camera.UpdateMatrix();

	// 最初の10フレームは赤い衝撃演出
	if (timer_ <= kFlashFrame) {
		float alpha = 0.55f * (1.0f - static_cast<float>(timer_ - 1) / static_cast<float>(kFlashFrame));

		fadeSprite_->SetColor({1.0f, 0.0f, 0.0f, alpha});
	} else {
		// その後、黒く暗転
		float alpha = static_cast<float>(timer_ - kFlashFrame) / static_cast<float>(kGameOverFrame - kFlashFrame);

		if (alpha > 1.0f) {
			alpha = 1.0f;
		}

		fadeSprite_->SetColor({0.0f, 0.0f, 0.0f, alpha});
	}

	// 演出終了
	if (timer_ >= kGameOverFrame) {
		isPlaying_ = false;
		return true;
	}

	return false;
}

// 描画
void GameOverEffect::Draw() {
	if (isPlaying_) {
		fadeSprite_->Draw();
	}
}

// 終了
GameOverEffect::~GameOverEffect() { delete fadeSprite_; }