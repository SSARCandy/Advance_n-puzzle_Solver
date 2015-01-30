#Advance_n-puzzle_Solver 
### -- 有障礙的變形n-puzzle

---

1. Using a sequence string to represent the puzzle state  

###
		For example:  
		12 1  3 4    will written as  
		11 2 13 5    ---------------->   12,1,3,4;11,2,13,5;15,0,14,6;10,9,8,7  
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
		12,1,3,4;11,2,13,5;15,0,14,6;10,9,8,7  
		1,2,3,4;12,13,14,5;11,0,15,6;10,9,8,7 
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
                

