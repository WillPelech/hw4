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

   mGameState.bgm = LoadMusicStream("assets/music/The Entertainer.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(gState.bgm);

   float sizeRatio  = 48.0f / 64.0f;
}

void start_screen::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
    


}

void start_screen::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   int fontSize = 40;
   int textWidth = MeasureText(start_text.c_str(), fontSize);
   int x = (GetScreenWidth() - textWidth) / 2;
   int y = (GetScreenHeight() - fontSize) / 2;
   DrawText(start_text.c_str(), x, y, fontSize, BLACK);
}

void start_screen::shutdown()
{

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}