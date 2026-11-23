#include "ClearSceneLayout.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <cstdio>
#include <fstream>

// レイアウトをファイルに保存
void ClearSceneLayout::Save() const {

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
void ClearSceneLayout::Load() {

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
void ClearSceneLayout::Reset() {

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

// ImGuiでレイアウトを編集
void ClearSceneLayout::EditWithImGui() {

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
		Save();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load Layout")) {
		Load();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Layout")) {
		Reset();
	}

	ImGui::End();

#endif
}