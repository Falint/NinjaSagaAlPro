#include "scenes/MissionRankScene.h"
#include "constants.h"
#include "raylib.h"
#include "types.h"
#include <iostream>

MissionRankScene::MissionRankScene(GameContext ctx) : context_(ctx) {}

MissionLayout MissionRankScene::CalcLayout() const {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  float scale = static_cast<float>(screenH) / MENU_BASE_SIZE;
  float drawW = MENU_BASE_SIZE * scale;
  float drawH = MENU_BASE_SIZE * scale;
  float imgX = (static_cast<float>(screenW) - drawW) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawH) / 2.0f;

  return {drawW, drawH, imgX, imgY};
}

void MissionRankScene::OnEnter() {
  missionFrames_[0] = LoadTexture(ASSET_MISSION_FRAME_1);
  missionFrames_[1] = LoadTexture(ASSET_MISSION_FRAME_2);
  missionFrames_[2] = LoadTexture(ASSET_MISSION_FRAME_3);
  missionFrames_[3] = LoadTexture(ASSET_MISSION_FRAME_4);
  missionFrames_[4] = LoadTexture(ASSET_MISSION_FRAME_5);
  missionFrames_[5] = LoadTexture(ASSET_MISSION_FRAME_6);

  menuBackgroundTex_ = LoadTexture(ASSET_MENU_BACKGROUND);

  for (int i = 0; i < 6; i++) {
    if (missionFrames_[i].id == 0) {
      std::cerr << "[ERROR] Gagal load mission rank frame index: " << i << std::endl;
    }
  }

  showLockedWarning_ = false;
  warningTimer_ = 0.0f;
}

SceneType MissionRankScene::Update(float dt) {
  if (warningTimer_ > 0.0f) {
    warningTimer_ -= dt;
    if (warningTimer_ <= 0.0f) {
      showLockedWarning_ = false;
    }
  }

  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::MainMenu;
  }

  Vector2 mouse = GetMousePosition();
  MissionLayout layout = CalcLayout();

  currentFrameIndex_ = 0; // Default frame (Idle)

  for (int i = 0; i < MISSION_RANK_COUNT; i++) {
    float bx = layout.imgX + (MISSION_RANK_HITBOXES[i][0] * layout.drawW);
    float by = layout.imgY + (MISSION_RANK_HITBOXES[i][1] * layout.drawH);
    float bw = MISSION_RANK_HITBOXES[i][2] * layout.drawW;
    float bh = MISSION_RANK_HITBOXES[i][3] * layout.drawH;
    Rectangle btn = {bx, by, bw, bh};

    if (CheckCollisionPointRec(mouse, btn)) {
      currentFrameIndex_ = i + 1;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (i == 4) {
          // Rank D -> Go to Gameplay
          std::cout << "[MissionRank] Rank D selected -> Go to Gameplay" << std::endl;
          return SceneType::Gameplay;
        } else {
          // Rank S, A, B, C -> Locked
          showLockedWarning_ = true;
          warningTimer_ = 1.5f;
          if (i == 0) lockedRankName_ = "S";
          else if (i == 1) lockedRankName_ = "A";
          else if (i == 2) lockedRankName_ = "B";
          else if (i == 3) lockedRankName_ = "C";
          std::cout << "[MissionRank] Rank " << lockedRankName_ << " is Locked!" << std::endl;
        }
      }
    }
  }

  if (currentFrameIndex_ > 5)
    currentFrameIndex_ = 5;

  return SceneType::None;
}

void MissionRankScene::Draw() {
  Texture2D texToDraw = missionFrames_[currentFrameIndex_];

  if (menuBackgroundTex_.id != 0) {
    DrawTexturePro(menuBackgroundTex_,
                   {0.0f, 0.0f, static_cast<float>(menuBackgroundTex_.width), static_cast<float>(menuBackgroundTex_.height)},
                   {0.0f, 0.0f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
                   {0.0f, 0.0f}, 0.0f, WHITE);
  } else {
    ClearBackground(BLACK);
  }

  MissionLayout layout = CalcLayout();

  Rectangle srcRect = {0, 0, static_cast<float>(texToDraw.width),
                       static_cast<float>(texToDraw.height)};
  Rectangle dstRect = {layout.imgX, layout.imgY, layout.drawW, layout.drawH};

  DrawTexturePro(texToDraw, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  if (currentFrameIndex_ != 0) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // Draw instruction text
  const char *infoText = "Press [ESC] / [BACKSPACE] to Return";
  int infoWidth = MeasureText(infoText, 20);
  DrawText(infoText, (GetScreenWidth() - infoWidth) / 2, 40, 20, LIGHTGRAY);

  // Draw locked warning
  if (showLockedWarning_) {
    std::string warnText = "Rank " + lockedRankName_ + " is LOCKED!";
    int tw = MeasureText(warnText.c_str(), 24);
    DrawText(warnText.c_str(), (GetScreenWidth() - tw) / 2, GetScreenHeight() - 80, 24, RED);
  }

  // Debug hitboxes
  if (MISSION_DEBUG_HITBOX) {
    Color debugColors[MISSION_RANK_COUNT] = {
        {255, 0, 0, 120},
        {255, 120, 0, 120},
        {255, 255, 0, 120},
        {0, 255, 0, 120},
        {0, 0, 255, 120}
    };
    for (int i = 0; i < MISSION_RANK_COUNT; i++) {
      float bx = layout.imgX + (MISSION_RANK_HITBOXES[i][0] * layout.drawW);
      float by = layout.imgY + (MISSION_RANK_HITBOXES[i][1] * layout.drawH);
      float bw = MISSION_RANK_HITBOXES[i][2] * layout.drawW;
      float bh = MISSION_RANK_HITBOXES[i][3] * layout.drawH;

      Rectangle rect = {bx, by, bw, bh};
      DrawRectangleRec(rect, debugColors[i]);
      DrawRectangleLinesEx(rect, 2.0f, WHITE);
    }
  }
}

void MissionRankScene::OnExit() {
  for (int i = 0; i < 6; i++) {
    if (missionFrames_[i].id != 0) {
      UnloadTexture(missionFrames_[i]);
      missionFrames_[i] = {};
    }
  }

  if (menuBackgroundTex_.id != 0) {
    UnloadTexture(menuBackgroundTex_);
    menuBackgroundTex_ = {};
  }
}
