#ifndef GAME_H
# define GAME_H

# include <GL/glut.h>
# include <bits/stdc++.h>

# define GL_SILENCE_DEPRECATION
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PI 3.1415926535
#define FIELD_SIZE 50.0f
#define SHIP_NUM 2
#define DEFAULT_LIFE 1

#define MAX_SHOT  3 // 同時に撃てる弾の最大数

typedef struct _MyShot{
	int isAlive; // 自機の弾が生きてるか？
	float x;  // 弾のx座標
	float z;  // 弾のz座標
	float vx; // 弾のx軸方向速度
	float vz; // 弾のz軸方向速度
} MyShot;

// 自機の情報を格納する構造体
typedef struct _MyShip{
	int Life;   // 自機が生きてるか？
	float x;  // 自機のx座標
	float z;  // 自機のz座標
	float theta;  // 自機の向き
	MyShot myShot[MAX_SHOT];// 弾を管理する構造体配列
} MyShip;


void display(void);

void keyboard(unsigned char key, int x, int y);

void keyboardUp(unsigned char key, int x, int y);

void lightInit(void);

void reshape(int w, int h);

void specialKey(int key, int x, int y);

void specialKeyUp(int key, int x, int y);

void timer(int t);

void setGluLookAt(int shipid =1);

#endif