#include "scenes/BattleScene.h"
#include "constants.h"
#include <iostream>
#include <raylib.h>

BattleScene::BattleScene(GameContext ctx) : context_(ctx) {}

void BattleScene::OnEnter() {
  battleBgm_ = LoadMusicStream(ASSET_BATTLE_BGM);
  if (battleBgm_.stream.buffer != nullptr) {
    PlayMusicStream(battleBgm_);
  } else {
    std::cerr << "[ERROR] Gagal load BGM: " << ASSET_BATTLE_BGM << std::endl;
  }

  // Load Background Texture
  backgroundTex_ = LoadTexture(ASSET_BATTLE_BACKGROUND);

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

  // Load Mana Bar
  manaBgTex_ = LoadTexture(ASSET_BATTLE_MANA_BG);
  for (int i = 0; i < 10; i++) {
    std::string path = std::string(ASSET_BATTLE_MANA_FILL_PREFIX) +
                       (i < 9 ? "0" : "") + std::to_string(i + 1) + ".png";
    manaFillTex_[i] = LoadTexture(path.c_str());
  }

  // Init Stats
  if (context_.player) {
    Player_ = *context_.player;
    // Mulai battle dengan HP & MP penuh sesuai maxHp & maxMp
    Player_.hp = Player_.maxHp;
    Player_.mp = Player_.maxMp;
  } else {
    Player_.maxHp = BATTLE_PLAYER_MAX_HP;
    Player_.hp = Player_.maxHp;
    Player_.maxMp = 10;
    Player_.mp = 10;
    Player_.attack = 2;
  }
  activeSkillIndex_ = -1;

  Enemy_.maxHp = BATTLE_PLAYER_MAX_HP;
  Enemy_.hp = Enemy_.maxHp;
  Enemy_.attack = BATTLE_ENEMY_DAMAGE;

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

  // Init Dash State
  dashState_ = DashState::Idle;
  dashTimer_ = 0.0f;
  // playerPosX_ diinisialisasi saat pertama kali draw, karena butuh screen width

  std::cout << "[BattleScene] Battle Start!" << std::endl;
}

SceneType BattleScene::Update(float dt) {
  if (battleBgm_.stream.buffer != nullptr) {
    UpdateMusicStream(battleBgm_);
  }

  stateTimer_ += dt;
  UpdateAnimations();

  switch (currentState_) {
  case BattleState::Start:
    if (stateTimer_ >= BATTLE_STATE_DELAY) {
      stateTimer_ = 0.0f;
      currentState_ = BattleState::PlayerIsChoosing;
    }
    break;

  case BattleState::PlayerIsChoosing:
    if (IsKeyPressed(KEY_ONE)) {
      std::cout << "[Battle] Player Pilih attack!\n";

      // Mulai dash ke enemy, damage diberikan nanti saat sudah sampai
      float screenW = static_cast<float>(GetScreenWidth());
      float frameW = static_cast<float>(playerIdleTex_.width) / BATTLE_PLAYER_FRAMES_IDLE;
      float drawW = frameW * BATTLE_PLAYER_SCALE;
      playerOriginX_ = screenW * 0.3f - (drawW / 2.0f);
      playerPosX_    = playerOriginX_;
      playerTargetX_ = screenW * 0.7f - drawW * 1.5f; // tepat di depan enemy

      dashState_ = DashState::DashingToEnemy;
      dashTimer_ = 0.0f;

      isPlayerAttacking_ = false;
      playerCurrentFrame_ = 0;
      playerFrameCounter_ = 0;

      currentState_ = BattleState::PlayerTurn;
      stateTimer_ = 0.0f;
    }

    for (size_t i = 0; i < Player_.skills.size(); ++i) {
      if (IsKeyPressed(static_cast<KeyboardKey>(KEY_TWO + i))) {
        const auto &skill = Player_.skills[i];

        // Jika skill memiliki efek heal dan darah player sudah penuh, batalkan penggunaan skill
        if (skill.healAmount > 0 && Player_.hp >= Player_.maxHp) {
          std::cout << "[Battle] Darah sudah penuh! Gagal menggunakan " << skill.name << ".\n";
          continue;
        }

        if (Player_.UseMana(skill.manaCost)) {
          std::cout << "[Battle] Player Menggunakan skill: " << skill.name << "\n";
          activeSkillIndex_ = static_cast<int>(i);

          if (skill.target == SkillTarget::Self) {
            // Self target skill (Heal)
            Player_.Heal(skill.healAmount);
            std::cout << "[Battle] Player heal " << skill.healAmount << " HP. HP: " << Player_.hp << "\n";
            currentState_ = BattleState::EnemyTurn;
            isEnemyAttacking_ = true;
            enemyCurrentFrame_ = 0;
            stateTimer_ = 0.0f;
          } else {
            // Offense skill
            float screenW = static_cast<float>(GetScreenWidth());
            float frameW = static_cast<float>(playerIdleTex_.width) / BATTLE_PLAYER_FRAMES_IDLE;
            float drawW = frameW * BATTLE_PLAYER_SCALE;
            playerOriginX_ = screenW * 0.3f - (drawW / 2.0f);
            playerPosX_    = playerOriginX_;
            playerTargetX_ = screenW * 0.7f - drawW * 1.5f;

            dashState_ = DashState::DashingToEnemy;
            dashTimer_ = 0.0f;

            isPlayerAttacking_ = false;
            playerCurrentFrame_ = 0;
            playerFrameCounter_ = 0;

            currentState_ = BattleState::PlayerTurn;
            stateTimer_ = 0.0f;
          }
        } else {
          std::cout << "[Battle] Mana tidak cukup untuk: " << skill.name << "!\n";
        }
      }
    }
    break;

  case BattleState::PlayerTurn:
    dashTimer_ += dt;

    if (dashState_ == DashState::DashingToEnemy) {
      // Lerp player dari origin ke target
      float t = dashTimer_ / dashDuration_;
      if (t >= 1.0f) {
        t = 1.0f;
        playerPosX_ = playerTargetX_;

        // Sudah sampai — play animasi attack dan berikan damage
        dashState_ = DashState::Attacking;
        dashTimer_ = 0.0f;

        isPlayerAttacking_ = true;
        playerCurrentFrame_ = 0;
        playerFrameCounter_ = 0;

        if (activeSkillIndex_ >= 0 && activeSkillIndex_ < static_cast<int>(Player_.skills.size())) {
          int damage = Player_.attack + Player_.skills[activeSkillIndex_].baseDamage;
          Enemy_.TakeDamage(damage);
          activeSkillIndex_ = -1; // reset
        } else {
          Enemy_.TakeDamage(Player_.attack);
        }
        isEnemyHurt_ = true;
        enemyCurrentFrame_ = 0;
      } else {
        playerPosX_ = playerOriginX_ + (playerTargetX_ - playerOriginX_) * t;
      }

    } else if (dashState_ == DashState::Attacking) {
      // Tunggu animasi attack selesai (gunakan BATTLE_STATE_DELAY)
      if (dashTimer_ >= BATTLE_STATE_DELAY) {
        isPlayerAttacking_ = false;
        isEnemyHurt_ = false;

        // Cek apakah enemy mati
        if (!Enemy_.IsAlive()) {
          std::cout << "You Win!" << std::endl;
          dashState_ = DashState::Idle;
          playerPosX_ = playerOriginX_;
          currentState_ = BattleState::Win;
          isEnemyDead_ = true;
          enemyCurrentFrame_ = 0;
          stateTimer_ = 0.0f;
        } else {
          // Dash balik ke posisi asal
          dashState_ = DashState::DashingBack;
          dashTimer_ = 0.0f;
        }
      }

    } else if (dashState_ == DashState::DashingBack) {
      // Lerp player dari target kembali ke origin
      float t = dashTimer_ / dashDuration_;
      if (t >= 1.0f) {
        t = 1.0f;
        playerPosX_ = playerOriginX_;
        dashState_ = DashState::Idle;

        // Selesai semua — sekarang giliran enemy
        std::cout << "Player Attack Animation Finished!" << std::endl;
        currentState_ = BattleState::EnemyTurn;
        isEnemyAttacking_ = true;
        enemyCurrentFrame_ = 0;
        stateTimer_ = 0.0f;
      } else {
        playerPosX_ = playerTargetX_ + (playerOriginX_ - playerTargetX_) * t;
      }
    }
    break;

  case BattleState::EnemyTurn:
    // Tunggu animasi enemy attack + lempar projectile sebelum kena player
    if (stateTimer_ >= BATTLE_STATE_DELAY) {
      Player_.TakeDamage(Enemy_.attack);
      std::cout << "Enemy Attack! Player HP: " << Player_.hp << std::endl;

      isEnemyAttacking_ = false;
      isEnemyHurt_ = false; // Reset hurt

      // Hitung projectile otomatis selesai saat delay
      isProjectileActive_ = false;

      stateTimer_ = 0.0f;
      if (!Player_.IsAlive()) {
        currentState_ = BattleState::Lose;
        std::cout << "You Lose!" << std::endl;
      } else {
        currentState_ = BattleState::PlayerIsChoosing;
        isPlayerAttacking_ = false; // Kembali ke idle, tunggu input user
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

    int pFrames = isPlayerAttacking_ ? BATTLE_PLAYER_FRAMES_ATTACK
                                     : BATTLE_PLAYER_FRAMES_IDLE;
    playerCurrentFrame_++;
    if (playerCurrentFrame_ >= pFrames) {
      playerCurrentFrame_ = 0;
    }
  }

  enemyFrameCounter_++;
  if (enemyFrameCounter_ >= (60 / BATTLE_ANIM_FPS)) {
    enemyFrameCounter_ = 0;

    int eFrames = BATTLE_ENEMY_FRAMES_IDLE;
    if (isEnemyDead_)
      eFrames = BATTLE_ENEMY_FRAMES_DEATH;
    else if (isEnemyHurt_)
      eFrames = BATTLE_ENEMY_FRAMES_HURT;
    else if (isEnemyAttacking_)
      eFrames = BATTLE_ENEMY_FRAMES_ATTACK;

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
  if (backgroundTex_.id != 0) {
    DrawTexturePro(backgroundTex_, 
                   {0.0f, 0.0f, static_cast<float>(backgroundTex_.width), static_cast<float>(backgroundTex_.height)}, 
                   {0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, 
                   {0.0f, 0.0f}, 0.0f, WHITE);
  } else {
    ClearBackground(DARKGRAY); // Fallback jika gambar gagal di-load
  }

  DrawPlayer();
  DrawEnemy();

  if (isProjectileActive_) {
    // Gambar Projectile terbang
    Rectangle src = {0, 0, static_cast<float>(projectileTex_.width),
                     static_cast<float>(projectileTex_.height)};
    Rectangle dst = {
        projectileX_, projectileY_,
        static_cast<float>(projectileTex_.width) * BATTLE_PROJECTILE_SCALE,
        static_cast<float>(projectileTex_.height) * BATTLE_PROJECTILE_SCALE};
    DrawTexturePro(projectileTex_, src, dst, {0, 0}, 0.0f, WHITE);
  }

  DrawBattleUI();
}

void BattleScene::DrawPlayer() {
  Texture2D tex = isPlayerAttacking_ ? playerAttackTex_ : playerIdleTex_;
  int frames = isPlayerAttacking_ ? BATTLE_PLAYER_FRAMES_ATTACK
                                  : BATTLE_PLAYER_FRAMES_IDLE;

  float frameW = static_cast<float>(tex.width) / frames;
  float frameH = static_cast<float>(tex.height);

  Rectangle src = {static_cast<float>(playerCurrentFrame_) * frameW, 0, frameW,
                   frameH};

  float drawW = frameW * BATTLE_PLAYER_SCALE;
  float drawH = frameH * BATTLE_PLAYER_SCALE;

  // Gunakan playerPosX_ jika sedang dash, otherwise posisi default
  float posX = (dashState_ != DashState::Idle)
                   ? playerPosX_
                   : GetScreenWidth() * 0.3f - (drawW / 2.0f);
  float posY = GetScreenHeight() / 2.0f - (drawH / 2.0f);

  Rectangle dst = {posX, posY, drawW, drawH};

  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);

  // Health Bar & Mana Bar Player (di bawah karakter, centered)
  float barX = posX + (drawW - 160.0f) / 2.0f;
  DrawHealthBar(barX, posY + drawH + 10, Player_.hp, Player_.maxHp);
  DrawManaBar(barX, posY + drawH + 35, Player_.mp, Player_.maxMp);
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

  // Lebar per-frame konstan 79px sesuai analisis sprite sheet
  constexpr float ENEMY_SPRITE_FRAME_WIDTH = 79.0f;
  float frameW = ENEMY_SPRITE_FRAME_WIDTH;
  if (isEnemyDead_) {
    frameW = static_cast<float>(tex.width) / frames;
  }

  float frameH = static_cast<float>(tex.height);

  // Flip horizontal dengan width negatif
  Rectangle src = {static_cast<float>(enemyCurrentFrame_) * frameW, 0, -frameW,
                   frameH};

  float drawW = frameW * BATTLE_ENEMY_SCALE;
  float drawH = frameH * BATTLE_ENEMY_SCALE;
  float posX = GetScreenWidth() * 0.7f - (drawW / 2.0f);
  float posY = GetScreenHeight() / 2.0f - (drawH / 2.0f);

  Rectangle dst = {posX, posY, drawW, drawH};

  DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);

  // Health Bar Enemy
  if (!isEnemyDead_) {
    float barX = posX + (drawW - 160.0f) / 2.0f;
    DrawHealthBar(barX, posY + drawH + 10, Enemy_.hp, Enemy_.maxHp);
  }
}

void BattleScene::DrawHealthBar(float x, float y, int currentHP, int maxHP) {
  float scale = 2.5f; // 64 * 2.5 = 160px, cocok dengan lebar Mana bar
  float drawW = static_cast<float>(healthBgTex_.width) * scale;
  float drawH = static_cast<float>(healthBgTex_.height) * scale;

  Rectangle src = {0, 0, static_cast<float>(healthBgTex_.width),
                   static_cast<float>(healthBgTex_.height)};
  Rectangle dst = {x, y, drawW, drawH};

  // Draw Background
  if (healthBgTex_.id != 0) {
    DrawTexturePro(healthBgTex_, src, dst, {0, 0}, 0.0f, WHITE);
  }

  // Draw Fill
  if (currentHP > 0) {
    int index = (currentHP * 10) / maxHP;
    if (index < 1) index = 1;
    if (index > 10) index = 10;

    if (healthFillTex_[index - 1].id != 0) {
      DrawTexturePro(healthFillTex_[index - 1], src, dst, {0, 0}, 0.0f, WHITE);
    }
  }

  // Draw HP Text di tengah bar
  std::string text = "HP: " + std::to_string(currentHP) + "/" + std::to_string(maxHP);
  int textWidth = MeasureText(text.c_str(), 12);
  DrawText(text.c_str(), x + (drawW - textWidth) / 2.0f, y + (drawH - 12) / 2.0f, 12, WHITE);
}

void BattleScene::DrawManaBar(float x, float y, int currentMP, int maxMP) {
  float scale = 2.5f; // 64 * 2.5 = 160px, cocok dengan lebar Health bar
  float drawW = static_cast<float>(manaBgTex_.width) * scale;
  float drawH = static_cast<float>(manaBgTex_.height) * scale;

  Rectangle src = {0, 0, static_cast<float>(manaBgTex_.width),
                   static_cast<float>(manaBgTex_.height)};
  Rectangle dst = {x, y, drawW, drawH};

  // Draw Background
  if (manaBgTex_.id != 0) {
    DrawTexturePro(manaBgTex_, src, dst, {0, 0}, 0.0f, WHITE);
  }

  // Draw Fill
  if (currentMP > 0) {
    int index = (currentMP * 10) / maxMP;
    if (index < 1) index = 1;
    if (index > 10) index = 10;

    if (manaFillTex_[index - 1].id != 0) {
      DrawTexturePro(manaFillTex_[index - 1], src, dst, {0, 0}, 0.0f, WHITE);
    }
  }

  // Draw MP Text di tengah bar
  std::string text = "MP: " + std::to_string(currentMP) + "/" + std::to_string(maxMP);
  int textWidth = MeasureText(text.c_str(), 12);
  DrawText(text.c_str(), x + (drawW - textWidth) / 2.0f, y + (drawH - 12) / 2.0f, 12, WHITE);
}

void BattleScene::DrawBattleUI() {
  const char *text = "";
  switch (currentState_) {
  case BattleState::Start:
    text = "BATTLE START!";
    break;
  case BattleState::Win:
    text = "YOU WIN!";
    break;
  case BattleState::Lose:
    text = "YOU LOSE!";
    break;
  default:
    break;
  }

  if (text[0] != '\0') {
    int w = MeasureText(text, 40);
    DrawText(text, (GetScreenWidth() - w) / 2, 100, 40, YELLOW);
  }

  if (currentState_ == BattleState::PlayerIsChoosing) {
    int skillCount = static_cast<int>(Player_.skills.size());
    int boxWidth = 450;
    int boxHeight = 60 + 30 * (1 + skillCount);
    int boxX = (GetScreenWidth() - boxWidth) / 2;
    int boxY = GetScreenHeight() - boxHeight - 20;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.8f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GOLD);

    // Tulis teks pilihan
    DrawText("Giliranmu!", boxX + 20, boxY + 15, 20, YELLOW);
    DrawText("Tekan [1] untuk Serang (Attack) [0 MP]", boxX + 20, boxY + 45, 18, WHITE);
    
    for (int i = 0; i < skillCount; ++i) {
      const auto &skill = Player_.skills[i];
      DrawText(TextFormat("Tekan [%d] untuk %s [%d MP]", i + 2, skill.name.c_str(), skill.manaCost),
               boxX + 20, boxY + 75 + i * 30, 18, SKYBLUE);
    }
  }
}

void BattleScene::OnExit() {
  // Guard: hanya UnloadTexture jika texture berhasil di-load
  if (backgroundTex_.id != 0) {
    UnloadTexture(backgroundTex_);
    backgroundTex_ = {};
  }
  if (playerIdleTex_.id != 0) {
    UnloadTexture(playerIdleTex_);
    playerIdleTex_ = {};
  }
  if (playerAttackTex_.id != 0) {
    UnloadTexture(playerAttackTex_);
    playerAttackTex_ = {};
  }

  if (enemyIdleTex_.id != 0) {
    UnloadTexture(enemyIdleTex_);
    enemyIdleTex_ = {};
  }
  if (enemyAttackTex_.id != 0) {
    UnloadTexture(enemyAttackTex_);
    enemyAttackTex_ = {};
  }
  if (enemyHurtTex_.id != 0) {
    UnloadTexture(enemyHurtTex_);
    enemyHurtTex_ = {};
  }
  if (enemyDeathTex_.id != 0) {
    UnloadTexture(enemyDeathTex_);
    enemyDeathTex_ = {};
  }
  if (projectileTex_.id != 0) {
    UnloadTexture(projectileTex_);
    projectileTex_ = {};
  }

  if (healthBgTex_.id != 0) {
    UnloadTexture(healthBgTex_);
    healthBgTex_ = {};
  }
  for (int i = 0; i < 10; i++) {
    if (healthFillTex_[i].id != 0) {
      UnloadTexture(healthFillTex_[i]);
      healthFillTex_[i] = {};
    }
  }

  if (manaBgTex_.id != 0) {
    UnloadTexture(manaBgTex_);
    manaBgTex_ = {};
  }
  for (int i = 0; i < 10; i++) {
    if (manaFillTex_[i].id != 0) {
      UnloadTexture(manaFillTex_[i]);
      manaFillTex_[i] = {};
    }
  }

  if (battleBgm_.stream.buffer != nullptr) {
    StopMusicStream(battleBgm_);
    UnloadMusicStream(battleBgm_);
    battleBgm_ = {};
  }
}
