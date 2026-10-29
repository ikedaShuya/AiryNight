#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"

class GameOverScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	~GameOverScene();

	bool IsFinished() const { return finished_; }

private:

	// 終了フラグ
	bool finished_ = false;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformTitle_;

	KamataEngine::Model* modelTitle_ = nullptr;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	// ゲームオーバー画面の文字
	KamataEngine::Sprite* retrySprite_ = nullptr;
	KamataEngine::Sprite* hintSprite_ = nullptr;

	float titleAnimationTimer_ = 0.0f;
};