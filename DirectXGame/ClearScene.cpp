#include "ClearScene.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <cstdio>
#include <fstream>
#include <cmath>

using namespace KamataEngine;

void ClearScene::Initialize(int stageNumber, int playTimer, int defeatedEnemyCount) {

	stageNumber_ = stageNumber;

	playTimer_ = playTimer;

	defeatedEnemyCount_ = defeatedEnemyCount;

	modelTitle_ = Model::CreateFromOBJ("clear", true);

	camera_.Initialize();

	worldTransformTitle_.Initialize();

	worldTransformTitle_.translation_.y = 5.0f;

	// 最初は小さいサイズにする
	worldTransformTitle_.scale_ = {0.1f, 0.1f, 0.1f};

	modelSkyDome_ = Model::CreateFromOBJ("sky", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &camera_);

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

	// 画面全体を覆う白いスプライト
	clearFlashSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {0.0f, 0.0f});

	// リザルト情報の背景パネル
	resultPanelSprite_ = Sprite::Create(TextureManager::Load("white1x1.png"), {390.0f, 300.0f});
	resultPanelSprite_->SetSize({500.0f, 320.0f});
	resultPanelSprite_->SetColor({0.03f, 0.06f, 0.12f, 0.90f});

	// CLEAR登場時の粒子を生成
	uint32_t particleTexture = TextureManager::Load("white1x1.png");

	for (int i = 0; i < kClearParticleCount; i++) {

		clearParticles_[i] = Sprite::Create(particleTexture, {634.0f, 354.0f});

		// 粒子の大きさ
		clearParticles_[i]->SetSize({24.0f, 24.0f});

		// 最初は透明にしておく
		clearParticles_[i]->SetColor({1.0f, 0.8f, 0.3f, 0.0f});

		// 粒子が飛ぶ方向と速度
		float angle = (2.0f * std::numbers::pi_v<float> * i) / kClearParticleCount;

		float speed = 5.0f + (i % 4) * 1.5f;

		particleVelocityX_[i] = std::cos(angle) * speed;
		particleVelocityY_[i] = std::sin(angle) * speed;
	}

	clearFlashSprite_->SetSize({1280.0f, 720.0f});
	clearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

	// リザルト項目名のスプライトを生成
	resultStageSprite_ = Sprite::Create(TextureManager::Load("result_stage.png"), {400.0f, 100.0f});
	resultStageSprite_->SetSize({160.0f, 40.0f});

	resultTimeSprite_ = Sprite::Create(TextureManager::Load("result_time.png"), {540.0f, 390.0f});
	resultTimeSprite_->SetSize({200.0f, 50.0f});

	resultDefeatedSprite_ = Sprite::Create(TextureManager::Load("result_defeated.png"), {480.0f, 545.0f});
	resultDefeatedSprite_->SetSize({320.0f, 50.0f});

	// PRESS SPACEのスプライトを生成
	pressSpaceSprite_ = Sprite::Create(TextureManager::Load("pressSpace.png"), {384.0f, 560.0f});
	pressSpaceSprite_->SetSize({512.0f, 128.0f});
	pressSpaceSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

	// ステージ番号のスプライトを生成
	stageNumberSprite_ = Sprite::Create(TextureManager::Load(textureFileName), {590.0f, 80.0f});

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
		timeSprites_[i] = Sprite::Create(TextureManager::Load(digitTextureNames[timeDigits_[i]]), {digitPositions[i], 455.0f});

		timeSprites_[i]->SetSize({60.0f, 80.0f});
	}

	// コロンを白い四角2つで作る
	uint32_t colonTexture = TextureManager::Load("white1x1.png");

	timeColonSprite_ = Sprite::Create(colonTexture, {625.0f, 480.0f});
	timeColonSprite_->SetSize({8.0f, 8.0f});

	timeColonDot2Sprite_ = Sprite::Create(colonTexture, {625.0f, 505.0f});
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
		defeatedCountSprites_[i] = Sprite::Create(TextureManager::Load(digitTextureNames[defeatedCountDigits_[i]]), {570.0f + i * 70.0f, 600.0f});

		defeatedCountSprites_[i]->SetSize({50.0f, 65.0f});
	}
	LoadLayout();
}

// レイアウトをファイルに保存
void ClearScene::SaveLayout() {

	std::ofstream file("ClearSceneLayout.txt");

	if (!file.is_open()) {
		return;
	}

	file << clearTitleScale_ << '\n';

	for (int i = 0; i < 2; i++) {
		file << resultStagePosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << resultStageSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << stageNumberPosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << stageNumberSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << resultTimePosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << resultTimeSize_[i] << '\n';
	}

	for (int i = 0; i < 4; i++) {
		file << timePosition_[i][0] << '\n';
		file << timePosition_[i][1] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << timeSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << colonPosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << colonPosition2_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << colonSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << resultDefeatedPosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << resultDefeatedSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << defeatedPosition_[i][0] << '\n';
		file << defeatedPosition_[i][1] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << defeatedSize_[i] << '\n';
	}

	for (int i = 0; i < 2; i++) {
		file << pressSpacePosition_[i] << '\n';
	}
	for (int i = 0; i < 2; i++) {
		file << pressSpaceSize_[i] << '\n';
	}
}

// 保存したレイアウトを読み込む
void ClearScene::LoadLayout() {

	std::ifstream file("ClearSceneLayout.txt");

	if (!file.is_open()) {
		return;
	}

	file >> clearTitleScale_;

	for (int i = 0; i < 2; i++) {
		file >> resultStagePosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> resultStageSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> stageNumberPosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> stageNumberSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> resultTimePosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> resultTimeSize_[i];
	}

	for (int i = 0; i < 4; i++) {
		file >> timePosition_[i][0];
		file >> timePosition_[i][1];
	}
	for (int i = 0; i < 2; i++) {
		file >> timeSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> colonPosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> colonPosition2_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> colonSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> resultDefeatedPosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> resultDefeatedSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> defeatedPosition_[i][0];
		file >> defeatedPosition_[i][1];
	}
	for (int i = 0; i < 2; i++) {
		file >> defeatedSize_[i];
	}

	for (int i = 0; i < 2; i++) {
		file >> pressSpacePosition_[i];
	}
	for (int i = 0; i < 2; i++) {
		file >> pressSpaceSize_[i];
	}
}

// レイアウトを初期状態に戻す
void ClearScene::ResetLayout() {

	clearTitleScale_ = 5.0f;

	resultStagePosition_[0] = 400.0f;
	resultStagePosition_[1] = 100.0f;
	resultStageSize_[0] = 160.0f;
	resultStageSize_[1] = 40.0f;

	stageNumberPosition_[0] = 590.0f;
	stageNumberPosition_[1] = 80.0f;
	stageNumberSize_[0] = 100.0f;
	stageNumberSize_[1] = 100.0f;

	resultTimePosition_[0] = 540.0f;
	resultTimePosition_[1] = 390.0f;
	resultTimeSize_[0] = 200.0f;
	resultTimeSize_[1] = 50.0f;

	timePosition_[0][0] = 480.0f;
	timePosition_[0][1] = 455.0f;
	timePosition_[1][0] = 550.0f;
	timePosition_[1][1] = 455.0f;
	timePosition_[2][0] = 680.0f;
	timePosition_[2][1] = 455.0f;
	timePosition_[3][0] = 750.0f;
	timePosition_[3][1] = 455.0f;
	timeSize_[0] = 60.0f;
	timeSize_[1] = 80.0f;

	colonPosition_[0] = 625.0f;
	colonPosition_[1] = 480.0f;
	colonPosition2_[0] = 625.0f;
	colonPosition2_[1] = 505.0f;
	colonSize_[0] = 8.0f;
	colonSize_[1] = 8.0f;

	resultDefeatedPosition_[0] = 480.0f;
	resultDefeatedPosition_[1] = 545.0f;
	resultDefeatedSize_[0] = 320.0f;
	resultDefeatedSize_[1] = 40.0f;

	defeatedPosition_[0][0] = 570.0f;
	defeatedPosition_[0][1] = 590.0f;
	defeatedPosition_[1][0] = 640.0f;
	defeatedPosition_[1][1] = 590.0f;
	defeatedSize_[0] = 50.0f;
	defeatedSize_[1] = 50.0f;

	pressSpacePosition_[0] = 448.0f;
	pressSpacePosition_[1] = 620.0f;
	pressSpaceSize_[0] = 384.0f;
	pressSpaceSize_[1] = 60.0f;
}

void ClearScene::Update() {

	if (clearFlashTimer_ >= 90 && Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	skyDome_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("ClearScene Result Editor");

	// CLEARモデル
	ImGui::DragFloat("CLEAR Scale", &clearTitleScale_, 0.05f, 0.1f, 20.0f);

	// ステージ表示
	if (ImGui::CollapsingHeader("Stage", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat2("Stage Text Position", resultStagePosition_, 1.0f);
		ImGui::DragFloat2("Stage Text Size", resultStageSize_, 1.0f);

		ImGui::DragFloat2("Stage Number Position", stageNumberPosition_, 1.0f);
		ImGui::DragFloat2("Stage Number Size", stageNumberSize_, 1.0f);
	}

	// クリアタイム表示
	if (ImGui::CollapsingHeader("Time", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat2("Time Text Position", resultTimePosition_, 1.0f);
		ImGui::DragFloat2("Time Text Size", resultTimeSize_, 1.0f);

		for (int i = 0; i < 4; i++) {
			char label[32];
			sprintf_s(label, "Time Digit %d Position", i + 1);
			ImGui::DragFloat2(label, timePosition_[i], 1.0f);
		}

		ImGui::DragFloat2("Time Digit Size", timeSize_, 1.0f);

		ImGui::DragFloat2("Colon Position", colonPosition_, 1.0f);
		ImGui::DragFloat2("Colon Second Position", colonPosition2_, 1.0f);
		ImGui::DragFloat2("Colon Size", colonSize_, 1.0f);
	}

	// 敵撃破数表示
	if (ImGui::CollapsingHeader("Defeated", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat2("Defeated Text Position", resultDefeatedPosition_, 1.0f);
		ImGui::DragFloat2("Defeated Text Size", resultDefeatedSize_, 1.0f);

		ImGui::DragFloat2("Defeated Digit 1 Position", defeatedPosition_[0], 1.0f);
		ImGui::DragFloat2("Defeated Digit 2 Position", defeatedPosition_[1], 1.0f);
		ImGui::DragFloat2("Defeated Digit Size", defeatedSize_, 1.0f);
	}

	// PRESS SPACE
	if (ImGui::CollapsingHeader("Press Space")) {
		ImGui::DragFloat2("Press Space Position", pressSpacePosition_, 1.0f);
		ImGui::DragFloat2("Press Space Size", pressSpaceSize_, 1.0f);
	}

	// レイアウト保存・読み込み
	ImGui::Separator();

	if (ImGui::Button("Save Layout")) {
		SaveLayout();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load Layout")) {
		LoadLayout();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Layout")) {
		ResetLayout();
	}

	ImGui::End();
#endif

	// 調整した位置とサイズを反映
	resultStageSprite_->SetPosition({resultStagePosition_[0], resultStagePosition_[1]});
	resultStageSprite_->SetSize({resultStageSize_[0], resultStageSize_[1]});

	stageNumberSprite_->SetPosition({stageNumberPosition_[0], stageNumberPosition_[1]});
	stageNumberSprite_->SetSize({stageNumberSize_[0], stageNumberSize_[1]});

	resultTimeSprite_->SetPosition({resultTimePosition_[0], resultTimePosition_[1]});
	resultTimeSprite_->SetSize({resultTimeSize_[0], resultTimeSize_[1]});

	for (int i = 0; i < 4; i++) {
		timeSprites_[i]->SetPosition({timePosition_[i][0], timePosition_[i][1]});
		timeSprites_[i]->SetSize({timeSize_[0], timeSize_[1]});
	}

	timeColonSprite_->SetPosition({colonPosition_[0], colonPosition_[1]});
	timeColonDot2Sprite_->SetPosition({colonPosition2_[0], colonPosition2_[1]});
	timeColonSprite_->SetSize({colonSize_[0], colonSize_[1]});
	timeColonDot2Sprite_->SetSize({colonSize_[0], colonSize_[1]});

	resultDefeatedSprite_->SetPosition({resultDefeatedPosition_[0], resultDefeatedPosition_[1]});
	resultDefeatedSprite_->SetSize({resultDefeatedSize_[0], resultDefeatedSize_[1]});

	for (int i = 0; i < 2; i++) {
		defeatedCountSprites_[i]->SetPosition({defeatedPosition_[i][0], defeatedPosition_[i][1]});
		defeatedCountSprites_[i]->SetSize({defeatedSize_[0], defeatedSize_[1]});
	}

	pressSpaceSprite_->SetPosition({pressSpacePosition_[0], pressSpacePosition_[1]});
	pressSpaceSprite_->SetSize({pressSpaceSize_[0], pressSpaceSize_[1]});

	// CLEAR文字の登場アニメーション
	if (clearTitleTimer_ < kClearTitleAnimationFrame) {
		clearTitleTimer_++;

		float t = static_cast<float>(clearTitleTimer_) / kClearTitleAnimationFrame;

		float scale = 5.0f;

		if (t < 0.8f) {
			// 最初は勢いよく大きくする
			float ease = t / 0.8f;
			scale = 0.1f + 5.4f * ease;
		} else {
			// 最後に少し縮んで落ち着く
			float ease = (t - 0.8f) / 0.2f;
			scale = 5.5f - 0.5f * ease;
		}

		worldTransformTitle_.scale_ = {scale, scale, scale};
	}

	if (clearTitleTimer_ >= kClearTitleAnimationFrame) {
		worldTransformTitle_.scale_ = {clearTitleScale_, clearTitleScale_, clearTitleScale_};
	}

	// CLEAR登場後に粒子を発生させる
	if (clearTitleTimer_ >= kClearTitleAnimationFrame && !clearParticlesStarted_) {
		clearParticlesStarted_ = true;
	}

	// 粒子の移動と消滅
	if (clearParticlesStarted_ && clearParticleTimer_ < 60) {

		clearParticleTimer_++;

		for (int i = 0; i < kClearParticleCount; i++) {

			float x = 640.0f + particleVelocityX_[i] * clearParticleTimer_;
			float y = 360.0f + particleVelocityY_[i] * clearParticleTimer_;

			clearParticles_[i]->SetPosition({x, y});

			float alpha = 1.0f - static_cast<float>(clearParticleTimer_) / 60.0f;

			float size = 24.0f * alpha;

			clearParticles_[i]->SetSize({size, size});
			clearParticles_[i]->SetColor({1.0f, 0.8f, 0.3f, alpha});
		}
	}

	// CLEAR登場後にフラッシュを開始
	if (clearTitleTimer_ >= kClearTitleAnimationFrame && !clearFlashStarted_) {
		clearFlashStarted_ = true;
	}

	// クリア時のフラッシュ
	if (clearFlashStarted_ && clearFlashTimer_ < 90) {

		clearFlashTimer_++;

		float alpha = 0.0f;

		// 最初の15フレームで白い光を消す
		if (clearFlashTimer_ <= 15) {
			alpha = 1.0f - static_cast<float>(clearFlashTimer_) / 15.0f;
		}

		clearFlashSprite_->SetColor({1.0f, 1.0f, 1.0f, alpha});
	}

	// クリア演出が終わったらPRESS SPACEを表示
	if (clearFlashTimer_ >= 90) {
		pressSpaceSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}

	WorldTransformUpdate(worldTransformTitle_);
}

void ClearScene::Draw() {

	Model::PreDraw();

	skyDome_->Draw();

	modelTitle_->Draw(worldTransformTitle_, camera_);

	Model::PostDraw();

	Sprite::PreDraw();

	clearFlashSprite_->Draw();

	// CLEAR登場時の粒子を描画
	if (clearParticlesStarted_) {
		for (int i = 0; i < kClearParticleCount; i++) {
			clearParticles_[i]->Draw();
		}
	}

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

ClearScene::~ClearScene() {
	delete modelTitle_;
	delete skyDome_;
	delete modelSkyDome_;
	delete stageNumberSprite_;

	// プレイ時間表示用スプライトを解放
	for (int i = 0; i < 4; i++) {
		delete timeSprites_[i];
	}

	delete timeColonSprite_;
	delete timeColonDot2Sprite_;

	// 敵の撃破数表示用スプライトを解放
	for (int i = 0; i < 2; i++) {
		delete defeatedCountSprites_[i];
	}

	delete clearFlashSprite_;
	delete resultPanelSprite_;

	delete resultStageSprite_;
	delete resultTimeSprite_;
	delete resultDefeatedSprite_;
	delete pressSpaceSprite_;
}