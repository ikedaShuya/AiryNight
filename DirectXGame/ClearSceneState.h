#pragma once

class ClearScene;

// クリアシーンの状態
class ClearSceneState {
public:
	virtual ~ClearSceneState() = default;
	virtual void Update(ClearScene& clearScene) = 0;
};

// クリア演出状態
class ClearSceneEffectState : public ClearSceneState {
public:
	void Update(ClearScene& clearScene) override;
};

// リザルト待機状態
class ClearSceneResultState : public ClearSceneState {
public:
	void Update(ClearScene& clearScene) override;
};