#include "Scene.h"

Scene::Scene() : mOrigin{{}} {}

Scene::Scene(Vector2 origin, const char *bgHexCode) : mOrigin{origin}, mBGColourHexCode {bgHexCode} 
{
    ClearBackground(ColorFromHex(bgHexCode));
    mGameState.camera.target = { 0.0f, 0.0f };
    mGameState.camera.offset = { 0.0f, 0.0f };
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}