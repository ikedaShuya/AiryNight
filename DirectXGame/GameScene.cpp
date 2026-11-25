#include "GameScene.h"
#include "Math.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

using namespace KamataEngine;

// ゲーム開始演出中
void GameSceneStartState::Update(GameScene& gameScene) {

	gameScene.gameStartEffect_.Update();

	gameScene.skyDome_->Update();
	gameScene.hpBar_.Update(gameScene.player_->GetHp(), gameScene.player_->GetMaxHp());

	gameScene.defeatedEnemyCount_ += gameScene.enemyManager_.RemoveDeadEnemies();

	gameScene.CController_->Update();
	gameScene.blockManager_.Update();
	gameScene.player_->Update();

	// 演出が終わったら操作を許可してプレイ状態へ
	if (gameScene.gameStartEffect_.IsFinished()) {
		gameScene.player_->SetControllable(true);
		gameScene.playTimer_++;
		gameScene.ChangeState(std::make_unique<GameScenePlayingState>());
	}
}

// ゲームプレイ中
void GameScenePlayingState::Update(GameScene& gameScene) {

	// プレイヤー死亡判定
	if (gameScene.player_->GetHp() <= 0) {
		gameScene.gameOverEffect_.Start(gameScene.camera_);
		gameScene.player_->SetControllable(false);
		gameScene.ChangeState(std::make_unique<GameSceneGameOverState>());
		return;
	}

	// ゴール判定
	Vector3 playerPosition = gameScene.player_->GetWorldPosition();

	if (gameScene.goal_.IsPlayerInGoalRange(playerPosition)) {
		gameScene.gameClearEffect_.Start();
		gameScene.player_->SetControllable(false);
		gameScene.ChangeState(std::make_unique<GameSceneClearState>());
		return;
	}

	gameScene.playTimer_++;

	gameScene.skyDome_->Update();
	gameScene.hpBar_.Update(gameScene.player_->GetHp(), gameScene.player_->GetMaxHp());

	gameScene.defeatedEnemyCount_ += gameScene.enemyManager_.RemoveDeadEnemies();

	gameScene.CController_->Update();
	gameScene.blockManager_.Update();

	gameScene.player_->Update();

	gameScene.enemyManager_.Update(gameScene.player_);
	gameScene.collisionManager_.CheckAllCollisions(gameScene.player_, gameScene.enemyManager_.GetEnemies());
}

// ゲームオーバー演出中
void GameSceneGameOverState::Update(GameScene& gameScene) {

	if (gameScene.gameOverEffect_.Update(gameScene.camera_)) {
		gameScene.result_ = GameResult::GameOver;
		gameScene.finished_ = true;
	}
}

// ゲームクリア演出中
void GameSceneClearState::Update(GameScene& gameScene) {

	if (gameScene.gameClearEffect_.Update()) {
		gameScene.result_ = GameResult::Clear;
		gameScene.finished_ = true;
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

	mapChipField_ = new MapChipField;

	stageMapLoader_.LoadMap(stageNumber_, *mapChipField_);

	// ゴールの旗の配置を読み込む
	stageDataLoader_.LoadFlagPosition(stageNumber_, stagePlacementData_);

	goal_.Initialize(mapChipField_->GetMapChipPositionByIndex(stagePlacementData_.GetFlagX(), stagePlacementData_.GetFlagY()));

	GenerateBlocks();

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	swordModel_ = Model::CreateFromOBJ("sword", true);

	// 自キャラの生成
	player_ = new Player();
	// プレイヤーの配置を読み込む
	stageDataLoader_.LoadPlayerPosition(stageNumber_, stagePlacementData_);

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(stagePlacementData_.GetPlayerX(), stagePlacementData_.GetPlayerY());
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);
	// ゲーム開始演出中は操作できないようにする
	player_->SetControllable(false);

	// 敵のモデル
	enemyManager_.Initialize(Model::CreateFromOBJ("enemy", true), &camera_);

	// ステージごとの敵配置を設定
	stageDataLoader_.LoadEnemyPositions(stageNumber_, stagePlacementData_);

	// 敵を生成
	GenerateEnemies();

	// カメラコントローラの初期化
	CController_ = new CameraController();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_);
	CController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {10.556f, 100 - 12.0f, 11.0f, 13.3f};
	CController_->SetMovableArea(cameraArea);

	// HPバー関連の処理の初期化
	hpBar_.Initialize();

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
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

void GameScene::Update() {

#ifdef USE_IMGUI
	mapEditorController_.Update(
	    mapEditor_, stageNumber_, stagePlacementData_, stageDataLoader_, stageMapLoader_, stagePlacementDataSaver_, blockManager_, enemyManager_, goal_, player_, mapChipField_, CController_);
#endif

	// マップ編集中はゲームの進行を停止する
#ifdef USE_IMGUI
	if (mapEditor_.IsEditing()) {
		return;
	}
#endif

	// 現在の状態を更新
	if (state_) {
		state_->Update(*this);
	}

	// 状態更新が終わってから状態を切り替える
	if (nextState_) {
		state_ = std::move(nextState_);
	}
}

void GameScene::Draw() {

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

	// カメラコントローラの解放
	delete CController_;

	// 自キャラの解放
	delete player_;

	// マップチップフィールドの解放
	delete mapChipField_;

	// モデルデータの解放
	delete swordModel_;
	delete modelPlayer_;
	delete modelBlock_;

	// スカイドームの解放
	delete skyDome_;
	delete modelSkyDome_;

	// スプライトの解放
	delete aSprite_;
}

void GameScene::GenerateBlocks() { blockManager_.Generate(mapChipField_); }

void GameScene::GenerateEnemies() { enemyManager_.Generate(stagePlacementData_, mapChipField_); }