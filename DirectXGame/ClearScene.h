#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"
#include "ClearSceneLayout.h"
#include "ClearSceneEffect.h"
#include "Scene.h"
#include "Effect.h"
#include "ClearSceneState.h"
#include <memory>
#include <numbers>

class ClearScene : public Scene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int stageNumber, int playTimer, int defeatedEnemyCount);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	~ClearScene() override;

	bool IsFinished() const override { return finished_; }

private:

	friend class ClearSceneEffectState;
	friend class ClearSceneResultState;

	// 状態変更
	void ChangeState(std::unique_ptr<ClearSceneState> newState);

	// リザルト画面のレイアウト管理
	ClearSceneLayout layout_;

	// 終了フラグ
	bool finished_ = false;

	// ステージ番号
	int stageNumber_ = 1;

	// ゲームのプレイ時間
	int playTimer_ = 0;

	// 敵の撃破数
	int defeatedEnemyCount_ = 0;

	// 敵の撃破数表示用
	KamataEngine::Sprite* defeatedCountSprites_[2] = {};

	// 撃破数の数字
	int defeatedCountDigits_[2] = {};

	// リザルト項目名の画像
	KamataEngine::Sprite* resultStageSprite_ = nullptr;
	KamataEngine::Sprite* resultTimeSprite_ = nullptr;
	KamataEngine::Sprite* resultDefeatedSprite_ = nullptr;
	KamataEngine::Sprite* resultPanelSprite_ = nullptr;

	// ステージ番号の画像
	KamataEngine::Sprite* stageNumberSprite_ = nullptr;

	// プレイ時間表示用
	KamataEngine::Sprite* timeSprites_[4] = {};

	// 時間の数字
	int timeDigits_[4] = {};

	// コロン表示用
	KamataEngine::Sprite* timeColonSprite_ = nullptr;

	KamataEngine::Sprite* timeColonDot2Sprite_ = nullptr;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformTitle_;

	KamataEngine::Model* modelTitle_ = nullptr;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	// クリア演出
	ClearSceneEffect clearSceneEffect_;

	// エフェクトの基底クラスのポインタ
	Effect* effect_ = nullptr;

	// PRESS SPACEのスプライト
	KamataEngine::Sprite* pressSpaceSprite_ = nullptr;

	// 現在の状態
	std::unique_ptr<ClearSceneState> state_;

	// 次の状態
	std::unique_ptr<ClearSceneState> nextState_;
};