#pragma once
//
//  board_formation.h
//  AI_assignment_2
//
//
//


#ifndef board_fromation_h
#define board_fromation_h

#include "point.h"

#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <queue>

using namespace std;

class Board
{
public:
	Board(int d, int n)
	{
		countUncovered = 0;
		dimension = d;		//the dimension of the minesweeper
		numberOfMines = n;	//the number of mines decided first by the board


		boardArray.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			boardArray[i].resize(dimension, 0);        //initializing all the board with 0
		}

		userBoardArray.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			userBoardArray[i].resize(dimension, -1);        //initializing all the board with -1
		}

		cout << "The number of mines:" << numberOfMines << endl;

		checkBoard.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			checkBoard[i].resize(dimension, false);        //initializing all the board with false
		}
	
		//shuffling the mines randomly
		srand(time(0));
        Shuffle_set();
		/*for (int i = 0; i < numberOfMines; i++)         //swap mine with an open position
		{
			int p1;
			int p2;
			do
			{
				p1 = 0 + rand() % (dimension);
				p2 = 0 + rand() % (dimension);
			} while (checkBoard[p1][p2] == true);
			cout << i + 1 << ". " << "(" << p1 << "," << p2 << ")" << endl;               //printing the open position to swap
			checkBoard[p1][p2] = true;
			boardArray[p1][p2] = 10;
			GenerateValuesAroundMines(p1, p2);
		}*/
		
	}

	void Shuffle_set()
	{
        srand(time(0));
		int shuffle_array[dimension * dimension];
        
        for(int i=0; i < dimension*dimension; i++)
        {
            if(i < numberOfMines)           //set the firnt N = number of mines == 10
                shuffle_array[i] = 10;
            else
                shuffle_array[i] = 0;
        }
        
        for(int i=0; i < numberOfMines; i++)
        {
            if(rand()%2==0)
            {
                int temp = shuffle_array[i];
                int iter = numberOfMines + rand() % ((dimension*dimension)-numberOfMines-1);
                shuffle_array[i] = shuffle_array[iter];
                shuffle_array[iter] = temp;
            }
        }
        
        for(int i = 0; i < dimension*dimension; i++)
        {
            if(shuffle_array[i] == 10)
            {
                int y = i%dimension;
                int x = i/dimension;
                boardArray[y][x] = 10;
                GenerateValuesAroundMines(y, x);
            }
        }
		
	}
    
    
    /*void print_shuffle_array(int a[])
    {
        for (int i=0;i<dimension*dimension;i++)
        {
            cout <<
        }
    }*/

	//filling values around a mine cell
	void GenerateValuesAroundMines(int i,int j)
	{
		if (i - 1 >= 0 && j - 1 >= 0 && boardArray[i-1][j-1]!=10 && boardArray[i-1][j-1]!=20)   //i-1,j-1
			boardArray[i - 1][j - 1] += 1;

		if (i - 1 >= 0 && boardArray[i-1][j] != 10 && boardArray[i-1][j]!=20 )       //i-1,j
			boardArray[i - 1][j] += 1;
		
		if (i - 1 >= 0 && j + 1 < dimension && boardArray[i-1][j+1] != 10 && boardArray[i-1][j+1] != 20)   //i-1,j+1
			boardArray[i - 1][j + 1] += 1;
		
		if (j - 1 >= 0 && boardArray[i][j-1] != 10 && boardArray[i][j-1] != 20)       //i,j-1
			boardArray[i][j - 1] += 1;
		
		if (j + 1 < dimension && boardArray[i][j+1] != 10 && boardArray[i][j+1] != 20)      //i,j+1
			boardArray[i][j + 1] += 1;
		
		if (i + 1 < dimension && j - 1 >= 0 && boardArray[i+1][j-1] != 10 && boardArray[i+1][j-1] != 20)   //i+1,j-1
			boardArray[i + 1][j - 1] += 1;

		if (i + 1 < dimension && boardArray[i+1][j] != 10 && boardArray[i+1][j] != 20)       //i+1,j
			boardArray[i + 1][j] += 1;

		if (i + 1 < dimension && j + 1 < dimension && boardArray[i+1][j+1] != 10 && boardArray[i+1][j+1] != 20)   //i+1,j+1
			boardArray[i + 1][j + 1] += 1;
	}


	


	void PrintActualBoard()
	{
		cout << "Printing actual board" << endl << endl;
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				if (boardArray[i][j] == 10)
					cout << "M" << " ";
				else
					cout << boardArray[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void PrintUserBoard()
	{
		cout << "Printing user board" << endl << endl;
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				
				if (userBoardArray[i][j] == -1)
					cout << "*" << " ";

				if (userBoardArray[i][j] == 10)
					cout << "M" << " ";

				if (userBoardArray[i][j] == 20)
					cout << "F" << " ";

				if (userBoardArray[i][j] >=0 && userBoardArray[i][j] <= 8)
					cout << userBoardArray[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	void OpenCell(int a, int b)
	{
		userBoardArray[a][b] = boardArray[a][b];
		countUncovered += 1;
	}

	bool IsVisited(int i, int j)
	{
		return userBoardArray[i][j]!=-1;
	}

	void SetFlag(int i, int j)
	{
		userBoardArray[i][j]=20;
		countUncovered+= 1 ;
	}

	int GetValue(int i, int j)
	{
		return userBoardArray[i][j];
	}

	int GetCountUncovered()
	{
		return countUncovered;
	}

	//returns a vector consisting of neighboring cells
	vector<Point> GetNeighbors(int i, int j)
	{
		vector<Point> neighbors;
		
		if (i - 1 >= 0 && j - 1 >= 0)   //i-1,j-1
		{
			Point cell(i - 1, j - 1);
			neighbors.push_back(cell);
		}
			

		if (i - 1 >= 0)       //i-1,j		
		{
			Point cell(i - 1, j);
			neighbors.push_back(cell);
		}
	

		if (i - 1 >= 0 && j + 1 < dimension)   //i-1,j+1
		{
			Point cell(i - 1, j + 1);
			neighbors.push_back(cell);
		}
			

		if (j - 1 >= 0)       //i,j-1
		{
			Point cell(i, j - 1);
			neighbors.push_back(cell);
		}


		if (j + 1 < dimension)      //i,j+1
		{
			Point cell(i, j + 1);
			neighbors.push_back(cell);
		}
			

		if (i + 1 < dimension && j - 1 >= 0)   //i+1,j-1
		{
			Point cell(i + 1, j - 1);
			neighbors.push_back(cell);
		}
		

		if (i + 1 < dimension)       //i+1,j
		{
			Point cell(i + 1, j);
			neighbors.push_back(cell);
		}
		

		if (i + 1 < dimension && j + 1 < dimension)   //i+1,j+1
		{
			Point cell(i + 1, j + 1);
			neighbors.push_back(cell);
		}
		return neighbors;
	}

	//returns number of possible neighbors around a cell
	int GetNeighborCount(int i, int j)
	{
		int count = 0;

		if (i - 1 >= 0 && j - 1 >= 0)   //i-1,j-1
		{
			count += 1;
		}


		if (i - 1 >= 0)       //i-1,j		
		{
			count += 1;
		}


		if (i - 1 >= 0 && j + 1 < dimension)   //i-1,j+1
		{
			count += 1;
		}


		if (j - 1 >= 0)       //i,j-1
		{
			count += 1;
		}


		if (j + 1 < dimension)      //i,j+1
		{
			count += 1;
		}


		if (i + 1 < dimension && j - 1 >= 0)   //i+1,j-1
		{
			count += 1;
		}


		if (i + 1 < dimension)       //i+1,j
		{
			count += 1;
		}


		if (i + 1 < dimension && j + 1 < dimension)   //i+1,j+1
		{
			count += 1;
		}

		return count;
	}



private:
	int dimension;					 //dimension of the board
	int numberOfMines;				 //total number of mines
	int countUncovered;				//number of uncovered cells
	vector<vector<int> > boardArray;		//2D array for storing actual board
	vector<vector<bool> > checkBoard;
	vector<vector<int> > userBoardArray;            //2D array for storing user facing board
	
};

#endif /* board_fromation_h */
