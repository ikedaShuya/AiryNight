#include "Goal.h"
#include "Math.h"

using namespace KamataEngine;

void Goal::Initialize(const Vector3& position) {
	position_ = position;

	worldTransformPole_.Initialize();
	worldTransformPole_.scale_ = {0.15f, 4.0f, 0.15f};

	worldTransformFlag_.Initialize();
	worldTransformFlag_.scale_ = {0.8f, 0.5f, 0.1f};

	SetPosition(position);
}

void Goal::SetPosition(const Vector3& position) {
	position_ = position;

	worldTransformPole_.translation_ = {position_.x, position_.y + 1.0f, position_.z};

	worldTransformFlag_.translation_ = {position_.x + 0.4f, position_.y + 2.8f, position_.z};

	WorldTransformUpdate(worldTransformPole_);
	WorldTransformUpdate(worldTransformFlag_);
}

void Goal::Draw(Model* model, const Camera& camera) {
	model->Draw(worldTransformPole_, camera);
	model->Draw(worldTransformFlag_, camera);
}

bool Goal::IsPlayerInGoalRange(const Vector3& playerPosition) const {

	float distanceX = playerPosition.x - position_.x;
	float distanceY = playerPosition.y - position_.y;

	return distanceX > -1.0f && distanceX < 1.0f && distanceY > -1.0f && distanceY < 3.0f;
}