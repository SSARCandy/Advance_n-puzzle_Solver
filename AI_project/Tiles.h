#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
using namespace std;

class Tiles
{
public:
	Tiles(){ width = 0; height = 0; }

	Tiles(string ss)
	{
		string s = ss;
		int w = 0, h = 0;

		/*--------³B²zstart State data & ¬ö¿ý¼e°ª-----------*/
		while (s.find(';') != string::npos){
			h++;
			s.replace(s.find(';'), 1, ",");
		}
		while (s.find(',') != string::npos){
			w++;
			s.replace(s.find(','), 1, " ");
		}

		height = h;
		width = w / h;

		stringstream streamSS(s);
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				int tmp;
				streamSS >> tmp;
				tiles.push_back(tmp);

			}
		}
	}

	Tiles(Tiles& tile)
	{
		copy(tile);
	}

	int& at(int y, int x){ /*if (x>height || y>width) return; */return tiles.at(width*y + x); }

	int& at(int position){ return tiles.at(position); }

	void copy(const Tiles& tile)
	{
		tiles = tile.tiles;
		width = tile.width;
		height = tile.height;
	};

	void print()
	{
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				cout.width(2);
				cout << tiles[width*i + j] << ' ';
			}
			cout << endl;
		}
	}

	////return the index of the target. If target doesn't exist, return -1.
	int find(const int target)
	{
		vector<int>::iterator it = std::find(tiles.begin(), tiles.end(), target);
		if (it != tiles.end())
			return it - tiles.begin();
		else
			return -1;
	}

	int getWidth(){ return width; }
	int getHeight(){ return height; }
	int getNumOfEmptySpaces()
	{
		//count spaces
		int count = 0;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				if (tiles[i*width + j] == 0)
					count++;
		return count;
	}

private:
	vector<int> tiles;
	int width;
	int height;
};