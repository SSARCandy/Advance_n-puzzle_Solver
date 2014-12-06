#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <math.h>

#define MAX_HEIGHT_AND_WIDTH 5
#define MAX_STACKS 70
using namespace std;

class puzzle{
public:
	puzzle(){
		// default constructor
	}

	puzzle(int arr[][MAX_HEIGHT_AND_WIDTH], int w, int h){
		width = w;
		height = h;
		MDscore = 999;
		actCount = 0;

		copyState(state, arr);
	}

	puzzle &operator= (puzzle & other){
		MDscore = other.MDscore;
		width = other.width;
		height = other.height;
		actCount = other.actCount;

		copyState(state, other.state);
		for (int i = 0; i < actCount; i++)
			action[i] = other.action[i];

		return *this;
	}

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

	void copyState(int dest[][MAX_HEIGHT_AND_WIDTH], int source[][MAX_HEIGHT_AND_WIDTH]){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				dest[i][j] = source[i][j];
			}
		}
	}

	// return true if the swap action is success.
	bool swap(int w, int h, int act){
		//action: 1-up, 2-down, 3-left, 4-right
		stringstream buffer;
		switch (act)
		{
			case 1://UP
				if (h - 1 >= 0){
					if (state[h - 1][w] != -1 && state[h - 1][w] != 0){
						int tmp = state[h - 1][w];
						state[h - 1][w] = state[h][w];
						state[h][w] = tmp;
						buffer << "(" << w + 1 << ", " << h + 1 << ", U)" << endl;
						action[actCount++] = buffer.str();
						return true;
					}
					return false;
				}
				return false;

				break;
			case 2://DOWN
				if (h + 1 < height){
					if (state[h + 1][w] != -1 && state[h + 1][w] != 0){
						int tmp = state[h + 1][w];
						state[h + 1][w] = state[h][w];
						state[h][w] = tmp;
						buffer << "(" << w + 1 << ", " << h + 1 << ", D)" << endl;
						action[actCount++] = buffer.str();
						return true;
					}
					return false;
				}
				return false;
				break;
			case 3://LEFT
				if (w - 1 >= 0){
					if (state[h][w - 1] != -1 && state[h][w - 1] != 0){
						int tmp = state[h][w - 1];
						state[h][w - 1] = state[h][w];
						state[h][w] = tmp;
						buffer << "(" << w + 1 << ", " << h + 1 << ", L)" << endl;
						action[actCount++] = buffer.str();
						return true;
					}
					return false;
				}
				return false;
				break;
			case 4://RIGHT
				if (w + 1 < width){
					if (state[h][w + 1] != -1 && state[h][w + 1] != 0){
						int tmp = state[h][w + 1];
						state[h][w + 1] = state[h][w];
						state[h][w] = tmp;
						buffer << "(" << w + 1 << ", " << h + 1 << ", R)" << endl;
						action[actCount++] = buffer.str();
						return true;
					}
					return false;
				}
				return false;
				break;
			default:
				break;
		}
	}

	void printCurrentState(){
		cout << "width: " << width << "  height: " << height << endl;
		cout << "|---------Current State----------|" << " MD: " << MDscore << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << state[i][j] << ' ';
			}
			cout << endl;
		}
	}

	int MDscore;
	int state[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
	string action[MAX_STACKS];
	int actCount;
private:
	int width;
	int height;
	//	int spaces;

};

class puzzleSolver{
public:
	puzzleSolver(string start, string goal){
		spaces = 0;
		frontier_length = 0;
		initPuzzle(start);
		setGoalState(goal);
		curState.MDscore = ManhattenDistance(curState);
		goalState.MDscore = ManhattenDistance(goalState);
	}

	void initPuzzle(string ss){
		int tmpArray[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
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
				streamSS >> tmpArray[i][j];
			}
		}
		//count spaces
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				if (tmpArray[i][j] == 0)
					spaces++;

		curState = puzzle(tmpArray, width, height);
	}

	void setGoalState(string goal)
	{
		int tmpArray[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
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
				stream >> tmpArray[i][j];
			}
		}

		goalState = puzzle(tmpArray, width, height);
	}

	bool goalTest(puzzle p)
	{
		if (ManhattenDistance(p) == 0)
			return true;
		else
			return false;
	}

	bool isSame(puzzle a, puzzle b){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (a.state[i][j] != b.state[i][j])
					return false;
			}
		}
		return true;
	}

	int MD_one_instence(puzzle p, int h, int w){
		//find the min distance to goal state
		int GoalState_h = 0, GoalState_w = 0;
		while (true){
			if (p.state[h][w] == goalState.state[GoalState_h][GoalState_w]){
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
	int ManhattenDistance(puzzle p){
		int score = 0;
		for (int CurState_h = 0; CurState_h < height; CurState_h++){
			for (int CurState_w = 0; CurState_w < width; CurState_w++){
				// not block or space
				if (p.state[CurState_h][CurState_w] != 0 && p.state[CurState_h][CurState_w] != -1){
					score += MD_one_instence(p, CurState_h, CurState_w);
				}
			}
		}
		return score;
	}

	//return one int = i + j*width
	int findIndexOf(int target, puzzle p, int position){
		int pos = 0;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (target == p.state[i][j]){
					if (pos == position)
						return i*width + j;
					pos++;
				}
			}
		}
	}

	void copyState(puzzle dest, puzzle source){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				dest.state[i][j] = source.state[i][j];
			}
		}
	}

	// return true if the swap action is success.
	bool swap(puzzle p, int w, int h, int action){
		//action: 1-up, 2-down, 3-left, 4-right
		switch (action)
		{
			case 1://UP
				if (h - 1 >= 0){
					if (p.state[h - 1][w] != -1 && p.state[h - 1][w] != 0){
						int tmp = p.state[h - 1][w];
						p.state[h - 1][w] = p.state[h][w];
						p.state[h][w] = tmp;
						//				cout << "---(" << w + 1 << ", " << h + 1 << ", U)" << endl;
						return true;
					}
					return false;
				}
				return false;
				break;
			case 2://DOWN
				if (h + 1 < height){
					if (p.state[h + 1][w] != -1 && p.state[h + 1][w] != 0){
						int tmp = p.state[h + 1][w];
						p.state[h + 1][w] = p.state[h][w];
						p.state[h][w] = tmp;
						//				cout << "---(" << w + 1 << ", " << h + 1 << ", D)" << endl;
						return true;
					}
					return false;
				}
				return false;
				break;
			case 3://LEFT
				if (w - 1 >= 0){
					if (p.state[h][w - 1] != -1 && p.state[h][w - 1] != 0){
						int tmp = p.state[h][w - 1];
						p.state[h][w - 1] = p.state[h][w];
						p.state[h][w] = tmp;
						//				cout << "---(" << w + 1 << ", " << h + 1 << ", L)" << endl;
						return true;
					}
					return false;
				}
				return false;
				break;
			case 4://RIGHT
				if (w + 1 < width){
					if (p.state[h][w + 1] != -1 && p.state[h][w + 1] != 0){
						int tmp = p.state[h][w + 1];
						p.state[h][w + 1] = p.state[h][w];
						p.state[h][w] = tmp;
						//				cout << "---(" << w + 1 << ", " << h + 1 << ", R)" << endl;
						return true;
					}
					return false;
				}
				return false;
				break;
			default:
				break;
		}
	}


	void expandNode(puzzle frontier[], int &frontier_length, puzzle explored[], int exploredCount, puzzle node){
		puzzle tmp_puzzle;
		//cout << "xxxxxxxxxxxxxxxxxxxxxxx" << endl;
		for (int space_counter = 0; space_counter < spaces; space_counter++){
			int w = findIndexOf(0, node, space_counter) % width;
			int h = findIndexOf(0, node, space_counter) / width;
			//		cout << "0's (w, h) = (" << w << ", " << h << ')' << endl;
			// try u, d, l, r and compute score
			for (int action = 1; action <= 4; action++){
				copyState(tmp_puzzle, node);
				if (swap(tmp_puzzle, w, h, action)){
					//bool had_explored = false;
					//for (int i = 0; i < exploredCount; i++){
					//	if (isSame(explored[i], tmp_puzzle))
					//		had_explored = true;
					//}
					//if (!had_explored){
					frontier_length++;
					frontier[frontier_length - 1] = node;
					frontier[frontier_length - 1].swap(w, h, action);
					frontier[frontier_length - 1].MDscore = ManhattenDistance(frontier[frontier_length - 1]);
					//}
					//		frontier[frontier_length - 1].printCurrentState();
					//if (minMD_score == -1){
					//	minMD_score = ManhattenDistance(childNodeStack[stacktop]);
					//	choosenChildNodeIndex = stacktop;
					//	choosenAction = action;
					//}
					//else{
					//	if (ManhattenDistance(childNodeStack[stacktop]) == minMD_score){
					//		if (rand() % 2){
					//			//minMD_score = ManhattenDistance(childNodeStack[stacktop]);
					//			choosenChildNodeIndex = stacktop;
					//			choosenAction = action;
					//		}
					//		//int choosenMD_one, newMD_one;
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
					//}
					//else if (ManhattenDistance(childNodeStack[stacktop]) < minMD_score){
					//	minMD_score = ManhattenDistance(childNodeStack[stacktop]);
					//	choosenChildNodeIndex = stacktop;
					//	choosenAction = action;
					//}
					//stacktop++;
					//}
				}
			}
		}
		//cout << "xxxxxxxxxxxxxxxxxxxxxxx" << endl;

		//no any good child node, all of them makes distance higher
		//if (choosenChildNodeIndex == 0 && ManhattenDistance(childNodeStack[choosenChildNodeIndex]) > ManhattenDistance(curState)){
		//	choosenChildNodeIndex = rand()%stacktop;
		//}
		//cout << "**********" << " cC: " << choosenChildNodeIndex<<" cA: "<<choosenAction << endl;
		//copyState(curState, childNodeStack[choosenChildNodeIndex]);
		//printCurrentState();

	}

	void sortFrontier(puzzle frontier[], int frontier_length){
		puzzle tmp;
		for (int i = frontier_length - 1; i > 0; i--)	{
			for (int j = 0; j <= i - 1; j++){
				if (frontier[j].MDscore > frontier[j + 1].MDscore){
					tmp = frontier[j];
					frontier[j] = frontier[j + 1];
					frontier[j + 1] = tmp;
				}
			}
		}
	}

	void removeFirstFrontier(puzzle frontier[], int &frontier_length){
		frontier_length--;
		for (int i = 0; i < frontier_length; i++)
			frontier[i] = frontier[i + 1];
	}

	puzzle graph_search(){
		frontier_length = 1;
		frontier[frontier_length - 1] = curState;
		puzzle node;
		puzzle explored[MAX_STACKS];
		int exploredCount = 0;

		while (true) {
			if (frontier_length != 0) {
				node = frontier[0];
				removeFirstFrontier(frontier, frontier_length);

				if (goalTest(node))
					return node;
				else {
					bool had_explored = false;
					for (int i = 0; i < exploredCount; i++){
						if (isSame(explored[i], node))
							had_explored = true;
					}
					if (!had_explored){
						explored[exploredCount++] = node;
						expandNode(frontier, frontier_length, explored, exploredCount, node);
						sortFrontier(frontier, frontier_length);
					}

					//if (!explored.has(node.state.toString())) {
					//	frontier.insert_all(problem.actions(node.state).map(function(a) {
					//		var new_state = problem.result(a, node.state);
					//		return{ state  : new_state,
					//				parent : node, 
					//				cost   : node.cost + problem.step_cost(node.state, new_state), 
					//				action : a };
					//	}));
					//	explored[explored.length] = node.state.toString();
					//}
				}
			}
			else
				return goalState;// failure
			if (frontier_length > 50)frontier_length = 30;

			//cout << "||||||||||||||||||||||||||||" << endl;
			//for (int i = frontier_length-1; i >=0; i--)
			//	cout <<i<<" MDscore: "<< frontier[i].MDscore << endl;
			//cout << "||||||||||||||||||||||||||||" << endl;
			//cout << "Frontier_len: " << frontier_length << endl;
			//node.printCurrentState();
			//string i;
			//cin >> i;
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
	//							//minMD_score = ManhattenDistance(childNodeStack[stacktop]);
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
	//					else if (ManhattenDistance(childNodeStack[stacktop]) < minMD_score){
	//						minMD_score = ManhattenDistance(childNodeStack[stacktop]);
	//						choosenChildNodeIndex = stacktop;
	//						choosenAction = action;
	//					}
	//					stacktop++;
	//				}
	//			}
	//		}
	//	}
	//	//no any good child node, all of them makes distance higher
	//	if (choosenChildNodeIndex == 0 && ManhattenDistance(childNodeStack[choosenChildNodeIndex]) > ManhattenDistance(curState)){
	//		choosenChildNodeIndex = rand()%stacktop;
	//	}
	//	cout << "**********" << " cC: " << choosenChildNodeIndex<<" cA: "<<choosenAction << endl;
	//	copyState(curState, childNodeStack[choosenChildNodeIndex]);
	//	printCurrentState();
	//}


	void printCurrentState(){
		//cout << "width: " << width << "  height: " << height << endl;
		cout << "|---------Current State----------|" << " MD: " << ManhattenDistance(curState) << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << curState.state[i][j] << ' ';
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
				cout << goalState.state[i][j] << ' ';
			}
			cout << endl;
		}
	}

private:
	puzzle curState, goalState;
	puzzle frontier[MAX_STACKS];
	int frontier_length;
	int width;
	int height;
	int spaces;
	//int startState[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
	//int goalState[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
	//int curState[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
	//int childNodeStack[MAX_STACKS][MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
};

