//
//  board_formation.h
//  AI_assignment_2
//
//

#include "point.h"
#include "board_formation.h"

#include <iostream>

int main()
{
    int d, n;
    vector<Point> neighbors;
    int flaggedMines = 0;
    int numberOfMinesExploded = 0;             //number of exploded mines

    cout << "Enter the dimension:";
    cin >> d;
    cout << "Enter the number of mines:";
    cin >> n;
    
    Board b1(d, n);
    b1.PrintActualBoard();
    b1.PrintUserBoard();
    
    int number_of_steps = 0;

    
    srand(23);                      //if the seed value is time(0) it reveals all the mines first!!

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
        
        b1.OpenCell(p1, p2);                                         //generated a random point and opened it

        if (b1.GetValue(p1, p2) == 10)
        {
            numberOfMinesExploded += 1;
        }
        b1.PrintUserBoard();
        
        //agent function
        Point currentPoint(p1, p2);
        
        queue<Point> unexploredNeighborCells;

        vector<Point> inference_queue;                 //queue for doing infering process again and again after each point evaluation
        
        unexploredNeighborCells.push(currentPoint);
        
        
        while (!unexploredNeighborCells.empty())
        {
            
            
            int numberOfRevealedMines = 0;              //this is to the particular current point
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
                    if (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 10)
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
                        //flagging the mines
                        b1.SetFlag(neighbors[i].ReturnY(), neighbors[i].ReturnX());
                        flaggedMines += 1;
                        unexploredNeighborCells.push(Point(neighbors[i].ReturnY(), neighbors[i].ReturnX()));
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
                    }
                }
            }


        }
        
        //**********************************************************************************INFERENCE CALCULATION*******************************************************************************************************************
        
        for(int i=0; i<inference_queue.size();i++)
        {
            Point inference_point = inference_queue[i];
            totalNumberOfMines = b1.GetValue(inference_point.ReturnY(), inference_point.ReturnX());
            neighbors = b1.GetNeighbors(inference_point.ReturnY(), inference_point.ReturnX());
            
            int numberOfRevealedMines_inference = 0;              //this is to the particular current point
            int numberOfRevealedSafe_inference = 0;
            int numberOfHiddenNeighbors_inference = 0;
            
            if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
                numberOfHiddenNeighbors_inference += 1;


            if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == true)
            {
                //updating number of revealed mines
                if (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == 10)
                    numberOfRevealedMines_inference += 1;

                //updating number of revealed safe neighbors
                if ((b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) >= 0) && (b1.GetValue(neighbors[i].ReturnY(), neighbors[i].ReturnX()) <= 8))
                    numberOfRevealedSafe_inference += 1;
            }
            
            //checking rule 1
            if (totalNumberOfMines - numberOfRevealedMines_inference == numberOfHiddenNeighbors_inference)
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
            if ((b1.GetNeighborCount(currentPoint.ReturnY(), currentPoint.ReturnX()) - totalNumberOfMines) - numberOfRevealedSafe_inference == numberOfHiddenNeighbors_inference)
            {
                for (int i = 0; i < neighbors.size(); i++)
                {
                    if (b1.IsVisited(neighbors[i].ReturnY(), neighbors[i].ReturnX()) == false)
                    {
                        b1.OpenCell(neighbors[i].ReturnY(), neighbors[i].ReturnX());
                    }
                }
            }
            
        }
        
        //**********************************************************************************INFERNECE CALCULATION*********************************************************************************************************************
        inference_queue.push_back(currentPoint);
        
        
        number_of_steps = number_of_steps + 1;
        
        
        cout << "number of flagged mines: " << flaggedMines<< endl;
        cout << "opened cells till now: " << b1.GetCountUncovered() << endl;
        
    }
    
    b1.PrintUserBoard();
    
    if(flaggedMines == n)
    {
        cout << "You won! You successfully guessed all the mines."<<endl;
    }
    
    cout << "The number of steps done:"<<number_of_steps<<endl;
    return 0;

}


