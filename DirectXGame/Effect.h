#pragma once

// エフェクトの基底クラス
class Effect {
public:
	// 初期化
	virtual void Initialize() = 0;

	// 描画
	virtual void Draw() = 0;

	// 仮想デストラクタ
	virtual ~Effect() = default;
};