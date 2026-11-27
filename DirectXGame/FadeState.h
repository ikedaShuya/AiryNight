#pragma once

class Fade;

// フェード状態の基底クラス
class FadeState {
public:
	virtual ~FadeState() = default;

	// 状態更新。終了したら true
	virtual bool Update(Fade& fade) = 0;

	// フェードイン状態かどうか
	virtual bool IsFadeIn() const { return false; }
};

// フェードアウト状態
class FadeOutState : public FadeState {
public:
	bool Update(Fade& fade) override;
};

// フェードイン状態
class FadeInState : public FadeState {
public:
	bool Update(Fade& fade) override;

	bool IsFadeIn() const override { return true; }
};