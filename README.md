#Advance_n-puzzle_Solver 


### Introduction 
[N-puzzle](https://en.wikipedia.org/wiki/15_puzzle) (also called Gem Puzzle, Boss Puzzle, Game of Fifteen, Mystic Square and many others) is a sliding puzzle that consists of a frame of numbered square tiles in random order with one tile missing. The puzzle also exists in other sizes, particularly the smaller 8-puzzle. If the size is 3×3 tiles, the puzzle is called the 8-puzzle or 9-puzzle, and if 4×4 tiles, the puzzle is called the 15-puzzle or 16-puzzle named, respectively, for the number of tiles and the number of spaces. The object of the puzzle is to place the tiles in order (see diagram) by making sliding moves that use the empty space.

In this project, we can solved n-puzzle with **obstacles**(represent as `-1`), or with more than 1 **space**(represent as `0`)  
The algorithm is based on **A* algorithm**, alone with **Manhatten distance** as the score.



### Run the code

- Use terminal with [**CMake**](https://cmake.org/) (3.0+) 

	```bash
	~$ cmake CMakeLists.txt
	~$ make 
	~$ ./puzzle
	```
- Use **XCode** or **Visual Studio**
  - cmake-gui is easier to configure with these IDEs
- Build yourself with g++

	```bash
	~$ g++ -o puzzle src/advance_8-puzzle.cpp include/puzzle.h
	~$ ./puzzle < test/in12.txt > out.txt
	```


---

1. Using a sequence string to represent the puzzle state  

###
		For example:  
		12 1  3 4    will written as  
		11 2 13 5    ---------------->   12,1,3,4;11,2,13,5;15,0,14,6;10,9,8,7;  
		15 0 14 6  
		10 9  8 7  

2. In input data, first row is "start state", second is "goal state".  
   The output format: First row is STEP1, second row is STEP2, until goal state。  
   e.g. `(5,2,L)` is move position `(5,2) to LEFT`

3. You can use standrad input(keyboard) or using redirect command  
	Something like `./a.out < in.txt > out.txt`

4. Sample Input and Output:  
  
###
		In:
		12,1,3,4;11,2,13,5;15,0,14,6;10,9,8,7;  
		1,2,3,4;12,13,14,5;11,0,15,6;10,9,8,7; 
###
		Out:  
		(2, 3, L)  
		(1, 3, U)  
		(1, 2, U)  
		(1, 1, R)  
		(2, 1, D)  
		(2, 2, R)  
		(3, 2, D)  
		(3, 3, L)  
                

