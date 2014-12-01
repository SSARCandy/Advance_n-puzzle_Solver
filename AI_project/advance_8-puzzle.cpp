#include "puzzle.h"
#include <cstdlib>


int main(){
	string SS, GS;
	cin >> SS >> GS;

	puzzle p(SS,GS);

	p.printCurrentState();
	p.printGoalState();

	cin >> SS;

	return 0;
}