#pragma once

#include "KamataEngine.h"
#include "Effect.h"

class GameClearEffect : public Effect {
public:
	// 初期化
	void Initialize() override;

	// 演出開始
	void Start();

	// 更新
	bool Update();

	// 描画
	void Draw() override;

	// 演出中かどうか
	bool IsPlaying() const { return isPlaying_; }

	// 終了
	~GameClearEffect() override;

private:
	// 演出中かどうか
	bool isPlaying_ = false;

	// 演出タイマー
	int timer_ = 0;

	// 演出時間
	static constexpr int kGameClearFrame = 120;

	// フラッシュ用スプライト
	KamataEngine::Sprite* flashSprite_ = nullptr;
};