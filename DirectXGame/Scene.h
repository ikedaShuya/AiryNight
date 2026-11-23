#pragma once

// シーンの基底クラス
class Scene {
public:
	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// シーン終了判定
	virtual bool IsFinished() const = 0;

	// 仮想デストラクタ
	virtual ~Scene() = default;
};