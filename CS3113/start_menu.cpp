#include "start_menu.h"

start_menu::start_menu() : Scene{{0.0f}, nullptr} {}
start_menu::start_menu(Vector2 origin, const char* bgHexCode)
    : Scene{origin, bgHexCode} {}

start_menu::~start_menu() { shutdown(); }

void start_menu::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.bgm = LoadMusicStream("assets/music/TheEntertainer.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);
}

void start_menu::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);
    // Choose which level to start at
    if (IsKeyPressed(KEY_ONE)) { mGameState.nextSceneID = 1; } // Level A
    if (IsKeyPressed(KEY_TWO)) { mGameState.nextSceneID = 2; } // Level B
    if (IsKeyPressed(KEY_THREE)) { mGameState.nextSceneID = 3; } // Level C
}

void start_menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char* title = "Escape the Desert";
    int titleSize = 48;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = static_cast<int>(mOrigin.x - titleWidth / 2);
    int titleY = static_cast<int>(mOrigin.y) - 140;
    DrawText(title, titleX, titleY, titleSize, BLACK);

    int fontSize = 28;
    const char* subtitle = "Select a level to start:";
    int subWidth = MeasureText(subtitle, fontSize);
    int subX = static_cast<int>(mOrigin.x - subWidth / 2);
    int subY = titleY + titleSize + 20;
    DrawText(subtitle, subX, subY, fontSize, BLACK);

    int lineY = subY + fontSize + 20;
    DrawText("1) Level A", subX, lineY, fontSize, BLACK);
    DrawText("2) Level B", subX, lineY + 36, fontSize, BLACK);
    DrawText("3) Level C", subX, lineY + 72, fontSize, BLACK);

    DrawText("Press Q to Quit", subX, lineY + 120, fontSize - 4, DARKGRAY);
}

void start_menu::shutdown()
{
    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);
}
