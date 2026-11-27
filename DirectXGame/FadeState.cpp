#include "FadeState.h"
#include "Fade.h"

// フェードアウト状態
bool FadeOutState::Update(Fade& fade) { return fade.AdvanceFadeOut(); }

// フェードイン状態
bool FadeInState::Update(Fade& fade) { return fade.AdvanceFadeIn(); }