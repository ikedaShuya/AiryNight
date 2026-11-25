#include "TitleScene.h"

#include "Math.h"

using namespace KamataEngine;

// タイトル待機状態
void TitleSceneIdleState::Update(TitleScene& titleScene) {

	// Spaceキーでゲーム開始状態へ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		titleScene.ChangeState(std::make_unique<TitleSceneStartingState>());
		return;
	}
}

// ゲーム開始状態
void TitleSceneStartingState::Update(TitleScene& titleScene) {

	titleScene.startTimer_ += 0.02f;

	// カメラを前進させる
	titleScene.camera_.translation_.z += titleScene.startTimer_ * 0.1f;

	// 演出終了
	if (titleScene.startTimer_ >= 1.5f) {
		titleScene.finished_ = true;
	}
}

// 状態変更
void TitleScene::ChangeState(std::unique_ptr<TitleSceneState> newState) { nextState_ = std::move(newState); }

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

	// タイトル待機状態にする
	state_ = std::make_unique<TitleSceneIdleState>();
}

void TitleScene::Update() {

	// タイトルの上下移動・回転
	titleTimer_ += 0.03f;
	worldTransformTitle_.translation_.y = sin(titleTimer_) * 0.3f;
	worldTransformTitle_.rotation_.y += 0.003f;

	// カメラの基本的な移動
	cameraTimer_ += 0.02f;
	camera_.translation_.z = -10.0f + sin(cameraTimer_) * 2.0f;
	camera_.translation_.x = sin(cameraTimer_ * 0.5f) * 0.5f;

	// 現在の状態を更新
	if (state_) {
		state_->Update(*this);
	}

	// 状態を切り替える
	if (nextState_) {
		state_ = std::move(nextState_);
	}

	camera_.UpdateMatrix();

	// 空をゆっくり回転
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