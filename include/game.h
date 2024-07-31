#ifndef GAME_H
# define GAME_H

# include <GL/glut.h>
# include <bits/stdc++.h>

# define GL_SILENCE_DEPRECATION

// グローバル変数
static bool KeyUpON    = false;	// 矢印キーの状態フラグ
static bool KeyDownON  = false;	// 矢印キーの状態フラグ
static bool KeyLeftON  = false;	// 矢印キーの状態フラグ
static bool KeyRightON = false;	// 矢印キーの状態フラグ

static int MouseX = 0;	// マウスX座標
static int MouseY = 0;	// マウスY座標
static float SpinX = 0;	// X軸周りの回転量
static float SpinY = 0;	// Y軸周りの回転量

static float MoveX = 0;	// X軸方向の移動量
static float MoveZ = 0;	// Y軸方向の移動量
static float Scale = 1.0; // 拡大量

#endif