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
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	~TitleScene() override;
	
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
		
	KamataEngine::Model* modelTitle_ = nullptr;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	// PRESS SPACE
	KamataEngine::Sprite* pressSpaceSprite_ = nullptr;

	// 現在の状態
	std::unique_ptr<TitleSceneState> state_;

	// 次の状態
	std::unique_ptr<TitleSceneState> nextState_;
};