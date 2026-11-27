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
	void OnUpdate() override;

	/// <summary>
	/// 描画
	/// </summary>
	void OnDraw() override;

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
	std::unique_ptr<KamataEngine::Sprite> defeatedCountSprites_[2];

	// 撃破数の数字
	int defeatedCountDigits_[2] = {};

	// リザルト項目名の画像
	std::unique_ptr<KamataEngine::Sprite> resultStageSprite_;
	std::unique_ptr<KamataEngine::Sprite> resultTimeSprite_;
	std::unique_ptr<KamataEngine::Sprite> resultDefeatedSprite_;
	std::unique_ptr<KamataEngine::Sprite> resultPanelSprite_;

	std::unique_ptr<KamataEngine::Sprite> stageNumberSprite_;
	std::unique_ptr<KamataEngine::Sprite> timeSprites_[4];
	std::unique_ptr<KamataEngine::Sprite> timeColonSprite_;
	std::unique_ptr<KamataEngine::Sprite> timeColonDot2Sprite_;

	// 時間の数字
	int timeDigits_[4] = {};

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformTitle_;

	std::unique_ptr<KamataEngine::Model> modelTitle_;
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<KamataEngine::Model> modelSkyDome_;
	std::unique_ptr<KamataEngine::Sprite> pressSpaceSprite_;

	// クリア演出
	ClearSceneEffect clearSceneEffect_;

	// エフェクトの基底クラスのポインタ
	Effect* effect_ = nullptr;

	// 現在の状態
	std::unique_ptr<ClearSceneState> state_;

	// 次の状態
	std::unique_ptr<ClearSceneState> nextState_;
};