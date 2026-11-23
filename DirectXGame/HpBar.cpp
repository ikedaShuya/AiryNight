#include "HpBar.h"

using namespace KamataEngine;

void HpBar::Initialize() {
	backgroundTexture_ = TextureManager::Load("hp_bar_bg.png");
	foregroundTexture_ = TextureManager::Load("hp_bar_fg.png");

	background_ = Sprite::Create(backgroundTexture_, {70.0f, 10.0f});
	foreground_ = Sprite::Create(foregroundTexture_, {70.0f, 10.0f});

	background_->SetPosition({90.0f, 20.0f});
	foreground_->SetPosition({90.0f, 20.0f});

	background_->SetAnchorPoint({0.0f, 0.0f});
	foreground_->SetAnchorPoint({0.0f, 0.0f});
}

void HpBar::Update(int hp, int maxHp) {
	if (maxHp <= 0) {
		return;
	}

	float hpRate = static_cast<float>(hp) / static_cast<float>(maxHp);
	foreground_->SetSize({100.0f * hpRate, 10.0f});
}

void HpBar::Draw() {
	background_->Draw();
	foreground_->Draw();
}

HpBar::~HpBar() {
	delete background_;
	delete foreground_;
}