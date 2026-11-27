#include "ClearScene.h"

#include <cmath>

using namespace KamataEngine;

// クリア演出状態
void ClearSceneEffectState::Update(ClearScene& clearScene) {

	// クリア演出を更新
	float clearTitleScale = clearScene.clearSceneEffect_.Update(clearScene.layout_.GetClearTitleScale());

	clearScene.worldTransformTitle_.scale_ = {clearTitleScale, clearTitleScale, clearTitleScale};

	// フラッシュ演出が終わったらリザルト待機状態へ
	if (clearScene.clearSceneEffect_.IsFlashFinished()) {
		clearScene.pressSpaceSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		clearScene.ChangeState(std::make_unique<ClearSceneResultState>());
	}
}

// リザルト待機状態
void ClearSceneResultState::Update(ClearScene& clearScene) {

	// Spaceキーで次のシーンへ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		clearScene.finished_ = true;
	}
}

// 状態変更
void ClearScene::ChangeState(std::unique_ptr<ClearSceneState> newState) { nextState_ = std::move(newState); }

void ClearScene::Initialize(int stageNumber, int playTimer, int defeatedEnemyCount) {

	stageNumber_ = stageNumber;

	playTimer_ = playTimer;

	defeatedEnemyCount_ = defeatedEnemyCount;

	modelTitle_.reset(Model::CreateFromOBJ("clear", true));

	camera_.Initialize();

	worldTransformTitle_.Initialize();

	worldTransformTitle_.translation_.y = 5.0f;

	// 最初は小さいサイズにする
	worldTransformTitle_.scale_ = {0.1f, 0.1f, 0.1f};

	modelSkyDome_.reset(Model::CreateFromOBJ("sky", true));
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize(modelSkyDome_.get(), &camera_);

	// ステージ番号に対応した画像を選択
	const char* textureFileName = "countdown_0.png";

	switch (stageNumber_) {
	case 1:
		textureFileName = "countdown_1.png";
		break;
	case 2:
		textureFileName = "countdown_2.png";
		break;
	case 3:
		textureFileName = "countdown_3.png";
		break;
	}

	// エフェクトを基底クラスのポインタに登録
	effect_ = &clearSceneEffect_;

	// エフェクトの初期化
	effect_->Initialize();

	// クリア演出状態にする
	state_ = std::make_unique<ClearSceneEffectState>();

	// リザルト情報の背景パネル
	resultPanelSprite_.reset(Sprite::Create(TextureManager::Load("white1x1.png"), {390.0f, 300.0f}));
	resultPanelSprite_->SetSize({500.0f, 320.0f});
	resultPanelSprite_->SetColor({0.03f, 0.06f, 0.12f, 0.90f});

	// リザルト項目名のスプライトを生成
	resultStageSprite_.reset(Sprite::Create(TextureManager::Load("result_stage.png"), {400.0f, 100.0f}));
	resultStageSprite_->SetSize({160.0f, 40.0f});

	resultTimeSprite_.reset(Sprite::Create(TextureManager::Load("result_time.png"), {540.0f, 390.0f}));
	resultTimeSprite_->SetSize({200.0f, 50.0f});

	resultDefeatedSprite_.reset(Sprite::Create(TextureManager::Load("result_defeated.png"), {480.0f, 545.0f}));
	resultDefeatedSprite_->SetSize({320.0f, 50.0f});

	// PRESS SPACEのスプライトを生成
	pressSpaceSprite_.reset(Sprite::Create(TextureManager::Load("pressSpace.png"), {384.0f, 560.0f}));
	pressSpaceSprite_->SetSize({512.0f, 128.0f});
	pressSpaceSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

	// ステージ番号のスプライトを生成
	stageNumberSprite_.reset(Sprite::Create(TextureManager::Load(textureFileName), {590.0f, 80.0f}));

	// サイズを設定
	stageNumberSprite_->SetSize({100.0f, 100.0f});

	// プレイ時間を分と秒に変換
	int totalSeconds = playTimer_ / 60;

	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	// 99分を超えた場合は99分にする
	if (minutes > 99) {
		minutes = 99;
	}

	// 4桁の数字に分解
	timeDigits_[0] = minutes / 10;
	timeDigits_[1] = minutes % 10;
	timeDigits_[2] = seconds / 10;
	timeDigits_[3] = seconds % 10;

	// 数字のテクスチャ名
	const char* digitTextureNames[10] = {"countdown_0.png", "countdown_1.png", "countdown_2.png", "countdown_3.png", "countdown_4.png",
	                                     "countdown_5.png", "countdown_6.png", "countdown_7.png", "countdown_8.png", "countdown_9.png"};

	// 数字の表示位置
	float digitPositions[4] = {480.0f, 550.0f, 680.0f, 750.0f};

	// 数字スプライトを生成
	for (int i = 0; i < 4; i++) {
		timeSprites_[i].reset(Sprite::Create(TextureManager::Load(digitTextureNames[timeDigits_[i]]), {digitPositions[i], 455.0f}));

		timeSprites_[i]->SetSize({60.0f, 80.0f});
	}

	// コロンを白い四角2つで作る
	uint32_t colonTexture = TextureManager::Load("white1x1.png");

	timeColonSprite_.reset(Sprite::Create(colonTexture, {625.0f, 480.0f}));
	timeColonSprite_->SetSize({8.0f, 8.0f});

	timeColonDot2Sprite_.reset(Sprite::Create(colonTexture, {625.0f, 505.0f}));
	timeColonDot2Sprite_->SetSize({8.0f, 8.0f});

	// 敵の撃破数を2桁に分解
	int defeatedCount = defeatedEnemyCount_;

	if (defeatedCount > 99) {
		defeatedCount = 99;
	}

	defeatedCountDigits_[0] = defeatedCount / 10;
	defeatedCountDigits_[1] = defeatedCount % 10;

	// 撃破数の数字スプライトを生成
	for (int i = 0; i < 2; i++) {
		defeatedCountSprites_[i].reset(Sprite::Create(TextureManager::Load(digitTextureNames[defeatedCountDigits_[i]]), {570.0f + i * 70.0f, 600.0f}));

		defeatedCountSprites_[i]->SetSize({50.0f, 65.0f});
	}
	layout_.Load();
}

void ClearScene::OnUpdate() {

	skyDome_->Update();

	layout_.EditWithImGui();

	// レイアウト設定を取得
	const auto& resultStagePosition = layout_.GetResultStagePosition();
	const auto& resultStageSize = layout_.GetResultStageSize();
	const auto& stageNumberPosition = layout_.GetStageNumberPosition();
	const auto& stageNumberSize = layout_.GetStageNumberSize();

	const auto& resultTimePosition = layout_.GetResultTimePosition();
	const auto& resultTimeSize = layout_.GetResultTimeSize();
	const auto& timePosition = layout_.GetTimePosition();
	const auto& timeSize = layout_.GetTimeSize();

	const auto& colonPosition = layout_.GetColonPosition();
	const auto& colonPosition2 = layout_.GetColonPosition2();
	const auto& colonSize = layout_.GetColonSize();

	const auto& resultDefeatedPosition = layout_.GetResultDefeatedPosition();
	const auto& resultDefeatedSize = layout_.GetResultDefeatedSize();
	const auto& defeatedPosition = layout_.GetDefeatedPosition();
	const auto& defeatedSize = layout_.GetDefeatedSize();

	const auto& pressSpacePosition = layout_.GetPressSpacePosition();
	const auto& pressSpaceSize = layout_.GetPressSpaceSize();

	// 調整した位置とサイズを反映
	resultStageSprite_->SetPosition({resultStagePosition[0], resultStagePosition[1]});
	resultStageSprite_->SetSize({resultStageSize[0], resultStageSize[1]});

	stageNumberSprite_->SetPosition({stageNumberPosition[0], stageNumberPosition[1]});
	stageNumberSprite_->SetSize({stageNumberSize[0], stageNumberSize[1]});

	resultTimeSprite_->SetPosition({resultTimePosition[0], resultTimePosition[1]});
	resultTimeSprite_->SetSize({resultTimeSize[0], resultTimeSize[1]});

	for (int i = 0; i < 4; i++) {
		timeSprites_[i]->SetPosition({timePosition[i][0], timePosition[i][1]});
		timeSprites_[i]->SetSize({timeSize[0], timeSize[1]});
	}

	timeColonSprite_->SetPosition({colonPosition[0], colonPosition[1]});
	timeColonDot2Sprite_->SetPosition({colonPosition2[0], colonPosition2[1]});
	timeColonSprite_->SetSize({colonSize[0], colonSize[1]});
	timeColonDot2Sprite_->SetSize({colonSize[0], colonSize[1]});

	resultDefeatedSprite_->SetPosition({resultDefeatedPosition[0], resultDefeatedPosition[1]});
	resultDefeatedSprite_->SetSize({resultDefeatedSize[0], resultDefeatedSize[1]});

	for (int i = 0; i < 2; i++) {
		defeatedCountSprites_[i]->SetPosition({defeatedPosition[i][0], defeatedPosition[i][1]});
		defeatedCountSprites_[i]->SetSize({defeatedSize[0], defeatedSize[1]});
	}

	pressSpaceSprite_->SetPosition({pressSpacePosition[0], pressSpacePosition[1]});
	pressSpaceSprite_->SetSize({pressSpaceSize[0], pressSpaceSize[1]});

	// 現在の状態を更新
	if (state_) {
		state_->Update(*this);
	}

	// 状態を切り替える
	if (nextState_) {
		state_ = std::move(nextState_);
	}

	WorldTransformUpdate(worldTransformTitle_);
}

void ClearScene::OnDraw() {

	Model::PreDraw();

	skyDome_->Draw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();

	Sprite::PreDraw();

	// エフェクトの描画
	effect_->Draw();

	resultPanelSprite_->Draw();

	stageNumberSprite_->Draw();

	// リザルト項目名を描画
	resultStageSprite_->Draw();
	resultTimeSprite_->Draw();
	resultDefeatedSprite_->Draw();
	pressSpaceSprite_->Draw();

	// プレイ時間の数字を描画
	for (int i = 0; i < 4; i++) {
		timeSprites_[i]->Draw();
	}

	// 敵の撃破数を描画
	for (int i = 0; i < 2; i++) {
		defeatedCountSprites_[i]->Draw();
	}

	// コロンを描画
	timeColonSprite_->Draw();
	timeColonDot2Sprite_->Draw();

	Sprite::PostDraw();
}