//
//  main.cpp
//  Game1
//
//  Created by 山口勉 on 2019/02/03.
//  Copyright © 2019年 山口勉. All rights reserved.
//

#include <iostream>
#include "Game.h"


int main(int argc, const char * argv[]) {
    Game game;
    bool success = game.Initialize();
    if(success)
    {
        printf("成功");
        game.RunLoop();//ゲームのループを開始
    }
    game.Shutdown();
    return 0;
}
