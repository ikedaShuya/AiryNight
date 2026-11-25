#pragma once

class GameScene;

// ゲームシーンの状態の基底クラス
class GameSceneState {
public:
	virtual ~GameSceneState() = default;
	virtual void Update(GameScene& gameScene) = 0;
};

// ゲーム開始演出中
class GameSceneStartState : public GameSceneState {
public:
	void Update(GameScene& gameScene) override;
};

// ゲームプレイ中
class GameScenePlayingState : public GameSceneState {
public:
	void Update(GameScene& gameScene) override;
};

// ゲームオーバー演出中
class GameSceneGameOverState : public GameSceneState {
public:
	void Update(GameScene& gameScene) override;
};

// ゲームクリア演出中
class GameSceneClearState : public GameSceneState {
public:
	void Update(GameScene& gameScene) override;
};