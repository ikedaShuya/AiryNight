#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	// ブロックモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("cube", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	
	// ゴールの位置を設定する
	goalPosition_ = mapChipField_->GetMapChipPositionByIndex(97, 12);

	// ゴールの棒を初期化する
	worldTransformGoalPole_.Initialize();
	worldTransformGoalPole_.scale_ = {0.15f, 4.0f, 0.15f};
	worldTransformGoalPole_.translation_ = {goalPosition_.x, goalPosition_.y + 1.0f, goalPosition_.z};

	// ゴールの旗を初期化する
	worldTransformGoalFlag_.Initialize();
	worldTransformGoalFlag_.scale_ = {0.8f, 0.5f, 0.1f};
	worldTransformGoalFlag_.translation_ = {goalPosition_.x + 0.4f, goalPosition_.y + 2.8f, goalPosition_.z};

	// ワールド変換を更新する
	WorldTransformUpdate(worldTransformGoalPole_);
	WorldTransformUpdate(worldTransformGoalFlag_);

	GenerateBlocks();

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	swordModel_ = Model::CreateFromOBJ("sword", true);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 12);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);
	// ゲーム開始演出中は操作できないようにする
	player_->SetControllable(false);

	// 敵のモデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	// 敵5体の配置位置を個別に指定
	const Vector3 enemyPositions[] = {
	    mapChipField_->GetMapChipPositionByIndex(3, 12), mapChipField_->GetMapChipPositionByIndex(25, 12), mapChipField_->GetMapChipPositionByIndex(45, 12),
	    mapChipField_->GetMapChipPositionByIndex(70, 12), mapChipField_->GetMapChipPositionByIndex(90, 12)};

	// 敵を生成して配置
	for (int i = 0; i < 5; ++i) {
		Enemy* newEnemy = new Enemy();

		newEnemy->Initialize(modelEnemy_, &camera_, enemyPositions[i]);

		enemies_.push_back(newEnemy);
	}

	// カメラコントローラの初期化
	CController_ = new CameraController();
	CController_->Initialize(&camera_);
	CController_->SetTarget(player_);
	CController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {10.556f, 100 - 12.0f, 11.0f, 13.3f};
	CController_->SetMovableArea(cameraArea);

	hpBarBgTex_ = TextureManager::Load("hp_bar_bg.png");
	hpBarFgTex_ = TextureManager::Load("hp_bar_fg.png");

	// 画像サイズそのまま
	hpBarBg_ = Sprite::Create(hpBarBgTex_, {70, 10});
	hpBarFg_ = Sprite::Create(hpBarFgTex_, {70, 10});

	// 表示位置（左上）
	hpBarBg_->SetPosition({90, 20});
	hpBarFg_->SetPosition({90, 20});

	hpBarBg_->SetAnchorPoint({0.0f, 0.0f});
	hpBarFg_->SetAnchorPoint({0.0f, 0.0f});

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();                    
	skyDome_->Initialize(modelSkyDome_, &camera_);

	aSprite_ = Sprite::Create(TextureManager::Load("a.png"), {960.0f, 0.0f});

	// ゲームオーバー時の暗転用スプライト
	gameOverFadeSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});

	gameOverFadeSprite_->SetSize({1280.0f, 720.0f});
	gameOverFadeSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});

	// カウントダウン画像を読み込む
	countdown3Sprite_ = Sprite::Create(TextureManager::Load("countdown_3.png"), {640.0f, 360.0f});

	countdown2Sprite_ = Sprite::Create(TextureManager::Load("countdown_2.png"), {640.0f, 360.0f});

	countdown1Sprite_ = Sprite::Create(TextureManager::Load("countdown_1.png"), {640.0f, 360.0f});

	countdownGoSprite_ = Sprite::Create(TextureManager::Load("countdown_go.png"), {640.0f, 360.0f});

	// 画像の中心を画面中央に合わせる
	countdown3Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdown2Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdown1Sprite_->SetAnchorPoint({0.5f, 0.5f});
	countdownGoSprite_->SetAnchorPoint({0.5f, 0.5f});
}

void GameScene::Update() {

	
	// ゲームオーバー演出中
	if (isGameOverPlaying_) {

		gameOverTimer_++;

		// カメラを30フレームかけてズームインする
		if (gameOverTimer_ <= kGameOverZoomFrame) {
			float t = static_cast<float>(gameOverTimer_) / static_cast<float>(kGameOverZoomFrame);

			// 最初はゆっくり、徐々にズームする
			float ease = t * t;

			// 画角を元の65%まで狭くする
			camera_.fovAngleY = gameOverStartFovY_ * (1.0f - 0.35f * ease);
		}

		// カメラの行列を更新する
		camera_.UpdateMatrix();

		// 最初の10フレームは赤い衝撃演出
		if (gameOverTimer_ <= kGameOverFlashFrame) {

			float alpha = 0.55f * (1.0f - static_cast<float>(gameOverTimer_ - 1) / static_cast<float>(kGameOverFlashFrame));

			gameOverFadeSprite_->SetColor({1.0f, 0.0f, 0.0f, alpha});

		} else {

			// 赤い演出の後、黒くゆっくり暗転する
			float alpha = static_cast<float>(gameOverTimer_ - kGameOverFlashFrame) / static_cast<float>(kGameOverFrame - kGameOverFlashFrame);

			if (alpha > 1.0f) {
				alpha = 1.0f;
			}

			gameOverFadeSprite_->SetColor({0.0f, 0.0f, 0.0f, alpha});
		}

		// 暗転が完了したらゲームオーバーシーンへ
		if (gameOverTimer_ >= kGameOverFrame) {
			result_ = GameResult::GameOver;
			finished_ = true;
		}

		return;
	}

	// プレイヤー死亡判定
	if (player_->GetHp() <= 0) {

		isGameOverPlaying_ = true;
		gameOverStartFovY_ = camera_.fovAngleY;
		gameOverTimer_ = 0;

		// プレイヤーの操作を禁止
		player_->SetControllable(false);

		return;
	}

	// ゴール判定
	KamataEngine::Vector3 playerPosition = player_->GetWorldPosition();

	float distanceX = playerPosition.x - goalPosition_.x;
	float distanceY = playerPosition.y - goalPosition_.y;

	if (distanceX > -1.0f && distanceX < 1.0f && distanceY > -1.0f && distanceY < 3.0f) {
		result_ = GameResult::Clear;
		finished_ = true;
		return;
	}

	// ゲーム開始演出のタイマー
	if (startTimer_ < kGameStartFrame) {
		startTimer_++;
	}

	// カウントダウンの数字を設定
	countdownNumber_ = 0;

	if (startTimer_ >= kStagePreviewFrame && startTimer_ < kStagePreviewFrame + 60) {
		countdownNumber_ = 3;
	} else if (startTimer_ >= kStagePreviewFrame + 60 && startTimer_ < kStagePreviewFrame + 120) {
		countdownNumber_ = 2;
	} else if (startTimer_ >= kStagePreviewFrame + 120 && startTimer_ < kStagePreviewFrame + 180) {
		countdownNumber_ = 1;
	} else if (startTimer_ >= kStagePreviewFrame + kCountdownFrame && startTimer_ < kGameStartFrame) {
		countdownNumber_ = 4; // GO!
	}

	// 演出が終わったら操作を許可
	if (startTimer_ >= kGameStartFrame) {
		player_->SetControllable(true);
	}

	skyDome_->Update();

	float hpRate = static_cast<float>(player_->GetHp()) / static_cast<float>(player_->GetMaxHp());

	// 横だけ縮める（高さは固定10）
	hpBarFg_->SetSize({100.0f * hpRate, 10.0f});

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	CController_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlcokLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlcokLine) {
			if (!worldTransformBlock)
				continue;

			WorldTransformUpdate(*worldTransformBlock);
		}
	}

	player_->Update();

	// ゲーム開始演出が終わってから敵を動かす
	if (startTimer_ >= kGameStartFrame) {

		for (Enemy* enemy_ : enemies_) {
			enemy_->Update(player_);
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();
	}
}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	skyDome_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlcokLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlcokLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// ゴールの旗を描画
	modelBlock_->Draw(worldTransformGoalPole_, camera_);
	modelBlock_->Draw(worldTransformGoalFlag_, camera_);

	// 自キャラの描画
	player_->Draw();

	for (Enemy* enemy_ : enemies_) {
		enemy_->Draw();
	}

	// 3Dモデル描画後処理
	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw();

	// HPバー描画
	hpBarFg_->Draw();
	hpBarBg_->Draw();
	aSprite_->Draw();

	// カウントダウン表示
	if (countdownNumber_ == 3) {
		countdown3Sprite_->Draw();
	} else if (countdownNumber_ == 2) {
		countdown2Sprite_->Draw();
	} else if (countdownNumber_ == 1) {
		countdown1Sprite_->Draw();
	} else if (countdownNumber_ == 4) {
		countdownGoSprite_->Draw();
	}

	// ゲームオーバー時の暗転
	if (isGameOverPlaying_) {
		gameOverFadeSprite_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}

GameScene::~GameScene() {

	// マップチップフィールドの解放
	delete mapChipField_;

	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// モデルデータの解放
	delete modelPlayer_;

	// 自キャラの解放
	delete player_;

	delete modelEnemy_;

	for (Enemy* enemy_ : enemies_) {
		delete enemy_;
	}

	delete hpBarBg_;
	delete hpBarFg_;

	delete skyDome_;
	delete modelSkyDome_;
	delete countdown3Sprite_;
	delete countdown2Sprite_;
	delete countdown1Sprite_;
	delete countdownGoSprite_;
	delete gameOverFadeSprite_;
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		// 1列の要素数を設定（横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	// キューブの生成
	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions(){

#pragma region 自キャラと敵キャラの当たり判定
    {// 判定対象1と2の座標
     AABB aabb1, aabb2;

// 自キャラの座標
aabb1 = player_->GetAABB();

// 自キャラと敵弾全ての当たり判定
for (Enemy* enemy_ : enemies_) {
	// 敵弾の座標
	aabb2 = enemy_->GetAABB();

	// AABB同士の交差判定
	if (IsCollision(aabb1, aabb2)) {
		// 自キャラの衝突時関数を呼び出す
		player_->OnCollision(enemy_);
		// 敵の衝突時関数を呼び出す
		enemy_->OnCollision(player_);
	}
}
}
#pragma endregion

#pragma region 近接攻撃と敵キャラの当たり判定（攻撃）
{
	// 攻撃中のみ判定する
	if (!player_->IsAttacking()) {
		return;
	}

	AABB attackAABB = player_->GetSwordAABB();

	for (Enemy* enemy_ : enemies_) {
		AABB enemyAABB = enemy_->GetAABB();

		if (IsCollision(attackAABB, enemyAABB)) {
			enemy_->OnHit(player_); // ダメージ処理
		}
	}
}
#pragma endregion
}