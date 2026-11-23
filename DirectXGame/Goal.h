#pragma once
#include "KamataEngine.h"

class Goal {
public:
	void Initialize(const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& position);
	void Draw(KamataEngine::Model* model, const KamataEngine::Camera& camera);

	const KamataEngine::Vector3& GetPosition() const { return position_; }

	// プレイヤーがゴール範囲内にいるか
	bool IsPlayerInGoalRange(const KamataEngine::Vector3& playerPosition) const;

private:
	KamataEngine::WorldTransform worldTransformPole_;
	KamataEngine::WorldTransform worldTransformFlag_;
	KamataEngine::Vector3 position_;
};