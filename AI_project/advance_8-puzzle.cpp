#include "puzzle.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;
	int steps = 0;
	puzzleSolver p(SS,GS);
	puzzle ans;

//	p.printCurrentState();

	//while (!p.goalTest()){
	//for (int i = 0; i < 10; i++){
	//	p.AstarSolverOneStep();
//		cin >> o;
	//	if (p.goalTest())
	//		break;
	//	steps++;
	//}
	//p.printGoalState();
	//cout << "Total Steps = " << steps << endl;

	ans = p.graph_search();

	for (int i = 0; i < ans.actCount; i++){
		cout << ans.action[i];
	}
	cout << ans.actCount << endl;

	cin >> SS;

	return 0;
}