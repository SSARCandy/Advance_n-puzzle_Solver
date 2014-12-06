#include "puzzle.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;
	int steps = 0;
	puzzleSolver p(SS,GS);
	puzzle ans;

	ans = p.graph_search();

	cout << "=============ANS==============" << endl;
	if (ans.actCount == 0)
		cout << "Failed" << endl;
	else{
		for (int i = 0; i < ans.actCount; i++)
			cout << ans.action[i];
	}
	cout << "total steps: "<<ans.actCount << endl;
	cout << "=============ANS==============" << endl;

	cin >> SS;

	return 0;
}