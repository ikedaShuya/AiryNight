#pragma once

#include "KamataEngine.h"
#include "Effect.h"
#include "GameStartEffectState.h"
#include <memory>

class GameStartEffect : public Effect {
public:
	// 初期化
	void Initialize() override;

	// 更新
	void Update();

	// 描画
	void Draw() override;

	// 演出が終了したか
	bool IsFinished() const { return timer_ >= kGameStartFrame; }

	// 演出中の表示番号
	int GetCountdownNumber() const { return countdownNumber_; }

	// 経過時間
	int GetTimer() const { return timer_; }

	~GameStartEffect() override;

private:

	friend class GameStartStagePreviewState;
	friend class GameStartCountdownState;
	friend class GameStartGoState;

	void ChangeState(std::unique_ptr<GameStartEffectState> newState);

	std::unique_ptr<GameStartEffectState> state_;
	std::unique_ptr<GameStartEffectState> nextState_;

	// 演出タイマー
	int timer_ = 0;

	// ステージ確認1秒 + カウントダウン3秒 + GO! 0.5秒
	static constexpr int kStagePreviewFrame = 60;
	static constexpr int kCountdownFrame = 180;
	static constexpr int kGoFrame = 30;
	static constexpr int kGameStartFrame = kStagePreviewFrame + kCountdownFrame + kGoFrame;

	// カウントダウン表示番号
	int countdownNumber_ = 0;

	// カウントダウン画像
	KamataEngine::Sprite* countdown3Sprite_ = nullptr;
	KamataEngine::Sprite* countdown2Sprite_ = nullptr;
	KamataEngine::Sprite* countdown1Sprite_ = nullptr;
	KamataEngine::Sprite* countdownGoSprite_ = nullptr;
};