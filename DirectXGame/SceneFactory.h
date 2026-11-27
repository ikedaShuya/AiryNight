#pragma once

#include "Scene.h"
#include <memory>

// シーンの種類
enum class SceneType {
	kUnknown = 0,
	kTitle,
	kGame,
	kClear,
	kGameOver,
};

// シーン生成時に渡す情報
struct SceneCreateData {
	int stageNumber = 1;
	int playTimer = 0;
	int defeatedEnemyCount = 0;
};

// シーン生成を担当する基底クラス
class SceneFactory {
public:
	virtual std::unique_ptr<Scene> CreateScene(SceneType sceneType, const SceneCreateData& data) = 0;

	virtual ~SceneFactory() = default;
};

// AiryNight用のシーン生成クラス
class AiryNightSceneFactory : public SceneFactory {
public:
	std::unique_ptr<Scene> CreateScene(SceneType sceneType, const SceneCreateData& data) override;
};