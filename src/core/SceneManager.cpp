#include "core/SceneManager.h"
#include "scenes/MainMenuScene.h"
#include "scenes/SplashScene.h"
#include "scenes/InventoryScene.h"
#include "scenes/AcademyScene.h"
#include "scenes/BattleScene.h"
#include "scenes/MissionRankScene.h"
#include "types.h"
#include <memory>

SceneManager::SceneManager(GameContext ctx)
    : context_(ctx), activeScene_(nullptr), activeSceneType_(SceneType::None) {
  ChangeScene(SceneType::Splash);
}

void SceneManager::ChangeScene(SceneType type) {
  if (type == SceneType::None || type == SceneType::Exit) {
    activeSceneType_ = type;
    return;
  }

  if (activeScene_) {
    activeScene_->OnExit();
    activeScene_.reset();
  }

  activeSceneType_ = type;

  switch (type) {
  case SceneType::Splash:
    activeScene_ = std::make_unique<SplashScene>(context_);
    break;
  case SceneType::MainMenu:
    activeScene_ = std::make_unique<MainMenuScene>(context_);
    break;
  case SceneType::MissionRank:
    activeScene_ = std::make_unique<MissionRankScene>(context_);
    break;
  case SceneType::Inventory:
    activeScene_ = std::make_unique<InventoryScene>(context_);
    break;
  case SceneType::Gameplay:
    activeScene_ = std::make_unique<BattleScene>(context_);
    break;
  case SceneType::Academy:
    activeScene_ = std::make_unique<AcademyScene>(context_);
    break;
  default:
    break;
  }
  if (activeScene_) {
    activeScene_->OnEnter();
  }
}

void SceneManager::Update(float dt) {
  if (activeScene_) {
    SceneType nextScene = activeScene_->Update(dt);
    if (nextScene != SceneType::None) {
      ChangeScene(nextScene);
    }
  }
}

void SceneManager::Draw() {
  if (activeScene_) {
    activeScene_->Draw();
  }
}
