#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <vector>
 #include <Windows.h>

#define MAX_HEIGHT_AND_WIDTH 10
#define MAX_SEARCH_NODE 250
#define MAX_STACKS 100
using namespace std;

class puzzle{
public:
	puzzle(){
		width = 0;
		height = 0;
		Score = 0;
		steps = 0;
		MDscore = 999;
		actCount = 0;

		for (int i = 0; i < MAX_STACKS; i++)
			action[i] = "";
	}

	puzzle(int arr[][MAX_HEIGHT_AND_WIDTH], int w, int h){
		width = w;
		height = h;
		Score = 0;
		steps = 0;
		MDscore = 999;
		actCount = 0;

		copyState(state, arr);
		for (int i = 0; i < MAX_STACKS; i++)
			action[i] = "";
	}

	puzzle &operator= (const puzzle & other){
		MDscore = other.MDscore;
		width = other.width;
		height = other.height;
		Score = other.Score;
		steps = other.steps;
		actCount = other.actCount;

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				state[i][j] = other.state[i][j];

		for (int i = 0; i < other.actCount; i++)
			action[i] = other.action[i];

		return *this;
	}	

	void copyState(int dest[][MAX_HEIGHT_AND_WIDTH], int source[][MAX_HEIGHT_AND_WIDTH]){
		stringstream buffer;
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
			return false;
			break;
		}
	}

	void printCurrentState(){
		cout << "|---------Current State----------|" << " MD: " << MDscore << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << state[i][j] << ' ';
			}
			cout << endl;
		}
	}

	void setScore(int md, int steps){
		MDscore = md;
		Score = md + steps;
	}

	int getMDscore(){
		return MDscore;
	}

	int steps;
	int Score;
	int state[MAX_HEIGHT_AND_WIDTH][MAX_HEIGHT_AND_WIDTH];
	int actCount;
	string action[MAX_STACKS];
private:
	int MDscore;
	int width;
	int height;
};

class puzzleSolver{
public:
	puzzleSolver(string start, string goal){
		spaces = 0;
		//frontier_length = 0;
		initPuzzle(start);
		setGoalState(goal);
		startState.setScore(ManhattenDistance(startState), 0);
		goalState.setScore(ManhattenDistance(goalState), 0);
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

		startState = puzzle(tmpArray, width, height);
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

	bool isSame(int a[][10], int b[][10]){
//		if (a.getMDscore() != b.getMDscore()) return false;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (a[i][j] != b[i][j])
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
		return -1;
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
			return false;
			break;
		}
	}

	void expandNode(vector<puzzle> &frontier,  puzzle node){
		puzzle tmp_puzzle;
		for (int space_counter = 0; space_counter < spaces; space_counter++){
			int w = findIndexOf(0, node, space_counter) % width;
			int h = findIndexOf(0, node, space_counter) / width;

			// try u, d, l, r and compute score
			for (int action = 1; action <= 4; action++){
				tmp_puzzle = node;
				if (tmp_puzzle.swap(w, h, action)){
					tmp_puzzle.setScore(ManhattenDistance(tmp_puzzle), ++tmp_puzzle.steps);
					InsertFrontier(frontier, tmp_puzzle);
				}
			}
		}
	}

	void InsertFrontier(vector<puzzle> &f, puzzle &t){
		if (f.size() == 0)
			f.push_back(t);
		else{
			unsigned lowerBound = 0, upperBound = f.size() - 1, middle, curIn = 0;
			if (t.Score > f[upperBound].Score){
				f.push_back(t);
			}
			else{
				for (unsigned i = 0; i < f.size(); i++){
					middle = (lowerBound + upperBound) / 2;
					if (lowerBound == upperBound){
						curIn = lowerBound;
						break;
					}
					if (t.Score < f[middle].Score){
						upperBound = middle;
					}
					else if (t.Score > f[middle].Score){
						lowerBound = middle + 1;
						if (lowerBound >= upperBound){
							curIn = upperBound;
							break;
						}
					}
					else if (t.Score == f[middle].Score){
						for (int i = middle; i >= 0; i--){
							if (t.Score > f[i].Score){
								curIn = i + 1;
								break;
							}
						}
						break;
					}
					else{
						cout << "ERROR" << endl;
						break;
					}
				}
				f.insert(f.begin() + curIn, t);
			}
		}
		//unsigned i;
		//for (i = 0; i < f.size(); i++){
		//	if (t.Score <= f[i].Score){
		//		f.insert(f.begin() + i,t);
		//		break;
		//	}
		//}
		//if (i == f.size())
		//	f.push_back(t);
	}
	void InsertExplored(vector<puzzle> &f, puzzle &t){
		if (f.size() == 0)
			f.push_back(t);
		else{
			unsigned lowerBound = 0, upperBound = f.size() - 1, middle, curIn = 0;
			if (t.getMDscore() > f[upperBound].getMDscore()){
				f.push_back(t);
			}
			else{
				for (unsigned i = 0; i < f.size(); i++){
					middle = (lowerBound + upperBound) / 2;
					if (lowerBound == upperBound){
						curIn = lowerBound;
						break;
					}
					if (t.getMDscore() < f[middle].getMDscore()){
						upperBound = middle;
					}
					else if (t.getMDscore() > f[middle].getMDscore()){
						lowerBound = middle + 1;
						if (lowerBound >= upperBound){
							curIn = upperBound;
							break;
						}
					}
					else if (t.getMDscore() == f[middle].getMDscore()){
						for (int i = middle; i >= 0; i--){
							if (t.getMDscore() > f[i].getMDscore()){
								curIn = i + 1;
								break;
							}
						}
						break;
					}
					else{
						cout << "ERROR" << endl;
						break;
					}
				}
				f.insert(f.begin() + curIn, t);
			}
		}
	}
	bool search(vector<puzzle> explored, puzzle node){
		for (unsigned i = 0; i < explored.size() && explored[i].getMDscore() <= node.getMDscore(); i++){
			if (isSame(explored[i].state, node.state)){
				return true;
			}
		}
		return false;
		//unsigned lowerBound = 0, upperBound = explored.size() - 1, middle, curIn = 0;
		//for (int i = 0; i < explored.size(); i++){
		//	middle = (lowerBound + upperBound) / 2;
		//	if (lowerBound == upperBound){
		//		if (isSame(explored[middle], node))
		//			return true;
		//	}
		//	if (node.Score < explored[middle].Score){
		//		upperBound = middle;
		//	}
		//	else if (node.Score > explored[middle].Score){
		//		lowerBound = middle + 1;
		//		if (lowerBound > upperBound){
		//			return false;
		//		}
		//	}
		//	else
		//	{
		//		for (int i = middle; i >= 0 && explored[i].Score == node.Score; i--){
		//			if (isSame(explored[i], node)){
		//				return true;
		//			}
		//		}
		//		for (int i = middle; i < upperBound && explored[i].Score == node.Score; i++){
		//			if (isSame(explored[i], node)){
		//				return true;
		//			}
		//		}
		//		break;
		//	}
		//}
		//return false;
	}


	void removeFrontier(puzzle frontier[], int &frontier_length, puzzle node){
		int node_index = 0;
		for (int i = 0; i < frontier_length; i++)
			if (isSame(frontier[i].state, node.state)){
				frontier[i] = node;
				node_index = i;
			}

		frontier_length--;
		for (int i = node_index; i < frontier_length; i++)
			frontier[i] = frontier[i + 1];
	}
	void removeFirstFrontier(puzzle frontier[], int &frontier_length){
		frontier_length--;
		for (int i = 0; i < frontier_length; i++)
			frontier[i] = frontier[i + 1];
	}


	puzzle graph_search(){
		puzzle node;
		vector<puzzle> explored;
		int search_nodes = 0, tmp = 0;

		frontier.clear();
		frontier.push_back(startState);

		while (true) {
			if (frontier.size() != 0) {
				node = frontier[0];
				frontier.erase(frontier.begin());

				if (goalTest(node)){
					//cout << "Explored len: " << explored.size() << endl;
					//cout << "Frontier len: " << frontier.size() << endl;
					return node;
				}
				else {
					//bool had_explored = search(explored, node);
					bool had_explored = false;
					//if (explored.size() == 0)
					//	had_explored = false;
					for (unsigned i = 0; i < explored.size(); i++){
						//isSame(explored[i], node);
						if (isSame(explored[i].state, node.state)){
							had_explored = true;
							break;
						}
						//if (explored[i].getMDscore() != node.getMDscore()){
						//	had_explored = false;
						//	break;
						//}
						//for (int i = 0; i < height; i++){
						//	for (int j = 0; j < width; j++){
						//		if (explored[i].state[i][j] != node.state[i][j])
						//		{
						//			had_explored = false;
						//		}
						//	}
						//	if (!had_explored) break;
						//}

					}
					if (!had_explored){
						search_nodes++;
						//InsertExplored(explored, node);
						explored.push_back(node);
						expandNode(frontier, node);
					}
				}
			}
			else
				return goalState;// failure
			
			//system("cls");
			//cout << "Explored len: " << explored.size() << endl;
			//cout << "Frontier len: " << frontier.size() << endl;
			//cout << "Search Nodes: " << search_nodes << endl;
			//node.printCurrentState();
			//cout << "||||||||||||||||||||||||||||" << endl;
			//for (unsigned i = 0; i < frontier.size() && i<15; i++)
			//	cout <<i<<" - Sccore: "<< frontier[i].Score << endl;
			//cout << "||||||||||||||||||||||||||||" << endl;

				//string i;
				//cin >> i;
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
	vector<puzzle>frontier;
	puzzle startState, goalState;
	int width;
	int height;
	int spaces;
};

