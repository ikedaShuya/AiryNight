#include "ParticleManager.h"

using namespace KamataEngine;

// 初期化
void ParticleManager::Initialize(Model* model, Camera* camera, bool isDeathParticle) {
	model_ = model;
	camera_ = camera;
	isDeathParticle_ = isDeathParticle;

	// 使用中のパーティクルを格納する領域を確保
	particles_.reserve(kPoolSize);

	// パーティクルをあらかじめ生成してプールに格納
	for (int i = 0; i < kPoolSize; i++) {
		particlePool_.push_back(std::make_unique<Particle>());
	}
}

// 粒子の生成
void ParticleManager::AddParticle(const Vector3& position, const Vector3& velocity, const Vector3& acceleration, const Vector3& scale, const Vector3& rotationVelocity, int lifeTime) {

	// 再利用できるパーティクルがなければ生成しない
	if (particlePool_.empty()) {
		return;
	}

	std::unique_ptr<Particle> particle = std::move(particlePool_.back());
	particlePool_.pop_back();

	particle->Initialize(model_, camera_, position, velocity, acceleration, scale, rotationVelocity, lifeTime, isDeathParticle_);

	particles_.push_back(std::move(particle));
}

// 更新
void ParticleManager::Update() {

	for (int i = 0; i < static_cast<int>(particles_.size());) {

		// 寿命が尽きたパーティクル
		if (!particles_[i]->Update()) {

			particlePool_.push_back(std::move(particles_[i]));

			if (i != static_cast<int>(particles_.size()) - 1) {
				particles_[i] = std::move(particles_.back());
			}

			particles_.pop_back();

		} else {
			i++;
		}
	}
}

// 描画
void ParticleManager::Draw() {

	for (const std::unique_ptr<Particle>& particle : particles_) {
		particle->Draw();
	}
}