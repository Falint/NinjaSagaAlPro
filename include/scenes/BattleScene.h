#pragma once
#include "core/BaseScene.h"
#include "entities/Character.h"
#include "raylib.h"
#include "types.h"

enum class BattleState {
  Start,
  PlayerIsChoosing,
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

  // Textures Background
  Texture2D backgroundTex_ = {};

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
  // int playerHP_ = 0;
  // int playerMaxHP_ = 0;
  // int enemyHP_ = 0;
  // int enemyMaxHP_ = 0;
  Character Player_;
  Character Enemy_;

  // Animation System Player
  int playerCurrentFrame_ = 0;
  int playerFrameCounter_ = 0;
  bool isPlayerAttacking_ = false;

  // Dash System Player
  enum class DashState { Idle, DashingToEnemy, Attacking, DashingBack };
  DashState dashState_ = DashState::Idle;
  float playerPosX_ = 0.0f;     // posisi X player saat ini (screen space)
  float playerOriginX_ = 0.0f;  // posisi X asal player
  float playerTargetX_ = 0.0f;  // posisi X tujuan dash (dekat enemy)
  float dashTimer_ = 0.0f;      // timer untuk lerp dash
  float dashDuration_ = 0.2f;   // durasi dash (detik)

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
