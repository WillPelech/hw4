#include "start_screen.h"

start_screen::start_screen() :Scene{ {0.0f},nullptr};
start_screen::start_screen(Vector2 origin, const char *bgHexCode, const char* start_text) : Scene { origin, bgHexCode } {}

start_screen::~start_screen() { shutdown(); }

void start_screen::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("");
   SetMusicVolume(mGameState.bgm, 0.33f);
   // PlayMusicStream(gState.bgm);

   float sizeRatio  = 48.0f / 64.0f;
}

void start_screen::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);



}

void start_screen::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));
   DrawText(start_text,0.0f,0.0f,BLACK);

}

void start_screen::shutdown()
{

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}