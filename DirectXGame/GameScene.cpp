#include "GameScene.h"
#include "Math.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

using namespace KamataEngine;

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

	// ゲームオーバー演出の初期化
	gameOverEffect_.Initialize();

	// ゲームクリア演出の初期化
	gameClearEffect_.Initialize();

	// ゲーム開始演出の初期化
	gameStartEffect_.Initialize();
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

	// ゲームオーバー演出中
	if (gameOverEffect_.IsPlaying()) {

		// ゲームオーバー演出を更新
		if (gameOverEffect_.Update(camera_)) {
			result_ = GameResult::GameOver;
			finished_ = true;
		}

		return;
	}

	// ゲームクリア演出中
	if (gameClearEffect_.IsPlaying()) {
		if (gameClearEffect_.Update()) {
			result_ = GameResult::Clear;
			finished_ = true;
		}

		return;
	}

	// プレイヤー死亡判定
	if (player_->GetHp() <= 0) {

		gameOverEffect_.Start(camera_);

		// プレイヤーの操作を禁止
		player_->SetControllable(false);

		return;
	}

	// ゴール判定
	KamataEngine::Vector3 playerPosition = player_->GetWorldPosition();

	if (goal_.IsPlayerInGoalRange(playerPosition)) {
		gameClearEffect_.Start();
		player_->SetControllable(false);
		return;
	}

	gameStartEffect_.Update();

	// 演出が終わったら操作を許可
	if (gameStartEffect_.IsFinished()) {
		player_->SetControllable(true);
	}

	// ゲーム開始後のプレイ時間を計測
	if (gameStartEffect_.IsFinished()) {
		playTimer_++;
	}

	skyDome_->Update();

	hpBar_.Update(player_->GetHp(), player_->GetMaxHp());

	// デスフラグの立った敵を削除
	defeatedEnemyCount_ += enemyManager_.RemoveDeadEnemies();

	CController_->Update();

	// ブロックの更新
	blockManager_.Update();

	player_->Update();

	// ゲーム開始演出が終わってから敵を動かす
	if (gameStartEffect_.IsFinished()) {

		enemyManager_.Update(player_);

		// 全ての当たり判定を行う
		collisionManager_.CheckAllCollisions(player_, enemyManager_.GetEnemies());
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

	gameStartEffect_.Draw();

	// ゲームオーバー演出
	gameOverEffect_.Draw();

	// ゲームクリア演出
	gameClearEffect_.Draw();

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