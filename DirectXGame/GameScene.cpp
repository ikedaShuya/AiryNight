#include "GameScene.h"
#include "Math.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

using namespace KamataEngine;

// ゲーム開始演出中
void GameSceneStartState::Update(GameScene& gameScene) { gameScene.UpdateStartState(); }

// ゲームプレイ中
void GameScenePlayingState::Update(GameScene& gameScene) { gameScene.UpdatePlayingState(); }

// ゲームオーバー演出中
void GameSceneGameOverState::Update(GameScene& gameScene) { gameScene.UpdateGameOverState(); }

// ゲームクリア演出中
void GameSceneClearState::Update(GameScene& gameScene) { gameScene.UpdateClearState(); }

// ゲーム開始演出中の処理
void GameScene::UpdateStartState() {

	gameStartEffect_.Update();

	skyDome_->Update();

	defeatedEnemyCount_ += enemyManager_.RemoveDeadEnemies();

	CController_->Update();
	blockManager_.Update();
	player_->Update();

	// 演出が終わったら操作を許可してプレイ状態へ
	if (gameStartEffect_.IsFinished()) {
		player_->SetControllable(true);
		playTimer_++;
		ChangeState(std::make_unique<GameScenePlayingState>());
	}
}

// ゲームプレイ中の処理
void GameScene::UpdatePlayingState() {

	// プレイヤー死亡判定
	if (player_->GetHp() <= 0) {
		gameOverEffect_.Start(camera_);
		player_->SetControllable(false);
		ChangeState(std::make_unique<GameSceneGameOverState>());
		return;
	}

	// ゴール判定
	Vector3 playerPosition = player_->GetWorldPosition();

	if (goal_.IsPlayerInGoalRange(playerPosition)) {
		gameClearEffect_.Start();
		player_->SetControllable(false);
		ChangeState(std::make_unique<GameSceneClearState>());
		return;
	}

	playTimer_++;

	skyDome_->Update();

	defeatedEnemyCount_ += enemyManager_.RemoveDeadEnemies();

	CController_->Update();
	blockManager_.Update();

	player_->Update();

	enemyManager_.Update(player_.get());

	// 全ての当たり判定を行う
	collisionManager_.CheckAllCollisions(player_.get(), enemyManager_.GetEnemies(), &particleManager_, &deathParticleManager_);
}

// ゲームオーバー演出中の処理
void GameScene::UpdateGameOverState() {

	if (gameOverEffect_.Update(camera_)) {
		result_ = GameResult::GameOver;
		finished_ = true;
	}
}

// ゲームクリア演出中の処理
void GameScene::UpdateClearState() {

	if (gameClearEffect_.Update()) {
		result_ = GameResult::Clear;
		finished_ = true;
	}
}

// 状態変更
void GameScene::ChangeState(std::unique_ptr<GameSceneState> newState) {
	if (!state_) {
		state_ = std::move(newState);
	} else {
		nextState_ = std::move(newState);
	}
}

void GameScene::Initialize(int stageNumber) {

	stageNumber_ = stageNumber;

	// カメラの初期化
	camera_.Initialize();

	// ブロックモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("cube", true);
	blockManager_.Initialize(modelBlock_);

	// パーティクル用モデルの生成
	modelParticle_ = Model::CreateFromOBJ("plane", true);

	// パーティクルの初期化
	particleManager_.Initialize(modelParticle_, &camera_);

	// 敵撃破用の立体パーティクルを初期化
	deathParticleManager_.Initialize(modelBlock_, &camera_, true);

	mapChipField_ = std::make_unique<MapChipField>();

	stageMapLoader_.LoadMap(stageNumber_, *mapChipField_);

	// ゴールの旗の配置を読み込む
	stageDataLoader_.LoadFlagPosition(stageNumber_, stagePlacementData_);

	goal_.Initialize(mapChipField_->GetMapChipPositionByIndex(stagePlacementData_.GetFlagX(), stagePlacementData_.GetFlagY()));

	GenerateBlocks();

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	swordModel_ = Model::CreateFromOBJ("sword", true);

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	// プレイヤーの配置を読み込む
	stageDataLoader_.LoadPlayerPosition(stageNumber_, stagePlacementData_);

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(stagePlacementData_.GetPlayerX(), stagePlacementData_.GetPlayerY());

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_.get());

	// ゲーム開始演出中は操作できないようにする
	player_->SetControllable(false);

	// 敵のモデル
	enemyManager_.Initialize(Model::CreateFromOBJ("enemy", true), &camera_);

	// ステージごとの敵配置を設定
	stageDataLoader_.LoadEnemyPositions(stageNumber_, stagePlacementData_);

	// 敵を生成
	GenerateEnemies();

	// カメラコントローラの初期化
	CController_ = std::make_unique<CameraController>();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_.get());
	CController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {10.556f, 100 - 12.0f, 11.0f, 13.3f};
	CController_->SetMovableArea(cameraArea);

	// HPバー関連の処理の初期化
	hpBar_.Initialize();

	// HPバーをオブザーバーとして登録
	player_->AddHpObserver(&hpBar_);

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize(modelSkyDome_, &camera_);

	aSprite_ = Sprite::Create(TextureManager::Load("a.png"), {960.0f, 0.0f});

	// エフェクトを基底クラスのポインタに登録
	effects_[0] = &gameOverEffect_;
	effects_[1] = &gameClearEffect_;
	effects_[2] = &gameStartEffect_;

	// すべてのエフェクトを初期化
	for (int i = 0; i < 3; i++) {
		effects_[i]->Initialize();
	}

	// ゲーム開始状態にする
	ChangeState(std::make_unique<GameSceneStartState>());
}

void GameScene::OnUpdate() {

#ifdef USE_IMGUI
	mapEditorController_.Update(
	    mapEditor_, stageNumber_, stagePlacementData_, stageDataLoader_, stageMapLoader_, stagePlacementDataSaver_, blockManager_, enemyManager_, goal_, player_.get(), mapChipField_.get(),
	    CController_.get());
#endif

	// マップ編集中はゲームの進行を停止する
#ifdef USE_IMGUI
	if (mapEditor_.IsEditing()) {
		return;
	}
#endif

	// パーティクルの更新
	particleManager_.Update();
	deathParticleManager_.Update();

	// 現在の状態を更新
	if (state_) {
		state_->Update(*this);
	}

	// 状態更新が終わってから状態を切り替える
	if (nextState_) {
		state_ = std::move(nextState_);
	}
}

void GameScene::OnDraw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	skyDome_->Draw();

	// ブロックの描画
	blockManager_.Draw(camera_);

	// ゴールの旗を描画
	goal_.Draw(modelBlock_, camera_);

	// 自キャラの描画
	player_->Draw();

	enemyManager_.Draw();

	// パーティクルの描画
	particleManager_.Draw();
	deathParticleManager_.Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw();

	// HPバー描画
	hpBar_.Draw();
	aSprite_->Draw();

	// すべてのエフェクトを描画
	for (int i = 0; i < 3; i++) {
		effects_[i]->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}

GameScene::~GameScene() {

	// モデルデータの解放
	delete swordModel_;
	delete modelPlayer_;
	delete modelBlock_;
	delete modelParticle_;

	delete modelSkyDome_;

	// スプライトの解放
	delete aSprite_;
}

void GameScene::GenerateBlocks() { blockManager_.Generate(mapChipField_.get()); }

void GameScene::GenerateEnemies() { enemyManager_.Generate(stagePlacementData_, mapChipField_.get()); }