#pragma once

#include "Particle.h"
#include <vector>
#include <memory>

class ParticleManager {
public:
	// パーティクルの最大保持数
	static constexpr int kPoolSize = 256;

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, bool isDeathParticle = false);

	// 粒子の生成
	void AddParticle(
	    const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, const KamataEngine::Vector3& acceleration, const KamataEngine::Vector3& scale,
	    const KamataEngine::Vector3& rotationVelocity, int lifeTime);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	std::vector<std::unique_ptr<Particle>> particles_;
	std::vector<std::unique_ptr<Particle>> particlePool_;

	bool isDeathParticle_ = false;
};