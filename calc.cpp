#include <iostream>
#include <string>
#include <vector>
#include "qbby.h"

using namespace std;

bool checkCrown(position curPos, vector<string> &map, unsigned int &crownNum){
	if(map[curPos.y][curPos.x]=='x'){
		map[curPos.y][curPos.x]='w';
		if(crownNum)
			crownNum--;
		else
			return true;
	}
	return false;
}

inline bool checkTrap(position curPos, vector<string> map){
	return (map[curPos.y][curPos.x]=='t');
}

inline bool checkDoor(position curPos, vector<string> map){
	return map[curPos.y][curPos.x]=='e';
}

bool checkNeedGravity(position curPos, vector<position> sugars, vector<string> map){
	return (curPos.y+1<map.size() && map[curPos.y+1][curPos.x]!='b');
}

int checkScore(position curPos, vector<string> &map, unsigned int &crownNum){
	if(checkCrown(curPos,map,crownNum)){
		cout<<"ERR\n";
		return 3;
	}
	if(checkTrap(curPos,map)){
		cout<<"You lost!\n";
		return 2;
	}
	if(checkDoor(curPos,map)){
		cout<<"You reach the door; ";
		if(crownNum)
			cout<<"but you lost "<<crownNum<<" point(s)."<<endl;
		else
			cout<<"and you got all the points!"<<endl;
		return 1;
	}
	return 0;
}

bool canMoveObj(vector<string> map, int curPos_x, int curPos_y, int dx=0, int dy=0){
	if(curPos_x+dx<0 || curPos_y+dy<0 || curPos_y+dy>=map.size()-1 || curPos_x+dx>=map[0].size()-1)
		return false;
	return (map[curPos_y+dy][curPos_x+dx]!='b' && map[curPos_y+dy][curPos_x+dx]!='c');
}

bool canMove(vector<string> map, position curPos, vector<position> sugars, int dx, int dy, bool isWithQbby){
	if(isWithQbby && !canMoveObj(map,curPos.x,curPos.y,dx,dy))
		return false;
	for(int i=0;i<sugars.size();i++)
		if(!canMoveObj(map,curPos.x+sugars[i].x,curPos.y+sugars[i].y,dx,dy))
			return false;
	return true;
}

bool putSugars(vector<string> &map, position curPos, vector<position> &sugars){
	for(int i=0;i<sugars.size();i++)
		map[curPos.y+sugars[i].y][curPos.x+sugars[i].x]='c';
	sugars.clear();
}

