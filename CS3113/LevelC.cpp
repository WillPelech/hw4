#include "LevelC.h"

LevelC::LevelC()                                      : Scene { {0.0f}, nullptr   } {}
LevelC::LevelC(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("assets/music/DelRioBravo.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   mGameState.jumpSound = LoadSound("assets/mario.mp3");
   mGameState.hitSound = LoadSound("assets/hit.mp3");
   mGameState.keySound = LoadSound("assets/coin.mp3");

   /*
      ----------- MAP -----------
   */
   const unsigned int layoutA[LEVEL_WIDTH3 * LEVEL_HEIGHT3] = {
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4,
   };
   const unsigned int layoutB[LEVEL_WIDTH3 * LEVEL_HEIGHT3] = {
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,4,
      0,4, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0,4,
      0,4, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0,4,
      0,4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4,
   };
   const unsigned int layoutC[LEVEL_WIDTH3 * LEVEL_HEIGHT3] = {
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 0,4,
      0,4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0,4,
      0,4, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,4,
      0,4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,4,
      0,4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4,
   };

   int pick = GetRandomValue(0, 2);
   const unsigned int *chosen = (pick == 0) ? layoutA : (pick == 1) ? layoutB : layoutC;
   for (int i = 0; i < LEVEL_WIDTH3 * LEVEL_HEIGHT3; ++i) {
      mLevelData[i] = chosen[i];
   }

   mGameState.map = new Map(
      LEVEL_WIDTH3, LEVEL_HEIGHT3,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/tilemap_packed.png",   // texture filepath
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
      std::map<Direction, std::vector<int>> hyenaAnimationAtlas = {
      {DOWN,  { 0,1,2,3,4,5}},
      {LEFT,  { 0,1,2,3,4,5}},
      {UP,    { 0,1,2,3,4,5}},
      {RIGHT, { 0,1,2,3,4,5}},
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

   ground_enemy1=  new Entity(
      {mOrigin.x + 300.0f, mOrigin.y - 100.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/4 Vulture/Vulture_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 4},                                 // atlas dimensions
      vultureAnimationAtlas,                    // actual atlas
      NPC                                      // entity type
   );
   ground_enemy2=  new Entity(
      {mOrigin.x + 200.0f, mOrigin.y + 200.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/3 Scorpio/Scorpio_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 4},                                 // atlas dimensions
      vultureAnimationAtlas,                    // actual atlas
      NPC                                      // entity type
   );
   jumper_enemy=  new Entity(
      {mOrigin.x , mOrigin.y + 200.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/2 Hyena/Hyena_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 6},                                 // atlas dimensions
      hyenaAnimationAtlas,                    // actual atlas
      NPC                                      // ejntity type
   );
   key = new Entity(
      {mOrigin.x + 350.0f, mOrigin.y -100.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/tile_0066.png",                   // texture file address
      KEY 
   );
   door = new Entity(
      {mOrigin.x - 350.0f, mOrigin.y +50.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/tile_0067.png",                   // texture file address
      DOOR  
   );
   
   ground_enemy1->setAIType(WANDERER);
   ground_enemy1->setAIState(IDLE); 
   ground_enemy2->setAIType(WANDERER);
   ground_enemy2->setAIState(IDLE); 
   jumper_enemy->setAIType(JUMPER);
   jumper_enemy->setAIState(IDLE); 
   jumper_enemy->setColliderDimensions({
      jumper_enemy->getScale().x ,
      jumper_enemy->getScale().y 
   });
   jumper_enemy->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
   jumper_enemy->setJumpingPower(550.0f);
   mGameState.hero->setJumpingPower(550.0f);
   mGameState.hero->setColliderDimensions({
      mGameState.hero->getScale().x ,
      mGameState.hero->getScale().y 
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
   
   mGameState.prevLives = mGameState.hero->getLives();
   mGameState.prevHasKey = mGameState.hero->get_key();
}

void LevelC::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.hero->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );
   ground_enemy1->update(
      deltaTime,
      mGameState.hero,
      mGameState.map,
      nullptr,
      0
   );
   ground_enemy2->update(
      deltaTime,
      mGameState.hero,
      mGameState.map,
      nullptr,
      0
   );
   jumper_enemy->update(
      deltaTime,
      mGameState.hero,
      mGameState.map,
      nullptr,
      0
   );
   key->update(
    deltaTime,
    mGameState.hero,  
    mGameState.map,
    nullptr,
    0);
   door->update(
     deltaTime,
    mGameState.hero,  
    mGameState.map,
    nullptr,
    0);   
   if (door->getActive()==INACTIVE &&mGameState.hero->get_key()){
      mGameState.hero->remove_key();
      mGameState.nextSceneID = 5;
   } 
   
   if (mGameState.hero->getLives() < mGameState.prevLives) {
      PlaySound(mGameState.hitSound);
   }
   if (!mGameState.prevHasKey && mGameState.hero->get_key()) {
      PlaySound(mGameState.keySound);
   }
   mGameState.prevLives = mGameState.hero->getLives();
   mGameState.prevHasKey = mGameState.hero->get_key();
   
   if (mGameState.hero->getLives() == 0){
      mGameState.nextSceneID = 4;
   }


   Vector2 currentPlayerPosition = { mGameState.hero->getPosition().x, mOrigin.y };

   if (mGameState.hero->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelC::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   mGameState.hero->render();
   ground_enemy1->render();
   ground_enemy2->render();
   jumper_enemy->render();
   key->render();
   door->render();
}

void LevelC::shutdown()
{
   delete mGameState.hero;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
   UnloadSound(mGameState.hitSound);
   UnloadSound(mGameState.keySound);
}