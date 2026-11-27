#include "SceneFactory.h"

#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

std::unique_ptr<Scene> AiryNightSceneFactory::CreateScene(SceneType sceneType, const SceneCreateData& data) {

	switch (sceneType) {

	case SceneType::kTitle: {
		auto scene = std::make_unique<TitleScene>();
		scene->Initialize();
		return scene;
	}

	case SceneType::kGame: {
		auto scene = std::make_unique<GameScene>();
		scene->Initialize(data.stageNumber);
		return scene;
	}

	case SceneType::kClear: {
		auto scene = std::make_unique<ClearScene>();
		scene->Initialize(data.stageNumber, data.playTimer, data.defeatedEnemyCount);
		return scene;
	}

	case SceneType::kGameOver: {
		auto scene = std::make_unique<GameOverScene>();
		scene->Initialize();
		return scene;
	}

	default:
		return nullptr;
	}
}