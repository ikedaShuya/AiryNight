#pragma once
#include "MapEditor.h"
#include "StagePlacementData.h"
#include "CameraController.h"
#include "KamataEngine.h"
#include "Player.h"

#include "Enemy.h"
#include "MapChipField.h"
#include "SkyDome.h"
#include "GameOverEffect.h"
#include "GameClearEffect.h"
#include "GameStartEffect.h"
#include "HpBar.h"
#include "Goal.h"
#include "StageDataLoader.h"
#include "StageMapLoader.h"
#include "StagePlacementDataSaver.h"
#include "BlockManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"

#ifdef USE_IMGUI
#include "MapEditorController.h"
#endif

#include "Scene.h"
#include "ParticleManager.h"

#include "GameSceneState.h"
#include <memory>

enum class GameResult { None, Clear, GameOver };

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Scene {
public:
	// 初期化
	void Initialize(int stageNumber = 1);

	// 更新
	void OnUpdate() override;

	// 描画
	void OnDraw() override;

	~GameScene() override;

	// デスフラグのgetter
	bool IsFinished() const override { return finished_; }

	GameResult GetResult() const { return result_; }

	int GetPlayTimer() const { return playTimer_; }

	// 敵の撃破数を取得
	int GetDefeatedEnemyCount() const { return defeatedEnemyCount_; }

	// 各ゲームシーン状態から呼び出す処理
	void UpdateStartState();
	void UpdatePlayingState();
	void UpdateGameOverState();
	void UpdateClearState();

private:

	// ============================
	// 内部処理
	// ============================

	// 状態変更
	void ChangeState(std::unique_ptr<GameSceneState> newState);

	// ブロック生成
	void GenerateBlocks();

	// 敵生成
	void GenerateEnemies();

	// 終了フラグ
	bool finished_ = false;

#ifdef USE_IMGUI

	// マップエディタ
	MapEditor mapEditor_;

	MapEditorController mapEditorController_;

#endif

	// ステージの配置データ
	StagePlacementData stagePlacementData_;

	StagePlacementDataSaver stagePlacementDataSaver_;
	StageDataLoader stageDataLoader_;
	StageMapLoader stageMapLoader_;

	Goal goal_;

	BlockManager blockManager_;

	EnemyManager enemyManager_;

	CollisionManager collisionManager_;

	// パーティクル管理
	ParticleManager particleManager_;
	ParticleManager deathParticleManager_;

	// カメラ
	KamataEngine::Camera camera_;

	// ブロックモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;	

	KamataEngine::Model* modelParticle_ = nullptr;

	// マップチップフィールド
	std::unique_ptr<MapChipField> mapChipField_;

	// 自キャラ
	std::unique_ptr<Player> player_;

	// 3Dモデルデータ
	KamataEngine::Model* modelPlayer_ = nullptr;

	std::unique_ptr<CameraController> CController_;

	KamataEngine::Model* swordModel_ = nullptr;

	std::unique_ptr<SkyDome> skyDome_;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	// 現在のステージ番号
	int stageNumber_ = 1;

	GameResult result_ = GameResult::None;

	// ゲームオーバー演出
	GameOverEffect gameOverEffect_;

	// ゲームクリア演出
	GameClearEffect gameClearEffect_;

	// ゲーム開始演出
	GameStartEffect gameStartEffect_;	

	// エフェクトを基底クラスのポインタで管理
	Effect* effects_[3] = {};

	// HPバー関連の処理
	HpBar hpBar_;

	// ゲームプレイ時間
	int playTimer_ = 0;

	// 敵の撃破数
	int defeatedEnemyCount_ = 0;

	KamataEngine::Sprite* aSprite_ = nullptr;

	// 現在のゲームシーンの状態
	std::unique_ptr<GameSceneState> state_;

	// 次に切り替える状態
	std::unique_ptr<GameSceneState> nextState_;
};