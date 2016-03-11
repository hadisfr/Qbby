#include <iostream>
#include <string>
#include <vector>
#include "qbby.h"

using namespace std;

bool inputMap(vector<string> &map, position worldSize){
	if(map.size())
		map.clear();
	string tempStr;
	while(cin>>tempStr)
		map.push_back(tempStr);
}

bool isValidMap(unsigned int crownNum, position worldSize, position stratPos, position endPos, vector<string> map){
	if(map.size()!=worldSize.y)
		return  false;
	for(int i=0;i<worldSize.y;i++){
		if(map[i].size()!=worldSize.x)
			return false;
		for(int j=0;j<worldSize.x;j++)
			switch(map[i][j]){
				case 'x':
				crownNum--;
				break;
				case 's':
				if(i!=stratPos.y || j!=stratPos.x)
					return false;
				break;
				case 'e':
				if(i!=endPos.y || j!=endPos.x)
					return false;
				break;
				case 'b':
				case 'w':
				case 't':
				break;
				default:
				return false;
			}
	}
	if(crownNum)
		return false;
	if(map[stratPos.y][stratPos.x]!='s' || map[endPos.y][endPos.x]!='e')
		return false;
	return true;
}

bool inputData(string &mapName, unsigned int &cubeLim, unsigned int &crownNum, position &worldSize, position &stratPos, position &endPos, vector <string> &map){
	getline(cin,mapName);
	if(cin.fail())
		return true;
	cin>>cubeLim>>crownNum>>stratPos.x>>stratPos.y>>endPos.x>>endPos.y>>worldSize.x>>worldSize.y;
	if(cin.fail())
		return true;
	if(inputMap(map, worldSize))
		return true;
	if(!isValidMap(crownNum,worldSize,stratPos,endPos,map)){
		cout<<"Map validation failed.\n";
		return false;
	}
	cout<<"Input successfully!("<<mapName<<' '<<worldSize.x<<'x'<<worldSize.y<<")\n";
	return false;
}