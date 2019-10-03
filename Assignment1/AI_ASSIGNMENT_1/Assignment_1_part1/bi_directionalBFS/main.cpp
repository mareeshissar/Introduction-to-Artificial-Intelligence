//
//  main.cpp
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/19/19.
//  Copyright © 2019 Kartik Rattan. All rights reserved.
//

#include <iostream>
#include "maze_formation.h"
#include "point.h"
using namespace std;

#include <queue>

int dimension;        //global variables so that they can be used in BFS directly
double probability;

bool BFS(maze &, point, point, vector<vector<point>> &);
void return_children(point, vector<point> &);

int main(int argc, const char * argv[]) {
    
    cout << "Enter the dimension (dim) of maze (dim X dim):";
    cin >>dimension;
    cout << "Enter the probability of obstacles associated with the maze:";
    cin >>probability;
   
    maze new_maze(dimension,probability);
    new_maze.print_maze();
    
    vector<vector<point>> prev;         //define a prev array for noting the path
    prev.resize(dimension);
    for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
    {
        prev[i].resize(dimension);
    }
    
    
    point start(0,0);                       //start point
    point goal(dimension-1,dimension-1);    //goal point
    
    
    bool is_there_path_bidirectional_bfs = BFS(new_maze, start, goal, prev);          //run BFS
    
    if(is_there_path_bidirectional_bfs)                            //if a path exists, return the path
    {
        cout<<"The path exist!"<<endl;
    }
    else                                    //path does not exists.
    {
        cout<<"There is no path"<<endl;
    }
    
    vector<point> fire_points;
    point fire_start(0,dimension-1);
    fire_points.push_back(fire_start);
    
    return 0;
}

bool BFS(maze &new_maze, point initial_state, point goal_state,vector<vector<point>> &prev)
{
    queue<point> fringe1;
    queue<point> fringe2;
    /////////////////CLOSED_SET_2/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool closed_set1[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unvisited
    {
        for(int j=0;j<dimension;j++)
        {
            closed_set1[i][j] = false;           //nothing is visited till now
        }
    }
    bool closed_set2[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unvisited
    {
        for(int j=0;j<dimension;j++)
        {
            closed_set2[i][j] = false;           //nothing is visited till now
        }
    }
    /////////////////fringe_added_2/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool fringe_added1[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unadded
    {
        for(int j=0;j<dimension;j++)
        {
            fringe_added1[i][j] = false;           //nothing is added till now
        }
    }
    bool fringe_added2[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unadded
    {
        for(int j=0;j<dimension;j++)
        {
            fringe_added2[i][j] = false;           //nothing is added till now
        }
    }
    /////////////////fringe_added_2/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fringe1.push(initial_state);
    fringe2.push(goal_state);
    while(!fringe1.empty() || !fringe2.empty())
    {
        {
            point current_state = fringe1.front();
            fringe1.pop();
            if(closed_set1[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
            {
               closed_set1[current_state.return_y()][current_state.return_x()] = true;
               if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
               {
                   cout<<"This should never happen"<<endl;
                   return true;
               }
               else
               {
                   vector<point> children;
                   return_children(current_state,children);    //find the accessible states from the current state
                   
                   for(int i=0;i<children.size();i++)
                   {
                       int y_cord = children[i].return_y();
                       int x_cord = children[i].return_x();
                       if(x_cord<0 || y_cord<0 || x_cord>dimension-1 || y_cord>dimension-1)
                           continue;
                       if((new_maze.return_maze())[y_cord][x_cord]==0 || closed_set1[y_cord][x_cord]==true) //restricted_state
                           continue;
                       if(fringe_added1[children[i].return_y()][children[i].return_x()]==false)
                       {
                           fringe_added1[children[i].return_y()][children[i].return_x()]=true;
                           fringe1.push(children[i]);
                           prev[children[i].return_y()][children[i].return_x()] = current_state;
                       }
                    }
                }
            }
        }
        {
            point current_state = fringe2.front();
            fringe2.pop();
            if(closed_set2[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
            {
               closed_set2[current_state.return_y()][current_state.return_x()] = true;
               if(current_state.return_x() == initial_state.return_x() && initial_state.return_y() == goal_state.return_y())
               {
                   cout<<"This should never happen part2"<<endl;
                   return true;
               }
               else
               {
                   vector<point> children;
                   return_children(current_state,children);    //find the accessible states from the current state
                   
                   for(int i=0;i<children.size();i++)
                   {
                       int y_cord = children[i].return_y();
                       int x_cord = children[i].return_x();
                       if(x_cord<0 || y_cord<0 || x_cord>dimension-1 || y_cord>dimension-1)
                           continue;
                       if((new_maze.return_maze())[y_cord][x_cord]==0 || closed_set2[y_cord][x_cord]==true) //restricted_state
                           continue;
                       if(fringe_added2[children[i].return_y()][children[i].return_x()]==false)
                       {
                           fringe_added2[children[i].return_y()][children[i].return_x()]=true;
                           fringe2.push(children[i]);
                           prev[children[i].return_y()][children[i].return_x()] = current_state;
                       }
                    }
                }
            }
        }
        ////                         check if you have same neigbours         ////////////////////////////////////////
        for(int i=0; i<dimension; i++)
        {
            for(int j=0;j<dimension; j++)
            {
                if(fringe_added1[i][j] == true && fringe_added2[i][j] == true)
                {
                    
                    cout<<"You got to the path early! You reached the point ("<<i<<","<<j<<") from both sides."<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

void return_children(point a, vector<point> &children)
{
    
    point c1(a.return_y()+1,a.return_x()+0);
    point c2(a.return_y()+0,a.return_x()+1);
    point c3(a.return_y()+0,a.return_x()-1);
    point c4(a.return_y()-1,a.return_x()+0);
    children.push_back(c1);
    children.push_back(c2);
    children.push_back(c3);
    children.push_back(c4);
}
