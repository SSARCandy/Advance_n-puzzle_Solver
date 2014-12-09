#include "Tiles.h"
#include <cstdlib>
#include <math.h>
#include <queue>

struct State
{
	Tiles tile;
	int g;
	int h;

	State()
	{
		g = 0;
		h = 0;
	}

	State(const State& a)
	{
		tile.copy(a.tile);
		g = a.g;
		h = a.h;
	}
};

struct compareState
{
	bool operator() (const State& a, const State& b){ return (a.g + a.h) > (b.g + b.h); }
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

		heuristicEvaluation(currentState);/*
										  cout << " \t" << currentState.h << endl;*/
		frontier.push(currentState);
	}

	puzzle(Tiles start, Tiles goal)
	{
		currentState.tile = start;
		goalState.tile = goal;
	}

	//void testing()
	//{
	//	currentState = frontier.top();
	//	frontier.pop();
	//	expandNode(currentState);
	//	State tmp = frontier.top();
	//	

	//	cout << "g(n)" << tmp.g << endl;
	//	cout << "h(n)" << tmp.h << endl;
	//	cout << heuristicEvaluation(tmp) << endl;
	//	tmp.tile.print();
	//}

	void initPuzzle(string ss){ currentState.tile = Tiles(ss); }

	void setGoalState(string goal){ goalState.tile = Tiles(goal); }

	bool goalTest(){ currentState.h == 0; }

	//currentState.tile[x][y]'s min distance to goal state
	int manhattenDistance(State node, int y, int x)
	{
		int index = goalState.tile.find(node.tile.at(y, x));
		int GoalState_w = index % width;
		int GoalState_h = index / width;

		return abs(y - GoalState_h) + abs(x - GoalState_w);
	}

	//Heuristic evaluation using Manhatten distance 
	int heuristicEvaluation(State& state)
	{
		int score = 0;
		for (int CurState_h = 0; CurState_h < height; CurState_h++){
			for (int CurState_w = 0; CurState_w < width; CurState_w++){
				// not block or space
				if (state.tile.at(CurState_h, CurState_w) != 0 && state.tile.at(CurState_h, CurState_w) != -1)
					score += manhattenDistance(state, CurState_h, CurState_w);
			}
		}
		state.h = score;
		return score;
	}

	void expandNode(State& parent)
	{
		for (int counter = 0; counter < spaces; counter++)
		{
			int tmp = findIndex(0, counter, parent);
			int x = tmp % width;
			int y = tmp / width;

			for (int dir = 0; dir < 4; dir++)
			{
				//Create parent's children
				State child(parent);
				tileMove(child, y, x, dir);

				//Calculate children's g and h
				heuristicEvaluation(child);
				child.g++;

				cout << "Direction: " << dir << " h(n): " << child.h << " g(n): " << child.g << endl;
				child.tile.print();
				//add child to froniter
				frontier.push(child);
			}
		}
	}

	void A_star()
	{
		while (!goalTest())
		{
			currentState = frontier.top();
			frontier.pop();

			currentState.tile.print();
			expandNode(currentState);
		}
	}
	//void AstarSolverOneStep(){
	//	int choosenChildNodeIndex = -1;
	//	int stacktop = 0;
	//	int choosenAction = 0;
	//	int minMD_score = -1;
	//	for (int space_counter = 0; space_counter < spaces; space_counter++){
	//		int w = findIndexOf(0, curState, space_counter) % width;
	//		int h = findIndexOf(0, curState, space_counter) / width;
	//		//		cout << "0's (w, h) = (" << w << ", " << h << ')' << endl;

	//		// try u, d, l, r and compute score
	//		for (int action = 1; action <= 4; action++){
	//			copyState(childNodeStack[stacktop], curState);
	//			if (swap(childNodeStack[stacktop], w, h, action)){
	//				if (minMD_score == -1){
	//					minMD_score = ManhattenDistance(childNodeStack[stacktop]);
	//					choosenChildNodeIndex = stacktop;
	//					choosenAction = action;
	//				}
	//				else{
	//					if (ManhattenDistance(childNodeStack[stacktop]) == minMD_score){
	//						if (rand() % 2){
	//							minMD_score = ManhattenDistance(childNodeStack[stacktop]);
	//							choosenChildNodeIndex = stacktop;
	//							choosenAction = action;
	//						}

	//						//int choosenMD_one, newMD_one;
	//						//if (choosenAction == 1) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h - 1, w);
	//						//if (choosenAction == 2) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h + 1, w);
	//						//if (choosenAction == 3) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h, w - 1);
	//						//if (choosenAction == 4) choosenMD_one = MD_one_instence(childNodeStack[stacktop], h, w + 1);
	//						//if (action == 1) newMD_one = MD_one_instence(childNodeStack[stacktop], h - 1, w);
	//						//if (action == 2) newMD_one = MD_one_instence(childNodeStack[stacktop], h + 1, w);
	//						//if (action == 3) newMD_one = MD_one_instence(childNodeStack[stacktop], h, w - 1);
	//						//if (action == 4) newMD_one = MD_one_instence(childNodeStack[stacktop], h, w + 1);
	//						//if (newMD_one < choosenMD_one){
	//						//	minMD_score = ManhattenDistance(childNodeStack[stacktop]);
	//						//	choosenChildNodeIndex = stacktop;
	//						//	choosenAction = action;
	//						//}
	//					}
	//					if (ManhattenDistance(childNodeStack[stacktop]) < minMD_score){
	//						minMD_score = ManhattenDistance(childNodeStack[stacktop]);
	//						choosenChildNodeIndex = stacktop;
	//						choosenAction = action;
	//					}

	//				}
	//				stacktop++;
	//			}
	//		}
	//	}
	//	//no any good child node, all of them makes distance higher
	//	if (choosenChildNodeIndex == 0 && ManhattenDistance(childNodeStack[choosenChildNodeIndex]) > ManhattenDistance(curState)){
	//		choosenChildNodeIndex = rand() % stacktop;
	//	}
	//	//cout << "**********" << " cC: " << choosenChildNodeIndex<<" cA: "<<choosenAction << endl;
	//	copyState(curState, childNodeStack[choosenChildNodeIndex]);
	//	printCurrentState();
	//}

	//return one int = i + j*width
	//int findIndexOf(int target, int arr[][MAX_HEIGHT_AND_WIDTH], int position){
	//	int p = 0;
	//	for (int i = 0; i < height; i++){
	//		for (int j = 0; j < width; j++){
	//			if (target == arr[i][j]){
	//				if (p == position)
	//					return i*width + j;
	//				p++;
	//			}
	//		}
	//	}
	//}

	//search the specific value, target, at the specific order.
	//If target doesn't exist, return -1.
	int findIndex(int target, int order, State state)
	{
		int counter = 0;
		for (int i = 0; i < width*height; i++)
		{
			if (state.tile.at(i) == target)
			{
				if (counter == order)
					return i;
				counter++;
			}
		}
		return -1;
	}

	//return true if the action success.
	//action: move empty space 1-up, 2-down, 3-left, 4-right 
	bool tileMove(State& state, int y, int x, int direction)
	{
		if (state.tile.at(y, x) != 0)
			return false;

		switch (direction)
		{
			case 0://UP
				if (y > 0)
				{
					if (state.tile.at(y - 1, x) > 0)
					{
						int tmp = state.tile.at(y - 1, x);
						state.tile.at(y - 1, x) = 0;
						state.tile.at(y, x) = tmp;
						return true;
					}
				}
				return false;

				break;

			case 1://DOWN

				if (y < height - 1)
				{
					if (state.tile.at(y + 1, x) > 0)
					{
						int tmp = state.tile.at(y + 1, x);
						state.tile.at(y + 1, x) = state.tile.at(y, x);
						state.tile.at(y, x) = tmp;
						return true;
					}
				}
				return false;

				break;

			case 2://LEFT
				if (x > 0){
					if (state.tile.at(y, x - 1) > 0)
					{
						int tmp = state.tile.at(y, x - 1);
						state.tile.at(y, x - 1) = state.tile.at(y, x);
						state.tile.at(y, x) = tmp;
						return true;
					}
				}
				return false;

				break;

			case 3://RIGHT
				if (x < width - 1)
				{
					if (state.tile.at(y, x + 1) > 0)
					{
						int tmp = state.tile.at(y, x + 1);
						state.tile.at(y, x + 1) = state.tile.at(y, x);
						state.tile.at(y, x) = tmp;
						return true;
					}
				}
				return false;

				break;

			default:
				break;
		}
	}

	void printCurrentState(){
		//cout << "width: " << width << "  height: " << height << endl;
		cout << "|---------Current State----------|" << " h(n): " << heuristicEvaluation(currentState) << endl;
		currentState.tile.print();
	}

	void printGoalState()
	{
		cout << "|---------Goal State----------|" << endl;
		goalState.tile.print();
	}

private:
	State currentState, goalState;
	priority_queue<State, vector<State>, compareState> frontier;
	int width;
	int height;
	int spaces;
};