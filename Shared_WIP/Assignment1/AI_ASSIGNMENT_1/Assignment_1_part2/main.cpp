//
//  main.cpp
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/19/19.
//  Copyright © 2019 Kartik Rattan. All rights reserved.
//

#include <iostream>
#include "maze_fromation.h"
#include "point.h"
using namespace std;

#include <queue>
#include <stack>
#include <cmath>

int dimension;        //global variables so that they can be used in BFS directly
double probability;


bool BFS(maze &, point, point, vector<vector<point>> &);
bool A_star(maze &, point, point,vector<vector<point>> &);
void return_children(point a, vector<point> &);
double return_heuristic(point,point);
double return_heuristic_euclidean(point, point);
bool DFS(maze&, point, point, vector<vector<point>>&);
bool A_star_euclidean(maze &, point, point ,vector<vector<point>> &);


#include <chrono>
using namespace std::chrono;


int main(int argc, const char * argv[]) {
    double n_t = 20;
    double iter = n_t;
    
    double n_true_bfs = 0;
    double n_true_A_star =0;
    double n_true_dfs = 0;
    double n_true_A_star_euclidean =0;
    
    cout << "Enter the dimension (dim) of maze (dim X dim):";
    cin >>dimension;
    cout << "Enter the probability of obstacles associated with the maze:";
    cin >>probability;
    
    auto avg_duration_bfs = std::chrono::microseconds(0);
    auto avg_duration_astar = std::chrono::microseconds(0);
    auto avg_duration_dfs = std::chrono::microseconds(0);
    auto avg_duration_astar_euclidean = std::chrono::microseconds(0);
    
    while(iter > 0)
    {
        maze new_maze(dimension,probability);
        //new_maze.print_maze();
        
        vector<vector<point>> prev1;         //define a prev array for noting the path
        prev1.resize(dimension);
        for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
        {
            prev1[i].resize(dimension);
        }
        
        vector<vector<point>> prev2;         //define a prev array for noting the path
        prev2.resize(dimension);
        for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
        {
            prev2[i].resize(dimension);
        }
        
        vector<vector<point>> prev3;         //define a prev array for noting the path
        prev3.resize(dimension);
        for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
        {
            prev3[i].resize(dimension);
        }
        
        vector<vector<point>> prev4;         //define a prev array for noting the path
        prev4.resize(dimension);
        for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
        {
            prev4[i].resize(dimension);
        }
        
        point start(0,0);                       //start point
        point goal(dimension-1,dimension-1);    //goal point
        
        auto start_bfs = high_resolution_clock::now();
        bool is_there_path_bfs = BFS(new_maze, start, goal, prev1);          //run BFS
        auto end_bfs = high_resolution_clock::now();
        
        auto start_astar = high_resolution_clock::now();
        bool is_there_path_a_star = A_star(new_maze, start, goal, prev2);
        auto end_astar = high_resolution_clock::now();
        
        auto start_dfs = high_resolution_clock::now();
        bool is_there_path_dfs = DFS(new_maze, start, goal, prev1);          //run DFS
        auto end_dfs = high_resolution_clock::now();
        
        auto start_astar_euclidean = high_resolution_clock::now();
        bool is_there_path_a_star_euclidean = A_star_euclidean(new_maze, start, goal, prev2);
        auto end_astar_euclidean = high_resolution_clock::now();

        auto duration_bfs = duration_cast<microseconds>(end_bfs-start_bfs);
        auto duration_astar = duration_cast<microseconds>(end_astar-start_astar);
        auto duration_dfs = duration_cast<microseconds>(end_dfs-start_dfs);
        auto duration_astar_euclidean = duration_cast<microseconds>(end_astar_euclidean-start_astar_euclidean);
        
        avg_duration_bfs = avg_duration_bfs + duration_bfs;
        avg_duration_astar = avg_duration_astar + duration_astar;
        avg_duration_dfs = avg_duration_dfs + duration_dfs;
        avg_duration_astar_euclidean = avg_duration_astar_euclidean + duration_astar_euclidean;
        
        if(is_there_path_bfs)
            n_true_bfs+=1;
        if(is_there_path_a_star)
            n_true_A_star+=1;
        if(is_there_path_dfs)
            n_true_dfs+=1;
        if(is_there_path_a_star_euclidean)
            n_true_A_star_euclidean+=1;
        
        iter-=1;
    }
    cout<<"For dimension " << dimension << "X"<< dimension<< " and obstacles "<< (dimension*dimension)*probability<<" , the probability for success for algorithms is:"<<endl;
    cout<<"BFS:"<<(n_true_bfs/n_t)<<endl;
    cout<<"DFS:"<<(n_true_dfs/n_t)<<endl;
    cout<<"A_Star_manhattan:"<<(n_true_A_star/n_t)<<endl;
    cout<<"A_Star_euclidean:"<<(n_true_A_star_euclidean/n_t)<<endl;
    
    cout<<"Avg time taken to complete the algorithms is:"<<endl;
    cout<<"BFS:"<<endl<<"Total time:"<<avg_duration_bfs.count()/1000<<" milliseconds"<<endl<<"Avg time:"<<avg_duration_bfs.count()/(1000 * 20)<<" milliseconds"<<endl;
    cout<<"DFS:"<<endl<<"Total time:"<<avg_duration_dfs.count()/1000<<" milliseconds"<<endl<<"Avg time:"<<avg_duration_dfs.count()/(1000 * 20)<<" milliseconds"<<endl;
    cout<<"A_Star_manhattan:"<<endl<<"Total time:"<<avg_duration_astar.count()/1000<<" milliseconds"<<endl<<"Avg. time:"<<avg_duration_astar.count()/(1000 * 20)<<" milliseconds"<<endl;
    cout<<"A_Star_euclidean:"<<endl<<"Total time:"<<avg_duration_astar_euclidean.count()/1000<<" milliseconds"<<endl<<"Avg. time:"<<avg_duration_astar_euclidean.count()/(1000 * 20)<<" milliseconds"<<endl;



    /*
    if(is_there_path_bfs)                           //if a path exists, return the path
    {
        cout<<"The path using BFS algorithm is:"<<endl;
        goal.print_point();
        num_steps_using_BFS+=1;
        cout<<"==>";
        point temp = prev1[goal.return_y()][goal.return_x()];
        while( (temp.return_y()!=start.return_y()) || (temp.return_x()!=start.return_x()) )
        {
            temp.print_point();
            cout << "==>";
            temp = prev1[temp.return_y()][temp.return_x()];
            num_steps_using_BFS+=1;
        }
        temp.print_point();
        cout<<endl;
        cout<<"The number of steps to run BFS: "<<num_steps_using_BFS<<endl;
    }
    else                                    //path does not exists.
    {
        cout<<"There is no path using BFS."<<endl;
    }
    if(is_there_path_a_star)                           //if a path exists, return the path
    {
        cout<<"The path using A_STAR algorithm is:"<<endl;
        goal.print_point();
        num_steps_using_A_STAR+=1;
        cout<<"==>";
        point temp = prev2[goal.return_y()][goal.return_x()];
        while( (temp.return_y()!=start.return_y()) || (temp.return_x()!=start.return_x()) )
        {
            temp.print_point();
            cout << "==>";
            temp = prev2[temp.return_y()][temp.return_x()];
            num_steps_using_A_STAR+=1;
        }
        temp.print_point();
        cout<<endl;
        cout<<"The number of steps to run A STAR: "<<num_steps_using_A_STAR<<endl;
    }
    else                                    //path does not exists.
    {
        cout<<"There is no path using A_STAR."<<endl;
    }
    */
    return 0;
}

bool BFS(maze &new_maze, point initial_state, point goal_state,vector<vector<point>> &prev)
{
    queue<point> fringe;
    bool closed_set[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unvisited
    {
        for(int j=0;j<dimension;j++)
        {
            closed_set[i][j] = false;           //nothing is visited till now
        }
    }
    bool fringe_added[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unadded
    {
        for(int j=0;j<dimension;j++)
        {
            fringe_added[i][j] = false;           //nothing is addedtill now
        }
    }
    fringe.push(initial_state);
    while(!fringe.empty())
    {
        point current_state = fringe.front();
        fringe.pop();
        if(closed_set[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true;
            if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                int number_states_visited =0;
                for(int i=0;i<dimension;i++)
                    for(int j=0;j<dimension;j++)
                    {
                        if(closed_set[i][j]==1)
                            number_states_visited+=1;
                    }
                cout<<"Total number of states visited by BFS: "<<number_states_visited<<endl;
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
                    if((new_maze.return_maze())[y_cord][x_cord]==0 || closed_set[y_cord][x_cord]==true) //restricted_state
                        continue;
                    if(fringe_added[children[i].return_y()][children[i].return_x()]==false)
                        {
                            fringe_added[children[i].return_y()][children[i].return_x()]=true;
                            fringe.push(children[i]);
                            prev[children[i].return_y()][children[i].return_x()] = current_state;
                        }
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

bool A_star(maze &new_maze, point initial_state, point goal_state,vector<vector<point>> &prev)
{
    priority_queue<point> fringe;
    bool closed_set[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unvisited
    {
        for(int j=0;j<dimension;j++)
        {
            closed_set[i][j] = false;           //nothing is visited till now
        }
    }
    bool fringe_added[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unadded
    {
        for(int j=0;j<dimension;j++)
        {
            fringe_added[i][j] = false;           //nothing is addedtill now
        }
    }
    
    double distance = 0;
    double f_n = distance + return_heuristic(initial_state,goal_state);
    initial_state.priority = f_n;
    fringe.push(initial_state);
    while(!fringe.empty())
    {
        point current_state = fringe.top();
        fringe.pop();
        
        if(closed_set[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true; //mark that you have visited it
            if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                int number_states_visited =0;
                for(int i=0;i<dimension;i++)
                    for(int j=0;j<dimension;j++)
                    {
                        if(closed_set[i][j]==1)
                            number_states_visited+=1;
                    }
                cout<<"Total number of states visited by A_star: "<<number_states_visited<<endl;
                return true;
            }
            else
            {
                distance = distance + 1;            //increase the distance from the starting point
                vector<point> children;
                return_children(current_state,children);    //find the accessible states from the current state
                
                for(int i=0;i<children.size();i++)
                {
                    int y_cord = children[i].return_y();
                    int x_cord = children[i].return_x();
                    
                    if(x_cord<0 || y_cord<0 || x_cord>dimension-1 || y_cord>dimension-1)
                        continue;
                    if((new_maze.return_maze())[y_cord][x_cord]==0 || closed_set[y_cord][x_cord]==true) //restricted_state
                        continue;
                    if(fringe_added[children[i].return_y()][children[i].return_x()]==false)
                        {
                            children[i].priority = distance + return_heuristic(children[i], goal_state);
                            fringe_added[children[i].return_y()][children[i].return_x()]=true;
                            fringe.push(children[i]);
                            prev[children[i].return_y()][children[i].return_x()] = current_state;
                        }
                    }
                }
            }
        }
    return false;

}

bool DFS(maze& new_maze, point initial_state, point goal_state, vector<vector<point>>& prev)
{
    stack<point> fringe;
    bool closed_set[dimension][dimension];
    for (int i = 0; i < dimension; i++)                //make all the states as unvisited
    {
        for (int j = 0; j < dimension; j++)
        {
            closed_set[i][j] = false;           //nothing is visited till now
        }
    }
    bool fringe_added[dimension][dimension];
    for (int i = 0; i < dimension; i++)                //make all the states as unadded
    {
        for (int j = 0; j < dimension; j++)
        {
            fringe_added[i][j] = false;           //nothing is addedtill now
        }
    }
    fringe.push(initial_state);
    while (!fringe.empty())
    {
        point current_state = fringe.top();
        fringe.pop();
        if (closed_set[current_state.return_y()][current_state.return_x()] == false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true;

            if (current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
                {
                    int number_states_visited =0;
                    for(int i=0;i<dimension;i++)
                        for(int j=0;j<dimension;j++)
                        {
                            if(closed_set[i][j]==1)
                                number_states_visited+=1;
                        }
                    cout<<"Total number of states visited by DFS: "<<number_states_visited<<endl;
                    return true;
                }
            }
            else
            {
                vector<point> children;
                return_children(current_state, children);    //find the accessible states from the current state
                for (int i = 0; i < children.size(); i++)
                {
                    int y_cord = children[i].return_y();
                    int x_cord = children[i].return_x();

                    if (x_cord<0 || y_cord<0 || x_cord>dimension - 1 || y_cord>dimension - 1)
                        continue;
                    if ((new_maze.return_maze())[y_cord][x_cord] == 0 || closed_set[y_cord][x_cord] == true) //restricted_state
                        continue;
                    if (fringe_added[children[i].return_y()][children[i].return_x()] == false)
                    {
                        fringe_added[children[i].return_y()][children[i].return_x()] = true;
                        fringe.push(children[i]);
                        prev[children[i].return_y()][children[i].return_x()] = current_state;
                    }
                }
            }

        }
    }
    return false;
}


bool A_star_euclidean(maze &new_maze, point initial_state, point goal_state,vector<vector<point>> &prev)
{
    priority_queue<point> fringe;
    bool closed_set[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unvisited
    {
        for(int j=0;j<dimension;j++)
        {
            closed_set[i][j] = false;           //nothing is visited till now
        }
    }
    bool fringe_added[dimension][dimension];
    for(int i=0;i<dimension;i++)                //make all the states as unadded
    {
        for(int j=0;j<dimension;j++)
        {
            fringe_added[i][j] = false;           //nothing is addedtill now
        }
    }
    
    double distance = 0;
    double f_n = distance + return_heuristic_euclidean(initial_state,goal_state);
    initial_state.priority = f_n;
    fringe.push(initial_state);
    while(!fringe.empty())
    {
        point current_state = fringe.top();
        fringe.pop();
        
        if(closed_set[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true; //mark that you have visited it
            if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                int number_states_visited =0;
                for(int i=0;i<dimension;i++)
                    for(int j=0;j<dimension;j++)
                    {
                        if(closed_set[i][j]==1)
                            number_states_visited+=1;
                    }
                cout<<"Total number of states visited by A_star: "<<number_states_visited<<endl;
                return true;
            }
            else
            {
                distance = distance + 1;            //increase the distance from the starting point
                vector<point> children;
                return_children(current_state,children);    //find the accessible states from the current state
                
                for(int i=0;i<children.size();i++)
                {
                    int y_cord = children[i].return_y();
                    int x_cord = children[i].return_x();
                    
                    if(x_cord<0 || y_cord<0 || x_cord>dimension-1 || y_cord>dimension-1)
                        continue;
                    if((new_maze.return_maze())[y_cord][x_cord]==0 || closed_set[y_cord][x_cord]==true) //restricted_state
                        continue;
                    if(fringe_added[children[i].return_y()][children[i].return_x()]==false)
                        {
                            children[i].priority = distance + return_heuristic_euclidean(initial_state,goal_state);
                            fringe_added[children[i].return_y()][children[i].return_x()]=true;
                            fringe.push(children[i]);
                            prev[children[i].return_y()][children[i].return_x()] = current_state;
                        }
                    }
                }
            }
        }
    return false;

}






double return_heuristic(point p,point goal)
{
    double h_n = abs(p.return_x() - goal.return_x())  + abs(p.return_y() - goal.return_y());
    return h_n;
}


double return_heuristic_euclidean(point p, point goal)
{

    double h_n = sqrt(pow((p.return_x() - goal.return_x()), 2) + pow((p.return_y() - goal.return_y()), 2));
    return h_n;
}
