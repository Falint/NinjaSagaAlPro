#pragma once
#include "core/BaseScene.h"
#include "raylib.h"
#include "types.h"

enum class BattleState {
  Start,
  PlayerTurn,
  EnemyTurn,
  Win,
  Lose,
  ReturningToMenu
};

class BattleScene : public BaseScene {
public:
  explicit BattleScene(GameContext ctx);
  ~BattleScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  GameContext context_;

  // Textures Player
  Texture2D playerIdleTex_ = {};
  Texture2D playerAttackTex_ = {};

  // Textures Enemy
  Texture2D enemyIdleTex_ = {};
  Texture2D enemyAttackTex_ = {};
  Texture2D enemyHurtTex_ = {};
  Texture2D enemyDeathTex_ = {};
  Texture2D projectileTex_ = {};

  // Textures Health Bar
  Texture2D healthBgTex_ = {};
  Texture2D healthFillTex_[10] = {}; // redMeter01 sampai redMeter10

  // Battle State
  BattleState currentState_ = BattleState::Start;
  float stateTimer_ = 0.0f;

  // Karakter Stats
  int playerHP_ = 0;
  int playerMaxHP_ = 0;
  int enemyHP_ = 0;
  int enemyMaxHP_ = 0;

  // Animation System Player
  int playerCurrentFrame_ = 0;
  int playerFrameCounter_ = 0;
  bool isPlayerAttacking_ = false;

  // Animation System Enemy
  int enemyCurrentFrame_ = 0;
  int enemyFrameCounter_ = 0;
  bool isEnemyAttacking_ = false;
  bool isEnemyHurt_ = false;
  bool isEnemyDead_ = false;

  // Projectile System
  bool isProjectileActive_ = false;
  float projectileX_ = 0.0f;
  float projectileY_ = 0.0f;

  // Helpers
  void DrawPlayer();
  void DrawEnemy();
  void DrawHealthBar(float x, float y, int currentHP, int maxHP);
  void DrawBattleUI();
  void UpdateAnimations();
};
