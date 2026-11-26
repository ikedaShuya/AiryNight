#pragma once

#include "Fade.h"
#include "SceneFactory.h"

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
	// シーン生成
	AiryNightSceneFactory sceneFactory_;

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