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
		spaces = 0;
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
				streamSS >> curState[i][j];
				startState[i][j] = curState[i][j];
			}
		}
		//count spaces
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				if (startState[i][j] == 0)
					spaces++;
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
		int choosenChildNodeIndex = -1;
		int stacktop = 0;
		int choosenAction = 0;
		int minMD_score = -1;
		for (int space_counter = 0; space_counter < spaces; space_counter++){
			int w = findIndexOf(0, curState, space_counter) % width;
			int h = findIndexOf(0, curState, space_counter) / width;
			//		cout << "0's (w, h) = (" << w << ", " << h << ')' << endl;

			// try u, d, l, r and compute score
			for (int action = 1; action <= 4; action++){
				copyState(childNodeStack[stacktop], curState);
				if (swap(childNodeStack[stacktop], w, h, action)){
					if (minMD_score == -1){
						minMD_score = ManhattenDistance(childNodeStack[stacktop]);
						choosenChildNodeIndex = stacktop;
						choosenAction = action;
					}
					else{
						if (ManhattenDistance(childNodeStack[stacktop]) == minMD_score){
							if (rand() % 2){
								minMD_score = ManhattenDistance(childNodeStack[stacktop]);
								choosenChildNodeIndex = stacktop;
								choosenAction = action;
							}

							//int choosenMD_one, newMD_one;
							//if (choosenAction == 1) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h - 1, w);
							//if (choosenAction == 2) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h + 1, w);
							//if (choosenAction == 3) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h, w - 1);
							//if (choosenAction == 4) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h, w + 1);
							//if (action == 1) newMD_one = MD_one_instence(childNodeStack[stacktop], h - 1, w);
							//if (action == 2) newMD_one = MD_one_instence(childNodeStack[stacktop], h + 1, w);
							//if (action == 3) newMD_one = MD_one_instence(childNodeStack[stacktop], h, w - 1);
							//if (action == 4) newMD_one = MD_one_instence(childNodeStack[stacktop], h, w + 1);
							//if (newMD_one < choosenMD_one){
							//	minMD_score = ManhattenDistance(childNodeStack[stacktop]);
							//	choosenChildNodeIndex = stacktop;
							//	choosenAction = action;
							//}
						}
						if (ManhattenDistance(childNodeStack[stacktop]) < minMD_score){
							minMD_score = ManhattenDistance(childNodeStack[stacktop]);
							choosenChildNodeIndex = stacktop;
							choosenAction = action;
						}

					}
					stacktop++;
				}
			}
		}
		//no any good child node, all of them makes distance higher
		if (choosenChildNodeIndex == 0 && ManhattenDistance(childNodeStack[choosenChildNodeIndex]) > ManhattenDistance(curState)){
			choosenChildNodeIndex = rand()%stacktop;
		}
		//cout << "**********" << " cC: " << choosenChildNodeIndex<<" cA: "<<choosenAction << endl;
		copyState(curState, childNodeStack[choosenChildNodeIndex]);
		printCurrentState();
	}

	//return one int = i + j*width
	int findIndexOf(int target, int arr[][15], int position){
		int p = 0;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (target == arr[i][j] ){
					if (p == position)
						return i*width + j;
					p++;
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
			if (w - 1 >= 0 && arr[h][w-1] != -1 && arr[h][w-1] != 0){
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
			if (w + 1 >= 0 && arr[h][w + 1] != -1 && arr[h][w + 1] != 0){
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
	int spaces;
};