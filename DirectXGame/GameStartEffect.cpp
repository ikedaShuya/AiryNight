#include "GameStartEffect.h"
#include <memory>

using namespace KamataEngine;

// ステージ確認状態
void GameStartStagePreviewState::Update(GameStartEffect& effect) {
	effect.countdownNumber_ = 0;

	if (effect.timer_ >= effect.kStagePreviewFrame) {
		effect.ChangeState(std::make_unique<GameStartCountdownState>());
	}
}

// カウントダウン状態
void GameStartCountdownState::Update(GameStartEffect& effect) {
	int countdownTimer = effect.timer_ - effect.kStagePreviewFrame;

	if (countdownTimer < 60) {
		effect.countdownNumber_ = 3;
	} else if (countdownTimer < 120) {
		effect.countdownNumber_ = 2;
	} else if (countdownTimer < 180) {
		effect.countdownNumber_ = 1;
	}

	if (effect.timer_ >= effect.kStagePreviewFrame + effect.kCountdownFrame) {
		effect.ChangeState(std::make_unique<GameStartGoState>());
	}
}

// GO表示状態
void GameStartGoState::Update(GameStartEffect& effect) { effect.countdownNumber_ = 4; }

// 状態変更
void GameStartEffect::ChangeState(std::unique_ptr<GameStartEffectState> newState) { nextState_ = std::move(newState); }

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

	state_ = std::make_unique<GameStartStagePreviewState>();
}

// 更新
void GameStartEffect::Update() {

	if (timer_ < kGameStartFrame) {
		timer_++;
	}

	if (state_) {
		state_->Update(*this);
	}

	if (nextState_) {
		state_ = std::move(nextState_);
	}

	if (timer_ >= kGameStartFrame) {
		countdownNumber_ = 0;
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