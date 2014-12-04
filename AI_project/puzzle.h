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

		/*--------�B�zstart State data & �����e��-----------*/
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
				streamSS >> curState[i][j];
				startState[i][j] = curState[i][j];
			}
		}
	}

	void setGoalState(string goal)
	{
		string s = goal;
		int w = 0, h = 0;

		/*--------�B�zgoal State data-------------*/
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

	bool goalTest()
	{
		if (ManhattenDistance(curState) == 0)
			return true;
		else
			return false;
	}
	
	int MD_one_instence(int nodeArray[][15], int h, int w){
		//find the min distance to goal state
		int GoalState_h = 0, GoalState_w = 0;
		while (true){
			if (nodeArray[h][w] == goalState[GoalState_h][GoalState_w]){
				return abs(h - GoalState_h) + abs(w - GoalState_w);
			}
			else{
				if (GoalState_w < width){
					GoalState_w++;
				}
				else{
					GoalState_h++;
					GoalState_w = 0;
				}
			}
		}
	}

	//Manhatten distance heuristic
	int ManhattenDistance(int nodeArray[][15]){
		int score = 0;
		for (int CurState_h = 0; CurState_h < height; CurState_h++){
			for (int CurState_w = 0; CurState_w < width; CurState_w++){
				// not block or space
				if (nodeArray[CurState_h][CurState_w] != 0 && nodeArray[CurState_h][CurState_w] != -1){
					score += MD_one_instence(nodeArray, CurState_h, CurState_w);
				}
			}
		}
		return score;
	}

	void AstarSolverOneStep(){
		int w = findIndexOf(0, curState) % width;
		int h = findIndexOf(0, curState) / width;
		int stacktop = 0;
		int choosenChildNodeIndex = -1;
		int minMD_score = -1;
//		cout << "0's (w, h) = (" << w << ", " << h << ')' << endl;

		// try u, d, l, r and compute score
		for (int action = 1; action <= 4; action++){
			copyState(childNodeStack[stacktop], curState);
			if (swap(childNodeStack[stacktop], w, h, action)){
				if (minMD_score == -1){
					minMD_score = ManhattenDistance(childNodeStack[stacktop]);
					choosenChildNodeIndex = stacktop;
				}
				else{
					if (ManhattenDistance(childNodeStack[stacktop]) < minMD_score){
						minMD_score = ManhattenDistance(childNodeStack[stacktop]);
						choosenChildNodeIndex = stacktop;
					}
				}
				stacktop++;
			}
		}
		copyState(curState, childNodeStack[choosenChildNodeIndex]);
		printCurrentState();
	}

	//return one int = i + j*width
	int findIndexOf(int target, int arr[][15]){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (target == arr[i][j]){
					return i*width + j;
				}
			}
		}
	}

	void copyState(int dest[][15], int source[][15]){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				dest[i][j] = source[i][j];
			}
		}
	}

	// return true if the swap action is success.
	bool swap(int arr[][15], int w, int h, int action){
		//action: 1-up, 2-down, 3-left, 4-right
		switch (action)
		{
		case 1://UP
			if (h - 1 >= 0 && arr[h - 1][w] != -1 && arr[h - 1][w] != 0){
				int tmp = arr[h - 1][w];
				arr[h - 1][w] = arr[h][w];
				arr[h][w] = tmp;
				return true;
			}
			else{
				return false;
			}
			break;
		case 2://DOWN
			if (h + 1 < height&& arr[h + 1][w] != -1 && arr[h + 1][w] != 0){
				int tmp = arr[h + 1][w];
				arr[h + 1][w] = arr[h][w];
				arr[h][w] = tmp;
				return true;
			}
			else{
				return false;
			}
			break;
		case 3://LEFT
			if (w - 1 >= 0 && arr[h][w-1] != -1 && arr[h][w-1] != -1){
				int tmp = arr[h][w- 1];
				arr[h][w-1] = arr[h][w];
				arr[h][w] = tmp;
				return true;
			}
			else{
				return false;
			}
			break;
		case 4://RIGHT
			if (w + 1 >= 0 && arr[h][w + 1] != -1 && arr[h][w + 1] != -1){
				int tmp = arr[h][w + 1];
				arr[h][w + 1] = arr[h][w];
				arr[h][w] = tmp;
				return true;
			}
			else{
				return false;
			}
			break;
		default:
			break;
		}
	}

	void printCurrentState(){
//		cout << "width: " << width << "  height: " << height << endl;
		cout << "|---------Current State----------|" << " MD: " << ManhattenDistance(curState) << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << curState[i][j] << ' ';
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
	int curState[15][15];
	int childNodeStack[15][15][15];
	int width;
	int height;
};