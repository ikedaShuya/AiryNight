#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"
#include "ClearSceneLayout.h"
#include "ClearSceneEffect.h"
#include <numbers>

class ClearScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int stageNumber, int playTimer, int defeatedEnemyCount);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	~ClearScene();

	bool IsFinished() const { return finished_; }

private:

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

	// PRESS SPACEのスプライト
	KamataEngine::Sprite* pressSpaceSprite_ = nullptr;
};