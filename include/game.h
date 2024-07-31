#ifndef GAME_H
# define GAME_H

# include <GL/glut.h>
# include <bits/stdc++.h>

# define GL_SILENCE_DEPRECATION
# define MAX_SHOT  10 // 同時に撃てる弾の最大数
# define MAX_ENEMY  10 // 同時に出現する敵の最大数

// // グローバル変数

// bool KeyUpON    = false;	// 矢印キーの状態フラグ
// bool KeyDownON  = false;	// 矢印キーの状態フラグ
// bool KeyLeftON  = false;	// 矢印キーの状態フラグ
// bool KeyRightON = false;	// 矢印キーの状態フラグ

// int MouseX = 0;	// マウスX座標
// int MouseY = 0;	// マウスY座標
// float SpinX = 0;	// X軸周りの回転量
// float SpinY = 0;	// Y軸周りの回転量

// float MoveX = 0;	// X軸方向の移動量
// float MoveZ = 0;	// Y軸方向の移動量
// float Scale = 1.0; // 拡大量

typedef struct _MyShot{
	int isAlive; // 自機の弾が生きてるか？
	float x;  // 弾のx座標
	float z;  // 弾のz座標
	float vx; // 弾のx軸方向速度
	float vz; // 弾のz軸方向速度
} MyShot;
// 自機の情報を格納する構造体
typedef struct _MyShip{
	int isAlive;   // 自機が生きてるか？
	float x;  // 自機のx座標
	float z;  // 自機のz座標
	MyShot myShot[MAX_SHOT];// 弾を管理する構造体配列
} MyShip;

typedef struct _Enemy{
	int isAlive;   // 敵が生きてるか？
	float x;  // 敵のx座標
	float z;  // 敵のz座標
	float vx; // 敵のx軸方向速度
	float vz; // 敵のz軸方向速度
} Enemy;

// MyShip myShip;
// Enemy enemy[MAX_ENEMY];


void display(void);

void keyboard(unsigned char key, int x, int y);

void lightInit(void);

void reshape(int w, int h);

void specialKey(int key, int x, int y);

void specialKeyUp(int key, int x, int y);

void timer(int t);

#endif