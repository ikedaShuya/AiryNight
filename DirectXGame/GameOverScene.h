#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"
#include "Scene.h"
#include <memory>

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

	bool IsFinished() const override { return finished_; }

private:

	// 終了フラグ
	bool finished_ = false;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformTitle_;

	std::unique_ptr<KamataEngine::Model> modelTitle_;

	std::unique_ptr<SkyDome> skyDome_;

	// 3Dモデル
	std::unique_ptr<KamataEngine::Model> modelSkyDome_;

	// ゲームオーバー画面の文字
	std::unique_ptr<KamataEngine::Sprite> retrySprite_;
	std::unique_ptr<KamataEngine::Sprite> hintSprite_;

	float titleAnimationTimer_ = 0.0f;
};