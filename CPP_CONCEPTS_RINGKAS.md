# C++ CONCEPTS - PSEUDOCODE RINGKAS
## Berdasarkan Ninja Saga AlPro Game Implementation

---

## 1. STRUCT
**Penggunaan:** Definisi model data (Character, Skill, Item)

```pseudocode
STRUCT Character:
    name : string
    hp : int
    maxHp : int
    mp : int
    maxMp : int
    attack : int
    defense : int
    gold : int
    skills : vector<Skill>
    inventory : vector<Item>
    
    FUNCTION IsAlive() -> bool:
        RETURN hp > 0
    END
END
```

---

## 2. REFERENCE (&)
**Penggunaan:** Efisien passing argumen fungsi (hindari copy), const reference

```pseudocode
FUNCTION SaveGame(filename : string, player : const reference to Character):
    // player adalah reference → tidak perlu copy data
    // const = hanya read, tidak bisa modify
    
    file = OPEN(filename, "WRITE")
    WRITE(file, "Name: " + player.name)
    WRITE(file, "Gold: " + player.gold)
    WRITE(file, "HP: " + player.hp)
    CLOSE(file)
END

// Usage:
CHARACTER player = CreatePlayer()
SaveGame("save.txt", player)  // Pass by const reference
```

---

## 3. POINTER (*)
**Penggunaan:** Referensi memory global, dynamic allocation

```pseudocode
STRUCT GameContext:
    player : pointer to Character = nullptr
    // Menyimpan alamat memory dari global player
END

// main.cpp:
CHARACTER player = CreatePlayer()
CONTEXT : GameContext
context.player = address_of(player)  // Point to player

// Access via pointer:
IF context.player != nullptr:
    (*context.player).hp = 20  // Dereference & modify
    (*context.player).TakeDamage(5)
END
```

---

## 4. STL VECTOR
**Penggunaan:** Dynamic container untuk skills & inventory

```pseudocode
CLASS Character:
    PRIVATE:
        skills : vector<Skill>
        inventory : vector<Item>
    
    PUBLIC:
        FUNCTION AddSkill(skill : Skill):
            skills.PUSH_BACK(skill)
        END
        
        FUNCTION GetSkillCount() -> int:
            RETURN skills.SIZE()
        END
        
        FUNCTION RemoveSkill(index : int):
            IF index >= 0 AND index < skills.SIZE():
                skills.ERASE(index)
            END
        END
END
```

---

## 5. NAMESPACE
**Penggunaan:** Organisasi code (std::, custom GameUtils)

```pseudocode
NAMESPACE std:
    // C++ standard library
    string, vector, map, iostream, fstream, algorithm
END

NAMESPACE GameUtils:
    // Custom namespace untuk fungsi-fungsi game
    FUNCTION CalculateDamage(base : int) -> int:
        RETURN base + RandomRange(-1, 2)
    END
    
    FUNCTION LoadAssets() -> bool:
        ...
    END
END

// Usage:
USING NAMESPACE std
USING NAMESPACE GameUtils

name : string = "Ninja"  // std::string
damage : int = CalculateDamage(5)  // GameUtils::
```

---

## 6. CALLBACK
**Penggunaan:** Function pointer untuk event handling

```pseudocode
// Define callback type
TYPE SaveCallback = FUNCTION(message : string) -> void

NAMESPACE GameUtils:
    saveCallback : SaveCallback = nullptr
    
    FUNCTION SetSaveCallback(callback : SaveCallback):
        saveCallback = callback
    END
    
    FUNCTION OnSaveComplete(msg : string):
        IF saveCallback != nullptr:
            saveCallback(msg)  // Panggil callback
        END
    END
    
    FUNCTION SaveGameData():
        // ... save logic ...
        OnSaveComplete("Game saved successfully!")
    END
END

// Usage:
FUNCTION LogMessage(msg : string):
    PRINT "[LOG] " + msg
END

SetSaveCallback(LogMessage)  // Register callback
SaveGameData()  // Will call LogMessage("Game saved...")
```

---

## 7. EXCEPTION
**Penggunaan:** Error handling dengan try-catch-throw

```pseudocode
FUNCTION LoadGameData(filename : string) -> bool:
    TRY:
        IF NOT FileExists(filename):
            THROW runtime_error("Save file not found: " + filename)
        END
        
        file = OPEN(filename, "READ")
        IF file == nullptr:
            THROW runtime_error("Cannot open file")
        END
        
        // Parse data...
        CLOSE(file)
        RETURN true
    
    CATCH runtime_error AS error:
        PRINT "Error: " + error.message
        RETURN false
    
    CATCH Exception AS e:
        PRINT "Unknown error occurred"
        RETURN false
    
    END TRY
END
```

---

## 8. LAMBDA
**Penggunaan:** Fungsi anonim untuk sorting, filtering, callback

```pseudocode
// Sort skills by mana cost (ascending)
FUNCTION SortSkillsByManaCost(player : reference to Character):
    player.skills.SORT([](skill1 : const Skill&, skill2 : const Skill&) -> bool:
        RETURN skill1.manaCost < skill2.manaCost
    END)
END

// Filter skills by damage type
FUNCTION CountHighDamageSkills(skills : vector<Skill>) -> int:
    count : int = 0
    
    FOR EACH skill IN skills:
        IF [](s : const Skill&) -> bool:
            RETURN s.baseDamage > 5
        END(skill):
            count += 1
        END
    END
    
    RETURN count
END

// Lambda as button callback
button.SetOnClick([this]() -> void:
    RequestSceneChange(SceneType::Gameplay)
END)
```

---

## 9. FILE HANDLING
**Penggunaan:** Save/Load game data ke file eksternal

```pseudocode
FUNCTION SaveGame(filename : string, player : const reference to Character):
    file : ofstream = OPEN(filename, "WRITE")
    
    IF NOT file.IS_OPEN():
        THROW runtime_error("Cannot create file")
    END
    
    // Write data
    file << "NAME:" << player.name << "\n"
    file << "HP:" << player.hp << ":" << player.maxHp << "\n"
    file << "MP:" << player.mp << ":" << player.maxMp << "\n"
    file << "GOLD:" << player.gold << "\n"
    
    // Write skills
    FOR EACH skill IN player.skills:
        file << "SKILL:" << skill.id << ":" << skill.name << "\n"
    END
    
    file.CLOSE()
END

FUNCTION LoadGame(filename : string, player : reference to Character):
    file : ifstream = OPEN(filename, "READ")
    
    IF NOT file.IS_OPEN():
        THROW runtime_error("Save file not found")
    END
    
    line : string
    WHILE READ_LINE(file, line):
        // Parse line dan populate player
        IF line STARTS_WITH "NAME:":
            player.name = ParseValue(line)
        ELSE IF line STARTS_WITH "GOLD:":
            player.gold = ParseInt(line)
        END
    END
    
    file.CLOSE()
END
```

---

## 10. SORT
**Penggunaan:** Mengurutkan data dengan std::sort + lambda

```pseudocode
// Sort player skills by manaCost (ascending)
FUNCTION SortSkillsByManaCost(player : reference to Character):
    std::sort(player.skills.BEGIN(), player.skills.END(),
        [](const Skill& a, const Skill& b) -> bool:
            RETURN a.manaCost < b.manaCost
        END)
    PRINT "Skills sorted by mana cost"
END

// Sort enemies by HP (lowest first)
FUNCTION SortEnemiesByHP(enemies : reference to vector<Character>):
    std::sort(enemies.BEGIN(), enemies.END(),
        [](const Character& e1, const Character& e2) -> bool:
            RETURN e1.hp < e2.hp
        END)
END

// Sort inventory items by sellPrice (descending)
FUNCTION SortItemsByPrice(inventory : reference to vector<Item>):
    std::sort(inventory.BEGIN(), inventory.END(),
        [](const Item& a, const Item& b) -> bool:
            RETURN a.sellPrice > b.sellPrice  // Descending
        END)
END
```

---

## 11. FIND
**Penggunaan:** Mencari elemen dengan std::find_if

```pseudocode
// Find skill by ID
FUNCTION FindSkillById(player : const reference to Character, 
                      skillId : int) -> pointer to Skill:
    
    iterator = std::find_if(player.skills.BEGIN(), player.skills.END(),
        [skillId](const Skill& s) -> bool:
            RETURN s.id == skillId
        END)
    
    IF iterator != player.skills.END():
        RETURN address_of(*iterator)
    END
    
    RETURN nullptr
END

// Find first alive enemy
FUNCTION FindAliveEnemy(enemies : const reference to vector<Character>) 
    -> pointer to Character:
    
    iterator = std::find_if(enemies.BEGIN(), enemies.END(),
        [](const Character& e) -> bool:
            RETURN e.IsAlive()
        END)
    
    IF iterator != enemies.END():
        RETURN address_of(*iterator)
    END
    
    RETURN nullptr
END

// Find item by name
FUNCTION FindItemByName(inventory : const reference to vector<Item>,
                       name : const string&) -> int:
    
    FOR i = 0 TO inventory.SIZE() - 1:
        IF inventory[i].name == name:
            RETURN i
        END
    END
    
    RETURN -1  // Not found
END
```

---

## 12. COUNT
**Penggunaan:** Menghitung elemen dengan std::count_if

```pseudocode
// Count skills dengan mana cost tertentu
FUNCTION CountSkillsWithMinManaCost(player : const reference to Character,
                                   minCost : int) -> int:
    
    count = std::count_if(player.skills.BEGIN(), player.skills.END(),
        [minCost](const Skill& s) -> bool:
            RETURN s.manaCost >= minCost
        END)
    
    RETURN count
END

// Count consumable items
FUNCTION CountConsumables(player : const reference to Character) -> int:
    count : int = 0
    
    FOR EACH item IN player.inventory:
        IF item.category == ItemCategory::Consumable:
            count += item.quantity
        END
    END
    
    RETURN count
END

// Count alive enemies
FUNCTION CountAliveEnemies(enemies : const reference to vector<Character>) -> int:
    count = std::count_if(enemies.BEGIN(), enemies.END(),
        [](const Character& e) -> bool:
            RETURN e.IsAlive()
        END)
    
    RETURN count
END
```

---

## 13. OVERLOADING
**Penggunaan:** Multiple function signatures & operator overloading

```pseudocode
// ========== METHOD OVERLOADING ==========
CLASS Character:
    PUBLIC:
        // Overload 1: Raw damage
        FUNCTION TakeDamage(rawDamage : int):
            actualDamage : int = rawDamage - defense
            IF actualDamage < 1:
                actualDamage = 1
            END
            hp -= actualDamage
        END
        
        // Overload 2: With ignore defense flag
        FUNCTION TakeDamage(rawDamage : int, ignoreDefense : bool):
            IF ignoreDefense:
                hp -= rawDamage
            ELSE:
                TakeDamage(rawDamage)  // Call overload 1
            END
        END
        
        // Overload 3: From skill
        FUNCTION TakeDamage(skill : const Skill&):
            TakeDamage(skill.baseDamage)
        END
END

// ========== OPERATOR OVERLOADING ==========
CLASS Character:
    PUBLIC:
        // operator+ untuk add gold
        FUNCTION operator+(amount : int) -> Character:
            CHARACTER result = this
            result.gold += amount
            RETURN result
        END
        
        // operator- untuk subtract gold
        FUNCTION operator-(amount : int) -> Character:
            CHARACTER result = this
            result.gold -= amount
            IF result.gold < 0:
                result.gold = 0
            END
            RETURN result
        END
END

// Usage:
CHARACTER player = CreatePlayer()

player.TakeDamage(10)              // Overload 1
player.TakeDamage(15, true)        // Overload 2 (ignore defense)
player.TakeDamage(fireSlashSkill)  // Overload 3

player = player + 50  // Add 50 gold (operator+)
player = player - 30  // Subtract 30 gold (operator-)
```

---

## RINGKASAN PENGGUNAAN

| # | Konsep | Lokasi | Fungsi |
|---|--------|--------|--------|
| 1 | Struct | Character.h, Skill.h, Item.h | Model data entities |
| 2 | Reference (&) | GameUtils.cpp | Efficient function parameters |
| 3 | Pointer (*) | types.h, main.cpp | Global context access |
| 4 | Vector | Character.h | Dynamic skills/inventory |
| 5 | Namespace | std::, GameUtils | Code organization |
| 6 | Callback | GameUtils.h | Event handling |
| 7 | Exception | GameUtils.cpp | Error handling |
| 8 | Lambda | GameUtils.cpp, Scenes | Sorting, filtering, events |
| 9 | File Handling | GameUtils.cpp | Save/Load game |
| 10 | Sort | GameUtils.cpp | Arrange skills/enemies |
| 11 | Find | GameUtils.cpp | Search elements |
| 12 | Count | GameUtils.cpp | Count by condition |
| 13 | Overloading | Character.h | Multiple TakeDamage(), operator+ |

---
