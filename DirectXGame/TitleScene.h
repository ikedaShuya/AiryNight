#pragma once
#include "KamataEngine.h"
#include "SkyDome.h"
#include "Scene.h"
#include "TitleSceneState.h"
#include <memory>

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene {
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
	
	// デスフラグのgetter
	bool IsFinished() const override { return finished_; }

private:

	friend class TitleSceneIdleState;
	friend class TitleSceneStartingState;

	// 状態変更
	void ChangeState(std::unique_ptr<TitleSceneState> newState);

	// 終了フラグ
	bool finished_ = false;

	// ビュープロジェクション
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransformTitle_;

	float skyRotation_ = 0.0f;

	float pressSpaceTimer_ = 0.0f;

	float titleTimer_ = 0.0f;

	float cameraTimer_ = 0.0f;

	float startTimer_ = 0.0f;
	bool isStarting_ = false;
		
	std::unique_ptr<KamataEngine::Model> modelTitle_;

	std::unique_ptr<SkyDome> skyDome_;

	// 3Dモデル
	std::unique_ptr<KamataEngine::Model> modelSkyDome_;

	// PRESS SPACE
	std::unique_ptr<KamataEngine::Sprite> pressSpaceSprite_;

	// 現在の状態
	std::unique_ptr<TitleSceneState> state_;

	// 次の状態
	std::unique_ptr<TitleSceneState> nextState_;
};