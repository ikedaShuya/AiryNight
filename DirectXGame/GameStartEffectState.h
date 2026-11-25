#pragma once

class GameStartEffect;

class GameStartEffectState {
public:
	virtual ~GameStartEffectState() = default;
	virtual void Update(GameStartEffect& effect) = 0;
};

class GameStartStagePreviewState : public GameStartEffectState {
public:
	void Update(GameStartEffect& effect) override;
};

class GameStartCountdownState : public GameStartEffectState {
public:
	void Update(GameStartEffect& effect) override;
};

class GameStartGoState : public GameStartEffectState {
public:
	void Update(GameStartEffect& effect) override;
};