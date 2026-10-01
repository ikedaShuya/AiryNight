#include "TitleScene.h"

#include "Math.h"

using namespace KamataEngine;

void TitleScene::Initialize() {

	modelTitle_ = Model::CreateFromOBJ("title", true);

	// カメラ初期化
	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);
}

void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	// タイトル画面だけ空をゆっくり回転
	skyRotation_ += 0.001f;
	skyDome_->SetRotation(skyRotation_);

	// PRESS SPACE用タイマー
	pressSpaceTimer_ += 1.0f;

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
}

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
}