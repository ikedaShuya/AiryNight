#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "Player.h"

#include "Enemy.h"
#include "MapChipField.h"
#include "SkyDome.h"

enum class GameResult { None, Clear, GameOver };

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public:
	// 初期化
	void Initialize(int stageNumber = 1);

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	GameResult GetResult() const { return result_; }

	void GenerateBlocks();

	// 敵を配置データから生成する
	void GenerateEnemies();

	// ステージごとの敵配置を設定する
	void SetEnemyPositionsForStage();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

private:
	// 終了フラグ
	bool finished_ = false;

#ifdef USE_IMGUI

	// マップエディタで配置するチップ
	int editMapType_ = 1; // 初期状態はブロック

	// エディタの編集モード
	int editorMode_ = 0; // 0: マップ編集、1: 敵配置、2: プレイヤー配置

	// マップ編集中かどうか
	bool isMapEditing_ = true;

	// マップエディタの拡大・縮小
	float mapCellSize_ = 8.0f;

	// 保存完了メッセージ
	int saveMessageTimer_ = 0;

	// 範囲選択モード
	bool isRangeSelectMode_ = false;
	bool isSelectingMapRange_ = false;

	// 範囲選択の開始位置と終了位置
	int rangeStartX_ = 0;
	int rangeStartY_ = 0;
	int rangeEndX_ = 0;
	int rangeEndY_ = 0;

#endif

	// 敵の配置位置
	struct EnemyMapPosition {
		int x;
		int y;
	};

	std::vector<EnemyMapPosition> enemyMapPositions_;

	// プレイヤーの配置位置
	int playerMapX_ = 1;
	int playerMapY_ = 12;

	// ステージごとのプレイヤー配置を読み込む
	void LoadPlayerPositionForStage();

	// カメラ
	KamataEngine::Camera camera_;

	// ブロックモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;

	// ゴールの旗
	KamataEngine::WorldTransform worldTransformGoalPole_;
	KamataEngine::WorldTransform worldTransformGoalFlag_;
	KamataEngine::Vector3 goalPosition_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelPlayer_ = nullptr;

	std::list<Enemy*> enemies_;

	KamataEngine::Model* modelEnemy_ = nullptr;

	CameraController* CController_ = nullptr;

	KamataEngine::Model* swordModel_ = nullptr;

	KamataEngine::Sprite* hpBarBg_ = nullptr;
	KamataEngine::Sprite* hpBarFg_ = nullptr;

	uint32_t hpBarBgTex_ = 0;
	uint32_t hpBarFgTex_ = 0;

	SkyDome* skyDome_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* modelSkyDome_ = nullptr;

	// 現在のステージ番号
	int stageNumber_ = 1;

	GameResult result_ = GameResult::None;

	// ゲームオーバー演出
	bool isGameOverPlaying_ = false;
	int gameOverTimer_ = 0;
	static constexpr int kGameOverFrame = 90;

	static constexpr int kGameOverFlashFrame = 10;

	// ゲームオーバー開始時のカメラ画角
	float gameOverStartFovY_ = 0.0f;

	// カメラズームにかける時間
	static constexpr int kGameOverZoomFrame = 30;

	// 画面を暗くするスプライト
	KamataEngine::Sprite* gameOverFadeSprite_ = nullptr;

	KamataEngine::Sprite* aSprite_ = nullptr;

	// ゲーム開始演出
	int startTimer_ = 0;

	// ステージ確認1秒 + カウントダウン3秒 + GO! 0.5秒
	static constexpr int kStagePreviewFrame = 60;
	static constexpr int kCountdownFrame = 180;
	static constexpr int kGoFrame = 30;
	static constexpr int kGameStartFrame = kStagePreviewFrame + kCountdownFrame + kGoFrame;

	// カウントダウン表示用
	KamataEngine::Sprite* countdown3Sprite_ = nullptr;
	KamataEngine::Sprite* countdown2Sprite_ = nullptr;
	KamataEngine::Sprite* countdown1Sprite_ = nullptr;
	KamataEngine::Sprite* countdownGoSprite_ = nullptr;

	int countdownNumber_ = 0;

	// ゴールの旗の配置位置
	int flagMapX_ = 97;
	int flagMapY_ = 12;

	// ステージごとの旗配置を読み込む
	void LoadFlagPositionForStage();
};