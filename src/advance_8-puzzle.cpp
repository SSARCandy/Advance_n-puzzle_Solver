#include "puzzle.h"
#include <cstdlib>
#include <time.h>


int main(){
	string sS, gS;
	puzzle ans;
	//clock_t start, finish;
	//double duration;

	while (1)
	{
		getline(cin, sS);
		getline(cin, gS);
		//sS.append(";");
		//gS.append(";");
		//if (sS[sS.size() - 1] != ';') sS.append(";");
		//if (gS[gS.size() - 1] != ';') gS.append(";");

		puzzleSolver p(sS, gS);

		//start = clock();  //測速開始
		ans = p.graph_search();
		//finish = clock(); //測速結束
		//duration = (double)(finish - start) / CLOCKS_PER_SEC;

		//cout << "=============ANS==============" << endl;
		if (ans.actCount == 0)
			cout << "Failed" << endl;
		else
		{
			for (int i = 0; i < ans.actCount; i++)
				cout << ans.action[i];
		}
		//printf("%f seconds\n", duration);
		//cout << "=============ANS==============" << endl;

		//cin >> sS;
	}
	return 0;
}