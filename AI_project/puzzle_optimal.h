#include "Tiles.h"
#include <cstdlib>
#include <math.h>
#include <queue>

struct State
{
	Tiles tile;
	int g = 0;
	int h = 0;
};

class compareState
{
public:
	bool operator() (State& a, State& b)
	{
		return (a.g + a.h) < (b.g + b.h);
	}
};

class puzzle{
public:
	puzzle(string start, string goal)
	{
		initPuzzle(start);
		setGoalState(goal);

		if (currentState.tile.getWidth() != goalState.tile.getWidth() ||
			currentState.tile.getHeight() != goalState.tile.getHeight())
			exit(EXIT_FAILURE);

		width = currentState.tile.getWidth();
		height = currentState.tile.getHeight();
		spaces = currentState.tile.getNumOfEmptySpaces();
	}

	puzzle(Tiles start, Tiles goal)
	{
		currentState.tile = start;
		goalState.tile = goal;
	}

	void initPuzzle(string ss){ currentState.tile = Tiles(ss); }

	void setGoalState(string goal){ goalState.tile = Tiles(goal); }

	bool goalTest(){ heuristicEvaluation(currentState) == 0; }

	//int MD_one_instence(int nodeArray[][MAX_HEIGHT_AND_WIDTH], int h, int w){
	//	//find the 
	//	int GoalState_h = 0, GoalState_w = 0;
	//	while (true){
	//		if (nodeArray[h][w] == goalState[GoalState_h][GoalState_w]){
	//			return abs(h - GoalState_h) + abs(w - GoalState_w);
	//		}
	//		else{
	//			if (GoalState_w < width){
	//				GoalState_w++;
	//			}
	//			else{
	//				GoalState_h++;
	//				GoalState_w = 0;
	//			}
	//		}
	//	}
	//}

	//currentState.tile[x][y]'s min distance to goal state
	int manhattenDistance(State node, int x, int y)
	{
		int index = goalState.tile.find(node.tile.at(x, y));
		int GoalState_w = index % width;
		int GoalState_h = index / width;

		return abs(x - GoalState_h) + abs(y - GoalState_w);
	}

	
	//int ManhattenDistance(int nodeArray[][MAX_HEIGHT_AND_WIDTH]){
	//	int score = 0;
	//	for (int CurState_h = 0; CurState_h < height; CurState_h++){
	//		for (int CurState_w = 0; CurState_w < width; CurState_w++){
	//			// not block or space
	//			if (nodeArray[CurState_h][CurState_w] != 0 && nodeArray[CurState_h][CurState_w] != -1){
	//				score += MD_one_instence(nodeArray, CurState_h, CurState_w);
	//			}
	//		}
	//	}
	//	return score;
	//}

	//Heuristic evaluation using Manhatten distance 
	int heuristicEvaluation(State state)
	{
		int score = 0;
		for (int CurState_h = 0; CurState_h < height; CurState_h++){
			for (int CurState_w = 0; CurState_w < width; CurState_w++){
				// not block or space
				if (state.tile.at(CurState_h,CurState_w) != 0 && state.tile.at(CurState_h,CurState_w) != -1)
					score += manhattenDistance(state, CurState_h, CurState_w);
			}
		}
		state.h = score;
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
			choosenChildNodeIndex = rand() % stacktop;
		}
		//cout << "**********" << " cC: " << choosenChildNodeIndex<<" cA: "<<choosenAction << endl;
		copyState(curState, childNodeStack[choosenChildNodeIndex]);
		printCurrentState();
	}

	//return one int = i + j*width
	int findIndexOf(int target, int arr[][MAX_HEIGHT_AND_WIDTH], int position){
		int p = 0;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (target == arr[i][j]){
					if (p == position)
						return i*width + j;
					p++;
				}
			}
		}
	}

	void copyState(int dest[][MAX_HEIGHT_AND_WIDTH], int source[][MAX_HEIGHT_AND_WIDTH]){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				dest[i][j] = source[i][j];
			}
		}
	}

	// return true if the swap action is success.
	//action: 1-up, 2-down, 3-left, 4-right
	bool swap(int arr[][MAX_HEIGHT_AND_WIDTH], int w, int h, int action){
		switch (action)
		{
			case 1://UP
				if (h - 1 >= 0){
					if (arr[h - 1][w] != -1 && arr[h - 1][w] != 0){
						int tmp = arr[h - 1][w];
						arr[h - 1][w] = arr[h][w];
						arr[h][w] = tmp;
						return true;
					}
				}
				else{
					return false;
				}
				break;
			case 2://DOWN
				if (h + 1 < height){
					if (arr[h + 1][w] != -1 && arr[h + 1][w] != 0){
						int tmp = arr[h + 1][w];
						arr[h + 1][w] = arr[h][w];
						arr[h][w] = tmp;
						return true;
					}
				}
				else{
					return false;
				}
				break;
			case 3://LEFT
				if (w - 1 >= 0){
					if (arr[h][w - 1] != -1 && arr[h][w - 1] != 0){
						int tmp = arr[h][w - 1];
						arr[h][w - 1] = arr[h][w];
						arr[h][w] = tmp;
						return true;
					}
				}
				else{
					return false;
				}
				break;
			case 4://RIGHT
				if (w + 1 >= 0){
					if (arr[h][w + 1] != -1 && arr[h][w + 1] != 0){
						int tmp = arr[h][w + 1];
						arr[h][w + 1] = arr[h][w];
						arr[h][w] = tmp;
						return true;
					}
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
		//cout << "width: " << width << "  height: " << height << endl;
		cout << "|---------Current State----------|" << " MD: " << ManhattenDistance(currentState) << endl;
		currentState.tile.print();
	}

	void printGoalState()
	{
		cout << "|---------Goal State----------|" << endl;
		goalState.tile.print();
	}

private:
	State currentState, goalState;
	priority_queue<State, vector<State>, compareState> childNodes;
	int width;
	int height;
	int spaces;
};