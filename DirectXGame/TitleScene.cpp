#include "TitleScene.h"

#include "Math.h"

using namespace KamataEngine;

void TitleScene::Initialize() {

	modelTitle_ = Model::CreateFromOBJ("title", true);

	// カメラ初期化
	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {1.5f, 1.5f, 1.5f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);

	// PRESS SPACE
	uint32_t textureHandle = TextureManager::Load("pressSpace.png");
	pressSpaceSprite_ = Sprite::Create(textureHandle, {384.0f, 560.0f});
}

void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isStarting_ = true;
	}

	titleTimer_ += 0.03f;

	// タイトルを上下に動かす
	worldTransformTitle_.translation_.y = sin(titleTimer_) * 0.3f;

	// タイトルを少し回転させる
	worldTransformTitle_.rotation_.y += 0.003f;

	// カメラを前後・左右に動かす
	cameraTimer_ += 0.02f;

	camera_.translation_.z = -10.0f + sin(cameraTimer_) * 2.0f;
	camera_.translation_.x = sin(cameraTimer_ * 0.5f) * 0.5f;

	// ゲーム開始演出
	if (isStarting_) {
		startTimer_ += 0.02f;

		camera_.translation_.z += startTimer_ * 0.1f;

		if (startTimer_ >= 1.5f) {
			finished_ = true;
		}
	}

	camera_.UpdateMatrix();

	// タイトル画面だけ空をゆっくり回転
	skyRotation_ += 0.001f;
	skyDome_->SetRotation(skyRotation_);

	// PRESS SPACE用タイマー
	pressSpaceTimer_ += 1.0f;

	if (pressSpaceTimer_ >= 120.0f) {
		pressSpaceTimer_ = 0.0f;
	}

	skyDome_->Update();

	WorldTransformUpdate(worldTransformTitle_);
}

void TitleScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	skyDome_->Draw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	// 3Dモデル描画後処理
	Model::PostDraw();

	// 2Dスプライト描画前処理
	Sprite::PreDraw();	

	if (pressSpaceTimer_ < 60.0f) {
		pressSpaceSprite_->Draw();
	}

	// 2Dスプライト描画後処理
	Sprite::PostDraw();
}

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
	delete pressSpaceSprite_;
}