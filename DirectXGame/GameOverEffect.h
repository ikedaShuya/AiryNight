#pragma once

#include "KamataEngine.h"
#include "Effect.h"
#include "GameOverEffectState.h"
#include <memory>

class GameOverEffect : public Effect {
public:
	// 初期化
	void Initialize() override;

	// ゲームオーバー演出開始
	void Start(const KamataEngine::Camera& camera);

	// 更新
	bool Update(KamataEngine::Camera& camera);

	// 描画
	void Draw() override;

	// 演出中かどうか
	bool IsPlaying() const { return isPlaying_; }

	// 終了
	~GameOverEffect() override;

private:

	friend class GameOverEffectFlashState;
	friend class GameOverEffectFadeState;

	void ChangeState(std::unique_ptr<GameOverEffectState> newState);

	std::unique_ptr<GameOverEffectState> state_;
	std::unique_ptr<GameOverEffectState> nextState_;

	// 演出中かどうか
	bool isPlaying_ = false;

	// 演出タイマー
	int timer_ = 0;

	// 演出時間
	static constexpr int kGameOverFrame = 90;

	// 赤い衝撃演出の時間
	static constexpr int kFlashFrame = 10;

	// カメラズームの時間
	static constexpr int kZoomFrame = 30;

	// 開始時のカメラ画角
	float startFovY_ = 0.0f;

	// 暗転用スプライト
	KamataEngine::Sprite* fadeSprite_ = nullptr;
};