#ifndef _QBBY_FUNC_H
#define _QBBY_FUNC_H

#include <iostream>
#include <vector>
#include "rsdl.h"

bool inputData(string &mapName, unsigned int &cubeLim, unsigned int &crownNum, position &worldSize, position &stratPos, position &endPos, vector<string> &map);
bool drawObj(window &w, char objCode, float pos_x, float pos_y, int animFrame=0);
bool drawMap(vector<string> map, position worldSize, window &w);
bool move(window &win, vector<string> map, position &curPos, vector<position> sugars, char objCode, int &animFrame, int dx=0, int dy=0, bool isWithQbby=true);
int checkScore(position curPos, vector<string> &map, unsigned int &crownNum);
void clearSugars(window &win, vector<string> map, position curPos, vector<position> &sugars);
bool canMove(vector<string> map, position curPos, vector<position> sugars, int dx=0, int dy=0, bool isWithQbby=true);
bool addSugar(window &win, vector<string> map, position curPos, vector<position> &sugars, int cubeLimit, int dx=0, int dy=0);
bool teleport(window &win, vector<string> map, position &curPos, vector<position> &sugars);
bool putSugars(vector<string> &map, position curPos, vector<position> &sugars);
bool throwSugar(window &win, vector<string> &map, position curPos, vector<position> &sugars, int dir);
bool jump(window &win, vector<string> &map, position &curPos, vector<position> sugars, int &animFrame, unsigned int &crownNum, int dir);
void loosGame();
void winGame();

#endif