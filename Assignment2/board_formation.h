#pragma once
//
//  board_formation.h
//  AI_assignment_2
//
//  Created by Mareesh Kumar Issar on 10/04/19.
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
		dimension = d;
		numberOfMines = n;
		int count = 1;
		int lastMineI=0, lastMineJ=0;

		boardArray.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			boardArray[i].resize(dimension, 0);        //initializing all the board with 0
		}
		userBoardArray.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			userBoardArray[i].resize(dimension, 0);        //initializing all the board with 0
		}
		cout << "The number of mines:" << numberOfMines << endl;
		checkBoard.resize(dimension);
		for (int i = 0; i < dimension; i++)
		{
			checkBoard[i].resize(dimension, false);        //initializing all the board with false
		}
		checkBoardZeros.resize(dimension);						 //for keeping track of already visited zeros
		for (int i = 0; i < dimension; i++)
		{
			checkBoardZeros[i].resize(dimension, false);       
		}
		////inserting mines at the beginning
		//for (int i = 0; i < dimension; i++)               
		//{
		//	for (int j = 0; j < dimension; j++)
		//	{
		//		if (count<=numberOfMines)
		//		{
		//			boardArray[i][j] = 10;
		//			count++;
		//			lastMineI = i;
		//			lastMineJ = j;
		//		}
		//		
		//	}
		//}
		//	

		//cout << "last mine i and j" << lastMineI << "  " << lastMineJ << endl;
		
		//shuffling the mines randomly
		srand(time(0));
		for (int i = 0; i < numberOfMines; i++)         //swap mine with an open position
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
		}
		
		/*for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				if (boardArray[i][j] == 10)
				{
					GenerateValuesAroundMines(i, j);
				}

			}
		}*/
		
	}
	//computing values around a mine cell
	void GenerateValuesAroundMines(int i,int j)
	{
		if (i - 1 >= 0 && j - 1 >= 0 && boardArray[i-1][j-1]!=10)   //i-1,j-1
			boardArray[i - 1][j - 1] += 1;

		if (i - 1 >= 0 && boardArray[i-1][j] != 10)       //i-1,j		
			boardArray[i - 1][j] += 1;
		
		if (i - 1 >= 0 && j + 1 < dimension && boardArray[i-1][j+1] != 10)   //i-1,j+1
			boardArray[i - 1][j + 1] += 1;
		
		if (j - 1 >= 0 && boardArray[i][j-1] != 10)       //i,j-1
			boardArray[i][j - 1] += 1;
		
		if (j + 1 < dimension && boardArray[i][j+1] != 10)      //i,j+1
			boardArray[i][j + 1] += 1;
		
		if (i + 1 < dimension && j - 1 >= 0 && boardArray[i+1][j-1] != 10)   //i+1,j-1
			boardArray[i + 1][j - 1] += 1;

		if (i + 1 < dimension && boardArray[i+1][j] != 10)       //i+1,j
			boardArray[i + 1][j] += 1;

		if (i + 1 < dimension && j + 1 < dimension && boardArray[i+1][j+1] != 10)   //i+1,j+1
			boardArray[i + 1][j + 1] += 1;
	}
	//setting values around a 0 cell
	void GetValuesAround(int i, int j)
	{	
		if (i - 1 >= 0 && j - 1 >= 0)					//i-1,j-1
		{
			userBoardArray[i - 1][j - 1] = boardArray[i - 1][j - 1];
			if (boardArray[i - 1][j - 1] == 0 && checkBoardZeros[i - 1][j - 1] == false)
			{
				zeros.push(Point(i - 1, j - 1));
				checkBoardZeros[i - 1][j - 1] = true;
			}
				
		}
			

		if (i - 1 >= 0)									 //i-1,j		
		{
			userBoardArray[i - 1][j] = boardArray[i - 1][j];
			if (boardArray[i - 1][j] == 0 && checkBoardZeros[i - 1][j] == false)
			{
				zeros.push(Point(i - 1, j));
				checkBoardZeros[i - 1][j] = true;
			}
				
		}
			

		if (i - 1 >= 0 && j + 1 < dimension)			//i-1,j+1
		{
			userBoardArray[i - 1][j + 1] = boardArray[i - 1][j + 1];
			if (boardArray[i - 1][j + 1] == 0 && checkBoardZeros[i - 1][j + 1] == false)
			{
				zeros.push(Point(i - 1, j + 1));
				checkBoardZeros[i - 1][j + 1] = true;
			}
				
		}
			

		if (j - 1 >= 0)									  //i,j-1
		{
			userBoardArray[i][j - 1] = boardArray[i][j - 1];
			if (boardArray[i][j - 1] == 0 && checkBoardZeros[i][j - 1] == false)
			{
				zeros.push(Point(i, j - 1));
				checkBoardZeros[i][j - 1] = true;
			}
				
		}
			

		if (j + 1 < dimension)							 //i,j+1
		{
			userBoardArray[i][j + 1] = boardArray[i][j + 1];
			if (boardArray[i][j + 1] == 0 && checkBoardZeros[i][j + 1] == false)
			{
				zeros.push(Point(i, j + 1));
				checkBoardZeros[i][j + 1] = true;
			}
				
		}
			

		if (i + 1 < dimension && j - 1 >= 0)			 //i+1,j-1
		{
			userBoardArray[i + 1][j - 1] = boardArray[i + 1][j - 1];
			if (boardArray[i + 1][j - 1] == 0 && checkBoardZeros[i + 1][j - 1] == false)
			{
				zeros.push(Point(i + 1, j - 1));
				checkBoardZeros[i + 1][j - 1] = true;
			}
				
		}
			

		if (i + 1 < dimension)							  //i+1,j
		{
			userBoardArray[i + 1][j] = boardArray[i + 1][j];
			if (boardArray[i + 1][j] == 0 && checkBoardZeros[i + 1][j] == false)
			{
				zeros.push(Point(i + 1, j));
				checkBoardZeros[i + 1][j] = true;
			}
				
		}
			

		if (i + 1 < dimension && j + 1 < dimension)		 //i+1,j+1
		{
			userBoardArray[i + 1][j + 1] = boardArray[i + 1][j + 1];
			if (boardArray[i + 1][j + 1] == 0 && checkBoardZeros[i + 1][j + 1] == false)
			{
				zeros.push(Point(i + 1, j + 1));
				checkBoardZeros[i + 1][j + 1] = true;
			}
				
		}
			
	}
	void PrintActualBoard()
	{
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
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				if (userBoardArray[i][j] == 10)
					cout << "M" << " ";
				else
					cout << userBoardArray[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	
	void CheckCell(int a, int b)
	{
		Point currentPoint(a,b);
		userBoardArray[a][b] = boardArray[a][b];

		if (userBoardArray[a][b] == 10)
			numberOfMinesExploded += 1;

		if (userBoardArray[a][b] == 0)
		{
			if (checkBoardZeros[a][b]==false)
			{
				zeros.push(currentPoint);
				checkBoardZeros[a][b] = true;
			}
				

			while (!zeros.empty())
			{
				currentPoint=zeros.front();
				cout << "points from queue "<<currentPoint.ReturnY() <<"---"<<currentPoint.ReturnX() << endl;
				zeros.pop();
				zeros.size();
				GetValuesAround(currentPoint.ReturnY(), currentPoint.ReturnX());
			}
			
		}
		if (userBoardArray[a][b] >= 1 && userBoardArray[a][b] <= 8) //if number of mines in range [1,8] 
		{

		}
	}
	int GetCell(int i, int j)
	{
		return userBoardArray[i][j];
	}

private:
	int dimension;						 //dimension of the board
	int numberOfMines;					 //total number of mines
	int numberOfMinesExploded=0;			 //number of exploded mines
	vector<vector<int>> boardArray;		 //2D array for storing actual board
	vector<vector<bool>> checkBoard;
	vector<vector<bool>> checkBoardZeros;
	vector<vector<int>> userBoardArray;  //2D array for storing user board
	queue<Point> zeros;
};

#endif /* board_fromation_h */