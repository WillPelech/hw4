#include "LevelB.h"

LevelB::LevelB()                                      : Scene { {0.0f}, nullptr   } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("assets/music/DelRioBravo.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH2, LEVEL_HEIGHT2,   // map grid cols & rows
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
      "assets/3 Scorpio/Scorpio_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 4},                                 // atlas dimensions
      vultureAnimationAtlas,                    // actual atlas
      NPC                                      // entity type
   );
   ground_enemy2=  new Entity(
      {mOrigin.x + 300.0f, mOrigin.y + 200.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/3 Scorpio/Scorpio_walk.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      {1, 4},                                 // atlas dimensions
      vultureAnimationAtlas,                    // actual atlas
      NPC                                      // entity type
   );
   key = new Entity(
      {mOrigin.x + 300.0f, mOrigin.y -100.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/tile_0066.png",                   // texture file address
      KEY 
   );
   door = new Entity(
      {mOrigin.x - 300.0f, mOrigin.y +200.0f}, // position
      {50.0f , 50.0f*sizeRatio},             // scale
      "assets/tile_0067.png",                   // texture file address
      DOOR  
   );
   
   ground_enemy1->setAIType(WANDERER);
   ground_enemy1->setAIState(IDLE); 
   ground_enemy2->setAIType(WANDERER);
   ground_enemy2->setAIState(IDLE); 
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
}

void LevelB::update(float deltaTime)
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
      mGameState.nextSceneID = 3;
   } 

   
   if (mGameState.hero->getLives() == 0){
      mGameState.nextSceneID = 4;
   }

   Vector2 currentPlayerPosition = { mGameState.hero->getPosition().x, mOrigin.y };

   if (mGameState.hero->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelB::render()
{
   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.map->render();
   mGameState.hero->render();
   ground_enemy1->render();
   ground_enemy2->render();
   key->render();
   door->render();
}

void LevelB::shutdown()
{
   delete mGameState.hero;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.jumpSound);
}