#include "GameOverEffect.h"
#include "GameOverEffectState.h"
#include <memory>

using namespace KamataEngine;

// 赤い衝撃演出状態
void GameOverEffectFlashState::Update(GameOverEffect& effect) {
	float alpha = 0.55f * (1.0f - static_cast<float>(effect.timer_ - 1) / static_cast<float>(effect.kFlashFrame));

	effect.fadeSprite_->SetColor({1.0f, 0.0f, 0.0f, alpha});

	// 赤い演出が終わったら暗転状態へ
	if (effect.timer_ >= effect.kFlashFrame) {
		effect.ChangeState(std::make_unique<GameOverEffectFadeState>());
	}
}

// 黒い暗転演出状態
void GameOverEffectFadeState::Update(GameOverEffect& effect) {
	float alpha = static_cast<float>(effect.timer_ - effect.kFlashFrame) / static_cast<float>(effect.kGameOverFrame - effect.kFlashFrame);

	if (alpha > 1.0f) {
		alpha = 1.0f;
	}

	effect.fadeSprite_->SetColor({0.0f, 0.0f, 0.0f, alpha});
}

// 状態変更
void GameOverEffect::ChangeState(std::unique_ptr<GameOverEffectState> newState) { nextState_ = std::move(newState); }

// 初期化
void GameOverEffect::Initialize() {
	fadeSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});

	fadeSprite_->SetSize({1280.0f, 720.0f});
	fadeSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

// 演出開始
void GameOverEffect::Start(const Camera& camera) {
	isPlaying_ = true;
	timer_ = 0;
	startFovY_ = camera.fovAngleY;

	state_ = std::make_unique<GameOverEffectFlashState>();
	nextState_.reset();
}

// 更新
bool GameOverEffect::Update(Camera& camera) {
	if (!isPlaying_) {
		return false;
	}

	timer_++;

	// カメラを30フレームかけてズームイン
	if (timer_ <= kZoomFrame) {
		float t = static_cast<float>(timer_) / static_cast<float>(kZoomFrame);
		float ease = t * t;

		camera.fovAngleY = startFovY_ * (1.0f - 0.35f * ease);
	}

	camera.UpdateMatrix();

	// 現在の状態を更新
	if (state_) {
		state_->Update(*this);
	}

	// 状態変更を反映
	if (nextState_) {
		state_ = std::move(nextState_);
	}

	// 演出終了
	if (timer_ >= kGameOverFrame) {
		isPlaying_ = false;
		return true;
	}

	return false;
}

// 描画
void GameOverEffect::Draw() {
	if (isPlaying_) {
		fadeSprite_->Draw();
	}
}

// 終了
GameOverEffect::~GameOverEffect() { delete fadeSprite_; }