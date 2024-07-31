#include "game.h"

#define MAP_WIDTH  10
#define MAP_HEIGHT 10

static const int map[MAP_HEIGHT][MAP_WIDTH]={
    {1,1,1,1,1,1,1,1,1,1},
    {1,2,1,0,1,0,1,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

static const double HEX_SIZE=1.0;
// static const double HEX_SIZE=1.0/MAP_WIDTH;


void draw_map(void)
{
    //図形の色 (白)
    static const GLfloat white[] = { 0.9, 0.9, 0.9, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    
    for(int r=0;r<MAP_HEIGHT;++r){
        for(int c=0;c<MAP_WIDTH;++c){
            glPushMatrix();{
                //床
                glTranslatef((c+0.5)*HEX_SIZE,-HEX_SIZE,(r+0.5)*HEX_SIZE);
                glutSolidCube(HEX_SIZE);
                if(map[r][c]==1){
                    //壁
                    glPushMatrix();{
                        glTranslatef(0,HEX_SIZE,0); //上に移動
                        glutSolidCube(HEX_SIZE);
                    }glPopMatrix();
                }
            }glPopMatrix();
        }}
}