#pragma once

#include "KamataEngine.h"
#include "Math.h"

// キャラクターの基底クラス
class Character {
public:
	// 描画
	virtual void Draw() = 0;

	// ワールド座標を取得
	virtual KamataEngine::Vector3 GetWorldPosition() const = 0;

	// 当たり判定を取得
	virtual AABB GetAABB() const = 0;

	// 仮想デストラクタ
	virtual ~Character() = default;
};