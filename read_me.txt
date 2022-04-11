Code Details

•	The programming language used is C++. Not sure if it is omega compatible.
•	The code has a some global variables for storing input file in maps, a root node for fringe, closed set, counters like nodes_popped, a vector for route, etc.
•	There is a class for creating node and holding its information of corresponding city like name, cost, etc. 
•	The code has functions to add and pop nodes from fringe. Also functions for graph search, expanding nodes and the main function to execute everything.
•	To run the code, I stored all relevant files in a folder, went to the folder’s directory in Terminal and then to execute the code and pass arguments in the Terminal type,
1.	g++ -o find_route find_route.cpp(my cpp file)
2.	find_route argument1(input.txt file) argument2(origin city) argument3(destination_city) argument4(heuristic.txt file if needed).

So basically just 2 steps to run code in terminal, for example,
	
1.	g++ -o find_route find_route.cpp
2.	find_route Sample_Input_File.txt Bremen Kassel Sample_Heuristic_File.txt

•	Compiler version - "compiler: \"C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin\\g++.exe\""


