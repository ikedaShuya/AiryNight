#include "ClearSceneEffect.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void ClearSceneEffect::Initialize() {

	// フラッシュ用スプライト
	clearFlashSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});
	clearFlashSprite_->SetSize({1280.0f, 720.0f});
	clearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

	// 粒子を生成
	uint32_t particleTexture = TextureManager::Load("white1x1.png");

	for (int i = 0; i < kClearParticleCount; i++) {

		clearParticles_[i] = Sprite::Create(particleTexture, {634.0f, 354.0f});
		clearParticles_[i]->SetSize({24.0f, 24.0f});
		clearParticles_[i]->SetColor({1.0f, 0.8f, 0.3f, 0.0f});

		float angle = (2.0f * std::numbers::pi_v<float> * i) / kClearParticleCount;
		float speed = 5.0f + (i % 4) * 1.5f;

		particleVelocityX_[i] = std::cos(angle) * speed;
		particleVelocityY_[i] = std::sin(angle) * speed;
	}
}

float ClearSceneEffect::Update(float finalTitleScale) {

	float scale = finalTitleScale;

	// CLEAR文字の登場アニメーション
	if (clearTitleTimer_ < kClearTitleAnimationFrame) {

		clearTitleTimer_++;

		float t = static_cast<float>(clearTitleTimer_) / kClearTitleAnimationFrame;

		if (t < 0.8f) {
			float ease = t / 0.8f;
			scale = 0.1f + 5.4f * ease;
		} else {
			float ease = (t - 0.8f) / 0.2f;
			scale = 5.5f - 0.5f * ease;
		}
	}

	// CLEAR登場後に粒子を発生させる
	if (clearTitleTimer_ >= kClearTitleAnimationFrame) {
		clearParticlesStarted_ = true;
		clearFlashStarted_ = true;
	}

	// 粒子の移動と消滅
	if (clearParticlesStarted_ && clearParticleTimer_ < 60) {

		clearParticleTimer_++;

		for (int i = 0; i < kClearParticleCount; i++) {

			float x = 640.0f + particleVelocityX_[i] * clearParticleTimer_;
			float y = 360.0f + particleVelocityY_[i] * clearParticleTimer_;

			clearParticles_[i]->SetPosition({x, y});

			float alpha = 1.0f - static_cast<float>(clearParticleTimer_) / 60.0f;
			float size = 24.0f * alpha;

			clearParticles_[i]->SetSize({size, size});
			clearParticles_[i]->SetColor({1.0f, 0.8f, 0.3f, alpha});
		}
	}

	// フラッシュ演出
	if (clearFlashStarted_ && clearFlashTimer_ < 90) {

		clearFlashTimer_++;

		float alpha = 0.0f;

		if (clearFlashTimer_ <= 15) {
			alpha = 1.0f - static_cast<float>(clearFlashTimer_) / 15.0f;
		}

		clearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, alpha});
	}

	return scale;
}

void ClearSceneEffect::Draw() {

	clearFlashSprite_->Draw();

	if (clearParticlesStarted_) {
		for (int i = 0; i < kClearParticleCount; i++) {
			clearParticles_[i]->Draw();
		}
	}
}

ClearSceneEffect::~ClearSceneEffect() {

	delete clearFlashSprite_;

	for (int i = 0; i < kClearParticleCount; i++) {
		delete clearParticles_[i];
	}
}