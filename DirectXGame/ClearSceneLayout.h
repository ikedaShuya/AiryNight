#pragma once

class ClearSceneLayout {
public:
	// 初期レイアウトに戻す
	void Reset();

	// レイアウトを保存する
	void Save() const;

	// 保存したレイアウトを読み込む
	void Load();

	// ImGuiでレイアウトを編集する
	void EditWithImGui();

	// CLEARモデルのサイズ
	float GetClearTitleScale() const { return clearTitleScale_; }

	// ステージ表示
	const float (&GetResultStagePosition() const)[2] { return resultStagePosition_; }
	const float (&GetResultStageSize() const)[2] { return resultStageSize_; }
	const float (&GetStageNumberPosition() const)[2] { return stageNumberPosition_; }
	const float (&GetStageNumberSize() const)[2] { return stageNumberSize_; }

	// クリアタイム表示
	const float (&GetResultTimePosition() const)[2] { return resultTimePosition_; }
	const float (&GetResultTimeSize() const)[2] { return resultTimeSize_; }
	const float (&GetTimePosition() const)[4][2] { return timePosition_; }
	const float (&GetTimeSize() const)[2] { return timeSize_; }
	const float (&GetColonPosition() const)[2] { return colonPosition_; }
	const float (&GetColonPosition2() const)[2] { return colonPosition2_; }
	const float (&GetColonSize() const)[2] { return colonSize_; }

	// 敵撃破数表示
	const float (&GetResultDefeatedPosition() const)[2] { return resultDefeatedPosition_; }
	const float (&GetResultDefeatedSize() const)[2] { return resultDefeatedSize_; }
	const float (&GetDefeatedPosition() const)[2][2] { return defeatedPosition_; }
	const float (&GetDefeatedSize() const)[2] { return defeatedSize_; }

	// PRESS SPACE
	const float (&GetPressSpacePosition() const)[2] { return pressSpacePosition_; }
	const float (&GetPressSpaceSize() const)[2] { return pressSpaceSize_; }

private:
	// CLEARモデル
	float clearTitleScale_ = 5.0f;

	// ステージ表示
	float resultStagePosition_[2] = {400.0f, 100.0f};
	float resultStageSize_[2] = {160.0f, 40.0f};

	float stageNumberPosition_[2] = {590.0f, 80.0f};
	float stageNumberSize_[2] = {100.0f, 100.0f};

	// クリアタイム表示
	float resultTimePosition_[2] = {540.0f, 390.0f};
	float resultTimeSize_[2] = {200.0f, 50.0f};

	float timePosition_[4][2] = {
	    {480.0f, 455.0f},
        {550.0f, 455.0f},
        {680.0f, 455.0f},
        {750.0f, 455.0f}
    };
	float timeSize_[2] = {60.0f, 80.0f};

	float colonPosition_[2] = {625.0f, 480.0f};
	float colonPosition2_[2] = {625.0f, 505.0f};
	float colonSize_[2] = {8.0f, 8.0f};

	// 敵撃破数表示
	float resultDefeatedPosition_[2] = {480.0f, 545.0f};
	float resultDefeatedSize_[2] = {320.0f, 40.0f};

	float defeatedPosition_[2][2] = {
	    {570.0f, 590.0f},
        {640.0f, 590.0f}
    };
	float defeatedSize_[2] = {50.0f, 50.0f};

	// PRESS SPACE
	float pressSpacePosition_[2] = {448.0f, 620.0f};
	float pressSpaceSize_[2] = {384.0f, 60.0f};
};