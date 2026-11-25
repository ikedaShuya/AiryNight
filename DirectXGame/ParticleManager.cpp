#include "ParticleManager.h"

using namespace KamataEngine;

// 初期化
void ParticleManager::Initialize(Model* model, Camera* camera, bool isDeathParticle) {
	model_ = model;
	camera_ = camera;
	isDeathParticle_ = isDeathParticle;
}

// 粒子の生成
void ParticleManager::AddParticle(const Vector3& position, const Vector3& velocity, const Vector3& acceleration, const Vector3& scale, const Vector3& rotationVelocity, int lifeTime) {

	Particle* particle = new Particle;

	particle->Initialize(model_, camera_, position, velocity, acceleration, scale, rotationVelocity, lifeTime, isDeathParticle_);

	particles_.push_back(particle);
}

// 更新
void ParticleManager::Update() {

	for (auto it = particles_.begin(); it != particles_.end();) {

		if (!(*it)->Update()) {
			delete *it;
			it = particles_.erase(it);
		} else {
			++it;
		}
	}
}

// 描画
void ParticleManager::Draw() {

	for (Particle* particle : particles_) {
		particle->Draw();
	}
}

// デストラクタ
ParticleManager::~ParticleManager() {

	for (Particle* particle : particles_) {
		delete particle;
	}

	particles_.clear();
}