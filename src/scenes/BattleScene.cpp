#include "scenes/BattleScene.h"
#include "constants.h"
#include <iostream>

BattleScene::BattleScene(GameContext ctx) : context_(ctx) {}

void BattleScene::OnEnter() {
  // Load Textures Player
  playerIdleTex_ = LoadTexture(ASSET_BATTLE_PLAYER_IDLE);
  playerAttackTex_ = LoadTexture(ASSET_BATTLE_PLAYER_ATTACK);

  // Load Textures Enemy
  enemyIdleTex_ = LoadTexture(ASSET_BATTLE_ENEMY_IDLE);
  enemyAttackTex_ = LoadTexture(ASSET_BATTLE_ENEMY_ATTACK);
  enemyHurtTex_ = LoadTexture(ASSET_BATTLE_ENEMY_HURT);
  enemyDeathTex_ = LoadTexture(ASSET_BATTLE_ENEMY_DEATH);
  projectileTex_ = LoadTexture(ASSET_BATTLE_ENEMY_PROJECTILE);

  // Load Health Bar
  healthBgTex_ = LoadTexture(ASSET_BATTLE_HEALTH_BG);
  for (int i = 0; i < 10; i++) {
    std::string path = std::string(ASSET_BATTLE_HEALTH_FILL_PREFIX) + 
                       (i < 9 ? "0" : "") + std::to_string(i + 1) + ".png";
    healthFillTex_[i] = LoadTexture(path.c_str());
  }

  // Init Stats
  playerMaxHP_ = BATTLE_PLAYER_MAX_HP;
  playerHP_ = playerMaxHP_;
  enemyMaxHP_ = BATTLE_ENEMY_MAX_HP;
  enemyHP_ = enemyMaxHP_;

  // Init State
  currentState_ = BattleState::Start;
  stateTimer_ = 0.0f;

  // Init Animation
  playerCurrentFrame_ = 0;
  playerFrameCounter_ = 0;
  isPlayerAttacking_ = false;

  enemyCurrentFrame_ = 0;
  enemyFrameCounter_ = 0;
  isEnemyAttacking_ = false;
  isEnemyHurt_ = false;
  isEnemyDead_ = false;

  isProjectileActive_ = false;
  projectileX_ = 0.0f;
  projectileY_ = 0.0f;

  std::cout << "[BattleScene] Battle Start!" << std::endl;
}

SceneType BattleScene::Update(float dt) {
  stateTimer_ += dt;
  UpdateAnimations();

  switch (currentState_) {
  case BattleState::Start:
    if (stateTimer_ >= BATTLE_STATE_DELAY) {
      stateTimer_ = 0.0f;
      currentState_ = BattleState::PlayerTurn;
      isPlayerAttacking_ = true;
      playerCurrentFrame_ = 0; // Reset animasi attack
    }
    break;

  case BattleState::PlayerTurn:
    // FUTURE:
    // if (selectedAction == BattleAction::Attack)
    // {
    //     isPlayerAttacking_ = true;
    //     playerCurrentFrame_ = 0;
    //     // Lanjut ke logika serang
    // }

    // Logic saat player nyerang otomatis
    if (stateTimer_ >= BATTLE_STATE_DELAY) {
      enemyHP_ -= BATTLE_PLAYER_DAMAGE;
      std::cout << "Player Attack! Enemy HP: " << enemyHP_ << std::endl;
      
      isPlayerAttacking_ = false;
      isEnemyHurt_ = true;
      enemyCurrentFrame_ = 0;
      
      stateTimer_ = 0.0f;
      if (enemyHP_ <= 0) {
        currentState_ = BattleState::Win;
        isEnemyDead_ = true;
        enemyCurrentFrame_ = 0;
        std::cout << "You Win!" << std::endl;
      } else {
        currentState_ = BattleState::EnemyTurn;
        isEnemyAttacking_ = true;
        enemyCurrentFrame_ = 0; // Reset animasi attack musuh
      }
    }
    break;

  case BattleState::EnemyTurn:
    // Tunggu animasi enemy attack + lempar projectile sebelum kena player
    if (stateTimer_ >= BATTLE_STATE_DELAY) {
      playerHP_ -= BATTLE_ENEMY_DAMAGE;
      std::cout << "Enemy Attack! Player HP: " << playerHP_ << std::endl;
      
      isEnemyAttacking_ = false;
      isEnemyHurt_ = false; // Reset hurt
      
      // Hitung projectile otomatis selesai saat delay
      isProjectileActive_ = false;

      stateTimer_ = 0.0f;
      if (playerHP_ <= 0) {
        currentState_ = BattleState::Lose;
        std::cout << "You Lose!" << std::endl;
      } else {
        currentState_ = BattleState::PlayerTurn;
        isPlayerAttacking_ = true;
        playerCurrentFrame_ = 0;
      }
    } else {
      // Setup Projectile logika dasar jika animasi musuh menyerang
      if (isEnemyAttacking_) {
        // Projectile muncul dari kanan (posisi musuh) ke kiri (posisi player)
        isProjectileActive_ = true;
        float progress = stateTimer_ / BATTLE_STATE_DELAY;
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        float startX = screenW * 0.7f;
        float endX = screenW * 0.3f;
        projectileX_ = startX + (endX - startX) * progress;
        projectileY_ = screenH / 2.0f;
      }
    }
    break;

  case BattleState::Win:
  case BattleState::Lose:
    if (stateTimer_ >= BATTLE_END_DELAY) {
      currentState_ = BattleState::ReturningToMenu;
    }
    break;

  case BattleState::ReturningToMenu:
    return SceneType::MainMenu;
  }

  return SceneType::None;
}

void BattleScene::UpdateAnimations() {
  playerFrameCounter_++;
  if (playerFrameCounter_ >= (60 / BATTLE_ANIM_FPS)) {
    playerFrameCounter_ = 0;
    
    int pFrames = isPlayerAttacking_ ? BATTLE_PLAYER_FRAMES_ATTACK : BATTLE_PLAYER_FRAMES_IDLE;
    playerCurrentFrame_++;
    if (playerCurrentFrame_ >= pFrames) {
      playerCurrentFrame_ = 0;
    }
  }

  enemyFrameCounter_++;
  if (enemyFrameCounter_ >= (60 / BATTLE_ANIM_FPS)) {
    enemyFrameCounter_ = 0;

    int eFrames = BATTLE_ENEMY_FRAMES_IDLE;
    if (isEnemyDead_) eFrames = BATTLE_ENEMY_FRAMES_DEATH;
    else if (isEnemyHurt_) eFrames = BATTLE_ENEMY_FRAMES_HURT;
    else if (isEnemyAttacking_) eFrames = BATTLE_ENEMY_FRAMES_ATTACK;

    enemyCurrentFrame_++;
    // Tahan frame terakhir kalau mati
    if (isEnemyDead_ && enemyCurrentFrame_ >= eFrames) {
      enemyCurrentFrame_ = eFrames - 1;
    } else if (enemyCurrentFrame_ >= eFrames) {
      enemyCurrentFrame_ = 0;
    }
  }
}

void BattleScene::Draw() {
  ClearBackground(DARKGRAY);

  DrawPlayer();
  DrawEnemy();
  
  if (isProjectileActive_) {
    // Gambar Projectile terbang
    Rectangle src = {0, 0, (float)projectileTex_.width, (float)projectileTex_.height};
    Rectangle dst = {projectileX_, projectileY_, projectileTex_.width * BATTLE_PROJECTILE_SCALE, projectileTex_.height * BATTLE_PROJECTILE_SCALE};
    DrawTexturePro(projectileTex_, src, dst, {0, 0}, 0.0f, WHITE);
  }

  DrawBattleUI();
}

void BattleScene::DrawPlayer() {
  Texture2D tex = isPlayerAttacking_ ? playerAttackTex_ : playerIdleTex_;
  int frames = isPlayerAttacking_ ? BATTLE_PLAYER_FRAMES_ATTACK : BATTLE_PLAYER_FRAMES_IDLE;

  float frameW = (float)tex.width / frames;
  float frameH = (float)tex.height;

  Rectangle src = { (float)playerCurrentFrame_ * frameW, 0, frameW, frameH };
  
  float drawW = frameW * BATTLE_PLAYER_SCALE;
  float drawH = frameH * BATTLE_PLAYER_SCALE;
  float posX = GetScreenWidth() * 0.3f - (drawW / 2.0f);
  float posY = GetScreenHeight() / 2.0f - (drawH / 2.0f);
  
  Rectangle dst = { posX, posY, drawW, drawH };

  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);
  
  // Health Bar Player (di bawah karakter)
  DrawHealthBar(posX, posY + drawH + 10, playerHP_, playerMaxHP_);
}

void BattleScene::DrawEnemy() {
  Texture2D tex = enemyIdleTex_;
  int frames = BATTLE_ENEMY_FRAMES_IDLE;

  if (isEnemyDead_) {
    tex = enemyDeathTex_;
    frames = BATTLE_ENEMY_FRAMES_DEATH;
  } else if (isEnemyHurt_) {
    tex = enemyHurtTex_;
    frames = BATTLE_ENEMY_FRAMES_HURT;
  } else if (isEnemyAttacking_) {
    tex = enemyAttackTex_;
    frames = BATTLE_ENEMY_FRAMES_ATTACK;
  }

  float frameW = 79.0f; // Sesuai analisis, width konstan
  if (isEnemyDead_) frameW = (float)tex.width / frames; // khusus death sedikit beda proporsinya (553/7 = 79 juga)
  
  float frameH = (float)tex.height;

  // Flip horizontal dengan width negatif
  Rectangle src = { (float)enemyCurrentFrame_ * frameW, 0, -frameW, frameH };
  
  float drawW = frameW * BATTLE_ENEMY_SCALE;
  float drawH = frameH * BATTLE_ENEMY_SCALE;
  float posX = GetScreenWidth() * 0.7f - (drawW / 2.0f);
  float posY = GetScreenHeight() / 2.0f - (drawH / 2.0f);
  
  Rectangle dst = { posX, posY, drawW, drawH };

  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);

  // Health Bar Enemy
  if (!isEnemyDead_) {
    DrawHealthBar(posX, posY + drawH + 10, enemyHP_, enemyMaxHP_);
  }
}

void BattleScene::DrawHealthBar(float x, float y, int currentHP, int maxHP) {
  // Option A: Gunakan RedMeter Sprite
  float scale = 2.0f;
  float drawW = healthBgTex_.width * scale;
  float drawH = healthBgTex_.height * scale;

  Rectangle src = { 0, 0, (float)healthBgTex_.width, (float)healthBgTex_.height };
  Rectangle dst = { x, y, drawW, drawH };

  // Draw Background
  DrawTexturePro(healthBgTex_, src, dst, {0, 0}, 0.0f, WHITE);

  // Draw Fill
  if (currentHP > 0) {
    int index = (currentHP * 10) / maxHP;
    if (index < 1) index = 1;
    if (index > 10) index = 10;
    
    DrawTexturePro(healthFillTex_[index - 1], src, dst, {0, 0}, 0.0f, WHITE);
  }

  // Draw Text HP
  DrawText(TextFormat("HP: %d/%d", currentHP, maxHP), x, y + drawH + 5, 20, RAYWHITE);
}

void BattleScene::DrawBattleUI() {
  const char* text = "";
  switch (currentState_) {
    case BattleState::Start: text = "BATTLE START!"; break;
    case BattleState::Win: text = "YOU WIN!"; break;
    case BattleState::Lose: text = "YOU LOSE!"; break;
    default: break;
  }

  if (text[0] != '\0') {
    int w = MeasureText(text, 40);
    DrawText(text, (GetScreenWidth() - w) / 2, 100, 40, YELLOW);
  }

  // FUTURE: Draw manual buttons
  // DrawAttackButton();
  // DrawSkillButton();
  // DrawItemButton();
  // DrawRunButton();
}

void BattleScene::OnExit() {
  UnloadTexture(playerIdleTex_);
  UnloadTexture(playerAttackTex_);

  UnloadTexture(enemyIdleTex_);
  UnloadTexture(enemyAttackTex_);
  UnloadTexture(enemyHurtTex_);
  UnloadTexture(enemyDeathTex_);
  UnloadTexture(projectileTex_);

  UnloadTexture(healthBgTex_);
  for (int i = 0; i < 10; i++) {
    UnloadTexture(healthFillTex_[i]);
  }
}
