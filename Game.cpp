//
//  Game.cpp
//  Game1
//
//  Created by 山口勉 on 2019/02/03.
//  Copyright © 2019年 山口勉. All rights reserved.
//

#include "Game.h"
#include <iostream>

const int thickness = 15;
const float PaddleH = 100.0f;

Game::Game()//class内のprivateのメンバ関数の値や状態の記述
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
        printf("SDLの初期化に失敗");
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
        printf("画面作成に失敗");
        SDL_Log("%s",SDL_GetError());
        return false;
    }
    mRenderer = SDL_CreateRenderer(mWindow,// mWindowに描画
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                                   );
    
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f;
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    
    return true;//上記が問題なければtrue
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
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
    while(SDL_PollEvent(&event))//キューにイベントがあればtrue 引数はSDL_Eventのポインタ
    {
        switch(event.type)
        {
            case SDL_QUIT:// バツボタンでウィンドウが消える
                mIsRunning = false;
                break;
        }
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_ESCAPE])
        {
            mIsRunning = false;// ESCでウィンドウが閉じる
        }
    }
}

void Game::UpdateGame()
{
    //wait update unill 16ms has elapsed since last frame
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    
    // deltatime is the difference between last last frame and current frame
    float deltatime = (SDL_GetTicks() - mTicksCount) / 1000.0f; //convert to secounds
    mTicksCount = SDL_GetTicks();//update time
    //デルタタイムが大きくなり過ぎないように最大値を設けて制限
    if(deltatime > 0.05f)
    {
        deltatime = 0.05f;
    }
    // update game objects
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer,0,0,255,255);//R,G,B,alpha
    SDL_RenderClear(mRenderer);// 現在の描画色でクリアする
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect wall{//top wall
        0,//x
        0,//y
        1024,//幅
        thickness//高さ
    };
    SDL_RenderFillRect(mRenderer, &wall);//Draw rectangle
    wall.y = 768-thickness;//bottom wall
    SDL_RenderFillRect(mRenderer, &wall);
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);// Right wall
    
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - PaddleH/2.0f),
        thickness,
        static_cast<int>(PaddleH),
    };
    SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_Rect ball{//Draw ball
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),//static_cast -> floatをintに変換する
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_RenderPresent(mRenderer);//フロントバッファとバックバッファを入れ替える 必ず描画したいものを上にmRendererに描画し終える
    
    
}
