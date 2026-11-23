#include "GameStartEffect.h"

using namespace KamataEngine;

// 初期化
void GameStartEffect::Initialize() {

	// カウントダウン画像を読み込む
	countdown3Sprite_ = Sprite::Create(TextureManager::Load("countdown_3.png"), {640.0f, 360.0f});
	countdown2Sprite_ = Sprite::Create(TextureManager::Load("countdown_2.png"), {640.0f, 360.0f});
	countdown1Sprite_ = Sprite::Create(TextureManager::Load("countdown_1.png"), {640.0f, 360.0f});
	countdownGoSprite_ = Sprite::Create(TextureManager::Load("countdown_go.png"), {640.0f, 360.0f});

	// 画像の中心を画面中央に合わせる
	countdown3Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdown2Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdown1Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdownGoSprite_->SetAnchorPoint({0.5f, 0.5f});
}

// 更新
void GameStartEffect::Update() {

	if (timer_ < kGameStartFrame) {
		timer_++;
	}

	// カウントダウンの数字を設定
	countdownNumber_ = 0;

	if (timer_ >= kStagePreviewFrame && timer_ < kStagePreviewFrame + 60) {
		countdownNumber_ = 3;
	} else if (timer_ >= kStagePreviewFrame + 60 && timer_ < kStagePreviewFrame + 120) {
		countdownNumber_ = 2;
	} else if (timer_ >= kStagePreviewFrame + 120 && timer_ < kStagePreviewFrame + 180) {
		countdownNumber_ = 1;
	} else if (timer_ >= kStagePreviewFrame + kCountdownFrame && timer_ < kGameStartFrame) {
		countdownNumber_ = 4;
	}
}

// 描画
void GameStartEffect::Draw() {

	if (countdownNumber_ == 3) {
		countdown3Sprite_->Draw();
	} else if (countdownNumber_ == 2) {
		countdown2Sprite_->Draw();
	} else if (countdownNumber_ == 1) {
		countdown1Sprite_->Draw();
	} else if (countdownNumber_ == 4) {
		countdownGoSprite_->Draw();
	}
}

// 終了
GameStartEffect::~GameStartEffect() {
	delete countdown3Sprite_;
	delete countdown2Sprite_;
	delete countdown1Sprite_;
	delete countdownGoSprite_;
}