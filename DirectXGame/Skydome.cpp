#include "SkyDome.h"
#include "Math.h"

using namespace KamataEngine;

void SkyDome::Initialize(Model* model, Camera* camera) {

	// NULLポインタチェック
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}

void SkyDome::Update() {

	// ワールド変換行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
}

void SkyDome::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}

void SkyDome::SetRotation(float rotationY) { worldTransform_.rotation_.y = rotationY; }