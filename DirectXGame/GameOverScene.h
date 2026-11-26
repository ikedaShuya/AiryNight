#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"
#include "Scene.h"

class GameOverScene : public Scene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void OnUpdate() override;

	/// <summary>
	/// 描画
	/// </summary>
	void OnDraw() override;

	~GameOverScene() override;

	bool IsFinished() const override { return finished_; }

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