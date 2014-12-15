#include "puzzle.h"
#include <cstdlib>
#include <time.h>


int main(){
	clock_t start, finish;
	double duration;
	string SS, GS;
	puzzle ans;

	getline(cin, SS);
	getline(cin, GS);
	puzzleSolver p(SS, GS);

	start = clock();  //測速開始
	ans = p.graph_search();
	finish = clock(); //測速結束
	duration = (double)(finish - start) / CLOCKS_PER_SEC;

	//cout << "=============ANS==============" << endl;
	if (ans.actCount == 0)
		cout << "Failed" << endl;
	else{
		for (int i = 0; i < ans.actCount; i++)
			cout << ans.action[i];
	}
	//cout << "total steps: "<<ans.actCount << endl;
	//printf("%f seconds\n", duration);
	//cout << "=============ANS==============" << endl;

	//cin >> SS;

	return 0;
}