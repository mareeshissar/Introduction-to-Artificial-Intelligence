//
//  main.cpp
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/19/19.
//  Copyright © 2019 Kartik Rattan. All rights reserved.
//

#include <iostream>
#include <chrono> 
#include "maze_fromation.h"
#include "point.h"
#include <fstream> 
using namespace std;

using namespace std::chrono;


#include <queue>
#include <math.h>
#include <stack>


bool BFS(maze&, point, point, vector<vector<point>>&);
bool DFS(maze&, point, point, vector<vector<point>>&);
bool A_star(maze&, point, point, vector<vector<point>>&);
void hill_climbing(maze&, point, point);
void simulated_annealing(maze&, point, point);
void simulated_annealing_dfs(maze&, point, point);
void simulated_annealing_astar(maze&, point, point);
void genetic_algorithm(maze&, point, point);
void return_children(point a, vector<point>&);
double return_heuristic_manhattan(point, point);
double return_heuristic_euclidean(point, point);
float acceptance_probability(int , int , float);

bool operator<(const maze& m1, const maze& m2)
{
	return m1.path_length_priority < m2.path_length_priority;
	//return m1.max_fringe_size_priority < m2.max_fringe_size_priority;
	//return m1.max_nodes_expanded_priority < m2.max_nodes_expanded_priority;
}


int dimension;        //global variables so that they can be used in BFS directly
double probability;

int main(int argc, const char* argv[]) {
	double n_t = 20;
	double iter = n_t;
	
	
	cout << "Enter the dimension (dim) of maze (dim X dim):";
	cin >> dimension;
	cout << "Enter the probability of obstacles associated with the maze:";
	cin >> probability;

	/*
	ofstream fout, fout1;
	fout.open("output_dfs.txt",ios::app);
	fout1.open("output_astar.txt",ios::app);
	for (float p = 0.1; p < 1; p += 0.01)
	{
		cout << p << endl;
		probability = p;
		iter = n_t;
		double n_true_dfs = 0;
		double n_true_A_star = 0;
		while (iter > 0)
		{
			maze new_maze(dimension, probability);
			//new_maze.print_maze();

			vector<vector<point>> prev;         //define a prev array for noting the path
			prev.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev[i].resize(dimension);
			}
			vector<vector<point>> prev1;         //define a prev array for noting the path
			prev1.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev1[i].resize(dimension);
			}

			point start(0, 0);                       //start point
			point goal(dimension - 1, dimension - 1);    //goal point

			bool is_there_path_dfs = DFS(new_maze, start, goal, prev);
			bool is_there_path_a_star = A_star(new_maze, start, goal, prev1);

			if (is_there_path_dfs)
				n_true_dfs += 1;
			if (is_there_path_a_star)
				n_true_A_star += 1;

			iter -= 1;
		}

		cout << "For dimension " << dimension << " X " << dimension << " and obstacles " << dimension * dimension * probability << " , the probability of success of the algorithms is: " << endl;
		cout << "DFS: " << double(n_true_dfs / n_t) << endl;
		cout << "A_star (euclidean metric): " << double(n_true_A_star / n_t) << endl;
		fout << double(n_true_dfs / n_t) << endl;
		fout1 << double(n_true_A_star / n_t) << endl;

	}
	fout.close();
	fout1.close();
	*/

	//auto start = high_resolution_clock::now();
	//bool is_there_path_bfs = BFS(new_maze, start, goal, prev);          //run BFS
	//auto stop = high_resolution_clock::now();
	//to calculate the running time
	//auto duration = duration_cast<microseconds>(stop - start);
	//cout << "Time in seconds- " << duration.count() << endl;
	
	
	maze new_maze(dimension, probability);
	new_maze.print_maze();

	vector<vector<point>> prev;         //define a prev array for noting the path
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}


	point start(0, 0);                       //start point
	point goal(dimension - 1, dimension - 1);    //goal point

	//bool is_there_path_a_star = A_star(new_maze, start, goal, prev);
	bool is_there_path_dfs = DFS(new_maze, start, goal, prev);

	if (is_there_path_dfs)                           //if a path exists, return the path
	{
		//cout << "The path exist:" << endl;
		//goal.print_point();
		new_maze.set_maze_array(goal.return_y(), goal.return_x(), 7);
		//cout << "==>";
		point temp = prev[goal.return_y()][goal.return_x()];
		while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
		{
			//temp.print_point();
			new_maze.set_maze_array(temp.return_y(), temp.return_x(), 7);
			//cout << "==>";
			temp = prev[temp.return_y()][temp.return_x()];
		}
		//temp.print_point();
		new_maze.set_maze_array(temp.return_y(), temp.return_x(), 7);
		//cout << endl;
		//hill_climbing(new_maze, start, goal);
	    //simulated_annealing(new_maze, start, goal);
		genetic_algorithm(new_maze, start, goal);
		//simulated_annealing_dfs(new_maze, start, goal);
		//simulated_annealing_astar(new_maze, start, goal);
	
		//new_maze.print_maze();
	}
	else                                    //path does not exists.
	{
		cout << "There is no path" << endl;
	}
	

	return 0;
}


bool BFS(maze& new_maze, point initial_state, point goal_state, vector<vector<point>>& prev)
{
	queue<point> fringe;
	//bool closed_set[dimension][dimension];
	vector<vector<bool>> closed_set;
	closed_set.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		closed_set[i].resize(dimension);
	}
	for (int i = 0; i < dimension; i++)                //make all the states as unvisited
	{
		for (int j = 0; j < dimension; j++)
		{
			closed_set[i][j] = false;           //nothing is visited till now
		}
	}
	//bool fringe_added[dimension][dimension];
	vector<vector<bool>> fringe_added;
	fringe_added.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		fringe_added[i].resize(dimension);
	}
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
		point current_state = fringe.front();
		fringe.pop();
		if (closed_set[current_state.return_y()][current_state.return_x()] == false)  //not visited till now
		{
			closed_set[current_state.return_y()][current_state.return_x()] = true;
			if (current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
			{
				int number_states_visited = 0;
				for (int i = 0; i < dimension; i++)
					for (int j = 0; j < dimension; j++)
					{
						if (closed_set[i][j] == 1)
							number_states_visited += 1;
					}
				cout << "Total number of states visited by BFS: " << number_states_visited << endl;
				return true;
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


bool DFS(maze& new_maze, point initial_state, point goal_state, vector<vector<point>>& prev)
{
	stack<point> fringe;
	//bool closed_set[dimension][dimension];
	vector<vector<bool>> closed_set;
	closed_set.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		closed_set[i].resize(dimension);
	}
	for (int i = 0; i < dimension; i++)                //make all the states as unvisited
	{
		for (int j = 0; j < dimension; j++)
		{
			closed_set[i][j] = false;           //nothing is visited till now
		}
	}
	//bool fringe_added[dimension][dimension];
	vector<vector<bool>> fringe_added;
	fringe_added.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		fringe_added[i].resize(dimension);
	}
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
			
			if ( (current_state.return_x() == goal_state.return_x()) && (current_state.return_y() == goal_state.return_y()) )
			{
				new_maze.max_fringe_size_priority = fringe.size();
				/*
				int number_states_visited = 0;
				for (int i = 0; i < dimension; i++)
					for (int j = 0; j < dimension; j++)
					{
						if (closed_set[i][j] == 1)
							number_states_visited += 1;
					}
				cout << "Total number of states visited by DFS: " << number_states_visited << endl;
				*/
				return true;
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

void return_children(point a, vector<point>& children)
{

	point c1(a.return_y() + 1, a.return_x() + 0);
	point c2(a.return_y() + 0, a.return_x() + 1);
	point c3(a.return_y() + 0, a.return_x() - 1);
	point c4(a.return_y() - 1, a.return_x() + 0);
	children.push_back(c1);
	children.push_back(c2);
	children.push_back(c3);
	children.push_back(c4);
}

bool A_star(maze& new_maze, point initial_state, point goal_state, vector<vector<point>>& prev)
{
	priority_queue<point> fringe;
	//bool closed_set[dimension][dimension];
	vector<vector<bool>> closed_set;
	closed_set.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		closed_set[i].resize(dimension);
	}
	for (int i = 0; i < dimension; i++)                //make all the states as unvisited
	{
		for (int j = 0; j < dimension; j++)
		{
			closed_set[i][j] = false;           //nothing is visited till now
		}
	}
	//bool fringe_added[dimension][dimension];
	vector<vector<bool>> fringe_added;
	fringe_added.resize(dimension);
	for (int i = 0; i < dimension; i++)
	{
		fringe_added[i].resize(dimension);
	}

	for (int i = 0; i < dimension; i++)                //make all the states as unadded
	{
		for (int j = 0; j < dimension; j++)
		{
			fringe_added[i][j] = false;           //nothing is addedtill now
		}
	}
	double distance = 0;
	double f_n = distance + return_heuristic_manhattan(initial_state, goal_state);
	initial_state.priority = f_n;
	fringe.push(initial_state);
	while (!fringe.empty())
	{
		point current_state = fringe.top();
		fringe.pop();
		new_maze.max_nodes_expanded_priority = new_maze.max_nodes_expanded_priority + 1;

		if (closed_set[current_state.return_y()][current_state.return_x()] == false)  //not visited till now
		{
			closed_set[current_state.return_y()][current_state.return_x()] = true;
			if (current_state.return_x() == goal_state.return_x() && current_state.return_y() == goal_state.return_y())
			{
				/*
				int number_states_visited = 0;
				for (int i = 0; i < dimension; i++)
					for (int j = 0; j < dimension; j++)
					{
						if (closed_set[i][j] == 1)
							number_states_visited += 1;
					}
				cout << "Total number of states visited by A-STAR: " << number_states_visited << endl;
				*/
				return true;
			}
			else
			{
				distance = distance + 1;
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
						children[i].priority = distance + return_heuristic_manhattan(children[i], goal_state);
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

double return_heuristic_manhattan(point p, point goal)
{
	double h_n = abs(p.return_x() - goal.return_x()) + abs(p.return_y() - goal.return_y());
	return h_n;
}

double return_heuristic_euclidean(point p, point goal)
{
	
	double h_n = sqrt( pow( ( p.return_x() - goal.return_x() ), 2) + pow( (p.return_y() - goal.return_y() ), 2));
	return h_n;
}

void hill_climbing(maze& new_maze, point start, point goal)
{
	static int max;
	static int cal = 0;
	vector<vector<point>> prev;                  //define a prev array for noting the path
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}

	//x is the initial state
	maze x = new_maze;
	bool is_there_path_a_star = A_star(x, start, goal, prev);
	if (is_there_path_a_star)                 //if a path exists, return the path
	{
		point temp = prev[goal.return_y()][goal.return_x()];
		int path_length = 1;
		while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
		{
			temp = prev[temp.return_y()][temp.return_x()];
			path_length++;
		}
		path_length = path_length;
		x.path_length_priority = path_length;  //setting the length of the shortest path as priority
		max = x.path_length_priority;
	}
	else
	{
		cout << "There is no path" << endl;
		return;
	}
	int obstacles = dimension * dimension * probability;
	
	
	
	for (; ; )  
	{
		priority_queue<maze> neighbour_mazes;
		if (x.path_length_priority == max)  
		{
			cal++;
					}
		if (cal == 20)  //termination condition
		{
			return;
		}

		maze x2 = x;
		//neighbour generation
		for (int i = 0; i < 1.5 * (dimension * dimension - obstacles - 2); i++)
		{
			maze x1 = x;
			int r1;
			int r2;
			int extra_obstacles = 1;
			if (i >= (dimension * dimension - obstacles - 2))
			{
				extra_obstacles = 2;
				x2 = x;
			}
			for (int k = 0; k < extra_obstacles; k++)
			{
				do
				{
					r1 = 0 + rand() % (dimension);
					r2 = 0 + rand() % (dimension);
				} while (x2.return_check_maze(r1, r2) == true || (r1 == 0 && r2 == 0) || (r1 == dimension - 1 && r2 == dimension - 1));
				//cout << i + 1 << ". " << "(" << r1 << "," << r2 << ")" << endl;
				x2.set_check_maze(r1, r2);
				x1.set_maze_array(r1, r2,0);
			}

			
			vector<vector<point>> prev1;                  //define a prev array for noting the path
			prev1.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev1[i].resize(dimension);
			}
			bool is_there_path_a_star = A_star(x1, start, goal, prev1);
			if (is_there_path_a_star)                 //if a path exists, return the path
			{
				
				point temp = prev1[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					
					temp = prev1[temp.return_y()][temp.return_x()];
					path_length++;
				}
				
				path_length = path_length;
				x1.path_length_priority = path_length;
				neighbour_mazes.push(x1);
			}
		
			
		}
		cout << "The priority before updation " << x.path_length_priority << endl;
		//x.print_maze();
		if (neighbour_mazes.top().path_length_priority > x.path_length_priority)
		{
			x = neighbour_mazes.top();
			max = x.path_length_priority;
			cal = 0;
			cout << "The priority of the best neighbor maze is (greater) than that of inital state is-  " << x.path_length_priority<<endl;
			
		}
		else if (neighbour_mazes.top().path_length_priority == x.path_length_priority)
		{
			cout << "The priority of the best neighbor maze is (equal) to that of inital state is- "<< x.path_length_priority << endl;
		}
		else
		{
			cout << "The priority of the best neighbor maze is (lesser) than that of inital state is-" << x.path_length_priority << endl;
		}
		
		cout << "The priority after updation " << x.path_length_priority << endl;
		//x.print_maze();

	}
	
	
}

void simulated_annealing(maze& new_maze, point start, point goal)
{
	static int max;
	static int cal = 0;
	vector<vector<point>> prev;                  //define a prev array for noting the path
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}

	

	//x is the initial state
	maze x = new_maze;
	bool is_there_path_a_star = A_star(x, start, goal, prev);
	if (is_there_path_a_star)                 //if a path exists, return the path
	{
		point temp = prev[goal.return_y()][goal.return_x()];
		int path_length = 1;
		while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
		{
			temp = prev[temp.return_y()][temp.return_x()];
			path_length++;
		}
		path_length = path_length;
		x.path_length_priority = path_length;
		max = x.path_length_priority;
	}
	else
	{
		cout << "There is no path" << endl;
		return;
	}
	int obstacles = dimension * dimension * probability;
	
	
	float t = 1.0;         //time variable 
	float temperature=0.0;
	while (t!=6)
	{
		priority_queue<maze> neighbour_mazes;
		if (x.path_length_priority == max)
		{
			cal++;
		}
		if (cal == 20)  //termination condition
		{
			return;
		}

		maze x2 = x;
		temperature = 1 / t;
		//neighbour generation by adding 1 and 2 obstacles
		for (int i = 0; i < 1.5 * (dimension * dimension - obstacles - 2); i++)
		{
			maze x1 = x;
			int r1;
			int r2;
			int extra_obstacles = 1;
			if (i >= (dimension * dimension - obstacles - 2))
			{
				extra_obstacles = 2;
				x2 = x;
			}
			for (int k = 0; k < extra_obstacles; k++)
			{
				do
				{
					r1 = 0 + rand() % (dimension);
					r2 = 0 + rand() % (dimension);
				} while (x2.return_check_maze(r1, r2) == true || (r1 == 0 && r2 == 0) || (r1 == dimension - 1 && r2 == dimension - 1));
				//cout << i + 1 << ". " << "(" << r1 << "," << r2 << ")" << endl;
				x2.set_check_maze(r1, r2);
				x1.set_maze_array(r1, r2,0);
			}

			
			vector<vector<point>> prev1;                  //define a prev array for noting the path
			prev1.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev1[i].resize(dimension);
			}
			bool is_there_path_a_star = A_star(x1, start, goal, prev1);
			if (is_there_path_a_star)                 //if a path exists, return the path
			{
				
				point temp = prev1[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					
					temp = prev1[temp.return_y()][temp.return_x()];
					path_length++;
				}
				
				path_length = path_length;
				x1.path_length_priority = path_length;
				neighbour_mazes.push(x1);

			}
			

		}

		//for storing the coordinates of the obstacles
		vector<point> obstacles_position;
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				if (x.get_value(i,j) == 0)
				{
					obstacles_position.push_back(point(i, j));
				}
			}
		}
		//generating neighbour maze by removing 1 obstacle
		for (int b = 0; b < obstacles_position.size() ; b++)
		{
			
			maze x1 = x;
			x1.set_maze_array(obstacles_position[b].return_y(), obstacles_position[b].return_x(), 1);
			
			vector<vector<point>> prev2;                  //define a prev array for noting the path
			prev2.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev2[i].resize(dimension);
			}
			bool is_there_path_a_star = A_star(x1, start, goal, prev2);
			if (is_there_path_a_star)                 //if a path exists, return the path
			{

				point temp = prev2[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{

					temp = prev2[temp.return_y()][temp.return_x()];
					path_length++;
				}

				path_length = path_length;
				x1.path_length_priority = path_length;
				
     			neighbour_mazes.push(x1);
			}
		}
		
		//generating neighbour maze by removing 2 obstacle
		for (int b = 0; b < obstacles_position.size(); b=b+2)
		{
			
			if (b + 1 == obstacles_position.size())
			{
				break;
			}
			
			cout << endl;

			maze x1 = x;
			x1.set_maze_array(obstacles_position[b].return_y(), obstacles_position[b].return_x(), 1);
			x1.set_maze_array(obstacles_position[b+1].return_y(), obstacles_position[b+1].return_x(), 1);
			
			vector<vector<point>> prev2;                  //define a prev array for noting the path
			prev2.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev2[i].resize(dimension);
			}
			bool is_there_path_a_star = A_star(x1, start, goal, prev2);
			if (is_there_path_a_star)                 //if a path exists, return the path
			{

				point temp = prev2[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{

					temp = prev2[temp.return_y()][temp.return_x()];
					path_length++;
				}

				path_length = path_length;
				x1.path_length_priority = path_length;

				neighbour_mazes.push(x1);
			}
		}

		cout << "The priority before updation for Simulated Annealing is- " << x.path_length_priority << endl;
		x.print_maze();
		cout << "The temperature is " << temperature << endl;
		if (neighbour_mazes.top().path_length_priority > x.path_length_priority)
		{
			x = neighbour_mazes.top();
			max = x.path_length_priority;
			cal = 0;
		}
		else
		{
			srand(time(0));
			if (((rand() % 101) / 100) <= acceptance_probability(x.path_length_priority, neighbour_mazes.top().path_length_priority, temperature))
			{
				x = neighbour_mazes.top();
				//cout << "--------------------jumped--------------" << endl;

			}
				
		
		}
		cout << "The priority after updation for Simulated Annealing is- " << x.path_length_priority << endl;
		x.print_maze();

		t = t + 1;
		
	} 


}


void simulated_annealing_dfs(maze& new_maze, point start, point goal)
{
	vector<vector<point>> prev;                  //define a prev array for noting the path
	priority_queue<maze> neighbour_mazes;
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}



	//x is the initial state
	maze x = new_maze;
	bool is_there_path_dfs = DFS(x, start, goal, prev);
	if (is_there_path_dfs)
	{
		cout << "maximal fringe size is " << x.max_fringe_size_priority << endl;
	}
	int obstacles = dimension * dimension * probability;
	maze x2 = new_maze;
	int streak = 0;
	int t = 1;
	float temperature = 0.0;
	while (t != 2)
	{

		temperature = 1 / t;
		//neighbour generation
		for (int i = 0; i < 1.5 * (dimension * dimension - obstacles - 2); i++)
		{
			maze x1 = new_maze;
			int r1;
			int r2;
			int extra_obstacles = 1;
			if (i >= (dimension * dimension - obstacles - 2))
			{
				extra_obstacles = 2;
				x2 = new_maze;
			}
			for (int k = 0; k < extra_obstacles; k++)
			{
				do
				{
					r1 = 0 + rand() % (dimension);
					r2 = 0 + rand() % (dimension);
				} while (x2.return_check_maze(r1, r2) == true || (r1 == 0 && r2 == 0) || (r1 == dimension - 1 && r2 == dimension - 1));
				//cout << i + 1 << ". " << "(" << r1 << "," << r2 << ")" << endl;
				x2.set_check_maze(r1, r2);
				x1.set_maze_array(r1, r2,0);
			}


			cout << "printing neighbour maze" << endl;
			//x1.print_maze();
			bool is_there_path_dfs = DFS(x1, start, goal, prev);
			if (is_there_path_dfs)                 //if a path exists, return the path
			{
				cout << "The path exist:" << endl;
				goal.print_point();
				cout << "==>";
				point temp = prev[goal.return_y()][goal.return_x()];
				
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					temp.print_point();
					cout << "==>";
					temp = prev[temp.return_y()][temp.return_x()];
					
				}
				temp.print_point();
				cout << "maximal fringe size is "<<x1.max_fringe_size_priority << endl;
				neighbour_mazes.push(x1);

			}
			else                                    //path does not exists.
			{
				cout << "There is no path" << endl;
			}

		}

		
		cout << "the priority of iniital state before comaprison (SA) " << x.max_fringe_size_priority << endl;
		x.print_maze();
		if (neighbour_mazes.top().max_fringe_size_priority > x.max_fringe_size_priority)
		{
			x = neighbour_mazes.top();
		}
		else
		{
			srand(time(0));
			if (((rand() % 101) / 100) <= acceptance_probability(x.max_fringe_size_priority, neighbour_mazes.top().max_fringe_size_priority, temperature))
				x = neighbour_mazes.top();

		}
		cout << "the priority of initial state after comparison (SA) " << x.max_fringe_size_priority << endl;
		x.print_maze();

		t = t + 1;
	}


}

void simulated_annealing_astar(maze& new_maze, point start, point goal)
{
	vector<vector<point>> prev;                  //define a prev array for noting the path
	priority_queue<maze> neighbour_mazes;
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}



	//x is the initial state
	maze x = new_maze;
	bool is_there_path_dfs = DFS(x, start, goal, prev);
	if (is_there_path_dfs)
	{
		cout << "maximal fringe size is " << x.max_fringe_size_priority << endl;
	}
	int obstacles = dimension * dimension * probability;
	maze x2 = new_maze;
	int streak = 0;
	int t = 1;
	float temperature = 0.0;
	while (t != 2)
	{

		temperature = 1 / t;
		//neighbour generation
		for (int i = 0; i < 1.5 * (dimension * dimension - obstacles - 2); i++)
		{
			maze x1 = new_maze;
			int r1;
			int r2;
			int extra_obstacles = 1;
			if (i >= (dimension * dimension - obstacles - 2))
			{
				extra_obstacles = 2;
				x2 = new_maze;
			}
			for (int k = 0; k < extra_obstacles; k++)
			{
				do
				{
					r1 = 0 + rand() % (dimension);
					r2 = 0 + rand() % (dimension);
				} while (x2.return_check_maze(r1, r2) == true || (r1 == 0 && r2 == 0) || (r1 == dimension - 1 && r2 == dimension - 1));
				//cout << i + 1 << ". " << "(" << r1 << "," << r2 << ")" << endl;
				x2.set_check_maze(r1, r2);
				x1.set_maze_array(r1, r2,0);
			}


			cout << "printing neighbour maze" << endl;
			//x1.print_maze();
			bool is_there_path_dfs = DFS(x1, start, goal, prev);
			if (is_there_path_dfs)                 //if a path exists, return the path
			{
				cout << "The path exist:" << endl;
				goal.print_point();
				cout << "==>";
				point temp = prev[goal.return_y()][goal.return_x()];

				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					temp.print_point();
					cout << "==>";
					temp = prev[temp.return_y()][temp.return_x()];

				}
				temp.print_point();
				cout << "maximal fringe size is " << x1.max_fringe_size_priority << endl;
				neighbour_mazes.push(x1);

			}
			else                                    //path does not exists.
			{
				cout << "There is no path" << endl;
			}

		}


		cout << "the priority of iniital state before comaprison (SA) " << x.max_fringe_size_priority << endl;
		x.print_maze();
		if (neighbour_mazes.top().max_fringe_size_priority > x.max_fringe_size_priority)
		{
			x = neighbour_mazes.top();
		}
		else
		{
			srand(time(0));
			if (((rand() % 101) / 100) <= acceptance_probability(x.max_fringe_size_priority, neighbour_mazes.top().max_fringe_size_priority, temperature))
				x = neighbour_mazes.top();

		}
		cout << "the priority of initial state after comparison (SA) " << x.max_fringe_size_priority << endl;
		x.print_maze();

		t = t + 1;
	}


}

float acceptance_probability(int x, int x1, float temp)
{
	float rate = 0.5;
	return exp(-1 * rate * ( (x-x1)/temp ) );
}

void genetic_algorithm(maze& new_maze, point start, point goal)
{
	int i1, i2;
	vector<vector<point>> prev;                  //define a prev array for noting the path
	priority_queue<maze> children_mazes;
	priority_queue<maze> children_and_parents_mazes;
	prev.resize(dimension);
	for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
	{
		prev[i].resize(dimension);
	}
	
	int N = 10; //number of children's to be generated
	maze x2 = new_maze;
	   
	//solution(children) generation
	for (int i = 0; i < N; i++)
	{
		maze x1 = new_maze;
		int r1;
		int r2;
		int extra_obstacles = 1;
		
		for (int k = 0; k < extra_obstacles; k++)
		{
			do
			{
				r1 = 0 + rand() % (dimension);
				r2 = 0 + rand() % (dimension);
			} while (x2.return_check_maze(r1, r2) == true || (r1 == 0 && r2 == 0) || (r1 == dimension - 1 && r2 == dimension - 1));
			//cout << i + 1 << ". " << "(" << r1 << "," << r2 << ")" << endl;
			x2.set_check_maze(r1, r2);
			x1.set_maze_array(r1, r2,0);
		}

		vector<vector<point>> prev1;                  //define a prev array for noting the path
		prev1.resize(dimension);
		for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
		{
			prev1[i].resize(dimension);
		}
		bool is_there_path_a_star = A_star(x1, start, goal, prev1);
		if (is_there_path_a_star)                 //if a path exists, return the path
		{
			point temp = prev1[goal.return_y()][goal.return_x()];
			int path_length = 1;
			while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
			{
			
				temp = prev1[temp.return_y()][temp.return_x()];
				path_length++;
			}
			path_length = path_length;

			x1.path_length_priority = path_length;

			children_mazes.push(x1);
			children_and_parents_mazes.push(x1);

		}
		else                                    //path does not exists.
		{
			cout << "There is no path" << endl;
			N = N + 1;  //if there is any children with no path then increase the value of N until 10 valid solutions are found
		}

	}

	
	static int max=0;
	static int cal = 0;
	//creating children solutions
	for (int gen = 0; gen < 10; gen++) //to specify the number of generation you want to run the algorithm
	{
		if (children_mazes.top().path_length_priority == max)
		{
			cal++;
		}
		if (cal == 20)  //termination condition
		{
			return;
		}
		while (!children_mazes.empty())
		{
			maze p1 = children_mazes.top();  //first parent
			children_mazes.pop();
			if (children_mazes.empty() == true)
			{
				break;
			}
			maze p2 = children_mazes.top();  //second parent
			children_mazes.pop();
			/*
			cout << "printing maze before crossover ---" << endl;
			p1.print_maze();
			cout << "----" << endl;
			p2.print_maze();
			cout << "maze priority before--" << p1.path_length_priority << endl;
			cout << "maze priority before--" << p2.path_length_priority << endl;
			*/
			int crossover_point = 0.6 * (dimension - 1); //taking 60% from fist parent and 40% from the second

			int temp_value;

			//creating crossover matrices
			for (int a = 0; a < dimension; a++)
			{
				for (int b = crossover_point + 1; b < dimension; b++)
				{
					temp_value = p1.get_value(a, b);
					p1.set_value(a, b, p2.get_value(a, b));
					p2.set_value(a, b, temp_value);
				}
			}

			//adding mutation to new p1
			do
			{
				i1 = 0 + rand() % (dimension);
				i2 = 0 + rand() % (dimension);
			} while (p1.return_check_maze(i1, i2) == true || (i1 == 0 && i2 == 0) || (i1 == dimension - 1 && i2 == dimension - 1));
			//cout << "muatation coordinate - " << "(" << i1 << "," << i2 << ")" << endl;
			p1.set_check_maze(i1, i2);
			p1.set_maze_array(i1, i2,0);

			//adding mutation to new p2
			do
			{
				i1 = 0 + rand() % (dimension);
				i2 = 0 + rand() % (dimension);
			} while (p2.return_check_maze(i1, i2) == true || (i1 == 0 && i2 == 0) || (i1 == dimension - 1 && i2 == dimension - 1));
			//cout << "muatation coordinate - " << "(" << i1 << "," << i2 << ")" << endl;
			p2.set_check_maze(i1, i2);
			p2.set_maze_array(i1, i2,0);


			//computing priority for p1 which is the new child
			vector<vector<point>> prev1;                  //define a prev array for noting the path
			prev1.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev1[i].resize(dimension);
			}

			bool is_there_path_a_star_p1 = A_star(p1, start, goal, prev1);
			if (is_there_path_a_star_p1)                 //if a path exists, return the path
			{
				point temp = prev1[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					temp = prev1[temp.return_y()][temp.return_x()];
					path_length++;
				}
				path_length = path_length;
				p1.path_length_priority = path_length;
				children_and_parents_mazes.push(p1);

			}
			else                                    //path does not exists.
			{
				cout << "There is no path" << endl;
			}

			//computing priority for p2 which is the new child
			vector<vector<point>> prev2;                  //define a prev array for noting the path
			prev2.resize(dimension);
			for (int i = 0; i < dimension; i++)        //initiate them with defaul points(-100,-100)
			{
				prev2[i].resize(dimension);
			}
			bool is_there_path_a_star_p2 = A_star(p2, start, goal, prev2);
			if (is_there_path_a_star_p2)                 //if a path exists, return the path
			{
				point temp = prev2[goal.return_y()][goal.return_x()];
				int path_length = 1;
				while ((temp.return_y() != start.return_y()) || (temp.return_x() != start.return_x()))
				{
					temp = prev2[temp.return_y()][temp.return_x()];
					path_length++;
				}
				path_length = path_length;
				p2.path_length_priority = path_length;
				children_and_parents_mazes.push(p2);

			}
			else                                    //path does not exists.
			{
				cout << "There is no path" << endl;
			}


		}

		if (children_and_parents_mazes.top().path_length_priority > max)
		{
			max = children_and_parents_mazes.top().path_length_priority;
		}
		//cout << "children and parent maze remaing size before moving to child maze" << children_and_parents_mazes.size() << endl;
		//selecting the best 10 solutions from parents and children
		int count = 0;
		while (count != 10 && !children_and_parents_mazes.empty())
		{
			maze m = children_and_parents_mazes.top();
			bool is_there_path = A_star(m, start, goal, prev);
			if (is_there_path)
			{
				children_mazes.push(m);
				count++;
			}
			
			children_and_parents_mazes.pop();
			
		}

		//no solvable children generated
		if (children_mazes.empty())
		{
			return;
		}
		
		cout << "children and parent maze remaing size" << children_and_parents_mazes.size() << endl;
		cout << "max path length priority"<<children_mazes.top().path_length_priority << endl;
		cout << "maximum value so far" << max<<endl;
		
	}
	
}