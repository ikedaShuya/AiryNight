#include "SceneManager.h"
#include "GameScene.h"

// 初期化
void SceneManager::Initialize() {

	// フェードの初期化
	fade_.Initialize();

	// 最初のシーンをタイトルにする
	scene_ = SceneType::kTitle;

	SceneCreateData data;
	data.stageNumber = currentStage_;

	currentScene_ = sceneFactory_.CreateScene(scene_, data);
}

// 更新
void SceneManager::Update() {

	// フェード更新
	fade_.Update();

	// シーン切り替え
	ChangeScene();

	// 現在のシーン更新
	UpdateScene();
}

// 描画
void SceneManager::Draw() {

	// 現在のシーン描画
	DrawScene();

	// フェード描画
	fade_.Draw();
}

// 終了処理
void SceneManager::Finalize() {

	// 現在のシーンを解放
	currentScene_.reset();
}

// シーン終了判定
bool SceneManager::IsSceneFinished() {

	if (currentScene_ == nullptr) {
		return false;
	}

	return currentScene_->IsFinished();
}

// シーン切り替え
void SceneManager::ChangeSceneNow() {

	SceneType nextScene = scene_;
	SceneCreateData data;

	data.stageNumber = currentStage_;

	switch (scene_) {

	case SceneType::kTitle:

		currentStage_ = 1;
		data.stageNumber = currentStage_;

		nextScene = SceneType::kGame;
		break;

	case SceneType::kGame: {

		GameScene* gameScene = static_cast<GameScene*>(currentScene_.get());

		if (gameScene->GetResult() == GameResult::Clear) {

			// ゲームのプレイ時間を取得
			data.playTimer = gameScene->GetPlayTimer();

			// 敵の撃破数を取得
			data.defeatedEnemyCount = gameScene->GetDefeatedEnemyCount();

			nextScene = SceneType::kClear;

		} else if (gameScene->GetResult() == GameResult::GameOver) {

			nextScene = SceneType::kGameOver;
		} else {

			// クリアでもゲームオーバーでもなければ何もしない
			return;
		}

		break;
	}

	case SceneType::kClear:

		// クリア後はタイトルへ戻る
		currentStage_ = 1;
		data.stageNumber = currentStage_;

		nextScene = SceneType::kTitle;
		break;

	case SceneType::kGameOver:

		// ゲームオーバーになったステージから再挑戦
		data.stageNumber = currentStage_;

		nextScene = SceneType::kGame;
		break;

	default:
		return;
	}

	// 現在のシーンを削除
	currentScene_.reset();

	// 次のシーンを生成
	scene_ = nextScene;
	currentScene_ = sceneFactory_.CreateScene(scene_, data);
}

// シーン切り替え
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

// 現在のシーン更新
void SceneManager::UpdateScene() {

	if (currentScene_ != nullptr) {
		currentScene_->Update();
	}
}

// 現在のシーン描画
void SceneManager::DrawScene() {

	if (currentScene_ != nullptr) {
		currentScene_->Draw();
	}
}