#pragma once

#include "Particle.h"
#include <list>

class ParticleManager {
public:
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

	// デストラクタ
	~ParticleManager();

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	std::list<Particle*> particles_;

	bool isDeathParticle_ = false;
};