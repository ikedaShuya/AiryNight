#pragma once

// シーンの基底クラス
class Scene {
public:
	// 更新
	void Update() { OnUpdate(); }

	// 描画
	void Draw() { OnDraw(); }

	// シーン終了判定
	virtual bool IsFinished() const = 0;

	// 仮想デストラクタ
	virtual ~Scene() = default;

protected:
	// 各シーン固有の更新処理
	virtual void OnUpdate() = 0;

	// 各シーン固有の描画処理
	virtual void OnDraw() = 0;
};