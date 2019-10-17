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


int main()
{

	int d, n;
	vector<Point> neighbors;
	int flaggedMines = 0;
	int numberOfMinesExploded = 0;			 //number of exploded mines
	int i = 0;
	vector<Point> neighboursWithClue;
	vector<Point> neighboursCSP;
	vector<vector<Point>> windowCSP;
	cout << "Enter the dimension:";
	cin >> d;
	cout << "Enter the number of mines:";
	cin >> n;
	Board b1(d, n);
	b1.PrintActualBoard();
	b1.PrintUserBoard();

	//if the seed value is time(0) it reveals all the mines first!!
	srand(23);

	while (b1.GetCountUncovered() != d * d)
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
		for (int i = 1; i < d - 1; i++)
		{

			for (int j = 1; j < d - 1; j++)
			{
				neighboursWithClue.clear();
				windowCSP.clear();
				//cout << "for central cell " << i << " , " << j << endl;

				neighboursWithClue = b1.GetNeighborsWithClues(i, j);

				for (int k = 0; k < neighboursWithClue.size(); k++)
				{
					neighboursCSP.clear();
					//cout << "neighbours with clue " << neighboursWithClue[k].ReturnY() << " , " << neighboursWithClue[k].ReturnX() << endl;


					int numberOfRevealedMines = 0;
					int numberOfRevealedSafe = 0;
					int numberOfHiddenNeighbors = 0;

					neighbors.clear();

					totalNumberOfMines = b1.GetValue(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX());
					neighbors = b1.GetNeighbors(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX());
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
								
							}
						}
					}

					//checking rule 2
					if ((b1.GetNeighborCount(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX()) - totalNumberOfMines) - numberOfRevealedSafe == numberOfHiddenNeighbors)
					{
						for (int i = 0; i < neighbors.size(); i++)
						{
							if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
							{
								b1.OpenCell(neighbors[i].ReturnY(), neighbors[i].ReturnX());
								
							}
						}
					}

					neighboursCSP = b1.GetNeighborsCSP(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX(), i, j);
					
					//cout << "no of hidden neighbors of "<< neighboursWithClue[k].ReturnY()<< " , "<<neighboursWithClue[k].ReturnX()<<"are " << neighboursCSP.size() << endl;

					//adding point with clue value at the end
					Point temp(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX());
					temp.SetPointClueValue(b1.GetValue(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX()));
					neighboursCSP.push_back(temp);
				
					windowCSP.push_back(neighboursCSP);
					

				}

				int longerEquationSize;
				int shorterEquationSize;
				int longerEquation;
				int shorterEquation;
				//solving CSP
				
				for (int a = 0; a < windowCSP.size(); a++)
				{
					for (int b = a + 1; b < windowCSP.size(); b++)
					{
						vector<Point> firstEquation;
						vector<Point> secondEquation;

						//copying the value of the first and second equation so that modification doesn't affect them
						firstEquation = windowCSP[a];
						secondEquation = windowCSP[b];

						longerEquationSize = 0;
						shorterEquationSize = 0;
						longerEquation = 0;
						shorterEquation = 0;
						if (windowCSP[a][windowCSP[a].size() - 1].GetPointClueValue() > windowCSP[b][windowCSP[b].size() - 1].GetPointClueValue())
						{

							longerEquationSize = windowCSP[a].size() - 1;
							shorterEquationSize = windowCSP[b].size() - 1;
							longerEquation = a;
							shorterEquation = b;
						}


						else
						{
							longerEquationSize = windowCSP[b].size() - 1;
							shorterEquationSize = windowCSP[a].size() - 1;
							longerEquation = b;
							shorterEquation = a;
						}

						int totalCount = 0;
						int diffenceOfValues = 0;
					
						for (int c = 0; c < longerEquationSize; c++)
						{
							for (int d = 0; d < shorterEquationSize; d++)
							{
								//if points are the same
								if (windowCSP[longerEquation][c].ReturnY() == windowCSP[shorterEquation][d].ReturnY() && windowCSP[longerEquation][c].ReturnX() == windowCSP[shorterEquation][d].ReturnX())
									windowCSP[longerEquation][c].SetIsHiddenNeighbor(0);
							}
							
							totalCount += windowCSP[longerEquation][c].GetIsHiddenNeighbor();

						}
						//cout << "----------------total count" << totalCount << endl;
						diffenceOfValues=abs(windowCSP[a][windowCSP[a].size() - 1].GetPointClueValue() - windowCSP[b][windowCSP[b].size() - 1].GetPointClueValue());
						
						//if totacount == difference of values then that cell is definetly a mine
						if (totalCount == diffenceOfValues && diffenceOfValues > 0)
						{
							for (int d = 0; d < longerEquationSize; d++)
							{
								//if point has hidden neighbor
								if (windowCSP[longerEquation][d].GetIsHiddenNeighbor() == 1)
								{
									cout << "-------------------------point " << windowCSP[longerEquation][d].ReturnY() << " " << windowCSP[longerEquation][d].ReturnX() << " flagged as mine by csp" << endl;
								}
								
							}

						}

						//copying back the original equation
						windowCSP[a] = firstEquation;
						windowCSP[b] = secondEquation;

					}

				}
			
			}

		}

	}
	cout << "number of flagged mines- " << flaggedMines << endl;
	cout << "opened cells till now- " << b1.GetCountUncovered() << endl;



	b1.PrintUserBoard();




	return 0;

}

