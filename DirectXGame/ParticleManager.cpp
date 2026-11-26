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
		particlePool_.push_back(new Particle());
	}
}

// 粒子の生成
void ParticleManager::AddParticle(const Vector3& position, const Vector3& velocity, const Vector3& acceleration, const Vector3& scale, const Vector3& rotationVelocity, int lifeTime) {

	// 再利用できるパーティクルがなければ生成しない
	if (particlePool_.empty()) {
		return;
	}

	// プールからパーティクルを取り出す
	Particle* particle = particlePool_.back();
	particlePool_.pop_back();

	// パーティクルを初期化
	particle->Initialize(model_, camera_, position, velocity, acceleration, scale, rotationVelocity, lifeTime, isDeathParticle_);

	// 使用中リストに追加
	particles_.push_back(particle);
}

// 更新
void ParticleManager::Update() {

	for (int i = 0; i < static_cast<int>(particles_.size());) {

		// 寿命が尽きたパーティクル
		if (!particles_[i]->Update()) {

			// プールに戻す
			particlePool_.push_back(particles_[i]);

			// 最後の要素と入れ替えて削除
			particles_[i] = particles_.back();
			particles_.pop_back();

		} else {
			i++;
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

	// 使用中のパーティクルを解放
	for (Particle* particle : particles_) {
		delete particle;
	}

	particles_.clear();

	// プール内のパーティクルを解放
	for (Particle* particle : particlePool_) {
		delete particle;
	}

	particlePool_.clear();
}