#include "GameScene.h"
#include "Math.h"
#include <fstream>

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

	mapChipField_ = new MapChipField;

	if (stageNumber_ == 1) {
		mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	} else if (stageNumber_ == 2) {
		mapChipField_->LoadMapChipCsv("Resources/blocks2.csv");
	} else {
		mapChipField_->LoadMapChipCsv("Resources/blocks3.csv");
	}

	// ゴールの旗の配置を読み込む
	LoadFlagPositionForStage();

	// ゴールの位置を設定する
	goalPosition_ = mapChipField_->GetMapChipPositionByIndex(flagMapX_, flagMapY_);

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
	// プレイヤーの配置を読み込む
	LoadPlayerPositionForStage();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(playerMapX_, playerMapY_);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, swordModel_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);
	// ゲーム開始演出中は操作できないようにする
	player_->SetControllable(false);

	// 敵のモデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	// ステージごとの敵配置を設定
	SetEnemyPositionsForStage();

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

	// ゲームクリア演出用フラッシュ
	gameClearFlashSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});
	gameClearFlashSprite_->SetSize({1280.0f, 720.0f});
	gameClearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

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

// マップチップ編集画面
#ifdef USE_IMGUI

	ImGui::Begin("Map Editor");

	// 編集モードの切り替え
	if (ImGui::Checkbox("Edit Mode", &isMapEditing_)) {
		// モード切り替え時の処理
	}

	// ステージ切り替え
	const char* stages[] = {"Stage 1", "Stage 2", "Stage 3"};

	int selectedStage = stageNumber_ - 1;

	if (ImGui::Combo("Stage", &selectedStage, stages, 3)) {

		stageNumber_ = selectedStage + 1;

		if (stageNumber_ == 1) {
			mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
		} else if (stageNumber_ == 2) {
			mapChipField_->LoadMapChipCsv("Resources/blocks2.csv");
		} else {
			mapChipField_->LoadMapChipCsv("Resources/blocks3.csv");
		}

		// ブロックを作り直す
		GenerateBlocks();

		// ゴールの旗の配置を読み込む
		LoadFlagPositionForStage();

		// ゴール位置を更新する
		goalPosition_ = mapChipField_->GetMapChipPositionByIndex(flagMapX_, flagMapY_);

		// ゴールの棒の位置を更新する
		worldTransformGoalPole_.translation_ = {goalPosition_.x, goalPosition_.y + 1.0f, goalPosition_.z};

		// ゴールの旗の位置を更新する
		worldTransformGoalFlag_.translation_ = {goalPosition_.x + 0.4f, goalPosition_.y + 2.8f, goalPosition_.z};

		WorldTransformUpdate(worldTransformGoalPole_);
		WorldTransformUpdate(worldTransformGoalFlag_);

		// 敵の配置を更新
		SetEnemyPositionsForStage();

		// 敵を作り直す
		GenerateEnemies();

		// プレイヤーの配置を読み込む
		LoadPlayerPositionForStage();

		Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(playerMapX_, playerMapY_);

		player_->SetPosition(playerPosition);

		// カメラの位置を更新
		CController_->Reset();
	}

	// 編集モードの切り替え
	ImGui::Text("Editor Mode");

	ImGui::RadioButton("Map Mode", &editorMode_, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Enemy Mode", &editorMode_, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Player Mode", &editorMode_, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Flag Mode", &editorMode_, 3);

	ImGui::Separator();

	// マップ編集モードのときだけチップ選択を表示
	if (editorMode_ == 0) {
		ImGui::Text("Tool");

		ImGui::RadioButton("Empty", &editMapType_, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Block", &editMapType_, 1);
	}

	ImGui::Separator();

	// 範囲選択モード
	ImGui::Checkbox("Range Select", &isRangeSelectMode_);

	ImGui::Text("Drag to select an area and apply the selected tool");

	ImGui::Separator();

	// マップの拡大・縮小
	ImGui::Text("Map Zoom");

	ImGui::SliderFloat("Cell Size", &mapCellSize_, 4.0f, 16.0f, "%.1f");

	ImGui::Separator();

	// マップ表示エリア
	float cellSize = mapCellSize_;

	float mapWidth = mapChipField_->GetNumBlockHorizontal() * cellSize;
	float mapHeight = mapChipField_->GetNumBlockVertical() * cellSize;

	ImGui::BeginChild("MapCanvasArea", ImVec2(0, 190), true, ImGuiWindowFlags_HorizontalScrollbar);

	// マップの左上座標
	ImVec2 mapStart = ImGui::GetCursorScreenPos();

	// マップ全体のクリック判定
	ImGui::InvisibleButton("MapCanvas", ImVec2(mapWidth, mapHeight));

	bool isHovered = ImGui::IsItemHovered();

	// マウス位置からマス目を計算
	ImVec2 mousePosition = ImGui::GetIO().MousePos;

	int mouseX = static_cast<int>((mousePosition.x - mapStart.x) / cellSize);
	int mouseY = static_cast<int>((mousePosition.y - mapStart.y) / cellSize);

	// マップの範囲内に収める
	if (mouseX < 0) {
		mouseX = 0;
	}
	if (mouseX >= static_cast<int>(mapChipField_->GetNumBlockHorizontal())) {
		mouseX = static_cast<int>(mapChipField_->GetNumBlockHorizontal()) - 1;
	}
	if (mouseY < 0) {
		mouseY = 0;
	}
	if (mouseY >= static_cast<int>(mapChipField_->GetNumBlockVertical())) {
		mouseY = static_cast<int>(mapChipField_->GetNumBlockVertical()) - 1;
	}

	// 敵配置モード
	if (editorMode_ == 1) {

		// 左クリックで敵を配置
		if (isHovered && ImGui::IsMouseClicked(0)) {

			bool alreadyExists = false;

			for (const EnemyMapPosition& position : enemyMapPositions_) {
				if (position.x == mouseX && position.y == mouseY) {
					alreadyExists = true;
					break;
				}
			}

			if (!alreadyExists) {
				enemyMapPositions_.push_back({mouseX, mouseY});
				GenerateEnemies();
			}
		}

		// 右クリックで敵を削除
		if (isHovered && ImGui::IsMouseClicked(1)) {

			for (auto it = enemyMapPositions_.begin(); it != enemyMapPositions_.end(); ++it) {
				if (it->x == mouseX && it->y == mouseY) {
					enemyMapPositions_.erase(it);
					break;
				}
			}

			GenerateEnemies();
		}
	}

	// プレイヤー配置モード
	if (editorMode_ == 2) {

		// 左クリックでプレイヤーを配置
		if (isHovered && ImGui::IsMouseClicked(0)) {

			playerMapX_ = mouseX;
			playerMapY_ = mouseY;

			Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(playerMapX_, playerMapY_);

			player_->SetPosition(playerPosition);

			// カメラの位置を更新
			CController_->Reset();
		}
	}

	// 旗配置モード
	if (editorMode_ == 3) {

		// 左クリックで旗を配置
		if (isHovered && ImGui::IsMouseClicked(0)) {

			flagMapX_ = mouseX;
			flagMapY_ = mouseY;

			goalPosition_ = mapChipField_->GetMapChipPositionByIndex(flagMapX_, flagMapY_);

			// ゴールの棒の位置を更新
			worldTransformGoalPole_.translation_ = {goalPosition_.x, goalPosition_.y + 1.0f, goalPosition_.z};

			// ゴールの旗の位置を更新
			worldTransformGoalFlag_.translation_ = {goalPosition_.x + 0.4f, goalPosition_.y + 2.8f, goalPosition_.z};

			WorldTransformUpdate(worldTransformGoalPole_);
			WorldTransformUpdate(worldTransformGoalFlag_);
		}
	}

	// 範囲選択モード
	// マップ編集モード
	if (editorMode_ == 0) {

		// 範囲選択モード
		if (isRangeSelectMode_) {

			// マウスを押した瞬間に開始位置を記録
			if (isHovered && ImGui::IsMouseClicked(0)) {
				isSelectingMapRange_ = true;

				rangeStartX_ = mouseX;
				rangeStartY_ = mouseY;

				rangeEndX_ = mouseX;
				rangeEndY_ = mouseY;
			}

			// ドラッグ中は終了位置を更新
			if (isSelectingMapRange_ && ImGui::IsMouseDown(0)) {
				rangeEndX_ = mouseX;
				rangeEndY_ = mouseY;
			}

			// マウスを離したら範囲内をまとめて変更
			if (isSelectingMapRange_ && ImGui::IsMouseReleased(0)) {

				rangeEndX_ = mouseX;
				rangeEndY_ = mouseY;

				int minX = rangeStartX_;
				int maxX = rangeEndX_;
				int minY = rangeStartY_;
				int maxY = rangeEndY_;

				if (minX > maxX) {
					int temp = minX;
					minX = maxX;
					maxX = temp;
				}

				if (minY > maxY) {
					int temp = minY;
					minY = maxY;
					maxY = temp;
				}

				MapChipType type = MapChipType::kBlank;

				if (editMapType_ == 1) {
					type = MapChipType::kBlock;
				}

				for (int y = minY; y <= maxY; y++) {
					for (int x = minX; x <= maxX; x++) {
						mapChipField_->SetMapChipTypeByIndex(x, y, type);
					}
				}

				GenerateBlocks();

				isSelectingMapRange_ = false;
			}

		} else {

			// 通常モード：クリック・ドラッグで編集
			if (isHovered && ImGui::IsMouseDown(0)) {

				MapChipType type = MapChipType::kBlank;

				if (editMapType_ == 1) {
					type = MapChipType::kBlock;
				}

				if (mapChipField_->GetMapChipTypeByIndex(mouseX, mouseY) != type) {

					mapChipField_->SetMapChipTypeByIndex(mouseX, mouseY, type);

					GenerateBlocks();
				}
			}
		}
	}

	// マップを描画
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	for (uint32_t y = 0; y < mapChipField_->GetNumBlockVertical(); y++) {

		for (uint32_t x = 0; x < mapChipField_->GetNumBlockHorizontal(); x++) {

			ImVec2 p1 = {mapStart.x + x * cellSize, mapStart.y + y * cellSize};

			ImVec2 p2 = {p1.x + cellSize, p1.y + cellSize};

			ImU32 color;

			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock) {
				color = IM_COL32(90, 130, 190, 255);
			} else {
				color = IM_COL32(45, 55, 65, 255);
			}

			drawList->AddRectFilled(p1, p2, color);

			drawList->AddRect(p1, p2, IM_COL32(100, 110, 120, 255));
		}
	}

	// 敵の配置位置を描画
	for (const EnemyMapPosition& position : enemyMapPositions_) {

		ImVec2 center = {mapStart.x + (position.x + 0.5f) * cellSize, mapStart.y + (position.y + 0.5f) * cellSize};

		drawList->AddCircleFilled(center, cellSize * 0.4f, IM_COL32(255, 70, 70, 255));
	}

	// プレイヤーの配置位置を描画
	ImVec2 playerCenter = {mapStart.x + (playerMapX_ + 0.5f) * cellSize, mapStart.y + (playerMapY_ + 0.5f) * cellSize};

	drawList->AddCircleFilled(playerCenter, cellSize * 0.45f, IM_COL32(50, 150, 255, 255));

	// ゴールの旗の配置位置を描画
	ImVec2 flagCenter = {mapStart.x + (flagMapX_ + 0.5f) * cellSize, mapStart.y + (flagMapY_ + 0.5f) * cellSize};

	drawList->AddCircleFilled(flagCenter, cellSize * 0.45f, IM_COL32(255, 220, 0, 255));

	// 範囲選択中の枠を描画
	if (isSelectingMapRange_) {

		int minX = rangeStartX_;
		int maxX = rangeEndX_;
		int minY = rangeStartY_;
		int maxY = rangeEndY_;

		if (minX > maxX) {
			int temp = minX;
			minX = maxX;
			maxX = temp;
		}

		if (minY > maxY) {
			int temp = minY;
			minY = maxY;
			maxY = temp;
		}

		ImVec2 p1 = {mapStart.x + minX * cellSize, mapStart.y + minY * cellSize};

		ImVec2 p2 = {mapStart.x + (maxX + 1) * cellSize, mapStart.y + (maxY + 1) * cellSize};

		drawList->AddRect(p1, p2, IM_COL32(255, 220, 0, 255), 0.0f, 0, 3.0f);
	}

	ImGui::EndChild();

	// 保存完了メッセージの表示時間を減らす
	if (saveMessageTimer_ > 0) {
		saveMessageTimer_--;
	}

	// 保存ボタン

	if (ImGui::Button("Save")) {

		// マップを保存
		if (stageNumber_ == 1) {
			mapChipField_->SaveMapChipCsv("Resources/blocks.csv");
		} else if (stageNumber_ == 2) {
			mapChipField_->SaveMapChipCsv("Resources/blocks2.csv");
		} else {
			mapChipField_->SaveMapChipCsv("Resources/blocks3.csv");
		}

		// 敵の配置ファイル
		std::string filePath;

		if (stageNumber_ == 1) {
			filePath = "Resources/enemies1.csv";
		} else if (stageNumber_ == 2) {
			filePath = "Resources/enemies2.csv";
		} else {
			filePath = "Resources/enemies3.csv";
		}

		// 敵の配置を保存
		std::ofstream file(filePath);

		if (file.is_open()) {
			for (const EnemyMapPosition& position : enemyMapPositions_) {
				file << position.x << "," << position.y << "\n";
			}

			file.close();
			saveMessageTimer_ = 120;
		}

		// プレイヤーの配置ファイル
		std::string playerFilePath;

		if (stageNumber_ == 1) {
			playerFilePath = "Resources/player1.csv";
		} else if (stageNumber_ == 2) {
			playerFilePath = "Resources/player2.csv";
		} else {
			playerFilePath = "Resources/player3.csv";
		}

		// プレイヤーの配置を保存
		std::ofstream playerFile(playerFilePath);

		if (playerFile.is_open()) {
			playerFile << playerMapX_ << "," << playerMapY_ << "\n";
			playerFile.close();

			saveMessageTimer_ = 120;
		}

		// 旗の配置ファイル
		std::string flagFilePath;

		if (stageNumber_ == 1) {
			flagFilePath = "Resources/flag1.csv";
		} else if (stageNumber_ == 2) {
			flagFilePath = "Resources/flag2.csv";
		} else {
			flagFilePath = "Resources/flag3.csv";
		}

		// 旗の配置を保存
		std::ofstream flagFile(flagFilePath);

		if (flagFile.is_open()) {
			flagFile << flagMapX_ << "," << flagMapY_ << "\n";
			flagFile.close();

			saveMessageTimer_ = 120;
		}
	}


	// 保存完了メッセージ
	if (saveMessageTimer_ > 0) {
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.3f, 1.0f), "Saved!");
	}

#endif

	// マップ編集中はゲームの進行を停止する
#ifdef USE_IMGUI
	if (isMapEditing_) {
		return;
	}
#endif

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

	// ゲームクリア演出中
	if (isGameClearPlaying_) {
		gameClearTimer_++;

		if (gameClearTimer_ >= kGameClearFrame) {
			result_ = GameResult::Clear;
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
		isGameClearPlaying_ = true;
		gameClearTimer_ = 0;
		player_->SetControllable(false);
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

	// ゲーム開始後のプレイ時間を計測
	if (startTimer_ >= kGameStartFrame) {
		playTimer_++;
	}

	skyDome_->Update();

	float hpRate = static_cast<float>(player_->GetHp()) / static_cast<float>(player_->GetMaxHp());

	// 横だけ縮める（高さは固定10）
	hpBarFg_->SetSize({100.0f * hpRate, 10.0f});

	// デスフラグの立った敵を削除
	enemies_.remove_if([this](Enemy* enemy) {
		if (enemy->IsDead()) {

			// 敵の撃破数を増やす
			defeatedEnemyCount_++;

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
	hpBarBg_->Draw();
	hpBarFg_->Draw();
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

	// ゲームクリア演出
	if (isGameClearPlaying_) {
		float alpha = 0.0f;

		if (gameClearTimer_ < 10) {
			alpha = 1.0f - (float)gameClearTimer_ / 10.0f;
		}

		gameClearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, alpha});
		gameClearFlashSprite_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}

GameScene::~GameScene() {

	// カメラコントローラの解放
	delete CController_;

	// 自キャラの解放
	delete player_;

	// 敵の解放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	// マップチップフィールドの解放
	delete mapChipField_;

	// ブロックのワールド変換を解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// モデルデータの解放
	delete swordModel_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelBlock_;

	// HPバーの解放
	delete hpBarBg_;
	delete hpBarFg_;

	// スカイドームの解放
	delete skyDome_;
	delete modelSkyDome_;

	// スプライトの解放
	delete aSprite_;
	delete countdown3Sprite_;
	delete countdown2Sprite_;
	delete countdown1Sprite_;
	delete countdownGoSprite_;
	delete gameOverFadeSprite_;
	delete gameClearFlashSprite_;
}

void GameScene::GenerateBlocks() {

	// 既存のブロックを削除
	for (std::vector<WorldTransform*>& line : worldTransformBlocks_) {
		for (WorldTransform* block : line) {
			delete block;
		}
	}

	worldTransformBlocks_.clear();

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

void GameScene::GenerateEnemies() {

	// 既存の敵を削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	// 配置データから敵を生成
	for (const EnemyMapPosition& position : enemyMapPositions_) {

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(position.x, position.y);

		Enemy* enemy = new Enemy;

		enemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemy->SetMapChipField(mapChipField_);

		enemies_.push_back(enemy);
	}
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定
	{ // 判定対象1と2の座標
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

void GameScene::LoadPlayerPositionForStage() {

	// 初期位置
	playerMapX_ = 1;
	playerMapY_ = 12;

	// ステージごとの配置ファイル
	std::string filePath;

	if (stageNumber_ == 1) {
		filePath = "Resources/player1.csv";
	} else if (stageNumber_ == 2) {
		filePath = "Resources/player2.csv";
	} else {
		filePath = "Resources/player3.csv";
	}

	// 保存ファイルがあれば読み込む
	std::ifstream file(filePath);

	if (file.is_open()) {
		char comma;
		file >> playerMapX_ >> comma >> playerMapY_;
	}
}

void GameScene::SetEnemyPositionsForStage() {

	enemyMapPositions_.clear();

	if (stageNumber_ == 1) {

		enemyMapPositions_.push_back({6, 12});
		enemyMapPositions_.push_back({20, 12});
		enemyMapPositions_.push_back({40, 12});
		enemyMapPositions_.push_back({65, 12});
		enemyMapPositions_.push_back({88, 12});

	} else if (stageNumber_ == 2) {

		enemyMapPositions_.push_back({5, 12});
		enemyMapPositions_.push_back({18, 12});
		enemyMapPositions_.push_back({30, 12});
		enemyMapPositions_.push_back({42, 12});
		enemyMapPositions_.push_back({55, 12});
		enemyMapPositions_.push_back({72, 12});
		enemyMapPositions_.push_back({88, 12});

	} else if (stageNumber_ == 3) {

		enemyMapPositions_.push_back({5, 12});
		enemyMapPositions_.push_back({15, 12});
		enemyMapPositions_.push_back({25, 12});
		enemyMapPositions_.push_back({35, 12});
		enemyMapPositions_.push_back({45, 12});
		enemyMapPositions_.push_back({55, 12});
		enemyMapPositions_.push_back({65, 12});
		enemyMapPositions_.push_back({78, 12});
		enemyMapPositions_.push_back({90, 12});
	}

	
    // ステージごとの敵配置ファイル
	std::string filePath;

	if (stageNumber_ == 1) {
		filePath = "Resources/enemies1.csv";
	} else if (stageNumber_ == 2) {
		filePath = "Resources/enemies2.csv";
	} else {
		filePath = "Resources/enemies3.csv";
	}

	// 保存ファイルがあれば、そちらの配置を読み込む
	std::ifstream file(filePath);

	if (file.is_open()) {
		enemyMapPositions_.clear();

		int x;
		int y;
		char comma;

		while (file >> x >> comma >> y) {
			if (comma == ',') {
				enemyMapPositions_.push_back({x, y});
			}
		}	
	}
}

void GameScene::LoadFlagPositionForStage() {

	// 初期位置
	flagMapX_ = 97;
	flagMapY_ = 12;

	// ステージごとの配置ファイル
	std::string filePath;

	if (stageNumber_ == 1) {
		filePath = "Resources/flag1.csv";
	} else if (stageNumber_ == 2) {
		filePath = "Resources/flag2.csv";
	} else {
		filePath = "Resources/flag3.csv";
	}

	// 保存ファイルがあれば読み込む
	std::ifstream file(filePath);

	if (file.is_open()) {
		char comma;
		file >> flagMapX_ >> comma >> flagMapY_;
	}
}