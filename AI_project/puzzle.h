#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
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

	void printCurrentState(){
		cout << "width: " << width << "  height: " << height << endl;
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