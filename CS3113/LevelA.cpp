#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("assets/music/DelRioBravo.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   //mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tilemap.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      15, 10,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> heroAnimationAtlas = {
      {DOWN,  { 0,1,2,3,4,5,6}},
      {LEFT,  { 0,1,2,3,4,5,6}},
      {UP,    { 0,1,2,3,4,5,6 }},
      {RIGHT, { 0,1,2,3,4,5,6}},
   };
   std::map<Direction, std::vector<int>> vultureAnimationAtlas = {
      {DOWN,  { 0,1,2,3}},
      {LEFT,  { 0,1,2,3}},
      {UP,    { 0,1,2,3}},
      {RIGHT, { 0,1,2,3}},
   };
   float sizeRatio  = 50.0f / 64.0f;

   mGameState.hero = new Entity(
      {mOrigin.x - 300.0f, mOrigin.y - 200.0f}, // position
      {50.0f * sizeRatio, 50.0f},             // scale
      "assets/owlet/Owlet_Monster_Walk_6.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 6},                                 // atlas dimensions
      heroAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   flying_enemy =  new Entity(
      {mOrigin.x + 300.0f, mOrigin.y - 200.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/4 Vulture/Vulture_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 4},                                 // atlas dimensions
      vultureAnimationAtlas,                    // actual atlas
      NPC                                    // entity type
   );
   flying_enemy->setAIType(FOLLOWER);
   flying_enemy->setAIState(IDLE); 
   mGameState.hero->setJumpingPower(550.0f);
   mGameState.hero->setColliderDimensions({
      mGameState.hero->getScale().x / 3.5f,
      mGameState.hero->getScale().y / 3.0f
   });
   mGameState.hero->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.hero->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelA::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.hero->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );
   flying_enemy->update(
      deltaTime,
      mGameState.hero,
      mGameState.map,
      nullptr,
      0
      );

   Vector2 currentPlayerPosition = { mGameState.hero->getPosition().x, mOrigin.y };

   if (mGameState.hero->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   mGameState.hero->render();
   flying_enemy->render();
}

void LevelA::shutdown()
{
   delete mGameState.hero;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}