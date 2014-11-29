#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
using namespace std;

class puzzle{
public:
	void initPuzzle(string in){
		string s = in;
		int w = 0, h = 0;
	
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
		stringstream stream(s);
		//streambuf* cin_backup = std::cin.rdbuf(stream.rdbuf());
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				stream >> pArray[i][j];
			}
		}
	}
	void printCurrentState(){
		cout << "width: " << width << "  height: " << height << endl;
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout << pArray[i][j] << ' ';
			}
			cout << endl;
		}
	}
private:
	int pArray[15][15];
	int width;
	int height;
};

int main(){
	puzzle pS, pG;
	string SS, GS;

	cin >> SS >> GS;

	pS.initPuzzle(SS);
	pG.initPuzzle(GS);

	cin >> SS;

	return 0;
}