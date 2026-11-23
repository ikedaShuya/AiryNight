#pragma once
#include "KamataEngine.h"

class ClearSceneEffect {
public:
	void Initialize();

	float Update(float finalTitleScale);

	void Draw();

	bool IsFlashFinished() const { return clearFlashTimer_ >= 90; }

	~ClearSceneEffect();

private:
	// CLEAR文字の登場演出
	int clearTitleTimer_ = 0;
	static constexpr int kClearTitleAnimationFrame = 30;

	// フラッシュ演出
	KamataEngine::Sprite* clearFlashSprite_ = nullptr;
	int clearFlashTimer_ = 0;
	bool clearFlashStarted_ = false;

	// 粒子演出
	static constexpr int kClearParticleCount = 32;

	KamataEngine::Sprite* clearParticles_[kClearParticleCount] = {};
	float particleVelocityX_[kClearParticleCount] = {};
	float particleVelocityY_[kClearParticleCount] = {};

	int clearParticleTimer_ = 0;
	bool clearParticlesStarted_ = false;
};