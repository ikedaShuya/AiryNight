#include "SceneManager.h"

void SceneManager::Initialize() {

	// フェードの初期化
	fade_.Initialize();

	// 最初のシーンをタイトルにする
	scene_ = Scene::kTitle;
	titleScene_ = new TitleScene;
	titleScene_->Initialize();
}

void SceneManager::Update() {

	// フェード更新
	fade_.Update();

	// シーン切り替え
	ChangeScene();

	// 現在のシーン更新
	UpdateScene();
}

void SceneManager::Draw() {

	// 現在のシーン描画
	DrawScene();

	// フェード描画
	fade_.Draw();
}

void SceneManager::Finalize() {

	// シーンの解放
	delete titleScene_;
	titleScene_ = nullptr;

	delete gameScene_;
	gameScene_ = nullptr;

	delete clearScene_;
	clearScene_ = nullptr;

	delete gameOverScene_;
	gameOverScene_ = nullptr;
}

bool SceneManager::IsSceneFinished() {

	switch (scene_) {
	case Scene::kTitle:
		return titleScene_->IsFinished();

	case Scene::kGame:
		return gameScene_->IsFinished();

	case Scene::kClear:
		return clearScene_->IsFinished();

	case Scene::kGameOver:
		return gameOverScene_->IsFinished();

	default:
		return false;
	}
}

void SceneManager::ChangeSceneNow() {

	switch (scene_) {
	case Scene::kTitle:

		currentStage_ = 1;

		scene_ = Scene::kGame;

		delete titleScene_;
		titleScene_ = nullptr;

		gameScene_ = new GameScene;
		gameScene_->Initialize(currentStage_);
		break;

	case Scene::kGame:

		if (gameScene_->GetResult() == GameResult::Clear) {

			// ゲームのプレイ時間を取得
			int clearPlayTimer = gameScene_->GetPlayTimer();

			// 敵の撃破数を取得
			int clearDefeatedEnemyCount = gameScene_->GetDefeatedEnemyCount();

			// ゲームシーンを削除
			delete gameScene_;
			gameScene_ = nullptr;

			// クリアリザルト画面へ
			scene_ = Scene::kClear;
			clearScene_ = new ClearScene;
			clearScene_->Initialize(currentStage_, clearPlayTimer, clearDefeatedEnemyCount);

		} else if (gameScene_->GetResult() == GameResult::GameOver) {

			scene_ = Scene::kGameOver;

			delete gameScene_;
			gameScene_ = nullptr;

			gameOverScene_ = new GameOverScene;
			gameOverScene_->Initialize();
		}
		break;

	case Scene::kClear:

		// リザルト画面を削除
		delete clearScene_;
		clearScene_ = nullptr;

		// クリア後はタイトルへ戻る
		currentStage_ = 1;

		scene_ = Scene::kTitle;
		titleScene_ = new TitleScene;
		titleScene_->Initialize();
		break;

	case Scene::kGameOver:

		delete gameOverScene_;
		gameOverScene_ = nullptr;

		// ゲームオーバーになったステージから再挑戦
		scene_ = Scene::kGame;
		gameScene_ = new GameScene;
		gameScene_->Initialize(currentStage_);
		break;

	default:
		break;
	}
}

void SceneManager::ChangeScene() {

	// シーン切り替えを開始
	if (!isChangingScene_ && IsSceneFinished()) {
		isChangingScene_ = true;
		sceneChanged_ = false;
		fade_.StartFadeOut();
	}

	// 画面が完全に黒くなったらシーンを切り替える
	if (isChangingScene_ && !sceneChanged_ && fade_.IsFadeOutFinished()) {
		ChangeSceneNow();
		fade_.StartFadeIn();
		sceneChanged_ = true;
	}

	// フェードインが終わったら通常状態に戻す
	if (isChangingScene_ && sceneChanged_ && fade_.IsFadeInFinished()) {
		isChangingScene_ = false;
		sceneChanged_ = false;
	}
}

void SceneManager::UpdateScene() {

	switch (scene_) {
	case Scene::kTitle:
		titleScene_->Update();
		break;

	case Scene::kGame:
		gameScene_->Update();
		break;

	case Scene::kClear:
		clearScene_->Update();
		break;

	case Scene::kGameOver:
		gameOverScene_->Update();
		break;
	}
}

void SceneManager::DrawScene() {

	switch (scene_) {
	case Scene::kTitle:
		titleScene_->Draw();
		break;

	case Scene::kGame:
		gameScene_->Draw();
		break;

	case Scene::kClear:
		clearScene_->Draw();
		break;

	case Scene::kGameOver:
		gameOverScene_->Draw();
		break;
	}
}