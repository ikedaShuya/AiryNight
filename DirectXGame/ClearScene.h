#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "SkyDome.h"
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

	// クリア文字の登場演出
	int clearTitleTimer_ = 0;
	static constexpr int kClearTitleAnimationFrame = 30;

	// クリア時のフラッシュ演出
	KamataEngine::Sprite* clearFlashSprite_ = nullptr;
	int clearFlashTimer_ = 0;
	bool clearFlashStarted_ = false;

	// PRESS SPACEのスプライト
	KamataEngine::Sprite* pressSpaceSprite_ = nullptr;

	// CLEARモデルのサイズ
	float clearTitleScale_ = 5.0f;

	// ステージ文字
	float resultStagePosition_[2] = {400.0f, 100.0f};
	float resultStageSize_[2] = {160.0f, 40.0f};

	// ステージ番号
	float stageNumberPosition_[2] = {590.0f, 80.0f};
	float stageNumberSize_[2] = {100.0f, 100.0f};

	// クリアタイム文字
	float resultTimePosition_[2] = {540.0f, 390.0f};
	float resultTimeSize_[2] = {200.0f, 50.0f};

	// プレイ時間の数字
	float timePosition_[4][2] = {
	    {480.0f, 455.0f},
        {550.0f, 455.0f},
        {680.0f, 455.0f},
        {750.0f, 455.0f}
    };
	float timeSize_[2] = {60.0f, 80.0f};

	// コロン
	float colonPosition_[2] = {625.0f, 480.0f};
	float colonPosition2_[2] = {625.0f, 505.0f};
	float colonSize_[2] = {8.0f, 8.0f};

	// 敵撃破数の文字
	float resultDefeatedPosition_[2] = {480.0f, 545.0f};
	float resultDefeatedSize_[2] = {320.0f, 40.0f};

	// 敵撃破数の数字
	float defeatedPosition_[2][2] = {
	    {570.0f, 590.0f},
        {640.0f, 590.0f}
    };
	float defeatedSize_[2] = {50.0f, 50.0f};

	// PRESS SPACE
	float pressSpacePosition_[2] = {448.0f, 620.0f};
	float pressSpaceSize_[2] = {384.0f, 60.0f};

	// レイアウト保存・読み込み・初期化
	void SaveLayout();
	void LoadLayout();
	void ResetLayout();

	// CLEAR登場時の粒子
	static constexpr int kClearParticleCount = 32;

	KamataEngine::Sprite* clearParticles_[kClearParticleCount] = {};

	float particleVelocityX_[kClearParticleCount] = {};
	float particleVelocityY_[kClearParticleCount] = {};

	int clearParticleTimer_ = 0;
	bool clearParticlesStarted_ = false;
};