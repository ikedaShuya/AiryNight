#include "SceneFactory.h"

#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

Scene* AiryNightSceneFactory::CreateScene(SceneType sceneType, const SceneCreateData& data) {

	switch (sceneType) {

	case SceneType::kTitle: {
		TitleScene* scene = new TitleScene;
		scene->Initialize();
		return scene;
	}

	case SceneType::kGame: {
		GameScene* scene = new GameScene;
		scene->Initialize(data.stageNumber);
		return scene;
	}

	case SceneType::kClear: {
		ClearScene* scene = new ClearScene;
		scene->Initialize(data.stageNumber, data.playTimer, data.defeatedEnemyCount);
		return scene;
	}

	case SceneType::kGameOver: {
		GameOverScene* scene = new GameOverScene;
		scene->Initialize();
		return scene;
	}

	default:
		return nullptr;
	}
}