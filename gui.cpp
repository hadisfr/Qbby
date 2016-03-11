#include <istream>
#include <string>
#include <vector>
#include "rsdl.h"
#include "qbby.h"
#include "qbbyFunc.h"

#define XSTR(foo) STR(foo)
#define STR(foo) #foo
#define CUR_ADD ./

using namespace std;

bool drawObj(window &w, char objCode, float pos_x, float pos_y, int animFrame){
	pos_x++;
	pos_y++;
	if(pos_x<0 || pos_y<0 || pos_x>=WORLD_X/OBJ_L || pos_y>=WORLD_Y/OBJ_L)
		return true;

	if(objCode=='w' || objCode=='s' || objCode=='b'){
		w.fill_rect((int)(pos_x*OBJ_L),(int)(pos_y*OBJ_L),OBJ_L,OBJ_L,objCode=='b'?GRAY_SCALE(64):GRAY_SCALE(256-8));
		w.draw_rect((int)(pos_x*OBJ_L),(int)(pos_y*OBJ_L),OBJ_L,OBJ_L,objCode=='b'?GRAY_SCALE(0):GRAY_SCALE(255));
		return false;
	}

	string imgFolderName;
	switch(objCode){
		case 't':
		imgFolderName="trap";
		break;
		case 'e':
		imgFolderName="door";
		break;
		case 'x':
		imgFolderName="crown";
		break;
		case 'l':
		imgFolderName="left";
		break;
		case 'r':
		imgFolderName="right";
		break;
		case 'j':
		imgFolderName="jump";
		break;
		case 'm':
		imgFolderName="make_box";
		break;
		case 'c':
		imgFolderName="sugar";
		break;
		/*case 'w':
		case 'b':
		break;*/
		default:
		return true;
	}
	w.draw_rect((int)(pos_x*OBJ_L),(int)(pos_y*OBJ_L),OBJ_L,OBJ_L,GRAY_SCALE(255));
	w.draw_bmp(string(XSTR(CUR_ADD))+"images/"+imgFolderName+"/bmp_"+XSTR(OBJ_L)+"/"+(char)(animFrame/10+'0')+(char)(animFrame%10+'0')+".bmp"
		,(int)(pos_x*OBJ_L),(int)(pos_y*OBJ_L),OBJ_L,OBJ_L);
	/*w.draw_transparent_bmp(string(XSTR(CUR_ADD))+"images/"+imgFolderName+"/bmp_"+XSTR(OBJ_L)+"/"+(char)(animFrame/10+'0')+(char)(animFrame%10+'0')+".bmp"
		,(int)(pos_x*OBJ_L),(int)(pos_y*OBJ_L),RED);*/

	return false;
}

bool drawMap(vector<string> map, position worldSize, window &w){
	if(worldSize.x<0 || worldSize.y<0 || worldSize.x>WORLD_X/OBJ_L-2 || worldSize.y>WORLD_Y/OBJ_L-2){
		cout<<"Map is too large.\n";
		return true;
	}
	for(int i=0;i<worldSize.y;i++)
		for(int j=0;j<worldSize.x;j++){
			position pos;
			pos.x=j;
			pos.y=i;
			if(drawObj(w,map[i][j],pos.x, pos.y)){
				cout<<"Invalid position.\n";
				return true;
			}
		}
	return false;
}

bool move(window &win, vector<string> map, position &curPos, vector<position> sugars, char objCode, int &animFrame, int dx, int dy, bool isWithQbby){
	if((dx!=1 && dx!=-1 && dx!=0) || (dy!=1 && dy!=0 && dy!=-1) || (dx && dy))
		return true;
	if(canMove(map, curPos, sugars, dx, dy, isWithQbby)){
		for(int i=0;i<=FPS;i++){
			if(isWithQbby){
				drawObj(win,map[curPos.y][curPos.x],curPos.x,curPos.y);
				drawObj(win,map[curPos.y+dy][curPos.x+dx],curPos.x+dx,curPos.y+dy);
			}
			for(int j=0;j<sugars.size();j++){
				drawObj(win,map[curPos.y+sugars[j].y][curPos.x+sugars[j].x],curPos.x+sugars[j].x,curPos.y+sugars[j].y);
				drawObj(win,map[curPos.y+sugars[j].y+dy][curPos.x+sugars[j].x+dx],curPos.x+sugars[j].x+dx,curPos.y+sugars[j].y+dy);
			}
			if(isWithQbby){
				if(objCode=='l' || objCode=='r')
					drawObj(win,objCode,curPos.x + i*(float)(dx)/FPS,curPos.y + i*(float)(dy)/FPS, animFrame=(animFrame+1)%FRAMES);
				else
					drawObj(win,objCode,curPos.x + i*(float)(dx)/FPS,curPos.y + i*(float)(dy)/FPS);
			}
			for(int j=0;j<sugars.size();j++){
				drawObj(win,'c',curPos.x+sugars[j].x+i*(float)(dx)/FPS,curPos.y+sugars[j].y+i*(float)(dy)/FPS);
			}
			SDL_Delay(DELAY_TIME/((objCode=='l' || objCode=='r')?1:2));
			win.update_screen();
		}
		curPos.x+=dx;
		curPos.y+=dy;
		return false;
	}
	return true;
}

void clearSugars(window &win, vector<string> map, position curPos, vector<position> &sugars){
	for(int i=0;i<sugars.size();i++)
		drawObj(win,map[curPos.y+sugars[i].y][curPos.x+sugars[i].x],curPos.x+sugars[i].x,curPos.y+sugars[i].y);
	sugars.clear();
}

bool addSugar(window &win, vector<string> map, position curPos, vector<position> &sugars, int cubeLimit, int dx, int dy){
	if(sugars.size()){
		curPos.x+=sugars[sugars.size()-1].x;
		curPos.y+=sugars[sugars.size()-1].y;
		position tempPos;
		if(sugars.size()>1)
			tempPos=sugars[sugars.size()-2];
		else
			tempPos.x=tempPos.y=0;
		if((-dx==(sugars[sugars.size()-1].x - tempPos.x)) && (-dy==(sugars[sugars.size()-1].y - tempPos.y))){
			drawObj(win, map[curPos.y][curPos.x], curPos.x, curPos.y);
			sugars.pop_back();
			return false;
		}
	}
	if(sugars.size()>=cubeLimit || map[curPos.y+dy][curPos.x+dx]=='b' || map[curPos.y+dy][curPos.x+dx]=='c')
		return true;
	position tempPos;
	tempPos.x=!sugars.size()?dx:sugars[sugars.size()-1].x+dx;
	tempPos.y=!sugars.size()?dy:sugars[sugars.size()-1].y+dy;
	drawObj(win,'c',curPos.x+dx,curPos.y+dy);
	sugars.push_back(tempPos);
	return false;
}

bool teleport(window &win, vector<string> map, position &curPos, vector<position> &sugars){
	if(!sugars.size())
		return true;
	position newPos=curPos;
	newPos.x+=sugars[sugars.size()-1].x;
	newPos.y+=sugars[sugars.size()-1].y;
	clearSugars(win,map,curPos,sugars);
	drawObj(win,map[curPos.y][curPos.x],curPos.x,curPos.y);
	curPos=newPos;
}

bool jump(window &win, vector<string> &map, position &curPos, vector<position> sugars, int &animFrame, unsigned int &crownNum, int dir){
	bool flag=!move(win,map,curPos,sugars,'j',animFrame,0,-1);
	if(checkScore(curPos,map,crownNum))
		return true;
	move(win,map,curPos,sugars,'j',animFrame,dir);
	if(checkScore(curPos,map,crownNum))
		return true;
	move(win,map,curPos,sugars,'j',animFrame,dir);
	/*if(checkScore(curPos,map,crownNum))
		return true;
	if(flag)
		move(win,map,curPos,sugars,'j',animFrame,0,+1);*/
	return false;
}

bool throwSugar(window &win, vector<string> &map, position curPos, vector<position> &sugars, int dir){
	int animFrame=0;
	position tempPos=curPos;
	bool flag=!move(win,map,tempPos,sugars,'j',animFrame,0,-1,false);
	move(win,map,tempPos,sugars,'j',animFrame,dir,0,false);
	move(win,map,tempPos,sugars,'j',animFrame,dir,0,false);
	if(flag)
			move(win,map,tempPos,sugars,'j',animFrame,0,+1,false);
	while(!move(win,map,tempPos,sugars,'j',animFrame,0,+1,false));
	putSugars(map,tempPos,sugars);
	return false;
}

void loosGame(){
	window loosWin(288,288,"QBBY :: Loos");
	loosWin.draw_bmp(string(XSTR(CUR_ADD))+"images/loos.bmp",0,0,288,288);
	loosWin.update_screen();
	SDL_Delay(3000);
	loosWin.close_window();
}

void winGame(){
	window loosWin(288,288,"QBBY :: win");
	loosWin.draw_bmp(string(XSTR(CUR_ADD))+"images/win.bmp",0,0,288,288);
	loosWin.update_screen();
	SDL_Delay(3000);
	loosWin.close_window();
}
