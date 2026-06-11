#pragma once
#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

enum class itemType { HEALTH_POTION, CHAKRA_POTION, WEAPON};

struct Item {
    string name;
    itemType type;
    int value;
    Texture2D icon;
};

enum class skillType { MELEE, PROJECTILE, HEALING, AREA};

struct Skill {
    string name;
    skillType type;
    int damage;
    int chakraCost;
    float chakraCooldown;
    
    Texture2D icon;
};

enum class weaponRarity { COMMON, RARE, EPIC, LEGENDARY};

struct weapon {
    string name;
    weaponRarity rarity;
    int damage;
    Texture2D icon;
};

struct playerStats {
    int exp;
    int level;
    int expToNextLevel;
    
    int hp;
    int maxHp;
    int chakra;
    int maxChakra;
};

struct Player {
    Vector2     position;
    Vector2     velocity;
    float       speed;
    bool        facingRight;
    bool        isAlive;

    weapon      equippedWeapon;
    playerStats stats;
    vector<Skill> skills;   // max
    vector<Item> inventory;

    Rectangle   hitbox;
    Texture2D   spriteSheet;
};

enum class enemyType { BANDIT, NINJA, BOSS};
enum class enemyState {
    IDLE,       // MENUNGGU GILIRAN NYERANG
    SELECTING,  // AI MEMILIH AKSI 
    ATTACKING,  // ANIMASI MENYERANG
    HURT,       // ANIMASI KENA DAMAGE
    DEAD
};

struct Enemy {
    enemyType type;
    enemyState state;

    int hp;
    int maxHp;
    int atk;
    int damage;
    int spd;  // urutan giliran

    vector<Skill> movePool; // skill yang bisa dipakai enemy

    Vector2     battlePosition; // posisi pas battle
    Vector2     velocity;
    float       speed;
    float       detectionRange;

    
    bool        isAlive;    
    Rectangle hitbox;
    Texture2D spriteSheet;

    int expReward;
    Item lootDrop; // bisa kosong

    
};

struct Projectile {
    Vector2 position;
    Vector2 velocity;

    int damage;
    bool isActive;
    bool fromPlayer;
    Texture2D texture;
    float lifetime;
};

enum class turnOwner { PLAYER, ENEMY};

enum class BattleActionType {
    ATTACK, // BASIC ATTACK
    SKILL,  // MILIH DAN PAKAI SKILL
    ITEM,   // MILIH DAN PAKAI ITEM
    RUN     // KABUR DARI BATTLE (TIDAK SELALU BERHASIL)
};

struct battleAction {
    BattleActionType type;
    int skillIndex; // diisi kalau type == skill
    int itemIndex;  // diisi kalau type == item
    int targetIndex; // index enemy yang diserang
};
enum class BattlePhase {
    START,          // animasi masuk battle
    PLAYER_TURN,    // menunggu input player
    PLAYER_ACTION,  // animasi aksi player
    ENEMY_TURN,     // AI enemy memilih aksi
    ENEMY_ACTION,   // animasi aksi enemy
    CHECK_WIN,      // cek apakah battle selesai
    WIN,            // semua enemy mati
    LOSE,           // player mati
    FLEE,           // player kabur
    END             // animasi keluar battle
};

struct battleState {
    BattlePhase phase;
    turnOwner currentTurn;
    Player* player;             // pointer ke player
    vector<Enemy> enemies;      // semua enemy di current battle
    vector<int> turnOrder;      // index di sortir by spd
    battleAction pendingAction; // aksi yang akan dieksekusi
    int activeEnemyIndex;       // giliran enemy
    string battleLog;           // teks log aksi terakhir
    float animTimer;            // timer untuk animasi
    bool isAnimating;           // sedang animasi, block input

};
enum class gameState {
    MAIN_MENU,
    PLAYING,   
    GAME_OVER,
    PAUSED,
    WIN
};