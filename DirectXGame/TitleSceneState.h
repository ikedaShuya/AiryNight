#pragma once

class TitleScene;

// タイトルシーンの状態
class TitleSceneState {
public:
	virtual ~TitleSceneState() = default;
	virtual void Update(TitleScene& titleScene) = 0;
};

// タイトル待機状態
class TitleSceneIdleState : public TitleSceneState {
public:
	void Update(TitleScene& titleScene) override;
};

// ゲーム開始状態
class TitleSceneStartingState : public TitleSceneState {
public:
	void Update(TitleScene& titleScene) override;
};