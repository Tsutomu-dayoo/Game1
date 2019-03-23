// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"

//二次元ベクトル
struct Vector2
{
    float x;
    float y;
};

struct Ball
{
    Vector2 Pos;
    Vector2 Vel;
};

class Game
{
public:
    Game();
    // ゲームの初期化
    bool Initialize();
    // ゲームループのを回す
    void RunLoop();
    // ゲームの終了
    void Shutdown();
private:
    // ゲームループのヘルパー関数
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    // Window created by SDL
    SDL_Window* mWindow;
    // 何かを描画したいときはSDL_Rendererを参照する
    SDL_Renderer* mRenderer;
    // ゲームが始まってからの時間
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    
    // Pong specific
    // Direction of paddle
    int mPaddleDir;
    int mPaddle2Dir;
    // Position of paddle
    Vector2 mPaddlePos;
    Vector2 mPaddle2Pos;
    // Position of ball
    Vector2 mBallPos;
    // Velocity of ball
    Vector2 mBallVel;
    
    Ball ball[2];
};
