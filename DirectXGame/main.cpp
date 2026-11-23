
#include "KamataEngine.h"
#include "SceneManager.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE3C_04_イケダ_シュウヤ_エアリーナイト");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// シーン管理クラス
	SceneManager sceneManager;
	sceneManager.Initialize();

	// メインループ
	while (true) {

		// エンジンの更新
		if (Update()) {
			break;
		}

#ifdef USE_IMGUI
		KamataEngine::ImGuiManager::GetInstance()->Begin();
#endif

		// シーン更新
		sceneManager.Update();

#ifdef USE_IMGUI
		KamataEngine::ImGuiManager::GetInstance()->End();
#endif

		// 描画開始
		dxCommon->PreDraw();

		// シーン描画
		sceneManager.Draw();

#ifdef USE_IMGUI
		KamataEngine::ImGuiManager::GetInstance()->Draw();
#endif

		// 描画終了
		dxCommon->PostDraw();
	}

	// シーンの終了処理
	sceneManager.Finalize();

	// エンジンの終了処理
	Finalize();

	return 0;
}