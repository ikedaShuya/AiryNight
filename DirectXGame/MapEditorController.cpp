#include "MapEditorController.h"

#ifdef USE_IMGUI
#include <imgui.h>

using namespace KamataEngine;

void MapEditorController::Update(
    MapEditor& mapEditor, int& stageNumber, StagePlacementData& placementData, StageDataLoader& dataLoader, StageMapLoader& mapLoader, StagePlacementDataSaver& dataSaver, BlockManager& blockManager,
    EnemyManager& enemyManager, Goal& goal, Player* player, MapChipField* mapChipField, CameraController* cameraController) {

	ImGui::Begin("Map Editor");

	bool isEditing = mapEditor.IsEditing();

	if (ImGui::Checkbox("Edit Mode", &isEditing)) {
		mapEditor.SetEditing(isEditing);
	}

	// ステージ切り替え
	const char* stages[] = {"Stage 1", "Stage 2", "Stage 3"};

	int selectedStage = stageNumber - 1;

	if (ImGui::Combo("Stage", &selectedStage, stages, 3)) {

		stageNumber = selectedStage + 1;

		mapLoader.LoadMap(stageNumber, *mapChipField);
		blockManager.Generate(mapChipField);

		dataLoader.LoadFlagPosition(stageNumber, placementData);
		goal.SetPosition(mapChipField->GetMapChipPositionByIndex(placementData.GetFlagX(), placementData.GetFlagY()));

		dataLoader.LoadEnemyPositions(stageNumber, placementData);
		enemyManager.Generate(placementData, mapChipField);

		dataLoader.LoadPlayerPosition(stageNumber, placementData);

		Vector3 playerPosition = mapChipField->GetMapChipPositionByIndex(placementData.GetPlayerX(), placementData.GetPlayerY());

		player->SetPosition(playerPosition);
		cameraController->Reset();
	}

	// 編集モード
	ImGui::Text("Editor Mode");

	int editorMode = mapEditor.GetEditorMode();
	int editMapType = mapEditor.GetEditMapType();

	ImGui::RadioButton("Map Mode", &editorMode, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Enemy Mode", &editorMode, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Player Mode", &editorMode, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Flag Mode", &editorMode, 3);

	mapEditor.SetEditorMode(editorMode);

	ImGui::Separator();

	if (editorMode == 0) {
		ImGui::Text("Tool");

		ImGui::RadioButton("Empty", &editMapType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Block", &editMapType, 1);

		mapEditor.SetEditMapType(editMapType);
	}

	ImGui::Separator();

	// 範囲選択
	bool isRangeSelectMode = mapEditor.IsRangeSelectMode();

	if (ImGui::Checkbox("Range Select", &isRangeSelectMode)) {
		mapEditor.SetRangeSelectMode(isRangeSelectMode);
	}

	ImGui::Text("Drag to select an area and apply the selected tool");
	ImGui::Separator();

	// マップの拡大・縮小
	ImGui::Text("Map Zoom");

	float mapCellSize = mapEditor.GetMapCellSize();

	if (ImGui::SliderFloat("Cell Size", &mapCellSize, 4.0f, 16.0f, "%.1f")) {
		mapEditor.SetMapCellSize(mapCellSize);
	}

	ImGui::Separator();

	float cellSize = mapEditor.GetMapCellSize();

	float mapWidth = mapChipField->GetNumBlockHorizontal() * cellSize;
	float mapHeight = mapChipField->GetNumBlockVertical() * cellSize;

	ImGui::BeginChild("MapCanvasArea", ImVec2(0, 190), true, ImGuiWindowFlags_HorizontalScrollbar);

	ImVec2 mapStart = ImGui::GetCursorScreenPos();

	ImGui::InvisibleButton("MapCanvas", ImVec2(mapWidth, mapHeight));

	bool isHovered = ImGui::IsItemHovered();

	ImVec2 mousePosition = ImGui::GetIO().MousePos;

	int mouseX = static_cast<int>((mousePosition.x - mapStart.x) / cellSize);
	int mouseY = static_cast<int>((mousePosition.y - mapStart.y) / cellSize);

	if (mouseX < 0) {
		mouseX = 0;
	}
	if (mouseX >= static_cast<int>(mapChipField->GetNumBlockHorizontal())) {
		mouseX = static_cast<int>(mapChipField->GetNumBlockHorizontal()) - 1;
	}
	if (mouseY < 0) {
		mouseY = 0;
	}
	if (mouseY >= static_cast<int>(mapChipField->GetNumBlockVertical())) {
		mouseY = static_cast<int>(mapChipField->GetNumBlockVertical()) - 1;
	}

	// 敵配置
	if (editorMode == 1) {

		if (isHovered && ImGui::IsMouseClicked(0)) {
			if (!placementData.HasEnemyPosition(mouseX, mouseY)) {
				placementData.AddEnemyPosition(mouseX, mouseY);
				enemyManager.Generate(placementData, mapChipField);
			}
		}

		if (isHovered && ImGui::IsMouseClicked(1)) {
			placementData.RemoveEnemyPosition(mouseX, mouseY);
			enemyManager.Generate(placementData, mapChipField);
		}
	}

	// プレイヤー配置
	if (editorMode == 2) {

		if (isHovered && ImGui::IsMouseClicked(0)) {

			placementData.SetPlayerPosition(mouseX, mouseY);

			Vector3 playerPosition = mapChipField->GetMapChipPositionByIndex(placementData.GetPlayerX(), placementData.GetPlayerY());

			player->SetPosition(playerPosition);
			cameraController->Reset();
		}
	}

	// ゴール配置
	if (editorMode == 3) {

		if (isHovered && ImGui::IsMouseClicked(0)) {

			placementData.SetFlagPosition(mouseX, mouseY);

			goal.SetPosition(mapChipField->GetMapChipPositionByIndex(placementData.GetFlagX(), placementData.GetFlagY()));
		}
	}

	// マップ編集
	if (editorMode == 0) {

		if (mapEditor.IsRangeSelectMode()) {

			if (isHovered && ImGui::IsMouseClicked(0)) {
				mapEditor.SetSelectingMapRange(true);
				mapEditor.SetRangeStart(mouseX, mouseY);
			}

			if (mapEditor.IsSelectingMapRange() && ImGui::IsMouseDown(0)) {
				mapEditor.SetRangeEnd(mouseX, mouseY);
			}

			if (mapEditor.IsSelectingMapRange() && ImGui::IsMouseReleased(0)) {

				mapEditor.SetRangeEnd(mouseX, mouseY);

				int minX = mapEditor.GetRangeStartX();
				int maxX = mapEditor.GetRangeEndX();
				int minY = mapEditor.GetRangeStartY();
				int maxY = mapEditor.GetRangeEndY();

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

				if (editMapType == 1) {
					type = MapChipType::kBlock;
				}

				for (int y = minY; y <= maxY; y++) {
					for (int x = minX; x <= maxX; x++) {
						mapChipField->SetMapChipTypeByIndex(x, y, type);
					}
				}

				blockManager.Generate(mapChipField);
				mapEditor.SetSelectingMapRange(false);
			}

		} else {

			if (isHovered && ImGui::IsMouseDown(0)) {

				MapChipType type = MapChipType::kBlank;

				if (editMapType == 1) {
					type = MapChipType::kBlock;
				}

				if (mapChipField->GetMapChipTypeByIndex(mouseX, mouseY) != type) {

					mapChipField->SetMapChipTypeByIndex(mouseX, mouseY, type);
					blockManager.Generate(mapChipField);
				}
			}
		}
	}

	// マップ描画
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	for (uint32_t y = 0; y < mapChipField->GetNumBlockVertical(); y++) {

		for (uint32_t x = 0; x < mapChipField->GetNumBlockHorizontal(); x++) {

			ImVec2 p1 = {mapStart.x + x * cellSize, mapStart.y + y * cellSize};
			ImVec2 p2 = {p1.x + cellSize, p1.y + cellSize};

			ImU32 color;

			if (mapChipField->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock) {
				color = IM_COL32(90, 130, 190, 255);
			} else {
				color = IM_COL32(45, 55, 65, 255);
			}

			drawList->AddRectFilled(p1, p2, color);
			drawList->AddRect(p1, p2, IM_COL32(100, 110, 120, 255));
		}
	}

	// 敵の配置位置
	for (const StagePlacementData::EnemyPosition& position : placementData.GetEnemyPositions()) {

		ImVec2 center = {mapStart.x + (position.x + 0.5f) * cellSize, mapStart.y + (position.y + 0.5f) * cellSize};

		drawList->AddCircleFilled(center, cellSize * 0.4f, IM_COL32(255, 70, 70, 255));
	}

	// プレイヤーの配置位置
	ImVec2 playerCenter = {mapStart.x + (placementData.GetPlayerX() + 0.5f) * cellSize, mapStart.y + (placementData.GetPlayerY() + 0.5f) * cellSize};

	drawList->AddCircleFilled(playerCenter, cellSize * 0.45f, IM_COL32(50, 150, 255, 255));

	// ゴールの配置位置
	ImVec2 flagCenter = {mapStart.x + (placementData.GetFlagX() + 0.5f) * cellSize, mapStart.y + (placementData.GetFlagY() + 0.5f) * cellSize};

	drawList->AddCircleFilled(flagCenter, cellSize * 0.45f, IM_COL32(255, 220, 0, 255));

	// 範囲選択中の枠
	if (mapEditor.IsSelectingMapRange()) {

		int minX = mapEditor.GetRangeStartX();
		int maxX = mapEditor.GetRangeEndX();
		int minY = mapEditor.GetRangeStartY();
		int maxY = mapEditor.GetRangeEndY();

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

	// 保存メッセージ
	mapEditor.UpdateSaveMessageTimer();

	if (ImGui::Button("Save")) {

		if (stageNumber == 1) {
			mapChipField->SaveMapChipCsv("Resources/blocks.csv");
		} else if (stageNumber == 2) {
			mapChipField->SaveMapChipCsv("Resources/blocks2.csv");
		} else {
			mapChipField->SaveMapChipCsv("Resources/blocks3.csv");
		}

		if (dataSaver.Save(stageNumber, placementData)) {
			mapEditor.StartSaveMessageTimer();
		}

		if (mapEditor.IsSaveMessageVisible()) {
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.3f, 1.0f), "Saved!");
		}
	}

	ImGui::End();
}

#endif