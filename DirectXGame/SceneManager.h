#pragma once

#include "ClearScene.h"
#include "Fade.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

class SceneManager {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了処理
	void Finalize();

private:
	// シーンの種類
	enum class Scene {
		kUnknown = 0,
		kTitle,
		kGame,
		kClear,
		kGameOver,
	};

	// シーン管理用ポインタ
	GameScene* gameScene_ = nullptr;
	TitleScene* titleScene_ = nullptr;
	ClearScene* clearScene_ = nullptr;
	GameOverScene* gameOverScene_ = nullptr;

	// 現在のシーン
	Scene scene_ = Scene::kUnknown;

	// 現在のステージ
	int currentStage_ = 1;

	// フェード
	Fade fade_;

	// シーン切り替え状態
	bool isChangingScene_ = false;
	bool sceneChanged_ = false;

	// シーン管理処理
	bool IsSceneFinished();
	void ChangeScene();
	void ChangeSceneNow();
	void UpdateScene();
	void DrawScene();
};