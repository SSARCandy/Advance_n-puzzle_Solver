#include "puzzle_optimal.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;/*
	int o;*/
	puzzle p(SS,GS);
	
	p.A_star();
	cout << "---------------------" << endl;
	//cout << tes
	p.printCurrentState();
	p.printGoalState();

	//cout << "------------------------------------" << endl;
	//p.testing();
//
//	p.printCurrentState();
//
////	while (!p.goalTest()){
//	for (int i = 0; i < 30; i++){
//		p.AstarSolverOneStep();
////		cin >> o;
//		if (p.goalTest())
//			break;
//	}
//	p.printGoalState();
//
//	cin >> SS;

	system("pause");
	return 0;
}