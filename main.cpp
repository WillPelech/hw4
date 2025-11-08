#include "cs3113.h"
#include "Scene.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "start_screen.h"
#include "game_lost.h"
#include "start_menu.h"
#include "game_won.h"
// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 7;
//make the camera follow the character
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

start_screen *gstart_screen = nullptr;
start_menu *gstart_menu = nullptr;
LevelA *gLevelA = nullptr ;
LevelB *gLevelB = nullptr ;
LevelC *gLevelC = nullptr ;
game_lost *ggame_lost = nullptr;
game_won *ggame_won = nullptr;
// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    gstart_screen = new start_screen(ORIGIN, "#C0897E", "Press S to Start  |  Press M for Menu");
    gstart_menu = new start_menu(ORIGIN, "#C0897E");
    gLevelA = new LevelA(ORIGIN, "#C0897E");
    gLevelB = new LevelB(ORIGIN, "#C0897E");
    gLevelC = new LevelC(ORIGIN, "#C0897E");
    ggame_lost= new game_lost(ORIGIN, "#000000", "GAME OVER");
    ggame_won = new game_won(ORIGIN, "#000000", "GAME WON");
    // gLevelC = new LevelC(ORIGIN, "#C0897E");

    gLevels.push_back(gstart_screen);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(ggame_lost);
    gLevels.push_back(ggame_won);
    gLevels.push_back(gstart_menu);
    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() 
{   

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) { gAppStatus = TERMINATED; return; }

    if (gCurrentScene == gstart_screen){
        if(IsKeyDown(KEY_S)){switchToScene(gLevels[1]);}
        if(IsKeyPressed(KEY_M)){switchToScene(gLevels[6]);}
        return;
    } 
    // Some scenes (e.g., game_lost) have no hero; skip player input in that case
    if (gCurrentScene->getState().hero == nullptr) return;

    gCurrentScene->getState().hero->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().hero->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().hero->moveRight();

    if (IsKeyPressed(KEY_W) && 
        gCurrentScene->getState().hero->isCollidingBottom())
    {
        gCurrentScene->getState().hero->jump();
    }

    if (GetLength(gCurrentScene->getState().hero->getMovement()) > 1.0f) 
        gCurrentScene->getState().hero->normaliseMovement();
    
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() 
{
    delete gstart_screen;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete ggame_lost;
    delete ggame_won;
    // delete gLevelC;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            if (id >= 0 && id < (int)gLevels.size() && gLevels[id] != nullptr) {
                int carriedLives = -1;
                if (gCurrentScene->getState().hero != nullptr) {
                    carriedLives = gCurrentScene->getState().hero->getLives();
                }
                switchToScene(gLevels[id]);
                if (carriedLives >= 0 && gCurrentScene->getState().hero != nullptr) {
                    gCurrentScene->getState().hero->setLives(carriedLives);
                }
            }
        }

        render();
    }

    shutdown();

    return 0;
}