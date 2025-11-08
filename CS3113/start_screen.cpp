#include "start_screen.h"

start_screen::start_screen() :Scene{ {0.0f},nullptr}{}
start_screen::start_screen(Vector2 origin, const char *bgHexCode, const char* start_text)
    : Scene { origin, bgHexCode }, 
    start_text(start_text)
{
}

start_screen::~start_screen() { shutdown(); }

void start_screen::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("assets/music/TheEntertainer.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   float sizeRatio  = 48.0f / 64.0f;
}

void start_screen::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
    


}

void start_screen::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   int titleSize = 50;
   const char* title = "Escape the Desert";
   int titleWidth = MeasureText(title, titleSize);
   int titleX = static_cast<int>(mOrigin.x - titleWidth / 2);
   int titleY = static_cast<int>(mOrigin.y) - 120;
   DrawText(title, titleX, titleY, titleSize, BLACK);

   int fontSize = 32;
   int textWidth = MeasureText(start_text.c_str(), fontSize);
   int x = static_cast<int>(mOrigin.x - textWidth / 2);
   int y = titleY + titleSize + 24;
   DrawText(start_text.c_str(), x, y, fontSize, BLACK);
}

void start_screen::shutdown()
{
   StopMusicStream(mGameState.bgm);
   UnloadMusicStream(mGameState.bgm);
}