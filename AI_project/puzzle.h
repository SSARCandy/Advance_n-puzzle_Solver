#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <math.h>
using namespace std;

class puzzle{
public:
	puzzle(string start, string goal)
	{
		initPuzzle(start);
		setGoalState(goal);
	}

	void initPuzzle(string ss){
		string s = ss;
		int w = 0, h = 0;

		/*--------處理start State data & 紀錄寬高-----------*/
		while (s.find(';') != string::npos){
			h++;
			s.replace(s.find(';'), 1, ",");
		}
		while (s.find(',') != string::npos){
			w++;
			s.replace(s.find(','), 1, " ");
		}

		height = h;
		width = (w / h);

		stringstream streamSS(s);
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				streamSS >> pArray[i][j];
				startState[i][j] = pArray[i][j];
			}
		}
	}

	void setGoalState(string goal)
	{
		string s = goal;
		int w = 0, h = 0;

		/*--------處理goal State data-------------*/
		while (s.find(';') != string::npos){
			h++;
			s.replace(s.find(';'), 1, ",");
		}while (s.find(',') != string::npos){
			w++;
			s.replace(s.find(','), 1, " ");
		}

		if (h != height || (w / h) != width)
		{
			cout << "illegal goal State" << endl;
			return;
		}

		stringstream stream(s);
		//streambuf* cin_backup = std::cin.rdbuf(stream.rdbuf());
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				stream >> goalState[i][j];
			}
		}
	}

	void goalTest()
	{

	}
	//Manhatten distance heuristic
	int ManhattenDistance(){
		int score = 0;
		for (int CurState_h = 0; CurState_h < height; CurState_h++){
			for (int CurState_w = 0; CurState_w < width; CurState_w++){
				// not block or space
				if (pArray[CurState_h][CurState_w] != 0 || pArray[CurState_h][CurState_w] != -1){

					//find the min distance to goal state
					int GoalState_h = 0, GoalState_w = 0;
					while (true){
						if (pArray[CurState_h][CurState_w] == goalState[GoalState_h][GoalState_w]){
							score += abs(CurState_h - GoalState_h) + abs(CurState_w - GoalState_w);
							break;
						}
						else{
							if (GoalState_w < width){
								GoalState_w++;
							}
							else{
								GoalState_h ++ ;
								GoalState_w = 0;
							}
						}
					}
				}
			}
		}
		return score;
	}

	void printCurrentState(){
//		cout << "width: " << width << "  height: " << height << endl;
		cout << "MD: " << ManhattenDistance() << endl;
		cout << "|---------Current State----------|" << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << pArray[i][j] << ' ';
			}
			cout << endl;
		}
	}

	void printGoalState()
	{
		cout << "|---------Goal State----------|" << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << goalState[i][j] << ' ';
			}
			cout << endl;
		}
	}

private:
	int startState[15][15];
	int goalState[15][15];
	int pArray[15][15];
	int width;
	int height;
};