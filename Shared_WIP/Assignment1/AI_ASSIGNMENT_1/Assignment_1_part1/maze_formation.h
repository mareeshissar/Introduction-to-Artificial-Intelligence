//
//  maze_fromation.h
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/19/19.
//  Copyright © 2019 Kartik Rattan. All rights reserved.
//

#ifndef maze_fromation_h
#define maze_fromation_h
#include <vector>
using namespace std;

class maze
{
public:
    vector<vector<int>> maze_array;
    maze(int d, double prob)
    {
        dim = d;
        maze_array.resize(dim);
        for(int i=0;i<dim;i++)                  //resize the maze vector
        {
            maze_array[i].resize(dim);
        }
        prob_obstacles = prob;
        for(int i=0;i<dim;i++)                  //initiate the whole maze as valid
        {
            for(int j=0;j<dim;j++)
            {
                maze_array[i][j] = 1;
            }
        }
        const int num_obst = (dim*dim)*(prob_obstacles);
        cout << "The number of obstacles:"<<num_obst<<endl;
        srand(time(0));
        bool check_maze[dim][dim];
        for(int i=0;i<dim;i++)                  //initiate the whole maze as valid for a potential obstacle
        {
            for(int j=0;j<dim;j++)
            {
                check_maze[i][j] = false;
            }
        }
        for(int i=0;i<num_obst;i++)         //assign a random number for the obstacle
        {
            int p1;
            int p2;
            do
            {
            p1 = 0 + rand()%(dim);
            p2 = 0 + rand()%(dim);
            }while(((p1==0 && p2==0) || (p1==dim-1 && p2==dim-1)) || check_maze[p1][p2]==true);
            //cout<<i+1<<". "<<"("<<p1<<","<<p2<<")"<<endl;               //printing the obstacles
            check_maze[p1][p2] = true;
            maze_array[p1][p2] = 0;
        }
    }
    vector<vector<int>> return_maze()
    {
        return maze_array;
    }
    void print_maze()
    {
        cout <<"The maze is :"<<endl;
        for(int i=0; i<dim; i++)
        {
            for(int j=0;j<dim;j++)
            {
                if(maze_array[i][j] == INT_MAX)
                    cout<<"*"<<" ";
                else if(maze_array[i][j]==0)
                    cout<<"X"<<" ";
                else
                    cout << maze_array[i][j] << " ";
            }
            cout << endl;
        }
        cout << "1: Free cell" << endl << "X: Obstacle"<<endl;
        
    }
private:
    int dim;               //the dimensions in the maze
    double prob_obstacles;   //the probability of number of obstacles
};

#endif /* maze_fromation_h */

