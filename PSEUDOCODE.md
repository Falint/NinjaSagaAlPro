# PSEUDOCODE - NINJA SAGA ALPRO GAME

## 1. OVERVIEW STRUKTUR GAME

```
NINJA SAGA ALPRO
├── Game Loop (main.cpp)
├── Scene Management System
├── Entities (Character, Skill, Item)
└── Game Scenes (Splash, MainMenu, Battle, Academy, Inventory)
```

---

## 2. GAME INITIALIZATION (main.cpp)

```pseudocode
PROGRAM NinjaSagaAlPro

    INITIALIZE RAYLIB:
        SetConfigFlags(FLAG_WINDOW_RESIZABLE)
        InitWindow(1200, 720, "Ninja Saga")
        SetExitKey(KEY_NULL)
        SetTargetFPS(60)
        InitAudioDevice()
    END

    CREATE GLOBAL PLAYER CHARACTER:
        player = new Character()
        player.name = "Ninja"
        player.hp = 10
        player.maxHp = 10
        player.mp = 10
        player.maxMp = 10
        player.attack = 2
        player.defense = 1
        player.gold = 200
        player.skills = []
        player.inventory = []
    END

    CREATE GAME CONTEXT:
        context.player = &player
    END

    CREATE SCENE MANAGER:
        sceneManager = new SceneManager(context)
        sceneManager.ChangeScene(SceneType::Splash)
    END

    MAIN GAME LOOP:
        WHILE NOT WindowShouldClose() AND NOT sceneManager.ShouldClose():
            dt = GetFrameTime()  // Delta time dari frame sebelumnya
            
            // UPDATE PHASE
            sceneManager.Update(dt)
            
            // RENDER PHASE
            BeginDrawing()
                ClearBackground(BLACK)
                sceneManager.Draw()
            EndDrawing()
        END WHILE
    END

    CLEANUP:
        CloseAudioDevice()
        CloseWindow()
    END

END PROGRAM
```

---

## 3. SCENE MANAGER SYSTEM

### 3.1 Base Scene Architecture

```pseudocode
ABSTRACT CLASS BaseScene:

    VIRTUAL FUNCTION OnEnter():
        // Dipanggil ketika scene ini menjadi active
        // Tempat untuk load assets, initialize game state
    END

    PURE VIRTUAL FUNCTION Update(float dt) -> SceneType:
        // Update logic setiap frame
        // Return SceneType untuk meminta perubahan scene
    END

    PURE VIRTUAL FUNCTION Draw():
        // Render scene ke layar
    END

    VIRTUAL FUNCTION OnExit():
        // Dipanggil ketika scene ini tidak lagi active
        // Tempat untuk cleanup, unload resources
    END

END CLASS BaseScene
```

### 3.2 Scene Manager

```pseudocode
CLASS SceneManager:

    PRIVATE:
        context : GameContext
        activeScene : unique_ptr<BaseScene>
        activeSceneType : SceneType = NONE

    PUBLIC:
        FUNCTION SceneManager(ctx : GameContext):
            context = ctx
            activeScene = nullptr
        END

        FUNCTION ChangeScene(newSceneType : SceneType):
            IF activeScene IS NOT NULL:
                activeScene.OnExit()  // Cleanup scene lama
            END

            // Buat scene baru berdasarkan type
            SWITCH newSceneType:
                CASE SceneType::Splash:
                    activeScene = new SplashScene(context)
                CASE SceneType::MainMenu:
                    activeScene = new MainMenuScene(context)
                CASE SceneType::Gameplay:
                    activeScene = new BattleScene(context)
                CASE SceneType::Academy:
                    activeScene = new AcademyScene(context)
                CASE SceneType::Inventory:
                    activeScene = new InventoryScene(context)
                CASE ELSE:
                    activeScene = nullptr
            END SWITCH

            activeSceneType = newSceneType

            IF activeScene IS NOT NULL:
                activeScene.OnEnter()  // Initialize scene baru
            END
        END

        FUNCTION Update(dt : float):
            IF activeScene IS NULL:
                RETURN
            END

            nextSceneType = activeScene.Update(dt)

            // Jika scene meminta perubahan
            IF nextSceneType != SceneType::None:
                ChangeScene(nextSceneType)
            END
        END

        FUNCTION Draw():
            IF activeScene IS NULL:
                RETURN
            END

            activeScene.Draw()
        END

        FUNCTION ShouldClose() -> bool:
            RETURN activeSceneType == SceneType::Exit
        END

END CLASS SceneManager
```

---

## 4. SCENE IMPLEMENTATIONS

### 4.1 SPLASH SCENE

```pseudocode
CLASS SplashScene EXTENDS BaseScene:

    PRIVATE:
        timer : float = 0.0
        alpha : float = 0.0  // Transparansi (0.0 - 1.0)
        context : GameContext

    PUBLIC:
        FUNCTION OnEnter():
            timer = 0.0
            alpha = 0.0
        END

        FUNCTION Update(dt : float) -> SceneType:
            timer += dt

            // Skip splash jika user tekan tombol apa saja atau klik
            IF IsKeyPressed(ANY_KEY) OR IsMouseButtonPressed(MOUSE_BUTTON_LEFT):
                RETURN SceneType::MainMenu
            END

            // PHASE 1: Fade In (alpha dari 0 ke 1)
            IF timer < SPLASH_FADE_IN_DURATION:
                alpha = timer / SPLASH_FADE_IN_DURATION
            
            // PHASE 2: Hold (tetap fully visible)
            ELSE IF timer < SPLASH_FADE_IN_DURATION + SPLASH_HOLD_DURATION:
                alpha = 1.0
            
            // PHASE 3: Fade Out (alpha dari 1 ke 0)
            ELSE IF timer < SPLASH_TOTAL_DURATION:
                remainingTime = timer - SPLASH_FADE_IN_DURATION - SPLASH_HOLD_DURATION
                alpha = 1.0 - (remainingTime / SPLASH_FADE_OUT_DURATION)
            
            // Selesai → pindah ke MainMenu
            ELSE:
                RETURN SceneType::MainMenu
            END IF

            RETURN SceneType::None  // Tetap di splash screen
        END

        FUNCTION Draw():
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()
            
            // Konversi alpha (0.0-1.0) ke byte (0-255)
            alphaByte = (int)(alpha * 255.0)

            // Title "NINJA SAGA"
            titleColor = {255, 200, 80, alphaByte}  // Warna emas
            titleText = "NINJA SAGA"
            titleSize = 80
            titleX = (screenWidth - MeasureText(titleText, titleSize)) / 2
            titleY = (screenHeight / 2) - titleSize
            
            DrawText(titleText, titleX, titleY, titleSize, titleColor)

            // Subtitle dengan efek kedip
            subtitleText = "Press any key to continue..."
            subtitleSize = 20
            subtitleX = (screenWidth - MeasureText(subtitleText, subtitleSize)) / 2
            subtitleY = titleY + titleSize + 30

            // Subtitle hanya muncul saat fase hold
            IF timer >= SPLASH_FADE_IN_DURATION:
                // Efek kedip halus menggunakan sine wave
                blinkValue = (sin(timer * 3.0) + 1.0) / 2.0
                subtitleAlpha = (int)(blinkValue * alpha * 255.0)
                subtitleColor = {200, 200, 200, subtitleAlpha}
                DrawText(subtitleText, subtitleX, subtitleY, subtitleSize, subtitleColor)
            END IF
        END

END CLASS SplashScene
```

### 4.2 MAIN MENU SCENE

```pseudocode
CLASS MainMenuScene EXTENDS BaseScene:

    PRIVATE:
        context : GameContext
        menuFrames : array[6] of Texture  // 6 frame animasi tombol
        menuBackgroundTex : Texture
        menuBgm : Music
        currentFrameIndex : int = 0

        // Hitbox untuk setiap tombol (normalized 0.0-1.0)
        MENU_HITBOXES = [
            [0.1, 0.3, 0.35, 0.2],   // Play
            [0.55, 0.3, 0.35, 0.2],  // Shop
            [0.1, 0.55, 0.35, 0.2],  // Inventory
            [0.55, 0.55, 0.35, 0.2], // Status
            [0.325, 0.8, 0.35, 0.15] // Exit
        ]

    PUBLIC:
        FUNCTION CalcLayout() -> MenuLayout:
            // Hitung scale agar gambar muat di layar dengan aspect ratio tepat
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()
            scale = screenHeight / MENU_BASE_SIZE
            
            drawWidth = MENU_BASE_SIZE * scale
            drawHeight = MENU_BASE_SIZE * scale
            imageX = (screenWidth - drawWidth) / 2
            imageY = (screenHeight - drawHeight) / 2
            
            RETURN MenuLayout{drawWidth, drawHeight, imageX, imageY}
        END

        FUNCTION OnEnter():
            // Load texture frame menu (untuk animasi hover)
            FOR i = 0 TO 5:
                menuFrames[i] = LoadTexture(FRAME_PATHS[i])
                IF menuFrames[i].id == 0:
                    PRINT ERROR: "Gagal load frame " + i
                END IF
            END FOR

            menuBackgroundTex = LoadTexture(ASSET_MENU_BACKGROUND)

            // Load Background Music
            menuBgm = LoadMusicStream(ASSET_MENU_BGM)
            IF menuBgm IS VALID:
                PlayMusicStream(menuBgm)
            END IF
        END

        FUNCTION Update(dt : float) -> SceneType:
            // Update musik
            IF menuBgm IS VALID:
                UpdateMusicStream(menuBgm)
            END

            // Tekan ESC untuk keluar
            IF IsKeyPressed(KEY_ESCAPE) OR IsKeyPressed(KEY_BACKSPACE):
                RETURN SceneType::Exit
            END

            layout = CalcLayout()
            mouse = GetMousePosition()
            currentFrameIndex = 0  // Default frame (idle)

            // CHECK SETIAP TOMBOL
            FOR i = 0 TO BUTTON_COUNT - 1:
                // Kalkulasi hitbox riil di layar
                buttonBox = {
                    layout.imageX + (MENU_HITBOXES[i][0] * layout.drawWidth),
                    layout.imageY + (MENU_HITBOXES[i][1] * layout.drawHeight),
                    MENU_HITBOXES[i][2] * layout.drawWidth,
                    MENU_HITBOXES[i][3] * layout.drawHeight
                }

                // Cek jika mouse ada di button
                IF CheckCollisionPointRec(mouse, buttonBox):
                    currentFrameIndex = i + 1  // Frame hover
                    
                    // Jika diklik
                    IF IsMouseButtonPressed(MOUSE_BUTTON_LEFT):
                        SWITCH i:
                            CASE 0:  // PLAY
                                PRINT "PLAY clicked → Go to Battle"
                                RETURN SceneType::Gameplay
                            
                            CASE 1:  // SHOP (Academy)
                                PRINT "SHOP clicked → Go to Academy"
                                RETURN SceneType::Academy
                            
                            CASE 2:  // INVENTORY
                                PRINT "INVENTORY clicked"
                                RETURN SceneType::Inventory
                            
                            CASE 3:  // STATUS
                                PRINT "STATUS clicked"
                                // Mungkin show character status (belum implement)
                            
                            CASE 4:  // EXIT
                                PRINT "EXIT clicked"
                                RETURN SceneType::Exit
                        END SWITCH
                    END IF
                END IF
            END FOR

            RETURN SceneType::None  // Tetap di Main Menu
        END

        FUNCTION Draw():
            layout = CalcLayout()

            // Draw background
            DrawTexturePro(menuBackgroundTex, ..., ...)

            // Draw current frame (dengan animasi hover)
            DrawTexturePro(menuFrames[currentFrameIndex], ..., ...)
        END

        FUNCTION OnExit():
            // Unload resources
            FOR i = 0 TO 5:
                UnloadTexture(menuFrames[i])
            END FOR
            UnloadTexture(menuBackgroundTex)
            UnloadMusicStream(menuBgm)
        END

END CLASS MainMenuScene
```

### 4.3 BATTLE SCENE (Gameplay)

```pseudocode
CLASS BattleScene EXTENDS BaseScene:

    PRIVATE:
        context : GameContext
        Player : Character
        Enemy : Character
        
        // State machine untuk battle
        ENUM BattleState: Start, PlayerIsChoosing, PlayerAttacking, 
                         EnemyAttacking, PlayerWon, PlayerLost, End
        
        currentState : BattleState = Start
        stateTimer : float = 0.0
        activeSkillIndex : int = -1

        // Sprites & Textures
        backgroundTex : Texture
        playerIdleTex, playerAttackTex : Texture
        enemyIdleTex, enemyAttackTex, enemyHurtTex, enemyDeathTex : Texture
        projectileTex : Texture
        healthBgTex : Texture
        healthFillTex[10] : Texture  // Health bar fill levels

        // Animation state
        playerCurrentFrame : int = 0
        playerFrameCounter : int = 0
        isPlayerAttacking : bool = false

        enemyCurrentFrame : int = 0
        enemyFrameCounter : int = 0
        isEnemyAttacking : bool = false
        isEnemyHurt : bool = false
        isEnemyDead : bool = false

        // Projectile state (untuk enemy attack animation)
        isProjectileActive : bool = false
        projectileX, projectileY : float = 0.0

        // Dash state (player bergerak ke enemy saat attack)
        ENUM DashState: Idle, Moving, HitEnemy, ReturnToBase
        dashState : DashState = Idle
        dashTimer : float = 0.0
        playerPosX : float = 0.0
        playerTargetX : float = 0.0

    PUBLIC:
        FUNCTION OnEnter():
            // Load semua texture
            backgroundTex = LoadTexture(ASSET_BATTLE_BACKGROUND)
            playerIdleTex = LoadTexture(ASSET_BATTLE_PLAYER_IDLE)
            playerAttackTex = LoadTexture(ASSET_BATTLE_PLAYER_ATTACK)
            enemyIdleTex = LoadTexture(ASSET_BATTLE_ENEMY_IDLE)
            enemyAttackTex = LoadTexture(ASSET_BATTLE_ENEMY_ATTACK)
            enemyHurtTex = LoadTexture(ASSET_BATTLE_ENEMY_HURT)
            enemyDeathTex = LoadTexture(ASSET_BATTLE_ENEMY_DEATH)
            projectileTex = LoadTexture(ASSET_BATTLE_ENEMY_PROJECTILE)

            // Load health bar textures
            healthBgTex = LoadTexture(ASSET_BATTLE_HEALTH_BG)
            FOR i = 0 TO 9:
                path = ASSET_BATTLE_HEALTH_FILL_PREFIX + (i < 9 ? "0" : "") + (i+1) + ".png"
                healthFillTex[i] = LoadTexture(path)
            END FOR

            // Setup player dari global context
            IF context.player IS NOT NULL:
                Player = *context.player
                Player.hp = Player.maxHp  // Mulai dengan HP penuh
                Player.mp = Player.maxMp
            ELSE:
                // Default stats jika tidak ada player
                Player.maxHp = BATTLE_PLAYER_MAX_HP
                Player.hp = Player.maxHp
                Player.maxMp = 10
                Player.mp = 10
                Player.attack = 2
            END IF

            // Setup enemy
            Enemy.maxHp = BATTLE_PLAYER_MAX_HP
            Enemy.hp = Enemy.maxHp
            Enemy.attack = BATTLE_ENEMY_DAMAGE

            // Initialize state
            currentState = BattleState::Start
            stateTimer = 0.0
            activeSkillIndex = -1

            // Initialize animations
            playerCurrentFrame = 0
            playerFrameCounter = 0
            isPlayerAttacking = false
            enemyCurrentFrame = 0
            enemyFrameCounter = 0
            isEnemyAttacking = false
            isEnemyHurt = false
            isEnemyDead = false

            // Initialize projectile
            isProjectileActive = false

            // Initialize dash
            dashState = DashState::Idle
            dashTimer = 0.0

            PRINT "Battle Started!"
        END

        FUNCTION Update(dt : float) -> SceneType:
            stateTimer += dt
            UpdateAnimations()  // Update frame counters

            SWITCH currentState:

                CASE BattleState::Start:
                    // Delay sebelum mulai
                    IF stateTimer >= BATTLE_STATE_DELAY:
                        stateTimer = 0.0
                        currentState = BattleState::PlayerIsChoosing
                    END IF

                CASE BattleState::PlayerIsChoosing:
                    // Player pilih action dengan tombol 1-5
                    IF IsKeyPressed(KEY_ONE):
                        // ATTACK
                        StartPlayerAttack()
                    
                    ELSE IF IsKeyPressed(KEY_TWO):
                        // SKILL 1 (jika ada dan ada MP)
                        IF context.player.skills.size() > 0:
                            skill = context.player.skills[0]
                            IF Player.UseMana(skill.manaCost):
                                StartPlayerSkillAttack(0)
                            END IF
                        END IF
                    
                    ELSE IF IsKeyPressed(KEY_THREE):
                        // SKILL 2
                        IF context.player.skills.size() > 1:
                            skill = context.player.skills[1]
                            IF Player.UseMana(skill.manaCost):
                                StartPlayerSkillAttack(1)
                            END IF
                        END IF
                    
                    ELSE IF IsKeyPressed(KEY_FOUR):
                        // ITEM / Heal
                        // Implementasi item usage
                    
                    ELSE IF IsKeyPressed(KEY_FIVE):
                        // RUN (escape battle)
                        RETURN SceneType::MainMenu
                    
                    END IF

                CASE BattleState::PlayerAttacking:
                    // Update dash movement menuju enemy
                    UpdatePlayerDash(dt)
                    
                    // Ketika player sampai ke enemy
                    IF dashState == DashState::HitEnemy:
                        // Berikan damage ke enemy
                        damage = CalculateDamage(Player.attack)
                        Enemy.TakeDamage(damage)
                        
                        // Enemy bereaksi
                        isEnemyHurt = true
                        dashState = DashState::ReturnToBase
                    END IF

                    // Player kembali ke posisi awal
                    IF dashState == DashState::ReturnToBase:
                        IF playerPosX <= playerOriginX:
                            playerPosX = playerOriginX
                            dashState = DashState::Idle
                            currentState = BattleState::EnemyAttacking
                            stateTimer = 0.0
                        END IF
                    END IF

                CASE BattleState::EnemyAttacking:
                    IF stateTimer >= BATTLE_STATE_DELAY:
                        // Enemy serang
                        isEnemyAttacking = true
                        isProjectileActive = true
                        projectileX = ENEMY_POS_X
                        projectileY = ENEMY_POS_Y
                        
                        stateTimer = 0.0
                        currentState = BattleState::PlayerIsChoosing
                    END IF

                CASE BattleState::PlayerWon:
                    // Player menang → berikan reward
                    // Gain EXP, gold, etc
                    // Kembali ke main menu
                    IF IsKeyPressed(KEY_ENTER):
                        RETURN SceneType::MainMenu
                    END IF

                CASE BattleState::PlayerLost:
                    // Player kalah → game over
                    IF IsKeyPressed(KEY_ENTER):
                        RETURN SceneType::MainMenu
                    END IF

            END SWITCH

            // Cek kondisi kemenangan / kekalahan
            IF currentState != BattleState::PlayerWon AND currentState != BattleState::PlayerLost:
                IF Enemy.hp <= 0:
                    currentState = BattleState::PlayerWon
                END IF
                IF Player.hp <= 0:
                    currentState = BattleState::PlayerLost
                END IF
            END IF

            RETURN SceneType::None
        END

        FUNCTION StartPlayerAttack():
            screenWidth = GetScreenWidth()
            frameWidth = playerIdleTex.width / BATTLE_PLAYER_FRAMES_IDLE
            drawWidth = frameWidth * BATTLE_PLAYER_SCALE
            
            playerOriginX = screenWidth * 0.3 - (drawWidth / 2)
            playerPosX = playerOriginX
            playerTargetX = screenWidth * 0.7 - drawWidth * 1.5  // Depan enemy
            
            dashState = DashState::Moving
            dashTimer = 0.0
            isPlayerAttacking = true
            currentState = BattleState::PlayerAttacking
        END

        FUNCTION UpdatePlayerDash(dt : float):
            dashTimer += dt
            dashDuration = 0.3  // 0.3 detik untuk dash

            IF dashTimer < dashDuration:
                // Linear interpolation dari origin ke target
                progress = dashTimer / dashDuration
                playerPosX = playerOriginX + (playerTargetX - playerOriginX) * progress
                dashState = DashState::Moving
            ELSE:
                playerPosX = playerTargetX
                dashState = DashState::HitEnemy
            END IF
        END

        FUNCTION UpdateAnimations():
            // Update player animation frame
            playerFrameCounter += 1
            IF playerFrameCounter >= ANIMATION_FRAME_SPEED:
                playerFrameCounter = 0
                playerCurrentFrame = (playerCurrentFrame + 1) % BATTLE_PLAYER_FRAMES_IDLE
            END IF

            // Update enemy animation frame
            enemyFrameCounter += 1
            IF enemyFrameCounter >= ANIMATION_FRAME_SPEED:
                enemyFrameCounter = 0
                enemyCurrentFrame = (enemyCurrentFrame + 1) % BATTLE_ENEMY_FRAMES_IDLE
            END IF
        END

        FUNCTION CalculateDamage(baseAttack : int) -> int:
            // Damage = base attack + random variation
            variation = RandomRange(-1, 2)
            RETURN baseAttack + variation
        END

        FUNCTION Draw():
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()

            // Draw background
            DrawTexture(backgroundTex, 0, 0, WHITE)

            // Draw player
            DrawPlayerAnimation()

            // Draw enemy
            DrawEnemyAnimation()

            // Draw projectile jika aktif
            IF isProjectileActive:
                DrawTexture(projectileTex, (int)projectileX, (int)projectileY, WHITE)
            END IF

            // Draw health bars
            DrawHealthBars()

            // Draw UI (skill buttons, action menu)
            DrawBattleUI()
        END

        FUNCTION DrawPlayerAnimation():
            screenWidth = GetScreenWidth()
            textureToUse = playerIdleTex

            IF isPlayerAttacking:
                textureToUse = playerAttackTex
            END IF

            frameWidth = textureToUse.width / BATTLE_PLAYER_FRAMES_IDLE
            sourceRect = {playerCurrentFrame * frameWidth, 0, frameWidth, textureToUse.height}
            
            destX = playerPosX
            destY = screenHeight * 0.3
            DrawTextureRec(textureToUse, sourceRect, {destX, destY}, WHITE)
        END

        FUNCTION DrawEnemyAnimation():
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()
            
            IF isEnemyDead:
                textureToUse = enemyDeathTex
            ELSE IF isEnemyHurt:
                textureToUse = enemyHurtTex
            ELSE IF isEnemyAttacking:
                textureToUse = enemyAttackTex
            ELSE:
                textureToUse = enemyIdleTex
            END IF

            frameWidth = textureToUse.width / BATTLE_ENEMY_FRAMES_IDLE
            sourceRect = {enemyCurrentFrame * frameWidth, 0, frameWidth, textureToUse.height}
            
            destX = screenWidth * 0.7 - (frameWidth * BATTLE_ENEMY_SCALE / 2)
            destY = screenHeight * 0.3
            DrawTextureRec(textureToUse, sourceRect, {destX, destY}, WHITE)
        END

        FUNCTION DrawHealthBars():
            // Draw player health bar
            playerHealthPercent = (float)Player.hp / (float)Player.maxHp
            playerHealthIndex = (int)(playerHealthPercent * 9.0)  // 0-9
            
            DrawTexture(healthBgTex, 50, 50, WHITE)
            DrawTexture(healthFillTex[playerHealthIndex], 50, 50, WHITE)

            // Draw player stats text
            DrawText("HP: " + Player.hp + "/" + Player.maxHp, 60, 100, 20, WHITE)
            DrawText("MP: " + Player.mp + "/" + Player.maxMp, 60, 130, 20, WHITE)

            // Draw enemy health bar
            enemyHealthPercent = (float)Enemy.hp / (float)Enemy.maxHp
            enemyHealthIndex = (int)(enemyHealthPercent * 9.0)
            
            DrawTexture(healthBgTex, screenWidth - 200, 50, WHITE)
            DrawTexture(healthFillTex[enemyHealthIndex], screenWidth - 200, 50, WHITE)

            DrawText("Enemy HP: " + Enemy.hp + "/" + Enemy.maxHp, 
                    screenWidth - 200, 100, 20, WHITE)
        END

        FUNCTION DrawBattleUI():
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()

            // Draw action menu (hanya di PlayerIsChoosing state)
            IF currentState == BattleState::PlayerIsChoosing:
                uiY = screenHeight - 150
                
                DrawText("[1] ATTACK", 50, uiY, 16, YELLOW)
                
                IF context.player.skills.size() > 0:
                    DrawText("[2] " + context.player.skills[0].name, 
                            250, uiY, 16, CYAN)
                END IF
                
                IF context.player.skills.size() > 1:
                    DrawText("[3] " + context.player.skills[1].name, 
                            450, uiY, 16, CYAN)
                END IF
                
                DrawText("[4] ITEM", 650, uiY, 16, GREEN)
                DrawText("[5] RUN", 800, uiY, 16, RED)
            END IF

            // Draw current state message
            IF currentState == BattleState::PlayerWon:
                DrawText("YOU WIN!", screenWidth/2 - 50, screenHeight/2, 40, GREEN)
                DrawText("Press ENTER to continue", screenWidth/2 - 120, screenHeight/2 + 50, 20, WHITE)
            END IF

            IF currentState == BattleState::PlayerLost:
                DrawText("YOU LOSE!", screenWidth/2 - 50, screenHeight/2, 40, RED)
                DrawText("Press ENTER to continue", screenWidth/2 - 120, screenHeight/2 + 50, 20, WHITE)
            END IF
        END

        FUNCTION OnExit():
            // Update global player stats sebelum keluar
            IF context.player IS NOT NULL:
                context.player.hp = Player.hp
                context.player.mp = Player.mp
            END IF

            // Unload semua textures
            UnloadTexture(backgroundTex)
            UnloadTexture(playerIdleTex)
            UnloadTexture(playerAttackTex)
            UnloadTexture(enemyIdleTex)
            UnloadTexture(enemyAttackTex)
            UnloadTexture(enemyHurtTex)
            UnloadTexture(enemyDeathTex)
            UnloadTexture(projectileTex)
            UnloadTexture(healthBgTex)
            FOR i = 0 TO 9:
                UnloadTexture(healthFillTex[i])
            END FOR
        END

END CLASS BattleScene
```

### 4.4 ACADEMY SCENE (Skill Shop)

```pseudocode
CLASS AcademyScene EXTENDS BaseScene:

    PRIVATE:
        context : GameContext
        academyFrames[4] : Texture  // 4 frame untuk animasi skill hover
        menuBackgroundTex : Texture
        skillIcons[3] : Texture     // Icon skill yang dijual
        
        shopSkills : vector<Skill>  // Daftar skill yang tersedia di shop
        currentFrameIndex : int = 0
        hoveredSkill : int = -1
        feedbackText : string = ""  // "Berhasil dibeli" atau "Gold tidak cukup"

    PUBLIC:
        FUNCTION CalcLayout() -> AcademyLayout:
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()

            scaleW = screenWidth / ACADEMY_BASE_W
            scaleH = screenHeight / ACADEMY_BASE_H
            scale = MIN(scaleW, scaleH)

            drawWidth = ACADEMY_BASE_W * scale
            drawHeight = ACADEMY_BASE_H * scale
            imageX = (screenWidth - drawWidth) / 2
            imageY = (screenHeight - drawHeight) / 2

            RETURN AcademyLayout{drawWidth, drawHeight, imageX, imageY}
        END

        FUNCTION OnEnter():
            // Load texture frame academy
            FOR i = 0 TO 3:
                academyFrames[i] = LoadTexture(ACADEMY_FRAME_PATHS[i])
            END FOR

            menuBackgroundTex = LoadTexture(ASSET_MENU_BACKGROUND)

            // Load skill icons
            skillIcons[0] = LoadTexture(ASSET_SKILL_ICON_01)
            skillIcons[1] = LoadTexture(ASSET_SKILL_ICON_02)
            skillIcons[2] = LoadTexture(ASSET_SKILL_ICON_03)

            // Define shop skills (hardcoded untuk demo)
            shopSkills = [
                Skill {
                    id: 1,
                    name: "Fire Slash",
                    description: "Menghantam musuh dengan pedang berapi. Damage +2.",
                    manaCost: 3,
                    baseDamage: 2,
                    healAmount: 0,
                    target: SkillTarget::SingleEnemy,
                    price: 50,
                    owned: false,
                    iconPath: ASSET_SKILL_ICON_01
                },
                Skill {
                    id: 2,
                    name: "Ice Slash",
                    description: "Tebasan es yang membekukan. 20% freeze.",
                    manaCost: 4,
                    baseDamage: 1,
                    healAmount: 0,
                    target: SkillTarget::SingleEnemy,
                    price: 75,
                    owned: false,
                    iconPath: ASSET_SKILL_ICON_02
                },
                Skill {
                    id: 3,
                    name: "Heal",
                    description: "Mengalirkan chakra penyembuh. Restore 5 HP.",
                    manaCost: 5,
                    baseDamage: 0,
                    healAmount: 5,
                    target: SkillTarget::Self,
                    price: 60,
                    owned: false,
                    iconPath: ASSET_SKILL_ICON_03
                }
            ]

            // Sinkronkan status owned dari player
            IF context.player IS NOT NULL:
                FOR EACH shopSkill IN shopSkills:
                    FOR EACH ownedSkill IN context.player.skills:
                        IF ownedSkill.id == shopSkill.id:
                            shopSkill.owned = true
                            BREAK
                        END IF
                    END FOR
                END FOR
            END IF

            currentFrameIndex = 0
            hoveredSkill = -1
            feedbackText = ""
        END

        FUNCTION Update(dt : float) -> SceneType:
            // Back to main menu
            IF IsKeyPressed(KEY_ESCAPE) OR IsKeyPressed(KEY_BACKSPACE):
                RETURN SceneType::MainMenu
            END IF

            layout = CalcLayout()
            mouse = GetMousePosition()
            hoveredSkill = -1
            currentFrameIndex = 0

            // Cek setiap skill
            FOR i = 0 TO shopSkills.size() - 1:
                // Kalkulasi hitbox untuk skill card
                skillX = layout.imageX + SKILL_CARD_POSITIONS[i][0] * layout.drawWidth
                skillY = layout.imageY + SKILL_CARD_POSITIONS[i][1] * layout.drawHeight
                skillW = SKILL_CARD_WIDTH * layout.drawWidth
                skillH = SKILL_CARD_HEIGHT * layout.drawHeight

                skillBox = {skillX, skillY, skillW, skillH}

                IF CheckCollisionPointRec(mouse, skillBox):
                    hoveredSkill = i
                    currentFrameIndex = i + 1  // Frame untuk hover

                    IF IsMouseButtonPressed(MOUSE_BUTTON_LEFT):
                        skill = shopSkills[i]

                        // Cek apakah sudah owned
                        IF skill.owned:
                            feedbackText = "Sudah memiliki skill ini!"
                        ELSE IF context.player.gold >= skill.price:
                            // Beli skill
                            context.player.gold -= skill.price
                            context.player.skills.push_back(skill)
                            shopSkills[i].owned = true
                            feedbackText = "Berhasil membeli " + skill.name + "!"
                        ELSE:
                            feedbackText = "Gold tidak cukup!"
                        END IF
                    END IF
                END IF
            END FOR

            RETURN SceneType::None
        END

        FUNCTION Draw():
            layout = CalcLayout()

            // Draw background
            DrawTexturePro(menuBackgroundTex, ..., ...)

            // Draw academy frame
            DrawTexturePro(academyFrames[currentFrameIndex], ..., ...)

            // Draw player stats (top-left)
            DrawText("Gold: " + context.player.gold, 20, 20, 20, YELLOW)
            DrawText("Skills: " + context.player.skills.size() + "/10", 20, 50, 20, WHITE)

            // Draw skill cards
            FOR i = 0 TO shopSkills.size() - 1:
                skill = shopSkills[i]
                
                cardX = layout.imageX + SKILL_CARD_POSITIONS[i][0] * layout.drawWidth
                cardY = layout.imageY + SKILL_CARD_POSITIONS[i][1] * layout.drawHeight

                // Draw skill icon
                DrawTexture(skillIcons[i], (int)cardX, (int)cardY, WHITE)

                // Draw skill info
                DrawText(skill.name, (int)cardX + 10, (int)cardY + 80, 16, WHITE)
                DrawText("Cost: " + skill.price + "g", (int)cardX + 10, (int)cardY + 100, 14, YELLOW)

                // Draw owned/buy status
                IF skill.owned:
                    DrawText("OWNED", (int)cardX + 10, (int)cardY + 120, 14, GREEN)
                ELSE:
                    DrawText("BUY", (int)cardX + 10, (int)cardY + 120, 14, BLUE)
                END IF
            END FOR

            // Draw feedback message
            IF feedbackText != "":
                messageX = layout.imageX + layout.drawWidth / 2 - 100
                messageY = layout.imageY + layout.drawHeight - 50
                DrawText(feedbackText, (int)messageX, (int)messageY, 16, WHITE)
            END IF
        END

        FUNCTION OnExit():
            FOR i = 0 TO 3:
                UnloadTexture(academyFrames[i])
            END FOR
            UnloadTexture(menuBackgroundTex)
            FOR i = 0 TO 2:
                UnloadTexture(skillIcons[i])
            END FOR
        END

END CLASS AcademyScene
```

### 4.5 INVENTORY SCENE

```pseudocode
CLASS InventoryScene EXTENDS BaseScene:

    PRIVATE:
        context : GameContext
        menuBackgroundTex : Texture
        invTexture : Texture          // Grid background 3x3
        hitboxTexture : Texture       // Highlight untuk hover
        charTexture : Texture         // Animasi karakter di samping
        btnKembaliTex : Texture       // Tombol back

        skillIcons[3] : Texture       // Icon skill
        
        hoveredSlot : int = -1        // Slot mana yang di-hover
        selectedSlot : int = -1       // Slot yang dipilih
        
        CONST INV_GRID_COLS = 3
        CONST INV_GRID_ROWS = 3
        CONST INV_SLOT_COUNT = 9

    PUBLIC:
        FUNCTION CalcLayout() -> InvLayout:
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()

            fitScale = MIN(screenWidth, screenHeight) / INV_BASE_SIZE
            finalScale = fitScale * INV_SCALE_FACTOR
            drawSize = INV_BASE_SIZE * finalScale

            imageX = (screenWidth - drawSize) / 2
            imageY = (screenHeight - drawSize) / 2

            cellWidth = drawSize / INV_GRID_COLS
            cellHeight = drawSize / INV_GRID_ROWS

            RETURN InvLayout{imageX, imageY, drawSize, cellWidth, cellHeight, finalScale}
        END

        FUNCTION OnEnter():
            menuBackgroundTex = LoadTexture(ASSET_MENU_BACKGROUND)
            invTexture = LoadTexture(ASSET_INVENTORY_FRAME)
            hitboxTexture = LoadTexture(ASSET_INVENTORY_HITBOX)
            charTexture = LoadTexture(ASSET_CHARACTER_IDLE)
            btnKembaliTex = LoadTexture(ASSET_BTN_KEMBALI)

            skillIcons[0] = LoadTexture(ASSET_SKILL_ICON_01)
            skillIcons[1] = LoadTexture(ASSET_SKILL_ICON_02)
            skillIcons[2] = LoadTexture(ASSET_SKILL_ICON_03)

            hoveredSlot = -1
            selectedSlot = -1
        END

        FUNCTION Update(dt : float) -> SceneType:
            // Back to main menu
            IF IsKeyPressed(KEY_ESCAPE) OR IsKeyPressed(KEY_BACKSPACE):
                RETURN SceneType::MainMenu
            END IF

            layout = CalcLayout()
            mouse = GetMousePosition()
            hoveredSlot = -1

            // Cek posisi mouse di mana
            FOR i = 0 TO INV_SLOT_COUNT - 1:
                col = i % INV_GRID_COLS
                row = i / INV_GRID_COLS

                slotX = layout.imageX + col * layout.cellWidth
                slotY = layout.imageY + row * layout.cellHeight
                slotBox = {slotX, slotY, layout.cellWidth, layout.cellHeight}

                IF CheckCollisionPointRec(mouse, slotBox):
                    hoveredSlot = i

                    IF IsMouseButtonPressed(MOUSE_BUTTON_LEFT):
                        selectedSlot = i
                        // Bisa add logic untuk use item, dst
                    END IF
                END IF
            END FOR

            // Cek tombol back
            btnBox = GetBtnKembaliHitboxRect(layout)
            IF CheckCollisionPointRec(mouse, btnBox):
                IF IsMouseButtonPressed(MOUSE_BUTTON_LEFT):
                    RETURN SceneType::MainMenu
                END IF
            END IF

            RETURN SceneType::None
        END

        FUNCTION Draw():
            layout = CalcLayout()
            screenWidth = GetScreenWidth()
            screenHeight = GetScreenHeight()

            // Draw background
            DrawTexturePro(menuBackgroundTex, ..., ...)

            // Draw inventory grid
            DrawTexturePro(invTexture, ..., layout.imageX, layout.imageY, ...)

            // Draw items dalam inventory
            itemIndex = 0
            FOR EACH item IN context.player.inventory:
                IF itemIndex >= INV_SLOT_COUNT:
                    BREAK  // Grid penuh
                END IF

                col = itemIndex % INV_GRID_COLS
                row = itemIndex / INV_GRID_COLS

                itemX = layout.imageX + col * layout.cellWidth + 5
                itemY = layout.imageY + row * layout.cellHeight + 5

                // Draw item icon (sesuai kategori)
                SWITCH item.category:
                    CASE ItemCategory::Weapon:
                        DrawText("⚔", (int)itemX, (int)itemY, 20, YELLOW)
                    CASE ItemCategory::Armor:
                        DrawText("🛡", (int)itemX, (int)itemY, 20, GRAY)
                    CASE ItemCategory::Consumable:
                        DrawText("🧪", (int)itemX, (int)itemY, 20, GREEN)
                    CASE ItemCategory::Material:
                        DrawText("⚙", (int)itemX, (int)itemY, 20, ORANGE)
                END SWITCH

                // Draw quantity
                DrawText("x" + item.quantity, (int)itemX + 15, (int)itemY + 10, 12, WHITE)

                itemIndex += 1
            END FOR

            // Draw highlight untuk slot yang di-hover
            IF hoveredSlot >= 0:
                col = hoveredSlot % INV_GRID_COLS
                row = hoveredSlot / INV_GRID_COLS

                highlightX = layout.imageX + col * layout.cellWidth
                highlightY = layout.imageY + row * layout.cellHeight
                
                DrawTexturePro(hitboxTexture, ..., highlightX, highlightY, ...)
            END IF

            // Draw tombol back
            btnVisualBox = GetBtnKembaliVisualRect(layout)
            DrawTexturePro(btnKembaliTex, ..., btnVisualBox.x, btnVisualBox.y, ...)

            // Draw character animation (di samping inventory)
            charX = layout.imageX + layout.drawSize + 20
            charY = layout.imageY + 50
            DrawTexture(charTexture, (int)charX, (int)charY, WHITE)

            // Draw player info (di samping)
            infoX = charX + 50
            infoY = charY + 200
            DrawText("Level: " + context.player.level, (int)infoX, (int)infoY, 16, WHITE)
            DrawText("HP: " + context.player.hp + "/" + context.player.maxHp, 
                    (int)infoX, (int)(infoY + 30), 16, RED)
            DrawText("MP: " + context.player.mp + "/" + context.player.maxMp, 
                    (int)infoX, (int)(infoY + 60), 16, BLUE)
            DrawText("ATK: " + context.player.attack, 
                    (int)infoX, (int)(infoY + 90), 16, YELLOW)
            DrawText("DEF: " + context.player.defense, 
                    (int)infoX, (int)(infoY + 120), 16, GREEN)

            // Draw selected slot info (jika ada)
            IF selectedSlot >= 0 AND selectedSlot < context.player.inventory.size():
                item = context.player.inventory[selectedSlot]
                infoBoxX = screenWidth - 200
                infoBoxY = 50

                DrawRectangle((int)infoBoxX - 10, (int)infoBoxY - 10, 200, 200, 
                            Color{50, 50, 50, 200})  // Semi-transparent background

                DrawText("Name: " + item.name, (int)infoBoxX, (int)infoBoxY, 14, WHITE)
                DrawText("Qty: " + item.quantity, (int)infoBoxX, (int)(infoBoxY + 30), 14, WHITE)
                DrawText("Desc: " + item.description, (int)infoBoxX, (int)(infoBoxY + 60), 12, GRAY)

                IF item.attackBonus > 0:
                    DrawText("ATK+" + item.attackBonus, (int)infoBoxX, (int)(infoBoxY + 90), 12, YELLOW)
                END IF
                IF item.defenseBonus > 0:
                    DrawText("DEF+" + item.defenseBonus, (int)infoBoxX, (int)(infoBoxY + 110), 12, GREEN)
                END IF
            END IF
        END

        FUNCTION OnExit():
            UnloadTexture(menuBackgroundTex)
            UnloadTexture(invTexture)
            UnloadTexture(hitboxTexture)
            UnloadTexture(charTexture)
            UnloadTexture(btnKembaliTex)
            FOR i = 0 TO 2:
                UnloadTexture(skillIcons[i])
            END FOR
        END

END CLASS InventoryScene
```

---

## 5. DATA STRUCTURES

### 5.1 Character Struct

```pseudocode
STRUCT Character:
    // Basic Info
    name : string = "Ninja"

    // Stats
    hp : int = 10
    maxHp : int = 10
    mp : int = 5
    maxMp : int = 5
    attack : int = 2
    defense : int = 1
    gold : int = 100

    // Collections
    skills : vector<Skill>
    inventory : vector<Item>

    // Methods
    FUNCTION IsAlive() -> bool:
        RETURN hp > 0
    END

    FUNCTION TakeDamage(rawDamage : int):
        actualDamage = rawDamage - defense
        IF actualDamage < 1:
            actualDamage = 1  // Minimal 1 damage
        END IF
        hp -= actualDamage
        IF hp < 0:
            hp = 0
        END IF
    END

    FUNCTION Heal(amount : int):
        hp += amount
        IF hp > maxHp:
            hp = maxHp
        END IF
    END

    FUNCTION RestoreMana(amount : int):
        mp += amount
        IF mp > maxMp:
            mp = maxMp
        END IF
    END

    FUNCTION UseMana(cost : int) -> bool:
        IF mp < cost:
            RETURN false
        END IF
        mp -= cost
        RETURN true
    END

END STRUCT Character
```

### 5.2 Skill Struct

```pseudocode
ENUM SkillTarget:
    SingleEnemy,  // Serang 1 musuh
    AllEnemies,   // AOE (semua musuh)
    Self,         // Buff diri sendiri
    SingleAlly    // Heal teman (untuk party)
END ENUM

STRUCT Skill:
    id : int = 0
    name : string = "Basic Attack"
    description : string = ""
    
    manaCost : int = 0
    baseDamage : int = 0
    healAmount : int = 0
    target : SkillTarget = SingleEnemy
    
    price : int = 0          // Untuk shop
    owned : bool = false
    iconPath : string = ""

    FUNCTION CanUse(currentMana : int) -> bool:
        RETURN currentMana >= manaCost
    END

END STRUCT Skill
```

### 5.3 Item Struct

```pseudocode
ENUM ItemCategory:
    Weapon,      // Senjata
    Armor,       // Pelindung
    Consumable,  // Bisa dipakai habis
    Material     // Bahan craft
END ENUM

STRUCT Item:
    id : int = 0
    name : string = "Unknown"
    description : string = ""
    category : ItemCategory = Consumable
    quantity : int = 1
    
    buyPrice : int = 0
    sellPrice : int = 0
    
    // Bonus stats (untuk equipment)
    attackBonus : int = 0
    defenseBonus : int = 0
    hpBonus : int = 0
    
    // Efek konsumsi
    healAmount : int = 0
    manaRestore : int = 0

END STRUCT Item
```

### 5.4 GameContext Struct

```pseudocode
STRUCT GameContext:
    player : pointer to Character = nullptr
    
    // Bisa ditambah:
    // - currentParty : vector<Character>
    // - gameState : GameStateData
    // - audioManager : AudioManager
    // - inputManager : InputManager

END STRUCT GameContext
```

---

## 6. GAME FLOW DIAGRAM

```
START
  │
  ├─→ Initialize Raylib & Assets
  │
  ├─→ Create Global Player Character
  │
  ├─→ Create SceneManager
  │   └─→ ChangeScene(Splash)
  │
  └─→ MAIN LOOP
       │
       ├─→ sceneManager.Update(dt)
       │   │
       │   └─→ activeScene.Update(dt)
       │       │
       │       ├─→ [SPLASH] → MainMenu (after duration)
       │       │
       │       ├─→ [MAIN MENU]
       │       │   ├─→ Play → Battle
       │       │   ├─→ Shop → Academy
       │       │   ├─→ Inventory → Inventory
       │       │   ├─→ Status → (TBD)
       │       │   └─→ Exit → Close Game
       │       │
       │       ├─→ [BATTLE]
       │       │   ├─→ Start
       │       │   ├─→ Player Choose Action
       │       │   │   ├─→ [1] Attack
       │       │   │   ├─→ [2] Skill 1
       │       │   │   ├─→ [3] Skill 2
       │       │   │   ├─→ [4] Item
       │       │   │   └─→ [5] Run (back to MainMenu)
       │       │   ├─→ Player Attack
       │       │   ├─→ Enemy Attack (if alive)
       │       │   ├─→ Check Win/Lose
       │       │   └─→ MainMenu
       │       │
       │       ├─→ [ACADEMY]
       │       │   ├─→ Display Shop Skills
       │       │   ├─→ Player Hover/Click Skill
       │       │   ├─→ IF Gold >= Price AND Not Owned:
       │       │   │   └─→ Add to inventory, reduce gold
       │       │   └─→ Back to MainMenu (ESC)
       │       │
       │       └─→ [INVENTORY]
       │           ├─→ Display Grid of Items
       │           ├─→ Show Item Info on Hover/Click
       │           ├─→ (Use item? TBD)
       │           └─→ Back to MainMenu (ESC)
       │
       ├─→ sceneManager.Draw()
       │   └─→ activeScene.Draw()
       │
       └─→ REPEAT until WindowShouldClose() or Exit scene

END
```

---

## 7. KEY GAME MECHANICS

### 7.1 Turn-Based Battle System

```pseudocode
BATTLE SYSTEM:
├── Player Turn
│   ├── Player chooses action (1-5 keys)
│   ├── IF attack:
│   │   ├── Calculate damage = attack + random(-1, 2)
│   │   ├── Player dashes to enemy
│   │   ├── Enemy takes damage
│   │   └── Enemy reacts (hurt animation)
│   ├── IF skill:
│   │   ├── Check mana cost
│   │   ├── IF enough mana:
│   │   │   ├── Use mana
│   │   │   ├── Calculate skill damage = baseDamage
│   │   │   ├── Apply effect (heal / damage / stun / etc)
│   │   │   └── Enemy reacts
│   │   └── ELSE: Show "Not enough mana"
│   ├── IF item:
│   │   ├── Use consumable item
│   │   ├── Apply effect (heal, buff, etc)
│   │   └── Decrease quantity
│   └── IF run:
│       └── Back to Main Menu
│
└── Enemy Turn
    ├── Calculate enemy action
    ├── Launch projectile at player
    ├── Player takes damage
    └── Check if player died

Check Win/Lose:
├── IF Enemy.hp <= 0: Player Won → Get Gold/EXP
├── IF Player.hp <= 0: Player Lost → Game Over
└── REPEAT if battle continues
```

### 7.2 Skill System

```pseudocode
SKILL MECHANICS:
├── Types
│   ├── Attack: Single/AoE damage
│   ├── Heal: Restore HP
│   ├── Buff: Increase stats
│   └── Debuff: Decrease enemy stats
├── Cost
│   └── Requires MP ≥ manaCost
├── Effectiveness
│   └── damage = baseDamage * (1.0 + 0.1 * player.attack)
└── Availability
    └── Only skills in player.skills[] can be used
```

### 7.3 Inventory System

```pseudocode
INVENTORY MECHANICS:
├── Storage
│   ├── Max 9 slots (3x3 grid)
│   └── Multiple items per slot (quantity)
├── Categories
│   ├── Weapon: +ATK
│   ├── Armor: +DEF
│   ├── Consumable: Use once, restore HP/MP
│   └── Material: Quest items / crafting
├── Usage
│   ├── Consumable: Click to use, quantity -1
│   ├── Equipment: Equip to get stat bonus
│   └── Material: For future crafting system
└── Shop Integration
    └── Sell items back for 50% of buy price
```

---

## 8. CONTROL SCHEME

| Input | Action |
|-------|--------|
| **Key 1** | Attack (in battle) |
| **Key 2** | Use Skill 1 (in battle) |
| **Key 3** | Use Skill 2 (in battle) |
| **Key 4** | Use Item (in battle) |
| **Key 5** | Run (escape battle) |
| **ESC** | Go back / Exit |
| **Mouse Move** | Hover buttons/items |
| **Mouse Click** | Select button/item |

---

## 9. FILE STRUCTURE

```
NinjaSagaAlPro/
├── src/
│   ├── main.cpp              // Entry point, game loop
│   └── scenes/
│       ├── SplashScene.cpp
│       ├── MainMenuScene.cpp
│       ├── BattleScene.cpp
│       ├── AcademyScene.cpp
│       └── InventoryScene.cpp
├── include/
│   ├── types.h               // Enums & structs
│   ├── constants.h           // Configuration
│   ├── core/
│   │   ├── BaseScene.h       // Abstract base class
│   │   └── SceneManager.h    // Scene management
│   ├── entities/
│   │   ├── Character.h       // Player & Enemy
│   │   └── Skill.h           // Skills
│   ├── items/
│   │   └── Item.h            // Items
│   └── scenes/
│       ├── SplashScene.h
│       ├── MainMenuScene.h
│       ├── BattleScene.h
│       ├── AcademyScene.h
│       └── InventoryScene.h
├── assets/
│   ├── sprites/
│   │   ├── player/           // Player animations
│   │   ├── enemies/          // Enemy animations
│   │   ├── skills/           // Skill effects
│   │   └── ui/               // UI elements
│   ├── audio/
│   │   ├── sfx/              // Sound effects
│   │   └── soundtrack/       // Music
│   ├── fonts/                // Font files
│   └── maps/                 // Game maps
├── CMakeLists.txt            // Build configuration
└── PSEUDOCODE.md             // This file
```

---

## 10. STATE MACHINES

### 10.1 Battle State Machine

```
BattleState Transitions:

Start
  ↓ (after delay)
PlayerIsChoosing ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ← ←
  ↓ (player action)                                        ↑
PlayerAttacking → EnemyAttacking → [if alive] back to PlayerIsChoosing
  ↓ (player dies)                                          ↑
PlayerLost                                                  │
                                                           │
[if enemy dies] → PlayerWon (back to MainMenu after ENTER)
```

### 10.2 Dash State Machine (in Player Attack animation)

```
Idle
  ↓
Moving (player slides to enemy position over 0.3 seconds)
  ↓
HitEnemy (apply damage)
  ↓
ReturnToBase (player slides back to origin position)
  ↓
Idle
```

---

## 11. FUTURE ENHANCEMENTS

```pseudocode
// Fitur yang bisa ditambahkan:

FUNCTION AddEnemyVariations():
    // Multiple enemy types dengan stats berbeda
    // Boss battles dengan HP lebih tinggi
    // Enemy AI yang lebih smart
END

FUNCTION AddLevelingSystem():
    // Player gains EXP dari battle
    // Level up untuk boost stats
    // Skill tree untuk unlock abilities
END

FUNCTION AddCraftingSystem():
    // Combine materials untuk create items
    // Upgrade equipment
END

FUNCTION AddQuestSystem():
    // Story quests
    // Repeatable daily quests
    // Reward items & gold
END

FUNCTION AddMultiplayer():
    // PvP battles dengan player lain
    // Guild system
END

FUNCTION AddSaveGame():
    // Save player progress ke file
    // Load last save
    // Multiple save slots
END

FUNCTION AddPartySystem():
    // Recruit multiple characters
    // Switch between party members
    // Team battles
END

FUNCTION AddStatusScreen():
    // Show player stats
    // Show equipment
    // Show achievements
END

FUNCTION ImplementItemUsage():
    // Consumable items di inventory
    // Use during battle
    // Use in world
END
```

---

## 12. NOTES

- **Game Loop**: Runs at 60 FPS (SetTargetFPS(60))
- **Resolution**: 1200x720 (resizable)
- **Graphics Library**: Raylib (cross-platform)
- **Audio**: Raylib Audio module
- **Scene System**: State-based with transition support
- **Input**: Keyboard & Mouse
- **Data Persistence**: Currently not saved (persists only during session)
- **Performance**: Simple scene switching for minimal memory overhead

---

## END OF PSEUDOCODE

```

Semua pseudocode di atas telah dibuat dan disimpan. Game "**Ninja Saga AlPro**" adalah:

**Tipe Game**: Turn-based RPG Battle Game dengan sistem Shop & Inventory

**Alur Utama**:
1. **Splash** → Intro screen dengan fade in/out
2. **Main Menu** → Pilih Play, Shop, Inventory, atau Exit
3. **Battle** → Fight enemy dengan Attack/Skills/Items
4. **Academy** → Beli skill dengan gold
5. **Inventory** → Kelola item dalam grid 3x3

**Fitur Utama**:
- ⚔️ **Turn-based Combat** dengan animasi dash
- 💰 **Shop System** untuk beli skill
- 🎒 **Inventory System** dengan kategori item
- 🎨 **Responsive UI** dengan hover effects
- 🎵 **Background Music & Sound Effects** (Raylib Audio)
- 📊 **Character Stats** (HP, MP, ATK, DEF)

Dokumentasi lengkap telah disimpan di file [PSEUDOCODE.md](PSEUDOCODE.md).