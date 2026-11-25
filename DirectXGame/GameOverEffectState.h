#pragma once

class GameOverEffect;

class GameOverEffectState {
public:
	virtual ~GameOverEffectState() = default;
	virtual void Update(GameOverEffect& effect) = 0;
};

// 赤い衝撃演出
class GameOverEffectFlashState : public GameOverEffectState {
public:
	void Update(GameOverEffect& effect) override;
};

// 黒い暗転演出
class GameOverEffectFadeState : public GameOverEffectState {
public:
	void Update(GameOverEffect& effect) override;
};