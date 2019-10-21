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
#include <vector>
void print_queue(vector<Point> a);

int main()
{ 
	for (int iter = 0; iter < 1; iter++)
	{ 
		int baseline = 0;
		int CSP = 0;
		int d=100, n=1000;
		vector<Point> neighbors;
		int flaggedMines = 0;
		int flaggedCSPMines = 0;
		int numberOfMinesExploded = 0;			 //number of exploded mines
		int i = 0;

		vector<Point> neighboursWithClue;		//vector for storing cells that have clue
		vector<Point> neighboursCSP;			//vector for storing individual csp equation
		vector<vector<Point>> windowCSP;		//vector of vector for storing all the csp equations

		/*cout << "Enter the dimension:";
		cin >> d;
		cout << "Enter the number of mines:";
		cin >> n;*/
		Board b1(d, n);
		//b1.PrintActualBoard();
		//b1.PrintUserBoard();

		//if the seed value is time(0) it reveals all the mines first!!
		srand(23);


		vector<Point> inference_queue;                 //queue for doing infering process again and again after each point evaluation
		while (b1.GetCountUncovered() != d * d)
		{

			int p1;
			int p2;
			int totalNumberOfMines = 0;

			//****************************************GET A RANDOM POINT********************************************************************************************************
			do
			{
				p1 = 0 + rand() % (d);
				p2 = 0 + rand() % (d);
			} while (b1.IsVisited(p1, p2) == true);
			//cout << "You opened a RANDOM POINT: (" << p1 << "," << p2 << ")" << endl;               //printing the open position to swap
			// ********************************************************************************************************************************************************************

			b1.OpenCell(p1, p2);//generated a random point and opened it

			if (b1.GetValue(p1, p2) == 10)
			{
				cout << "You opened a mine!"<<endl;
				numberOfMinesExploded += 1;
			}

			Point currentPoint(p1, p2);
			queue<Point> unexploredNeighborCells;
			unexploredNeighborCells.push(currentPoint);

			Point add_point(p1, p2);
			if (b1.GetValue(add_point.ReturnY(), add_point.ReturnX()) != 10 && b1.GetValue(add_point.ReturnY(), add_point.ReturnX()) != 20)
				inference_queue.push_back(add_point);

			while (!unexploredNeighborCells.empty())
			{
				int numberOfRevealedMines = 0;              //this is to the particular current point
				int numberOfRevealedSafe = 0;
				int numberOfHiddenNeighbors = 0;

				currentPoint = unexploredNeighborCells.front();
				unexploredNeighborCells.pop();

				totalNumberOfMines = b1.GetValue(currentPoint.ReturnY(), currentPoint.ReturnX());
				neighbors = b1.GetNeighbors(currentPoint.ReturnY(), currentPoint.ReturnX());


				for (int i = 0; i < neighbors.size(); i++)
				{
					//cout << "neighbor, x=" << neighbors[i].ReturnY() << " , y=" << neighbors[i].ReturnX() << endl;

					//updating number of hidden neighbors

					if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
					{
						numberOfHiddenNeighbors += 1;
					}

					if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == true)
					{
						//updating number of revealed mines. This should include both the flagged and the flagged mine.
						if (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 10 || b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 20)
							numberOfRevealedMines += 1;

						//updating number of revealed safe neighbors
						if ((b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) >= 0) && (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) <= 8))
							numberOfRevealedSafe += 1;
					}
				}

				//checking rule 1
				if (totalNumberOfMines - numberOfRevealedMines == numberOfHiddenNeighbors)
				{
					for (int i = 0; i < neighbors.size(); i++)
					{
						if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
						{
							b1.SetFlag(neighbors[i].ReturnY(), neighbors[i].ReturnX());                                 //flagging the mines
							flaggedMines += 1;
							++baseline;
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
							unexploredNeighborCells.push(Point(neighbors[i].ReturnY(), neighbors[i].ReturnX()));
							inference_queue.push_back(neighbors[i]);                                                            //adding safe points to the queue.
						}
					}
				}

			}
			 //       cout<< "After 2 rules the user board:"<<endl;
			 //       b1.PrintUserBoard();
			//        cout << "The points in the inference queue is:"<<endl;
			//        print_queue(inference_queue);


			//*******************************************************************INFERENCE CALCULATION**********************************************

			for (int i = 0; i < inference_queue.size(); i++)
			{
				Point inference_point = inference_queue[i];

				int totalNumberOfMines_infernece = b1.GetValue(inference_point.ReturnY(), inference_point.ReturnX());

				vector<Point> neighbors_inference = b1.GetNeighbors(inference_point.ReturnY(), inference_point.ReturnX());

				int numberOfRevealedMines_inference = 0;              //this is to the particular current point
				int numberOfRevealedSafe_inference = 0;
				int numberOfHiddenNeighbors_inference = 0;

				for (int i = 0; i < neighbors_inference.size(); i++)
				{
					if (b1.IsVisited(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == false)
						numberOfHiddenNeighbors_inference += 1;


					if (b1.IsVisited(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == true)
					{
						//updating number of revealed mines
						if (b1.GetValue(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == 10 || b1.GetValue(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == 20)
							numberOfRevealedMines_inference += 1;

						//updating number of revealed safe neighbors
						if ((b1.GetValue(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) >= 0) && (b1.GetValue(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) <= 8))
							numberOfRevealedSafe_inference += 1;
					}
				}

				//checking rule 1
				if (totalNumberOfMines_infernece - numberOfRevealedMines_inference == numberOfHiddenNeighbors_inference)
				{
					for (int i = 0; i < neighbors_inference.size(); i++)
					{
						if (b1.IsVisited(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == false)
						{
							//flagging the mines
							b1.SetFlag(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX());
							flaggedMines += 1;
							baseline++;
						}
					}
				}

				//checking rule 2
				if ((b1.GetNeighborCount(inference_point.ReturnY(), inference_point.ReturnX()) - totalNumberOfMines_infernece - numberOfRevealedSafe_inference) == numberOfHiddenNeighbors_inference)
				{
					for (int i = 0; i < neighbors_inference.size(); i++)
					{
						if (b1.IsVisited(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX()) == false)
						{
							b1.OpenCell(neighbors_inference[i].ReturnY(), neighbors_inference[i].ReturnX());
							inference_queue.push_back(neighbors_inference[i]);
						}
					}
				}
				//cout << "After inference the user board from the point ";
				//inference_point.print();
				//cout <<" : "<<endl;
				//b1.PrintUserBoard();
			}

		
			//**********************************************************************************INFERNECECALCULATION*************************************************************

			cout << "number of flagged mines: " << flaggedMines << endl;
			cout << "opened cells till now: " << b1.GetCountUncovered() << endl;

			cout << "Printing user board after all the deductions are made using rule1 and rule 2" << endl;
			b1.PrintUserBoard();



			cout << "-----------------------------------------Now using CSP to infer more " << endl;
			//**********************************************************************************APPLYING CSP*************************************************************

			//check for early termination
			if (b1.GetCountUncovered() < d * d)
			{

				//agent function - CSP
				for (int i = 2; i < d - 2; i++)
				{
					for (int j = 2; j < d - 2; j++)
					{
						neighboursWithClue.clear();
						windowCSP.clear();
						//cout << "for central cell " << i << " , " << j << endl;

						//get all the neighbours of the central cell that have clue values
						neighboursWithClue = b1.GetNeighborsWithClues(i, j);


						for (int k = 0; k < neighboursWithClue.size(); k++)
						{
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


							neighboursCSP = b1.GetNeighborsCSP(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX(), i, j);

							if (neighboursCSP.size() != 0)
								//cout << "no of hidden neighbors of " << neighboursWithClue[k].ReturnY() << " , " << neighboursWithClue[k].ReturnX() << "are " << neighboursCSP.size() << endl;

								if (neighboursCSP.size() != 0 && neighboursCSP.size() != (totalNumberOfMines - numberOfRevealedMines))
								{
									//adding point with clue value at the end
									Point temp(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX());
									//reduce the number of mines from clue value
									temp.SetPointClueValue(b1.GetValue(neighboursWithClue[k].ReturnY(), neighboursWithClue[k].ReturnX()) - numberOfRevealedMines);
									neighboursCSP.push_back(temp);

									windowCSP.push_back(neighboursCSP);
								}


						}


						/*cout << "displaying all the equations in a window" << endl;
						for (int a = 0; a < windowCSP.size(); a++)
						{
							for (int b = 0; b < windowCSP[a].size(); b++)
							{
								if (b == windowCSP[a].size() - 1)
									cout << windowCSP[a][b].GetPointClueValue() << "..." << endl;
								else
									cout << "hidden point is " << windowCSP[a][b].ReturnY() << " , " << windowCSP[a][b].ReturnX()  << endl;
							}
						}*/


						//applying CSP on a 5X5 board taking clues at the center 3X3
						int longerEquationSize;
						int shorterEquationSize;
						int longerEquation;
						int shorterEquation;


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

								if (windowCSP[a].size() >= windowCSP[b].size())
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

								int totalCountLongerEquation = 0;
								int totalCountShorterEquation = 0;
								int diffenceOfValues = 0;

								for (int c = 0; c < longerEquationSize; c++)
								{
									for (int d = 0; d < shorterEquationSize; d++)
									{
										totalCountShorterEquation = 0;
										//checking if points are the same
										if (windowCSP[longerEquation][c].ReturnY() == windowCSP[shorterEquation][d].ReturnY() && windowCSP[longerEquation][c].ReturnX() == windowCSP[shorterEquation][d].ReturnX())
										{
											windowCSP[longerEquation][c].SetIsHiddenNeighbor(0);
											windowCSP[shorterEquation][d].SetIsHiddenNeighbor(0);
										}
										totalCountShorterEquation += windowCSP[shorterEquation][d].GetIsHiddenNeighbor();

									}

									totalCountLongerEquation += windowCSP[longerEquation][c].GetIsHiddenNeighbor();

								}
								//cout << "----------------total count" << totalCount << endl;
								diffenceOfValues = abs(windowCSP[a][windowCSP[a].size() - 1].GetPointClueValue() - windowCSP[b][windowCSP[b].size() - 1].GetPointClueValue());

								//if totacount == difference of values then that cell is definetly a mine and the shorter equation is a subset of the longer equation
								if (totalCountLongerEquation == diffenceOfValues && diffenceOfValues > 0 && totalCountShorterEquation == 0)
								{
									for (int d = 0; d < longerEquationSize; d++)
									{
										//if point has hidden neighbor
										if (windowCSP[longerEquation][d].GetIsHiddenNeighbor() == 1 && b1.IsVisited(windowCSP[longerEquation][d].ReturnY(), windowCSP[longerEquation][d].ReturnX()) == false)
										{
											cout << "-------------------------point " << windowCSP[longerEquation][d].ReturnY() << " " << windowCSP[longerEquation][d].ReturnX() << " flagged as mine by csp" << endl;
											//flagging the mines
											b1.SetFlag(windowCSP[longerEquation][d].ReturnY(), windowCSP[longerEquation][d].ReturnX());

											flaggedMines += 1;
											flaggedCSPMines += 1;
											++CSP;
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
			else
				break;
			cout << "number of flagged mines after csp- " << flaggedMines << endl;
			cout << "opened cells till now after csp- " << b1.GetCountUncovered() << endl;


			//cout << "Printing user board after all the deductions are made using CSP" << endl;
			//b1.PrintUserBoard();

		}
		cout << "number of flagged mines- " << flaggedMines << endl;
		cout << "number of flagged CSP mines- " << flaggedCSPMines << endl;
		cout << "opened cells till now- " << b1.GetCountUncovered() << endl;
		cout << "number of flagged mines using baseline- " << baseline << endl;
		cout << "number of flagged mines using CSP- " << CSP << endl;


		b1.PrintUserBoard();


	}
	


	return 0;

}

void print_queue(vector<Point> a)
{
	for (int i = 0; i < a.size(); i++)
	{
		cout << i + 1 << ". ";
		cout<< "(" << a[i].ReturnY()<<" , "<<a[i].ReturnX()<< ")" << endl;
		cout << endl;

	}
}