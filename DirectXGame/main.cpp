#include "ClearScene.h"
#include "Fade.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
ClearScene* clearScene = nullptr;
GameOverScene* gameOverScene = nullptr;

Fade fade;

// シーンの表示時間（秒）
const float kTitleDisplayTime = 1.5f;
const float kGameDisplayTime = 2.0f;

bool isChangingScene = false;
bool sceneChanged = false;

// シーン（型）
enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kClear,
	kGameOver,
};

// 現在シーン（型）
Scene scene = Scene::kUnknown;

static bool IsSceneFinished() {
	switch (scene) {
	case Scene::kTitle:
		return titleScene->IsFinished();

	case Scene::kGame:
		return gameScene->IsFinished();

	case Scene::kClear:
		return clearScene->IsFinished();

	case Scene::kGameOver:
		return gameOverScene->IsFinished();

	default:
		return false;
	}
}

static void ChangeSceneNow() {
	switch (scene) {
	case Scene::kTitle:
		scene = Scene::kGame;

		delete titleScene;
		titleScene = nullptr;

		gameScene = new GameScene;
		gameScene->Initialize();
		break;

	case Scene::kGame:
		if (gameScene->GetResult() == GameResult::Clear) {
			scene = Scene::kClear;

			delete gameScene;
			gameScene = nullptr;

			clearScene = new ClearScene;
			clearScene->Initialize();
		} else if (gameScene->GetResult() == GameResult::GameOver) {
			scene = Scene::kGameOver;

			delete gameScene;
			gameScene = nullptr;

			gameOverScene = new GameOverScene;
			gameOverScene->Initialize();
		}
		break;

	case Scene::kClear:
		scene = Scene::kTitle;

		delete clearScene;
		clearScene = nullptr;

		titleScene = new TitleScene;
		titleScene->Initialize();
		break;

	case Scene::kGameOver:
		// ゲームオーバー画面を削除
		delete gameOverScene;
		gameOverScene = nullptr;

		// ゲームシーンを最初から作り直す
		scene = Scene::kGame;

		gameScene = new GameScene;
		gameScene->Initialize();
		break;

	default:
		break;
	}
}

static void ChangeScene() {
	// シーン切り替えを開始
	if (!isChangingScene && IsSceneFinished()) {
		isChangingScene = true;
		sceneChanged = false;
		fade.StartFadeOut();
	}

	// 画面が完全に黒くなったらシーンを切り替える
	if (isChangingScene && !sceneChanged && fade.IsFadeOutFinished()) {
		ChangeSceneNow();
		fade.StartFadeIn();
		sceneChanged = true;
	}

	// フェードインが終わったら通常状態に戻す
	if (isChangingScene && sceneChanged && fade.IsFadeInFinished()) {
		isChangingScene = false;
		sceneChanged = false;
	}
}

static void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	}
}

static void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE3C_04_イケダ_シュウヤ_エアリーナイト");
	fade.Initialize();

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		// フェード更新
		fade.Update();

		ChangeScene();
		// 現在シーン更新
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		// フェード描画
		fade.Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;

	// エンジンの終了処理
	Finalize();

	return 0;
}