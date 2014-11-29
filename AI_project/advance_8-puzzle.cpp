#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
using namespace std;

class puzzle{
public:
	void initPuzzle(string ss, string gs){
		string s = ss, g = gs;
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
		/*--------處理goal State data-------------*/
		while (g.find(';') != string::npos)	g.replace(g.find(';'), 1, ",");
		while (g.find(',') != string::npos)	g.replace(g.find(','), 1, " ");

		height = h;
		width = (w / h);

		stringstream streamSS(s), streamGS(g);
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				streamSS >> pArray[i][j];
				streamGS >> goalState[i][j];
				startState[i][j] = pArray[i][j];
			}
		}
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
		cout <<"|---------Goal State----------|"<< endl;
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

int main(){
	puzzle pS, pG;
	string SS, GS;

	cin >> SS >> GS;

	pS.initPuzzle(SS, GS);
	pS.printCurrentState();

	cin >> SS;

	return 0;
}