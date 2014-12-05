#include "puzzle.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;
	int o;
	puzzle p(SS,GS);

	p.printCurrentState();

//	while (!p.goalTest()){
	for (int i = 0; i < 30; i++){
		p.AstarSolverOneStep();
//		cin >> o;
		if (p.goalTest())
			break;
	}
	p.printGoalState();

	cin >> SS;

	return 0;
}