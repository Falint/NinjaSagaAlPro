#include "scenes/AcademyScene.h"
#include "constants.h"
#include "entities/Character.h"
#include "raylib.h"
#include <iostream>

// ─── Constructor ─────────────────────────────────────────────
AcademyScene::AcademyScene(GameContext ctx) : context_(ctx) {}

// ── Kalkulasi layout terpusat (aspect-ratio aware) ───────────
AcademyLayout AcademyScene::CalcLayout() const {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Scale agar muat di layar tanpa distorsi (fit by smaller dimension)
  float scaleW = static_cast<float>(screenW) / ACADEMY_BASE_W;
  float scaleH = static_cast<float>(screenH) / ACADEMY_BASE_H;
  float scale = (scaleW < scaleH) ? scaleW : scaleH;

  float drawW = ACADEMY_BASE_W * scale;
  float drawH = ACADEMY_BASE_H * scale;
  float imgX = (static_cast<float>(screenW) - drawW) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawH) / 2.0f;

  return {drawW, drawH, imgX, imgY};
}

// ─── OnEnter ─────────────────────────────────────────────────
void AcademyScene::OnEnter() {
  // Load 4 frame Academy
  academyFrames_[0] = LoadTexture(ASSET_ACADEMY_FRAME_1);
  academyFrames_[1] = LoadTexture(ASSET_ACADEMY_FRAME_2);
  academyFrames_[2] = LoadTexture(ASSET_ACADEMY_FRAME_3);
  academyFrames_[3] = LoadTexture(ASSET_ACADEMY_FRAME_4);

  for (int i = 0; i < 4; i++) {
    if (academyFrames_[i].id == 0) {
      std::cerr << "[ERROR] Gagal load Academy frame index: " << i
                << std::endl;
    }
  }

  // Load 3 icon skill
  skillIcons_[0] = LoadTexture(ASSET_SKILL_ICON_01);
  skillIcons_[1] = LoadTexture(ASSET_SKILL_ICON_02);
  skillIcons_[2] = LoadTexture(ASSET_SKILL_ICON_03);

  // Definisikan daftar skill yang dijual (deskripsi manual)
  shopSkills_ = {
      {1,
       "Fire Slash",
       "Menghantam musuh dengan pedang\nyang dilapisi api.\nDamage +2.",
       3,  // manaCost
       2,  // baseDamage
       0,  // healAmount
       SkillTarget::SingleEnemy,
       50, // price
       false,
       ASSET_SKILL_ICON_01},

      {2,
       "Ice Slash",
       "Tebasan es yang membekukan\nmusuh. 20% peluang musuh\nmelewatkan gilirannya.\nDamage +1.",
       4,  // manaCost
       1,  // baseDamage
       0,  // healAmount
       SkillTarget::SingleEnemy,
       75, // price
       false,
       ASSET_SKILL_ICON_02},

      {3,
       "Heal",
       "Mengalirkan chakra penyembuh.\nMemulihkan 5 HP player.",
       5,  // manaCost
       0,  // baseDamage
       5,  // healAmount
       SkillTarget::Self,
       60, // price
       false,
       ASSET_SKILL_ICON_03},
  };

  // Sinkronkan status owned dari player global
  if (context_.player) {
    for (auto &shopSkill : shopSkills_) {
      for (const auto &ownedSkill : context_.player->skills) {
        if (ownedSkill.id == shopSkill.id) {
          shopSkill.owned = true;
          break;
        }
      }
    }
  }

  currentFrameIndex_ = 0;
  hoveredSkill_ = -1;
  feedbackText_ = "";
  feedbackTimer_ = 0.0f;

  std::cout << "[AcademyScene] Academy opened! Gold: "
            << (context_.player ? context_.player->gold : 0) << std::endl;
}

// ─── Update ──────────────────────────────────────────────────
SceneType AcademyScene::Update(float dt) {
  // Countdown timer feedback
  if (feedbackTimer_ > 0.0f) {
    feedbackTimer_ -= dt;
    if (feedbackTimer_ <= 0.0f) {
      feedbackText_ = "";
    }
  }

  // ESC / Backspace → kembali ke menu
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::MainMenu;
  }

  Vector2 mouse = GetMousePosition();
  AcademyLayout layout = CalcLayout();

  // Reset hover
  currentFrameIndex_ = 0;
  hoveredSkill_ = -1;

  // ── Deteksi hover pada tombol X (close) ──
  {
    float cx = layout.imgX + (ACADEMY_CLOSE_HITBOX[0] * layout.drawW);
    float cy = layout.imgY + (ACADEMY_CLOSE_HITBOX[1] * layout.drawH);
    float cw = ACADEMY_CLOSE_HITBOX[2] * layout.drawW;
    float ch = ACADEMY_CLOSE_HITBOX[3] * layout.drawH;
    Rectangle closeBtn = {cx, cy, cw, ch};

    if (CheckCollisionPointRec(mouse, closeBtn)) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return SceneType::MainMenu;
      }
      return SceneType::None; // Jangan cek skill kalau hover di X
    }
  }

  // ── Deteksi hover pada 3 baris skill ──
  for (int i = 0; i < ACADEMY_SKILL_COUNT; i++) {
    float bx = layout.imgX + (ACADEMY_SKILL_HITBOXES[i][0] * layout.drawW);
    float by = layout.imgY + (ACADEMY_SKILL_HITBOXES[i][1] * layout.drawH);
    float bw = ACADEMY_SKILL_HITBOXES[i][2] * layout.drawW;
    float bh = ACADEMY_SKILL_HITBOXES[i][3] * layout.drawH;
    Rectangle skillBtn = {bx, by, bw, bh};

    if (CheckCollisionPointRec(mouse, skillBtn)) {
      currentFrameIndex_ = i + 1; // Frame 1-3 = hover state
      hoveredSkill_ = i;
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

      // ── Klik untuk membeli ──
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && context_.player) {
        Skill &skill = shopSkills_[i];

        if (skill.owned) {
          feedbackText_ = "Skill sudah dimiliki!";
          feedbackTimer_ = 2.0f;
        } else if (context_.player->skills.size() >=
                   ACADEMY_OWNED_SLOT_COUNT) {
          feedbackText_ = "Slot skill penuh! (Maks 5)";
          feedbackTimer_ = 2.0f;
        } else if (context_.player->gold >= skill.price) {
          // Berhasil beli!
          context_.player->gold -= skill.price;
          skill.owned = true;
          context_.player->skills.push_back(skill);

          feedbackText_ = skill.name + " berhasil dibeli!";
          feedbackTimer_ = 2.0f;
          std::cout << "[Academy] Bought " << skill.name
                    << " | Gold remaining: " << context_.player->gold
                    << std::endl;
        } else {
          feedbackText_ = "Gold tidak cukup!";
          feedbackTimer_ = 2.0f;
        }
      }
      break; // Hanya satu skill yang bisa di-hover
    }
  }

  // Reset cursor jika tidak hover apapun
  if (hoveredSkill_ < 0) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  return SceneType::None;
}

// ─── Draw ────────────────────────────────────────────────────
void AcademyScene::Draw() {
  ClearBackground(BLACK);

  AcademyLayout layout = CalcLayout();

  // ── 1. Gambar frame Academy (1 dari 4) ──
  Texture2D texToDraw = academyFrames_[currentFrameIndex_];
  Rectangle srcRect = {0, 0, static_cast<float>(texToDraw.width),
                       static_cast<float>(texToDraw.height)};
  Rectangle dstRect = {layout.imgX, layout.imgY, layout.drawW, layout.drawH};
  DrawTexturePro(texToDraw, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  // ── 2. Panel detail kanan: tampilkan preview skill yang di-hover ──
  if (hoveredSkill_ >= 0 && hoveredSkill_ < ACADEMY_SKILL_COUNT) {
    const Skill &skill = shopSkills_[hoveredSkill_];

    float detX = layout.imgX + (ACADEMY_DETAIL_X * layout.drawW);
    float detY = layout.imgY + (ACADEMY_DETAIL_Y * layout.drawH);
    float detW = ACADEMY_DETAIL_W * layout.drawW;

    // Skala icon (60×54 asli, diperbesar menggunakan ACADEMY_ICON_SCALE)
    float iconScale = layout.drawW / ACADEMY_BASE_W * ACADEMY_ICON_SCALE;
    float iconW = 60.0f * iconScale;
    float iconH = 54.0f * iconScale;

    // Icon skill (di tengah atas panel detail)
    if (skillIcons_[hoveredSkill_].id != 0) {
      float iconX = detX + (detW - iconW) / 2.0f;
      float iconY = detY + 10.0f;
      Rectangle iconSrc = {
          0, 0, static_cast<float>(skillIcons_[hoveredSkill_].width),
          static_cast<float>(skillIcons_[hoveredSkill_].height)};
      Rectangle iconDst = {iconX, iconY, iconW, iconH};
      DrawTexturePro(skillIcons_[hoveredSkill_], iconSrc, iconDst, {0, 0},
                     0.0f, WHITE);
    }

    // Nama skill
    int fontSize = static_cast<int>(layout.drawH * ACADEMY_FONT_SCALE);
    if (fontSize < 12) fontSize = 12;
    float textStartY = detY + iconH + 20.0f;
    int nameW = MeasureText(skill.name.c_str(), fontSize + 4);
    DrawText(skill.name.c_str(),
             static_cast<int>(detX + (detW - nameW) / 2.0f),
             static_cast<int>(textStartY), fontSize + 4, WHITE);

    // Harga
    const char *priceText =
        skill.owned ? "OWNED" : TextFormat("Harga: %d Gold", skill.price);
    Color priceColor = skill.owned ? GREEN : GOLD;
    int priceW = MeasureText(priceText, fontSize);
    DrawText(priceText, static_cast<int>(detX + (detW - priceW) / 2.0f),
             static_cast<int>(textStartY + fontSize + 10), fontSize,
             priceColor);

    // Deskripsi (multi-line, DrawText otomatis handle \n)
    DrawText(skill.description.c_str(), static_cast<int>(detX + 10),
             static_cast<int>(textStartY + fontSize * 2 + 25), fontSize - 2,
             LIGHTGRAY);
  }

  // ── 3. Tampilkan skill yang sudah dimiliki di 5 slot kanan bawah ──
  if (context_.player) {
    for (int i = 0; i < ACADEMY_OWNED_SLOT_COUNT; i++) {
      float slotX =
          layout.imgX +
          (ACADEMY_OWNED_SLOTS_X + i * (ACADEMY_OWNED_SLOT_W +
                                        ACADEMY_OWNED_SLOT_GAP)) *
              layout.drawW;
      float slotY = layout.imgY + (ACADEMY_OWNED_SLOTS_Y * layout.drawH);
      float slotW = ACADEMY_OWNED_SLOT_W * layout.drawW;
      float slotH = ACADEMY_OWNED_SLOT_H * layout.drawH;

      // Jika player punya skill di slot ini, gambar ikonnya
      if (i < static_cast<int>(context_.player->skills.size())) {
        const Skill &ownedSkill = context_.player->skills[i];
        // Cari icon dari shopSkills_
        for (int j = 0; j < ACADEMY_SKILL_COUNT; j++) {
          if (shopSkills_[j].id == ownedSkill.id &&
              skillIcons_[j].id != 0) {
            Rectangle iconSrc = {
                0, 0, static_cast<float>(skillIcons_[j].width),
                static_cast<float>(skillIcons_[j].height)};
            Rectangle iconDst = {slotX + 2, slotY + 2, slotW - 4,
                                 slotH - 4};
            DrawTexturePro(skillIcons_[j], iconSrc, iconDst, {0, 0}, 0.0f,
                           WHITE);
            break;
          }
        }
      }
    }
  }

  // ── 4. Gold player (kiri atas) ──
  if (context_.player) {
    const char *goldText =
        TextFormat("Gold: %d", context_.player->gold);
    DrawText(goldText, 20, 20, 24, GOLD);
  }

  // ── 5. Feedback teks (tengah bawah) ──
  if (!feedbackText_.empty() && feedbackTimer_ > 0.0f) {
    int fbW = MeasureText(feedbackText_.c_str(), 24);
    int fbX = (GetScreenWidth() - fbW) / 2;
    int fbY = GetScreenHeight() - 50;

    // Fade out effect
    float alphaF = 255.0f * (feedbackTimer_ / 2.0f);
    if (alphaF > 255.0f) alphaF = 255.0f;
    unsigned char alpha = static_cast<unsigned char>(alphaF);
    DrawText(feedbackText_.c_str(), fbX, fbY, 24,
             Color{255, 255, 255, alpha});
  }

  // ── Debug Hitbox ──
  if (ACADEMY_DEBUG_HITBOX) {
    // Skill hitboxes
    Color debugColors[3] = {
        {255, 0, 0, 100},   // Skill 1 - Merah
        {0, 255, 0, 100},   // Skill 2 - Hijau
        {0, 0, 255, 100},   // Skill 3 - Biru
    };
    for (int i = 0; i < ACADEMY_SKILL_COUNT; i++) {
      float bx = layout.imgX + (ACADEMY_SKILL_HITBOXES[i][0] * layout.drawW);
      float by = layout.imgY + (ACADEMY_SKILL_HITBOXES[i][1] * layout.drawH);
      float bw = ACADEMY_SKILL_HITBOXES[i][2] * layout.drawW;
      float bh = ACADEMY_SKILL_HITBOXES[i][3] * layout.drawH;
      Rectangle rect = {bx, by, bw, bh};
      DrawRectangleRec(rect, debugColors[i]);
      DrawRectangleLinesEx(rect, 1.0f, WHITE);
    }

    // Close button hitbox
    float cx = layout.imgX + (ACADEMY_CLOSE_HITBOX[0] * layout.drawW);
    float cy = layout.imgY + (ACADEMY_CLOSE_HITBOX[1] * layout.drawH);
    float cw = ACADEMY_CLOSE_HITBOX[2] * layout.drawW;
    float ch = ACADEMY_CLOSE_HITBOX[3] * layout.drawH;
    DrawRectangle(static_cast<int>(cx), static_cast<int>(cy),
                  static_cast<int>(cw), static_cast<int>(ch),
                  Color{255, 0, 255, 100});

    // Detail panel area
    float dx = layout.imgX + (ACADEMY_DETAIL_X * layout.drawW);
    float dy = layout.imgY + (ACADEMY_DETAIL_Y * layout.drawH);
    float dw = ACADEMY_DETAIL_W * layout.drawW;
    float dh = ACADEMY_DETAIL_H * layout.drawH;
    DrawRectangleLinesEx({dx, dy, dw, dh}, 1.0f, YELLOW);

    // Owned slot areas
    for (int i = 0; i < ACADEMY_OWNED_SLOT_COUNT; i++) {
      float slotX =
          layout.imgX +
          (ACADEMY_OWNED_SLOTS_X +
           i * (ACADEMY_OWNED_SLOT_W + ACADEMY_OWNED_SLOT_GAP)) *
              layout.drawW;
      float slotY = layout.imgY + (ACADEMY_OWNED_SLOTS_Y * layout.drawH);
      float slotW = ACADEMY_OWNED_SLOT_W * layout.drawW;
      float slotH = ACADEMY_OWNED_SLOT_H * layout.drawH;
      DrawRectangleLinesEx({slotX, slotY, slotW, slotH}, 1.0f,
                           Color{0, 255, 255, 200});
    }
  }
}

// ─── OnExit ──────────────────────────────────────────────────
void AcademyScene::OnExit() {
  for (int i = 0; i < 4; i++) {
    if (academyFrames_[i].id != 0) {
      UnloadTexture(academyFrames_[i]);
      academyFrames_[i] = {};
    }
  }
  for (int i = 0; i < ACADEMY_SKILL_COUNT; i++) {
    if (skillIcons_[i].id != 0) {
      UnloadTexture(skillIcons_[i]);
      skillIcons_[i] = {};
    }
  }
  hoveredSkill_ = -1;
  currentFrameIndex_ = 0;
  std::cout << "[AcademyScene] Academy closed." << std::endl;
}
