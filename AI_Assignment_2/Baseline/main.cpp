//
//  board_formation.h
//  AI_assignment_2
//
//

#include "point.h"
#include "board_formation.h"

#include <iostream>
void print_queue(vector<Point> a);


int main()
{
    int d;
    int n;
    
    cout << "Enter the dimension:";
    cin >> d;
    cout << "Enter the number of mines:";
    cin >> n;
    
    int total_number_flagged_mines =0;
    
    int iter_simulate = 10;
    int divide = iter_simulate;
    while(iter_simulate > 0)
    {
        iter_simulate = iter_simulate - 1;
        
        vector<Point> neighbors;
        
        int flaggedMines = 0;
        int numberOfMinesExploded = 0;             //number of exploded mines
         
        Board b1(d,n);
    //    cout << "The original board is:"<<endl;
    //    b1.PrintActualBoard();
    //    b1.PrintUserBoard();
    //
        
        srand(23);                      //if the seed value is time(0) it reveals all the mines first!!
        
        vector<Point> inference_queue;                 //queue for doing infering process again and again after each point evaluation
        
        while(b1.GetCountUncovered()!= d*d)
        {
            
            int p1;
            int p2;
            int totalNumberOfMines = 0;
            
            //****************************************GET A RANDOM POINT***********************************************************************************
            do
            {
                p1 = 0 + rand() % (d);
                p2 = 0 + rand() % (d);
            }
            while (b1.IsVisited(p1, p2) == true);
            b1.OpenCell(p1, p2);//generated a random point and opened it
            // ********************************************************************************************************************************************
            

            if (b1.GetValue(p1, p2) == 10)
            {
                //cout << "You opened a mine!";
                numberOfMinesExploded += 1;
            }
            
            Point currentPoint(p1, p2);
            queue<Point> unexploredNeighborCells;
            unexploredNeighborCells.push(currentPoint);
            
            Point add_point(p1,p2);
            if(b1.GetValue(add_point.ReturnY(), add_point.ReturnX()) != 10 && b1.GetValue(add_point.ReturnY(), add_point.ReturnX()) != 20)
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
    //        cout<< "After 2 rules the user board:"<<endl;
    //        b1.PrintUserBoard();
    //        cout << "The points in the inference queue is:"<<endl;
    //        print_queue(inference_queue);

            
    //*******************************************************************INFERENCE CALCULATION**********************************************
            
            for(int i=0; i<inference_queue.size();i++)
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
            
//**************************************************************INFERNECECALCULATION*************************************************************
    //        cout << "number of flagged mines: " << flaggedMines<< endl;
    //        cout << "opened cells till now: " << b1.GetCountUncovered() << endl;
        }
        
        //cout<< "Final glimpse at the user board:"<<endl;
        //b1.PrintUserBoard();
        
       
        
        if(flaggedMines == n)
        {
            cout << "You won! You successfully guessed all the mines."<<endl;
        }
        
        cout << "Total number of mines:"<<n<< endl;
        cout << "Number of mines exploded by you:"<<numberOfMinesExploded << endl;
        cout << "Number of flagged Mines:" << flaggedMines << endl;
        
        total_number_flagged_mines = total_number_flagged_mines + flaggedMines;
//**************************************************************END*************************************************************

    }
    
    cout << "Avg number of flagged mines for "<< d<<"X"<<d<<" minesweeper is:";
    cout << static_cast<double>(total_number_flagged_mines / static_cast<double>(n*divide)) << endl;
    
    return 0;

}



void print_queue(vector<Point> a)
{
    for(int i=0; i<a.size(); i++)
    {
        cout<<i+1<<". ";
        a[i].print();
        cout << endl;
        
    }
}


