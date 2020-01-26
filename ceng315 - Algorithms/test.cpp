#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cassert>
#include "the3.h"


void printVector(std::vector<int>& array){

    std::cout << "size: " << array.size() << std::endl;
    for (unsigned i = 0; i < array.size(); ++i){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}




bool TestTask()
{
    
    // time variables
  clock_t begin;
  clock_t end;
  double duration;
  int expectedTime = 500;
  int true_answer = -1;
  int solution = -1;
  
    // generate solution
  if ((begin = clock ()) == -1) 
      std::cerr << "clock err" << std::endl;
  int n = 6;
	int** edgeList;
	double* scores = new double[n];
	edgeList = new int* [n];
	for (int i = 0; i < n; i++) {
		edgeList[i] = new int[n];
	}

	edgeList[0][0] = 0;
	edgeList[0][1] = 0;
	edgeList[0][2] = 0;
	edgeList[0][3] = 3;
	edgeList[0][4] = 0;
	edgeList[0][5] = 2;

	edgeList[1][0] = 0;
	edgeList[1][1] = 0;
	edgeList[1][2] = 3;
	edgeList[1][3] = 0;
	edgeList[1][4] = 2;
	edgeList[1][5] = 0;

	edgeList[2][0] = 0;
	edgeList[2][1] = 3;
	edgeList[2][2] = 0;
	edgeList[2][3] = 0;
	edgeList[2][4] = 0;
	edgeList[2][5] = 0;

	edgeList[3][0] = 3;
	edgeList[3][1] = 0;
	edgeList[3][2] = 0;
	edgeList[3][3] = 0;
	edgeList[3][4] = 0;
	edgeList[3][5] = 4;

	edgeList[4][0] = 0;
	edgeList[4][1] = 2;
	edgeList[4][2] = 0;
	edgeList[4][3] = 0;
	edgeList[4][4] = 0;
	edgeList[4][5] = 0;

	edgeList[5][0] = 2;
	edgeList[5][1] = 0;
	edgeList[5][2] = 0;
	edgeList[5][3] = 4;
	edgeList[5][4] = 0;
	edgeList[5][5] = 0;

	scores[0] = 0;
	scores[1] = 0;
	scores[2] = 0;
	scores[3] = 0;
	scores[4] = 0;
	scores[5] = 0;

	int result = Important(n, edgeList, scores);

	std::cout << "result is: " << result << std::endl;

	for (int i = 0; i < n; i++) {
		std::cout << scores[i] << " -- ";
	}
	std::cout << std::endl;

	for (int i = 0; i < n; i++) {
		delete[] edgeList[i];
	}
	delete[] edgeList;
	delete[] scores;
      
  //solution =Important (n, edgeList, scores);


  if ((end = clock ()) == -1) 
      std::cerr << "clock err" << std::endl;
      
  duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;
  
  cout << "Elapsed time  " << duration << " vs " << expectedTime << " microsecs" << std::endl;

  return (solution == true_answer);
}





int main()
{
    srandom(time(0));       // no need to initialize second time

  if (TestTask())
    std::cout << "PASSED TASK" << std::endl << std::endl;
  else
    std::cout << "FAILED TASK" << std::endl << std::endl;
  

  return 0;
}
