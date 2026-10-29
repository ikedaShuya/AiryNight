#include "GameOverScene.h"
#include <cmath>

using namespace KamataEngine;

void GameOverScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("gameOver", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {5.0f, 5.0f, 5.0f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);

	// リトライ案内
	retrySprite_ = Sprite::Create(TextureManager::Load("gameover_retry.png"), {640.0f, 570.0f});

	// 攻略ヒント
	hintSprite_ = Sprite::Create(TextureManager::Load("gameover_hint.png"), {640.0f, 490.0f});

	// 画像の中心を基準にする
	retrySprite_->SetAnchorPoint({0.5f, 0.5f});
	hintSprite_->SetAnchorPoint({0.5f, 0.5f});
}

void GameOverScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	skyDome_->Update();

	// ゲームオーバータイトルをゆっくり脈動させる
	titleAnimationTimer_ += 1.0f / 60.0f;

	float scale = 5.0f + std::sin(titleAnimationTimer_ * 2.0f) * 0.15f;

	worldTransformTitle_.scale_ = {scale, scale, scale};

	WorldTransformUpdate(worldTransformTitle_);
}

void GameOverScene::Draw() {

	Model::PreDraw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	skyDome_->Draw();

	Model::PostDraw();

	 Sprite::PreDraw();

	//hintSprite_->Draw();
	retrySprite_->Draw();

	Sprite::PostDraw();
}

GameOverScene::~GameOverScene() { 
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
	delete retrySprite_;
	delete hintSprite_;
}