#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Sound music[10];

Game::Game()
{
    state = GameState::MAIN_MENU;
    scene = nullptr;
    img_menu = nullptr;
    img_insertcoin = nullptr;
    img_player_selc = nullptr;
    img_lvl1=nullptr;
    img_lvl6=nullptr;
    img_lvl34=nullptr;
    img_lvl46=nullptr;
    img_lvl100=nullptr;


    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }

}

AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Bubble Bobble v0.5");

    InitAudioDevice();

    //Load custom icon image from memory
    Image customIcon = LoadImage("images/Bubble_Bobble_sprite_bubblun.png");
    SetWindowIcon(customIcon);
    UnloadImage(customIcon);

    music[0] = LoadSound("BubbleBobble_Audio&SFX/SFX_WAV/TitleSFX.wav");
    PlaySound(music[0]);
    SetSoundVolume(music[0], 0.1f);
    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "BubbleBobble_Art/UI/Title1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_INSERTCOIN, "BubbleBobble_Art/UI/InsertCoinScreenBB.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_insertcoin = data.GetTexture(Resource::IMG_INSERTCOIN);

    if (data.LoadTexture(Resource::IMG_PLAYER_SELC, "BubbleBobble_Art/UI/SelectPlayerScreenBB.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_player_selc = data.GetTexture(Resource::IMG_PLAYER_SELC);

//lvl1-100
    if (data.LoadTexture(Resource::IMG_LVL1, "BubbleBobble_Art/Levels/level1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lvl1 = data.GetTexture(Resource::IMG_LVL1);

    if (data.LoadTexture(Resource::IMG_LVL6, "BubbleBobble_Art/Levels/level6.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lvl6 = data.GetTexture(Resource::IMG_LVL6);

    if (data.LoadTexture(Resource::IMG_LVL34, "BubbleBobble_Art/Levels/level34.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lvl34 = data.GetTexture(Resource::IMG_LVL34);

    if (data.LoadTexture(Resource::IMG_LVL46, "BubbleBobble_Art/Levels/level46.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lvl46 = data.GetTexture(Resource::IMG_LVL46);

    if (data.LoadTexture(Resource::IMG_LVL100, "BubbleBobble_Art/UI/SelectPlayerScreenBB.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lvl100 = data.GetTexture(Resource::IMG_LVL100);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    music[1] = LoadSound("BubbleBobble_Audio&SFX/Music_OGG/MainTheme_Music.ogg");
    PlaySound(music[1]);
    SetSoundVolume(music[1], 0.1f);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }
    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::MAIN_MENU:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::INSERT_COIN;

        }
        break;
    case GameState::INSERT_COIN:

        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            state = GameState::PLAYER_SELC;
        }
        break;
    case GameState::PLAYER_SELC:

        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            state = GameState::PLAYING;
        }
        break;
    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            FinishPlay();
            state = GameState::MAIN_MENU;
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
    case GameState::SCROLL:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            FinishPlay();
            state = GameState::MAIN_MENU;
        } 
        else if (IsKeyPressed(KEY_E)) {
            state = GameState::SCROLL;
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
	}
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;
    case GameState::INSERT_COIN:
        DrawTexture(*img_insertcoin, 0, 0, WHITE);
        break;
    case GameState::PLAYER_SELC:
        DrawTexture(*img_player_selc, 0, 0, WHITE);
        break;
    case GameState::PLAYING:
        scene->Render();
        break;
    case GameState::SCROLL:
        float scroll=time_elapsed/total_time;
        float ypos2=WINDOW_HEIGHT*-scroll;
        if(time_elapsed<total_time)
        {
            DrawTexture(*img_lvl1, 0, ypos2, WHITE);
            DrawTexture(*img_lvl6, 0, ypos2+WINDOW_HEIGHT, WHITE);
            DrawTexture(*img_lvl34, 0, ypos2+WINDOW_HEIGHT, WHITE);
            DrawTexture(*img_lvl46, 0, ypos2+WINDOW_HEIGHT, WHITE);
            DrawTexture(*img_lvl100, 0, ypos2+WINDOW_HEIGHT, WHITE);

            time_elapsed+=GetFrameTime();

        }
        else
		{
            time_elapsed=0.0f;
			state=GameState::PLAYING;
            scene->LoadLevel(2);
		}
    
        break;
    
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseAudioDevice();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_INSERTCOIN);
    data.ReleaseTexture(Resource::IMG_PLAYER_SELC);
    data.ReleaseTexture(Resource::IMG_LVL1);
    data.ReleaseTexture(Resource::IMG_LVL6);
    data.ReleaseTexture(Resource::IMG_LVL34);
    data.ReleaseTexture(Resource::IMG_LVL46);
    data.ReleaseTexture(Resource::IMG_LVL100);

    UnloadRenderTexture(target);
}