#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

enum bonus {
        NOBONUS='n',
        LASER='l',
        BIGGER='b',
        CATCH='c',
        SLOW='s',
        TRIPLE='t',
        HEAL='h'
    };

enum color {
        GOLD,
        NOCOLOR=0,
        WHITE=50,
        ORANGE=60,
        CYAN=70,
        GREEN=80,
        RED=90,
        BLUE=100,
        MAGENTA=110,
        YELLOW=120,
        SILVER=200
    };

class Cell
{
private:

    int posx_;
    int posy_;
    bool broken_=false;
    int length=10;
    int wide=21;
    std::vector<int> color_map = {255,255,255};
    enum color color=NOCOLOR;
    unsigned short will_break = 1;
    enum bonus bonus = NOBONUS;

public:

    Cell(int, int);
    virtual ~Cell();
    virtual void cellBreak();
    virtual bool isBroken();
    virtual int getPosX();
    virtual int getPosY();
    virtual int getLen();
    virtual int getWide();
    virtual void reset();
    virtual void setColor(char);
    virtual std::vector<int> getColor();
    virtual int getPoints();
    virtual unsigned short getState();
    virtual char getBonus();
    virtual void setBonus(char);
};

class Blocs
{
private:
    unsigned short sizes[2] = {0,0};
    Cell* cellMatrice[25][25];
    unsigned max_score = 0;
    void unloadLastLevel(unsigned short,short);

public:

    Blocs();
    virtual ~Blocs();
    virtual Cell* getCell(int,int);
    virtual void resetCells();
    virtual unsigned short* getMatriceSizes();
    virtual unsigned getMaxScore();
    unsigned short getLevelSize(unsigned short,unsigned short);
    virtual void loadLevel(unsigned short,short);
};

class Racket
{
private:

    float posx_=310;
    float posy_=650;
    float lenSide_[2]={50,10};
    int baseLen_=50;

public:

    Racket();
    virtual ~Racket();
    virtual float getPosX();
    virtual float getPosY();
    virtual float* getlenSide();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void moveTo(int);
    virtual void setLenSide(int);
    virtual int getBaseLen();

};

class Ball
{
private:

    float posx_=310;
    float posy_=600;
    float radius_=7;
    float speed_=-6;
    float base_speed_=-6;
    float decompSpeed_[2] = {0,0}; 

public:

    Ball();
    virtual ~Ball();
    virtual float getPosY();
    virtual float getPosX();
    virtual void setPosX(float);
    virtual void setPosY(float);
    virtual float getRadius();
    virtual void move();
    virtual float* getDecompSpeed();
    virtual void setDecompSpeed(float,int);
    virtual float getSpeed();
    virtual void ballReset();
    virtual void setSpeed(float);
    virtual float getBaseSpeed();

};

class Bonus : public Ball
{
private:
    enum bonus bonus;
    std::vector<int> color = {255,255,255};
public:
    Bonus(int,int);
    virtual char getBonus();
    virtual void setBonus(char);
    virtual std::vector<int> getColor();
    virtual void setColor(int,int,int);
};

#endif