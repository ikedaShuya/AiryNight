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
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	GameResult GetResult() const { return result_; }

	void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

private:
	// 終了フラグ
	bool finished_ = false;

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

	GameResult result_ = GameResult::None;	

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
};