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
  Texture2D playerIdleTex_;
  Texture2D playerAttackTex_;

  // Textures Enemy
  Texture2D enemyIdleTex_;
  Texture2D enemyAttackTex_;
  Texture2D enemyHurtTex_;
  Texture2D enemyDeathTex_;
  Texture2D projectileTex_;

  // Textures Health Bar
  Texture2D healthBgTex_;
  Texture2D healthFillTex_[10]; // redMeter01 sampai redMeter10

  // Battle State
  BattleState currentState_;
  float stateTimer_;

  // Karakter Stats
  int playerHP_;
  int playerMaxHP_;
  int enemyHP_;
  int enemyMaxHP_;

  // Animation System Player
  int playerCurrentFrame_;
  int playerFrameCounter_;
  bool isPlayerAttacking_;

  // Animation System Enemy
  int enemyCurrentFrame_;
  int enemyFrameCounter_;
  bool isEnemyAttacking_;
  bool isEnemyHurt_;
  bool isEnemyDead_;

  // Projectile System
  bool isProjectileActive_;
  float projectileX_;
  float projectileY_;

  // Helpers
  void DrawPlayer();
  void DrawEnemy();
  void DrawHealthBar(float x, float y, int currentHP, int maxHP);
  void DrawBattleUI();
  void UpdateAnimations();
};
