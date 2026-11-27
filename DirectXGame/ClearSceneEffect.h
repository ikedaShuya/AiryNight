#pragma once
#include "KamataEngine.h"
#include "Effect.h"
#include <memory>

class ClearSceneEffect : public Effect {
public:
	void Initialize() override;

	float Update(float finalTitleScale);

	void Draw() override;

	bool IsFlashFinished() const { return clearFlashTimer_ >= 90; }

private:
	// CLEAR文字の登場演出
	int clearTitleTimer_ = 0;
	static constexpr int kClearTitleAnimationFrame = 30;

	// フラッシュ演出
	std::unique_ptr<KamataEngine::Sprite> clearFlashSprite_;
	int clearFlashTimer_ = 0;
	bool clearFlashStarted_ = false;

	// 粒子演出
	static constexpr int kClearParticleCount = 32;

	std::unique_ptr<KamataEngine::Sprite> clearParticles_[kClearParticleCount];
	float particleVelocityX_[kClearParticleCount] = {};
	float particleVelocityY_[kClearParticleCount] = {};

	int clearParticleTimer_ = 0;
	bool clearParticlesStarted_ = false;
};