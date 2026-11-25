#pragma once

#include "KamataEngine.h"

class Particle {
public:
	// 初期化
	void Initialize(
	    KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, const KamataEngine::Vector3& acceleration,
	    const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotationVelocity, int lifeTime, bool isDeathParticle = false);

	// 更新
	bool Update();

	// 描画
	void Draw();

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	// 初期サイズ
	KamataEngine::Vector3 initialScale_{};

	// 移動
	KamataEngine::Vector3 velocity_{};
	KamataEngine::Vector3 acceleration_{};

	// 回転
	KamataEngine::Vector3 rotationVelocity_{};

	// 寿命
	int lifeTime_ = 0;
	int timer_ = 0;

	// 撃破時の粒子かどうか
	bool isDeathParticle_ = false;
};