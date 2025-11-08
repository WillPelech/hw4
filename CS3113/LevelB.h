#include "Scene.h"

constexpr int LEVEL_WIDTH2 = 16,
              LEVEL_HEIGHT2 = 8;
class LevelB : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH2 * LEVEL_HEIGHT2] = {
        0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
        0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
        0,4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
        2,4, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,4,
        0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
        0,4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,4,
        0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
        0,4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4,
    };

public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;
    Entity * ground_enemy1;
    Entity * ground_enemy2;
    Entity * key;
    Entity * door;
    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};