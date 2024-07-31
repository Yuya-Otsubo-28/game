#ifndef G_VAR_H
# define G_VAR_H

# include "game.h"

bool KeyUpON    = false;	// 矢印キーの状態フラグ
bool KeyDownON  = false;	// 矢印キーの状態フラグ
bool KeyLeftON  = false;	// 矢印キーの状態フラグ
bool KeyRightON = false;	// 矢印キーの状態フラグ
bool KeyUpON1 = false;	// 矢印キーの状態フラグ
bool KeyDownON1 = false;	// 矢印キーの状態フラグ
bool KeyLeftON1 = false;	// 矢印キーの状態フラグ
bool KeyRightON1 = false;	// 矢印キーの状態フラグ
int MouseX = 0;	// マウスX座標
int MouseY = 0;	// マウスY座標
float SpinX = 0;	// X軸周りの回転量
float SpinY = 0;	// Y軸周りの回転量
float MoveX = 0;	// X軸方向の移動量
float MoveZ = 0;	// Y軸方向の移動量
float Scale = 1.0; // 拡大量
MyShip ship[SHIP_NUM]; // 自機の構造体を作成

int	ship_count;

int stage[MAP_HEIGHT][MAP_WIDTH]={
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,1,0,1,0,0,1},
    {1,0,1,3,1,0,1,1,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,3,0,1},
    {1,0,1,3,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,0,1},
    {1,0,0,2,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

#endif