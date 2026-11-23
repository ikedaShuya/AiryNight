#pragma once

#ifdef USE_IMGUI

class MapEditor {
public:
	// 編集中かどうか
	bool IsEditing() const { return isMapEditing_; }

	// 編集状態を設定
	void SetEditing(bool isEditing) { isMapEditing_ = isEditing; }

	// エディタのモードを取得
	int GetEditorMode() const { return editorMode_; }

	// エディタのモードを設定
	void SetEditorMode(int mode) { editorMode_ = mode; }

	// 配置するマップチップを取得
	int GetEditMapType() const { return editMapType_; }

	// 配置するマップチップを設定
	void SetEditMapType(int type) { editMapType_ = type; }

	// マップの拡大・縮小を取得
	float GetMapCellSize() const { return mapCellSize_; }

	// マップの拡大・縮小を設定
	void SetMapCellSize(float size) { mapCellSize_ = size; }

	// 範囲選択モードかどうか
	bool IsRangeSelectMode() const { return isRangeSelectMode_; }

	// 範囲選択モードを設定
	void SetRangeSelectMode(bool isSelecting) { isRangeSelectMode_ = isSelecting; }

	// 範囲選択中かどうか
	bool IsSelectingMapRange() const { return isSelectingMapRange_; }

	// 範囲選択中の状態を設定
	void SetSelectingMapRange(bool isSelecting) { isSelectingMapRange_ = isSelecting; }

	// 範囲選択の開始位置を設定
	void SetRangeStart(int x, int y) {
		rangeStartX_ = x;
		rangeStartY_ = y;
		rangeEndX_ = x;
		rangeEndY_ = y;
	}

	// 範囲選択の終了位置を設定
	void SetRangeEnd(int x, int y) {
		rangeEndX_ = x;
		rangeEndY_ = y;
	}

	// 範囲選択の座標を取得
	int GetRangeStartX() const { return rangeStartX_; }
	int GetRangeStartY() const { return rangeStartY_; }
	int GetRangeEndX() const { return rangeEndX_; }
	int GetRangeEndY() const { return rangeEndY_; }

	// 保存完了メッセージの時間を減らす
	void UpdateSaveMessageTimer() {
		if (saveMessageTimer_ > 0) {
			saveMessageTimer_--;
		}
	}

	// 保存完了メッセージを開始
	void StartSaveMessageTimer() { saveMessageTimer_ = 120; }

	// 保存完了メッセージを表示するか
	bool IsSaveMessageVisible() const { return saveMessageTimer_ > 0; }

private:
	// マップ編集中かどうか
	bool isMapEditing_ = true;

	// エディタの編集モード
	int editorMode_ = 0;

	// 配置するマップチップ
	int editMapType_ = 1;

	// マップの拡大・縮小
	float mapCellSize_ = 8.0f;

	// 保存完了メッセージ
	int saveMessageTimer_ = 0;

	// 範囲選択モード
	bool isRangeSelectMode_ = false;
	bool isSelectingMapRange_ = false;

	// 範囲選択の開始位置と終了位置
	int rangeStartX_ = 0;
	int rangeStartY_ = 0;
	int rangeEndX_ = 0;
	int rangeEndY_ = 0;
};

#endif