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
const float windowW = 1024.0f;//ウィンドウの幅
const float windowH = 768.0f;//ウィンドウの高さ
Game::Game()//class内のprivateのメンバ関数の値や状態の記述
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
,mPaddle2Dir(0)
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
                               static_cast<int>(windowW),//ウィンドウの幅
                               static_cast<int>(windowH),//ウィンドウの高さ
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
    //ゲームオブジェクトの初期位置
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = windowH/2.0f;
    mPaddle2Pos.x = windowW - 10.0f - thickness;
    mPaddle2Pos.y = windowH/2.0f;
    mBallPos.x = windowW/2.0f;
    mBallPos.y = windowH/2.0f;
    mBallVel.x = 200.0f;
    mBallVel.y = -235.0f;
    
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
    }
    
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;// ESCでウィンドウが閉じる
    }
    
    mPaddleDir = 0;
    if(state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if(state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }
    mPaddle2Dir = 0;
    if(state[SDL_SCANCODE_I])
    {
        mPaddle2Dir -= 1;
    }
    if(state[SDL_SCANCODE_K])
    {
        mPaddle2Dir += 1;
    }
    
}

void Game::UpdateGame()
{
    //前回の更新から16ms待つようにする
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
    if(mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltatime;//方向＊ピクセル数＊デルタタイム
        //パドルが画面外に行かないようにする
        if(mPaddlePos.y < PaddleH/2.0f + thickness)
        {
            mPaddlePos.y = PaddleH/2.0f + thickness;
        }
        else if(mPaddlePos.y > windowH - PaddleH/2.0f - thickness)
        {
            mPaddlePos.y = windowH - PaddleH/2.0f - thickness;
        }
    }
    if(mPaddle2Dir != 0)
    {
        mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltatime;//方向＊ピクセル数＊デルタタイム
        //パドルが画面外に行かないようにする
        if(mPaddle2Pos.y < PaddleH/2.0f + thickness)
        {
            mPaddle2Pos.y = PaddleH/2.0f + thickness;
        }
        else if(mPaddle2Pos.y > windowH - PaddleH/2.0f - thickness)
        {
            mPaddle2Pos.y = windowH - PaddleH/2.0f - thickness;
        }
    }
    //ボールの位置の更新
    mBallPos.x += mBallVel.x * deltatime;
    mBallPos.y += mBallVel.y * deltatime;
    //ボールの接触判定
    float diff = mPaddlePos.y - mBallPos.y;
    diff = (diff<0.0f)? -diff : diff;
    float diff2 = mPaddle2Pos.y - mBallPos.y;
    diff2 = (diff2<0.0f)? -diff2 : diff2;
    if(mBallPos.y <= thickness && mBallVel.y < 0.0f)//上の壁にボールが接触し、かつ速度が負方向の時に反転
    {
        mBallVel.y *= -1;
    }
    else if(mBallPos.y >= (windowH - thickness) && mBallVel.y > 0.0f)//下の壁にボールが接触し、かつ速度が負方向の時に反転
    {
        mBallVel.y *= -1;
    }
    /*
    if(mBallPos.x >= (windowW - thickness) && mBallVel.x > 0.0f)//右の壁との接触判定
    {
        mBallVel.x *= -1;
    }
     */
    if(mBallVel.x < 0.0f && mBallPos.x >= 20.0f && mBallPos.x <= 25.0f && diff < PaddleH/2.0f)
    {
        mBallVel.x *= -1;
    }
    else if(mBallPos.x < 0.0f)
    {
        mIsRunning = false;
    }
    if(mBallVel.x > 0.0f && mBallPos.x >= mPaddle2Pos.x && mBallPos.x <= mPaddle2Pos.x + thickness && diff2 < PaddleH/2.0f)
    {
        mBallVel.x *= -1;
    }
    else if(mBallPos.x > windowW)
    {
        mIsRunning = false;
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer,0,0,255,255);//R,G,B,alpha
    SDL_RenderClear(mRenderer);// 現在の描画色でクリアする
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect wall{//top wall
        0,//x （左上隅）
        0,//y （左上隅）
        static_cast<int>(windowW),//幅
        thickness//高さ
    };
    SDL_RenderFillRect(mRenderer, &wall);//Draw rectangle
    wall.y = windowH-thickness;//bottom wall
    SDL_RenderFillRect(mRenderer, &wall);
    /*
    wall.x = windowW - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = windowW;
    SDL_RenderFillRect(mRenderer, &wall);// Right wall
    */
    
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - PaddleH/2.0f),
        thickness,
        static_cast<int>(PaddleH),
    };
    SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_Rect paddle2{
        static_cast<int>(mPaddle2Pos.x),
        static_cast<int>(mPaddle2Pos.y - PaddleH/2.0f),
        thickness,
        static_cast<int>(PaddleH),
    };
    SDL_RenderFillRect(mRenderer, &paddle2);
    
    SDL_Rect ball{//Draw ball
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),//static_cast -> floatをintに変換する
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_RenderPresent(mRenderer);//フロントバッファとバックバッファを入れ替える 必ず描画したいものを上にmRendererに描画し終える
    
    
}
