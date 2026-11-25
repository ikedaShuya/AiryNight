#include "Particle.h"
#include "Math.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

// 初期化
void Particle::Initialize(
    Model* model, Camera* camera, const Vector3& position, const Vector3& velocity, const Vector3& acceleration, const Vector3& scale, const Vector3& rotationVelocity, int lifeTime,
    bool isDeathParticle) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	worldTransform_.rotation_.z = std::atan2(velocity.y, velocity.x);

	initialScale_ = scale;

	velocity_ = velocity;
	acceleration_ = acceleration;
	rotationVelocity_ = rotationVelocity;

	lifeTime_ = lifeTime;
	timer_ = 0;

	isDeathParticle_ = isDeathParticle;

	WorldTransformUpdate(worldTransform_);
}

// 更新
bool Particle::Update() {

	// 寿命を確認
	if (timer_ >= lifeTime_) {
		return false;
	}

	// 速度に加速度を加える
	velocity_ += acceleration_;

	// 移動
	worldTransform_.translation_ += velocity_;

	// 回転
	worldTransform_.rotation_ += rotationVelocity_;

	// 寿命に合わせて縮小する
	float lifeRate = static_cast<float>(timer_) / static_cast<float>(lifeTime_);

	float scaleRate = 1.0f;

	if (lifeRate < 0.5f) {
		scaleRate = 1.0f - lifeRate * 0.4f;
	} else {
		float remainingRate = (1.0f - lifeRate) / 0.5f;
		scaleRate = 0.8f * remainingRate * remainingRate;
	}

	// 敵死亡時の粒子だけキラキラさせる
	if (isDeathParticle_) {
		float sparkle = 0.75f + 0.25f * std::sin(static_cast<float>(timer_) * 0.7f);
		scaleRate *= sparkle;
	}

	worldTransform_.scale_ = initialScale_ * scaleRate;

	// 時間を進める
	timer_++;

	// ワールド行列を更新
	WorldTransformUpdate(worldTransform_);

	return true;
}

// 描画
void Particle::Draw() { model_->Draw(worldTransform_, *camera_); }