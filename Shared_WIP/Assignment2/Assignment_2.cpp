//
//  board_formation.h
//  AI_assignment_2
//
//  Created by Mareesh Kumar Issar on 10/04/19.
// 
//

#include "point.h"
#include "board_formation.h"

#include <iostream>
#include<vector>
#include<unordered_map>

int main()
{
	
	int d, n;
	vector<Point> neighbors;
	int flaggedMines = 0;
	int numberOfMinesExploded = 0;			 //number of exploded mines
	int i = 0;

	cout << "Enter the dimension:";
	cin >> d;
	cout << "Enter the number of mines:";
	cin >> n;
	Board b1(d, n);
	b1.PrintActualBoard();
	b1.PrintUserBoard();

	//if the seed value is time(0) it reveals all the mines first!!
	srand(23);

	while(b1.GetCountUncovered()!=d*d)         
	{
		int p1;
		int p2;
	
		int totalNumberOfMines = 0;
		
		do {
			p1 = 0 + rand() % (d);
			p2 = 0 + rand() % (d);
		} while (b1.IsVisited(p1, p2) == true);
		cout << "(" << p1 << "," << p2 << ")" << endl;               //printing the open position to swap
		b1.OpenCell(p1, p2);

		if (b1.GetValue(p1, p2) == 10)
		{
			numberOfMinesExploded += 1;
		}
		b1.PrintUserBoard();
		
		//agent function	
		Point currentPoint(p1, p2);
		queue<Point> unexploredNeighborCells;
		
		unexploredNeighborCells.push(currentPoint);
		while (!unexploredNeighborCells.empty())
		{
			int numberOfRevealedMines = 0;
			int numberOfRevealedSafe = 0;
			int numberOfHiddenNeighbors = 0;
			currentPoint = unexploredNeighborCells.front();
			//cout << "points from queue "<<currentPoint.ReturnY() <<"---"<<currentPoint.ReturnX() << endl;
			unexploredNeighborCells.pop();
		
			totalNumberOfMines = b1.GetValue(currentPoint.ReturnY(), currentPoint.ReturnX());
			neighbors = b1.GetNeighbors(currentPoint.ReturnY(), currentPoint.ReturnX());
			for (int i = 0; i < neighbors.size(); i++)
			{
				//cout << "neighbor, x=" << neighbors[i].ReturnY() << " , y=" << neighbors[i].ReturnX() << endl;

				//updating number of hidden neighbors
				if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
					numberOfHiddenNeighbors += 1;


				if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == true)
				{
					//updating number of revealed mines
					if (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 10 || b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 20)
						numberOfRevealedMines += 1;

					//updating number of revealed safe neighbors
					if ((b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) >= 0) && (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) <= 8))
						numberOfRevealedSafe += 1;
				}


			}
			
			//cout << "total number of mines- " << totalNumberOfMines << endl;
			//cout << "if total number of mines is 10 then the selected cell is a mine" << endl;
			//cout << "hidden neighbors-" << numberOfHiddenNeighbors << endl;
			//cout << "revealed mines-" << numberOfRevealedMines << endl;
			//cout << "revealed safe neighbors-" << numberOfRevealedSafe << endl;
			//cout << endl;


			//checking rule 1
			if (totalNumberOfMines - numberOfRevealedMines == numberOfHiddenNeighbors)
			{
				for (int i = 0; i < neighbors.size(); i++)
				{
					if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
					{
						//flagging the mines
						b1.SetFlag(neighbors[i].ReturnY(), neighbors[i].ReturnX());
						flaggedMines += 1;
						//unexploredNeighborCells.push(Point(neighbors[i].ReturnY(), neighbors[i].ReturnX()));
						//cout << "cell pushed in queue is " << neighbors[i].ReturnY() << " , " << neighbors[i].ReturnX() << endl;
					}
				}
			}

			//checking rule 2
			if ((b1.GetNeighborCount(currentPoint.ReturnY(), currentPoint.ReturnX()) - totalNumberOfMines) - numberOfRevealedSafe == numberOfHiddenNeighbors)
			{
				for (int i = 0; i < neighbors.size(); i++)
				{
					if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
					{
						b1.OpenCell(neighbors[i].ReturnY(), neighbors[i].ReturnX());
						unexploredNeighborCells.push( Point(neighbors[i].ReturnY(), neighbors[i].ReturnX()) );
						//cout << "cell pushed in queue is " << neighbors[i].ReturnY() << " , " << neighbors[i].ReturnX() << endl;
					}
				}
			}


		}
		cout << "number of flagged mines- " << flaggedMines<< endl;
		cout << "opened cells till now- " << b1.GetCountUncovered() << endl;
	}
	b1.PrintUserBoard();
	


	return 0;

}

