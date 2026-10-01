#pragma once
#include "KamataEngine.h"
#include "SkyDome.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
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

	~TitleScene();
	
	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	// 終了フラグ
	bool finished_ = false;

	// ビュープロジェクション
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransformTitle_;

	float skyRotation_ = 0.0f;

	float pressSpaceTimer_ = 0.0f;

	KamataEngine::Model* modelTitle_ = nullptr;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;
};