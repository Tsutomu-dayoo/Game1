//
//  Game.cpp
//  Game1
//
//  Created by 山口勉 on 2019/02/03.
//  Copyright © 2019年 山口勉. All rights reserved.
//

#include "Game.h"
#include <iostream>

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
{
    
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);//SDLの初期化に成功なら０を返す。
    if(sdlResult != 0)
    {
        printf("失敗");
        SDL_Log("%s", SDL_GetError());//SDL_GetErrorでエラーメッセージをC言語のスタイルにする
        return false;
    }
    
    mWindow = SDL_CreateWindow("Game Programming Sample1",//タイトル
                               100,//左上隅のx座標
                               100,//左上隅のy座標
                               1024,//ウィンドウの幅
                               768,//ウィンドウの高さ
                               0 //フラグ
                               );
    
    if(!mWindow)
    {
        printf("失敗");
        SDL_Log("%s",SDL_GetError());
        return false;
    }
    
    return true;//上記が問題なければtrue
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){//キューにイベントがあればtrue 引数はSDL_Eventのポインタ
        
    }
}

void Game::UpdateGame()
{
    
}

void Game::GenerateOutput()
{
    
}
