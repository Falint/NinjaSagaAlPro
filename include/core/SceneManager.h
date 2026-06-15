#pragma once
#include "BaseScene.h"
#include "types.h"
#include <memory>

class SceneManager {
public:
  explicit SceneManager(GameContext ctx);
  ~SceneManager() = default;

  // Changes the active scene, calling OnExit on old and OnEnter on new
  void ChangeScene(SceneType type);

  // Updates active scene and checks if scene change is requested
  void Update(float dt);

  // Renders active scene
  void Draw();

  // Checks if the exit command has been given
  bool ShouldClose() const { return activeSceneType_ == SceneType::Exit; }

private:
  GameContext context_;
  std::unique_ptr<BaseScene> activeScene_;
  SceneType activeSceneType_;
};
