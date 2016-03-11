#include "qbby.h"
#include "qbbyFunc.h"

using namespace std;

int main(){
	string mapName;
	unsigned int cubeLim, crownNum;
	position worldSize, curPos, endPos;
	vector<string> map;
	vector<position> sugars;
	
	while(inputData(mapName, cubeLim, crownNum, worldSize, curPos, endPos, map))
		cout<<"ERR Reading map.\n";
	
	window win(WORLD_X, WORLD_Y, string("QBBY :: "+mapName).c_str());
	win.fill_rect(0,0,WORLD_X,WORLD_Y,GRAY_SCALE(0));
	if(drawMap(map, worldSize, win)){
		cout<<"ERR Drawing map.\n";
		return 1;
	}

	drawObj(win,'j',curPos.x, curPos.y);
	int animFrame=0;

	bool isMakingSugar=false;
	SDL_Event e;
	while(true){
		move(win,map,curPos,sugars,'j',animFrame,0,+1);
		if(int flag=checkScore(curPos,map,crownNum)){
			if(flag==1)
				winGame();
			if(flag==2)
				loosGame();
			break;
		}
		if(!SDL_PollEvent(&e)){
			SDL_Delay(DELAY_TIME);
			//animFrame=0;
			drawObj(win, 'j', curPos.x, curPos.y);
			win.update_screen();
			continue;
		}
		QUIT(break;)
		KEY_PRESS(ESCAPE,break;)
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_w && e.key.keysym.sym != SDLK_a && e.key.keysym.sym != SDLK_s && e.key.keysym.sym != SDLK_d)
			isMakingSugar=false;
		if(isMakingSugar){
			KEY_PRESS(w, addSugar(win,map,curPos,sugars,cubeLim,0,-1);)
			KEY_PRESS(s, addSugar(win,map,curPos,sugars,cubeLim,0,+1);)
			KEY_PRESS(a, addSugar(win,map,curPos,sugars,cubeLim,-1);)
			KEY_PRESS(d, addSugar(win,map,curPos,sugars,cubeLim,+1);)
		}
		if(sugars.size()){
			KEY_PRESS(SPACE, teleport(win,map,curPos,sugars);)
			KEY_PRESS(c, throwSugar(win, map, curPos, sugars, +1);)
			KEY_PRESS(z, throwSugar(win, map, curPos, sugars, -1);)
		}
		KEY_PRESS(LEFT,move(win,map,curPos,sugars,'l',animFrame,-1);)
		KEY_PRESS(RIGHT,move(win,map,curPos,sugars,'r',animFrame,+1);)
		KEY_PRESS(UP,move(win,map,curPos,sugars,'j',animFrame,0,-1);)
		KEY_PRESS(q, if(jump(win, map, curPos, sugars, animFrame, crownNum, -1)) break;)
		KEY_PRESS(e, if(jump(win, map, curPos, sugars, animFrame, crownNum, +1)) break;)
		KEY_PRESS(LCTRL, clearSugars(win,map,curPos,sugars); isMakingSugar=true;)
		win.update_screen();
		if(int flag=checkScore(curPos,map,crownNum)){
			if(flag==1)
				winGame();
			if(flag==2)
				loosGame();
			break;
		}
		/*while(!move(win,map,curPos,sugars,'j',animFrame,0,+1)){
			if(int flag=checkScore(curPos,map,crownNum)){
				if(flag==1)
					winGame();
				if(flag==2)
					loosGame();
				break;
			}
		}*/
		move(win,map,curPos,sugars,'j',animFrame,0,+1);
		if(int flag=checkScore(curPos,map,crownNum)){
			if(flag==1)
				winGame();
			if(flag==2)
				loosGame();
			break;
		}
		SDL_Delay(DELAY_TIME);
		while(SDL_PollEvent(&e));
	}

	return 0;
}