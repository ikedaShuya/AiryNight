#pragma once

#include "ClearScene.h"
#include "Fade.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Scene.h"

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
	enum class SceneType {
		kUnknown = 0,
		kTitle,
		kGame,
		kClear,
		kGameOver,
	};

	// 現在のシーンを管理する基底クラスのポインタ
	Scene* currentScene_ = nullptr;

	// 現在のシーン
	SceneType scene_ = SceneType::kUnknown;

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