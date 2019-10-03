

#include <iostream>
#include <cmath>

#include "maze_formation.h"
#include "point.h"

using namespace std;

#include <queue>

bool A_star(maze &, point, point,vector<vector<point>> &, vector<point> &, vector<vector<bool>> &);
bool A_star_without_fire(maze &, point, point);
void return_children(point a, vector<point> &);
double return_heuristic(point,point, maze);
void update_maze_fire(maze &,vector<point> &, vector<vector<bool>> &);
bool find_if_point_on_fire(point, vector<vector<bool>> &, double);
bool surrounded_with_fire(point current_point, vector<vector<bool>> &fire_points_record);

int dimension;        //global variables so that they can be used in BFS directly
double probability;

int main(int argc, const char * argv[]) {
    
    double p_o;
    cout << "Enter the dimension (dim) of maze (dim X dim):";
    cin >>dimension;
    cout << "Enter the probability of obstacles associated with the maze:";
    cin >>p_o;
//////////////////////////////////////////////////QUESTION 4///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int iter = 20;
    bool path_person = false;
    bool path_fire = false;
    
    point start_person(0,0);
    point goal_person(dimension-1,dimension-1);
    point start_fire(0,dimension-1);
    point goal_fire(dimension-1,0);
    
    maze new_maze_fire(dimension,p_o);
    
//    cout<<"The first maze was:"<<endl;
//    new_maze_fire.print_maze();
    
    int counter=0;
    while(!(path_person && path_fire))  //now we have a maze which has a path for both a person and fire
    {
        counter = counter + 1;
        maze temp(dimension,p_o);
        path_person = A_star_without_fire(temp, start_person, goal_person);
        path_fire = A_star_without_fire(temp, start_fire, goal_fire);
        new_maze_fire = temp;
        //cout<<path_person<<" and "<<path_fire<<endl;
    }
//    cout<<"The new_maze_fire is:"<<endl;
    new_maze_fire.print_maze();
    cout<<"The number of iterations to generate a valid Fire_matrix:"<<counter<<endl;
    
    
    vector<point> fire_points;
    
    vector<vector<bool>> fire_points_record;
    fire_points_record.resize(dimension);
    for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
    {
        fire_points_record[i].resize(dimension);
    }
    for(int i=0;i<dimension;i++)                  //initiate the fire maze as valid with no fire at any tiles
    {
        for(int j=0;j<dimension;j++)
        {
            fire_points_record[i][j] = false;
        }
    }
    
    
    point fire_start(0,dimension-1);
    fire_points.push_back(fire_start);
    fire_points_record[0][dimension-1] = true;
    
    (new_maze_fire.maze_array)[fire_start.return_y()][fire_start.return_x()]=-1;
    cout<<"After setting the first point on fire:"<<endl;
    new_maze_fire.print_maze();
    
    
    vector<vector<point>> prev1;         //define a prev array for noting the path
    prev1.resize(dimension);
    for(int i=0;i<dimension;i++)        //initiate them with defaul points(-100,-100)
    {
        prev1[i].resize(dimension);
    }
    
    bool is_there_a_path_withfire_A_star = A_star(new_maze_fire, start_person, goal_person, prev1, fire_points, fire_points_record);
    
    if(is_there_a_path_withfire_A_star)                           //if a path exists, return the path
    {
        int number_of_steps = 0;
        cout<<"The path exist:"<<endl;
        goal_person.print_point();
        number_of_steps=number_of_steps+1;
        cout<<"==>";
        point temp = prev1[goal_person.return_y()][goal_person.return_x()];
        while( (temp.return_y()!=start_person.return_y()) || (temp.return_x()!=start_person.return_x()) )
        {
            temp.print_point();
            cout << "==>";
            temp = prev1[temp.return_y()][temp.return_x()];
            number_of_steps=number_of_steps+1;
        }
        temp.print_point();
        cout<<endl;
        cout<<"The number of steps to reach the goal is "<<number_of_steps<<endl;
    }
    else                                    //path does not exists.
    {
        cout<<"There is no path."<<endl;
    }
    
    cout<<"The fire matrix is:"<<endl;
    for(int i=0;i<dimension;i++)
    {
        for(int j=0; j<dimension; j++)
        {
            if(fire_points_record[i][j]==true)
                cout<<"$"<<" ";
            else
                cout<<fire_points_record[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;
    new_maze_fire.print_maze();
    
//////////////////////////////////////////////////QUESTION 4///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}

bool A_star_without_fire(maze &new_maze, point initial_state, point goal_state)
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
    double f_n = distance + return_heuristic(initial_state,goal_state, new_maze);
    initial_state.priority = f_n;
    fringe.push(initial_state);
    while(!fringe.empty())
    {
        point current_state = fringe.top();
        fringe.pop();
        
        if(closed_set[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true;
            if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                return true;
            }
            else
            {
                distance = distance + 1;
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
                            children[i].priority = distance + return_heuristic(children[i], goal_state, new_maze);
                            fringe_added[children[i].return_y()][children[i].return_x()]=true;
                            fringe.push(children[i]);
                        }
                    }
                }
            }
        }
    return false;

}

bool A_star(maze &new_maze, point initial_state, point goal_state,vector<vector<point>> &prev, vector<point> &fire_points, vector<vector<bool>> &fire_points_record)
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
    double f_n = distance + return_heuristic(initial_state,goal_state,new_maze);
    
    
    initial_state.priority = f_n;
    fringe.push(initial_state);
    
    
    while(!fringe.empty())
    {
        point current_state = fringe.top();
        //cout<<"#######Yes you moved a step##########"<<endl;
        fringe.pop();

        if(fire_points_record[current_state.return_y()][current_state.return_x()]==true)      //if the current position catches fire
        {
            cout<<"You are on fire!."<<endl;
            cout<<"You were at point:";
            current_state.print_point();
            cout<<endl;
            return false;
        }
        
        
        
        if(surrounded_with_fire(current_state, fire_points_record) == true)
        {
            cout<<"Nowhere to go, you are surrounded with fire!."<<endl;
            cout<<"You are at point:";
            current_state.print_point();
            cout<<endl;
            return false;
        }
        
        
        if(closed_set[current_state.return_y()][current_state.return_x()]==false)  //not visited till now
        {
            closed_set[current_state.return_y()][current_state.return_x()] = true;
            if(current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
            {
                return true;
            }
            else
            {
                distance = distance + 1;
                
                vector<point> children;
                return_children(current_state,children);    //find the accessible states from the current state
                
                for(int i=0;i<children.size();i++)
                {
                    int y_cord = children[i].return_y();
                    int x_cord = children[i].return_x();
                    
                    if(x_cord<0 || y_cord<0 || x_cord>dimension-1 || y_cord>dimension-1)
                        continue;
                    
                    if(new_maze.maze_array[y_cord][x_cord]==0 || fire_points_record[y_cord][x_cord]==true || closed_set[y_cord][x_cord]==true) //restricted_state: both obstacles and fire
                        continue;
                    
                    if(fringe_added[children[i].return_y()][children[i].return_x()]==false)
                        {
                            children[i].priority = distance + return_heuristic(children[i], goal_state, new_maze);
                            fringe_added[children[i].return_y()][children[i].return_x()]=true;
                            fringe.push(children[i]);
                            prev[children[i].return_y()][children[i].return_x()] = current_state;
                        }
                    }
                }
            }
        
        static int number_of_times_you_update_for_fire = 0;
        update_maze_fire(new_maze, fire_points, fire_points_record);   //update the maze for the current position
        number_of_times_you_update_for_fire=number_of_times_you_update_for_fire+1;
        cout<<"The number of times you update for fire:"<<number_of_times_you_update_for_fire<<endl;
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
double return_heuristic(point p,point goal, maze new_maze)
{
    int x_cord = p.return_x();
    int y_cord = p.return_y();
    if(new_maze.maze_array[y_cord][x_cord] == -1)
        return INT_MAX;
    double h_n = abs(p.return_x() - goal.return_x())  + abs(p.return_y() - goal.return_y());
    return h_n;
}

void update_maze_fire(maze &new_maze,vector<point> &fire_points, vector<vector<bool>> &fire_points_record)
{
    double prob_fire = 0.5;                     //define the probability for maze to be on fire
    int number_of_current_fire_points = 0;
    
    unsigned long size_current_fire_matrix = fire_points.size();
    
    while(number_of_current_fire_points<size_current_fire_matrix)                 //already added the first one on fire
    {
        point current_point = fire_points[number_of_current_fire_points];
        number_of_current_fire_points = number_of_current_fire_points + 1;
        
        //fire_points.pop_back();     //you dont have to remove the points from fire table
        int y_cord = current_point.return_y();
        int x_cord = current_point.return_x();
        
        point c1(y_cord+1,x_cord);                  //find all the neighbours of the current point
        point c2(y_cord-1,x_cord);
        point c3(y_cord,x_cord+1);
        point c4(y_cord,x_cord-1);
        
        vector<point> neigh_check_fire;             // push them in an array to iterate
        
        if(y_cord+1 < dimension)
            neigh_check_fire.push_back(c1);
        
        if(x_cord+1 < dimension)
            neigh_check_fire.push_back(c3);
        
        if(y_cord-1 >= 0)
            neigh_check_fire.push_back(c2);
        
        if(x_cord-1 >= 0)
            neigh_check_fire.push_back(c4);
        
        for(int i=0;i<neigh_check_fire.size();i++)
        {
            if(neigh_check_fire[i].return_y() > dimension-1 || neigh_check_fire[i].return_x() > dimension-1 || neigh_check_fire[i].return_y() < 0 || neigh_check_fire[i].return_x() < 0)            //iterate only those neighbours which are valid and not an obstacle
                continue;
            if(new_maze.maze_array[neigh_check_fire[i].return_y()][neigh_check_fire[i].return_x()]==0)
            {
//                neigh_check_fire[i].print_point();
//                cout<<" : Its an obstacle. Cannot lit it up on fire!"<<endl;
                continue;
            }
            
            if(fire_points_record[neigh_check_fire[i].return_y()][neigh_check_fire[i].return_x()] == true)
            {
//              cout<<"The point is already on fire, no need to check."<<endl;
                continue;
            }
            else
            {
                bool res = find_if_point_on_fire(neigh_check_fire[i], fire_points_record, prob_fire);       //if neighbour on fire and prob leads to it being on fire
//                cout<<"The value of res is:"<<res<<endl;
                if(res)
                {
//                    cout<<"The number of fire points are:"<<fire_points.size()<<endl;
                    {
                        fire_points.push_back(neigh_check_fire[i]);       //add it to the list of points on fire if its not a block
                        fire_points_record[neigh_check_fire[i].return_y()][neigh_check_fire[i].return_x()] = true;  //mark them as on fire
                        new_maze.maze_array[neigh_check_fire[i].return_y()][neigh_check_fire[i].return_x()]=-1;
                    }
                }
            }
        }
        cout<<"Number of times the loop work for finding fire points:"<<number_of_current_fire_points<<endl;
    }
}

bool surrounded_with_fire(point current_point, vector<vector<bool>> &fire_points_record) //surrounded by fire + obstacles
{
    int y_cord = current_point.return_y();
    int x_cord = current_point.return_x();
    
    bool neighbour_1;
    bool neighbour_2;
    bool neighbour_3;
    bool neighbour_4;
    
    if(y_cord+1 > dimension-1)
        neighbour_1 = true;             //it is not a path as its not a point on the maze. therefore you cannot go there
    else
        neighbour_1 = fire_points_record[(current_point.return_y())+1][current_point.return_x()];
    
    if(x_cord+1 > dimension-1)
        neighbour_2 = true;
    else
        neighbour_2 = fire_points_record[current_point.return_y()][current_point.return_x()+1];
    
    if(y_cord-1 < 0)
        neighbour_3 = true;
    else
        neighbour_3 = fire_points_record[current_point.return_y()-1][current_point.return_x()];
    
    if(x_cord-1 < 0)
        neighbour_4 = true;
    else
        neighbour_4 = fire_points_record[current_point.return_y()][current_point.return_x()-1];
    
    if(neighbour_1 && neighbour_2 && neighbour_3 && neighbour_4)
        return true;
    else
        return false;
}
bool find_if_point_on_fire(point current_point, vector<vector<bool>> &fire_points_record, double q)
{
    int y_cord = current_point.return_y();
    int x_cord = current_point.return_x();
    //cout<<"The points checked for fire ("<<y_cord<<","<<x_cord<<")"<<endl;
    double k=0;
    
    if(y_cord+1<dimension)
        if(fire_points_record[y_cord+1][x_cord]==true)
            k = k+1;     //find all the valid neighbours on fire and increase k
    if(y_cord-1>=0)
        if(fire_points_record[y_cord-1][x_cord]==true)
            k = k+1;
    if(x_cord+1<dimension)
        if(fire_points_record[y_cord][x_cord+1]==true)
            k = k+1;
    if(x_cord-1>=0)
        if(fire_points_record[y_cord][x_cord-1]==true)
            k = k+1;
    
    double prob = 1 - pow((1 - q),k);              //check whether it should be on fire
//    cout<<"The value of k is:"<<k<<endl;
//    cout<<"The value of prob is"<<prob<<endl;
    if(prob >= 0.5)
        return true;
    else
        return false;
}
