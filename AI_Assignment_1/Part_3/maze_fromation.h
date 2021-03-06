#pragma once
//
//  maze_fromation.h
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/19/19.
//  Copyright � 2019 Kartik Rattan. All rights reserved.
//

#ifndef maze_fromation_h
#define maze_fromation_h
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

class maze
{
public:
	
	maze(int d, double prob)
	{
		dim = d;
		maze_array.resize(dim);
		for (int i = 0; i < dim; i++)
		{
			maze_array[i].resize(dim);
		}
		prob_obstacles = prob;
		for (int i = 0; i < dim; i++)                  //initiate the whole maze as valid
		{
			for (int j = 0; j < dim; j++)
			{
				maze_array[i][j] = 1;
			}
		}
		//cout << "Dim:" << dim << endl;
		//cout << "The prob is:" << prob_obstacles << endl;
		const int num_obst = (dim * dim) * (prob_obstacles);
		//cout << "The number of obstacles:" << num_obst << endl;
		srand(time(0));
		//vector<vector<bool>> check_maze;
		
		check_maze.resize(dim);
		for (int i = 0; i < dim; i++)
		{
			check_maze[i].resize(dim);
		}
		//bool check_maze[dim][dim];
		for (int i = 0; i < dim; i++)                  //initiate the whole maze as valid
		{
			for (int j = 0; j < dim; j++)
			{
				check_maze[i][j] = false;
			}
		}
		for (int i = 0; i < num_obst; i++)         //assign a random number for the obstacle
		{
			int p1;
			int p2;
			do
			{
				p1 = 0 + rand() % (dim);
				p2 = 0 + rand() % (dim);
			} while (((p1 == 0 && p2 == 0) || (p1 == dim - 1 && p2 == dim - 1)) || check_maze[p1][p2] == true);
			//cout << i + 1 << ". " << "(" << p1 << "," << p2 << ")" << endl;
			check_maze[p1][p2] = true;
			maze_array[p1][p2] = 0;
		}
	}
	vector<vector<int>> return_maze()
	{
		return maze_array;
	}
	bool return_check_maze(int i, int j)
	{
		return check_maze[i][j];
	}
	void set_check_maze(int i, int j)
	{
		check_maze[i][j]=true;
	}
	void set_maze_array(int i, int j, int val)
	{
		maze_array[i][j] = val;
	}
	void print_maze()
	{
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				cout << maze_array[i][j] << " ";
			}
			cout << endl;
		}
	}
	void set_value(int i, int j, int val)
	{
		maze_array[i][j] = val;
	}
	int get_value(int i, int j)
	{
		return maze_array[i][j];
	}
	
	int path_length_priority;
	int max_fringe_size_priority;
	int max_nodes_expanded_priority = 0;
	
private:
	int dim;               //the dimensions in the maze
	double prob_obstacles;   //the probability of number of obstacles
	vector<vector<bool>> check_maze;
	vector<vector<int>> maze_array;
};

#endif /* maze_fromation_h */
