#include "GameClearEffect.h"

using namespace KamataEngine;

// 初期化
void GameClearEffect::Initialize() {
	flashSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});
	flashSprite_->SetSize({1280.0f, 720.0f});
	flashSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});
}

// 演出開始
void GameClearEffect::Start() {
	isPlaying_ = true;
	timer_ = 0;
}

// 更新
bool GameClearEffect::Update() {
	if (!isPlaying_) {
		return false;
	}

	timer_++;

	if (timer_ >= kGameClearFrame) {
		isPlaying_ = false;
		return true;
	}

	return false;
}

// 描画
void GameClearEffect::Draw() {
	if (!isPlaying_) {
		return;
	}

	float alpha = 0.0f;

	if (timer_ < 10) {
		alpha = 1.0f - static_cast<float>(timer_) / 10.0f;
	}

	flashSprite_->SetColor({1.0f, 1.0f, 1.0f, alpha});
	flashSprite_->Draw();
}

// 終了
GameClearEffect::~GameClearEffect() { delete flashSprite_; }