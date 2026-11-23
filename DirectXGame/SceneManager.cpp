#include "SceneManager.h"

void SceneManager::Initialize() {

	// フェードの初期化
	fade_.Initialize();

	// 最初のシーンをタイトルにする
	scene_ = SceneType::kTitle;

	TitleScene* titleScene = new TitleScene;
	titleScene->Initialize();

	currentScene_ = titleScene;
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

	// 現在のシーンを解放
	delete currentScene_;
	currentScene_ = nullptr;
}

bool SceneManager::IsSceneFinished() {

	if (currentScene_ == nullptr) {
		return false;
	}

	return currentScene_->IsFinished();
}

void SceneManager::ChangeSceneNow() {

	switch (scene_) {
	case SceneType::kTitle:

		currentStage_ = 1;

		// タイトルシーンを削除
		delete currentScene_;
		currentScene_ = nullptr;

		// ゲームシーンを生成
		scene_ = SceneType::kGame;

		{
			GameScene* gameScene = new GameScene;
			gameScene->Initialize(currentStage_);
			currentScene_ = gameScene;
		}
		break;

	case SceneType::kGame: {
		// ゲームシーン固有の情報を取得
		GameScene* gameScene = static_cast<GameScene*>(currentScene_);

		if (gameScene->GetResult() == GameResult::Clear) {

			// ゲームのプレイ時間を取得
			int clearPlayTimer = gameScene->GetPlayTimer();

			// 敵の撃破数を取得
			int clearDefeatedEnemyCount = gameScene->GetDefeatedEnemyCount();

			// ゲームシーンを削除
			delete currentScene_;
			currentScene_ = nullptr;

			// クリアリザルト画面へ
			scene_ = SceneType::kClear;

			ClearScene* clearScene = new ClearScene;
			clearScene->Initialize(currentStage_, clearPlayTimer, clearDefeatedEnemyCount);
			currentScene_ = clearScene;

		} else if (gameScene->GetResult() == GameResult::GameOver) {

			// ゲームシーンを削除
			delete currentScene_;
			currentScene_ = nullptr;

			// ゲームオーバー画面へ
			scene_ = SceneType::kGameOver;

			GameOverScene* gameOverScene = new GameOverScene;
			gameOverScene->Initialize();
			currentScene_ = gameOverScene;
		}
		break;
	}

	case SceneType::kClear:

		// リザルト画面を削除
		delete currentScene_;
		currentScene_ = nullptr;

		// クリア後はタイトルへ戻る
		currentStage_ = 1;
		scene_ = SceneType::kTitle;

		{
			TitleScene* titleScene = new TitleScene;
			titleScene->Initialize();
			currentScene_ = titleScene;
		}
		break;

	case SceneType::kGameOver:

		// ゲームオーバー画面を削除
		delete currentScene_;
		currentScene_ = nullptr;

		// ゲームオーバーになったステージから再挑戦
		scene_ = SceneType::kGame;

		{
			GameScene* gameScene = new GameScene;
			gameScene->Initialize(currentStage_);
			currentScene_ = gameScene;
		}
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

	if (currentScene_ != nullptr) {
		currentScene_->Update();
	}
}

void SceneManager::DrawScene() {

	if (currentScene_ != nullptr) {
		currentScene_->Draw();
	}
}