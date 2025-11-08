#include "Scene.h"

constexpr int LEVEL_WIDTH3 = 16,
              LEVEL_HEIGHT3 = 8;
class LevelC : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH3 * LEVEL_HEIGHT3] = {
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4, 
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0,  0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4,
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;
    Entity * ground_enemy1;
    Entity * ground_enemy2;
    Entity * jumper_enemy;
    Entity * key;
    Entity * door;
    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};