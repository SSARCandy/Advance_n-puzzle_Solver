#include "puzzle.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;

	puzzle p(SS,GS);

	p.printCurrentState();

	//while (!p.goalTest()){
	for (int i = 0; i < 10; i++){
		p.AstarSolverOneStep();
		if (p.goalTest())
			break;
	}
	p.printGoalState();

	cin >> SS;

	return 0;
}