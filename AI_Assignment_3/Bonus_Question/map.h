//
//  map.h
//  assignment3_AI
//
//  
//  
//

#ifndef map_h
#define map_h

#include "point.h"
#include <vector>
#include <time.h>
#include<functional>
#include<random>
using namespace std;

class map
{
public:
	map()
	{
		dim = 50;
		map_env.resize(dim, {});
		map_prob.resize(dim, {});
		map_prob_found.resize(dim, {});
		map_prob_distance.resize(dim, {});
		double initial_belief = 1.0 / (dim* dim);
		//cout << "Initial belief: " << initial_belief << endl;
		for (int i = 0; i < map_env.size(); i++)
		{
			map_env[i].resize(dim);
			map_prob[i].resize(dim, initial_belief);
			map_prob_found[i].resize(dim, initial_belief);
			map_prob_distance[i].resize(dim, initial_belief);
		}
		generate_environment(map_env);
	};

	void generate_environment(vector<vector<point>>& map_env)
	{
		int size_map = dim * dim;
		vector<double> num_terrain_cells = { 0.2 * size_map,0.3 * size_map,0.3 * size_map,0.2 * size_map };
		/*for (int i = 0; i < num_terrain_cells.size(); i++)
			cout << num_terrain_cells[i] << endl;*/

		vector<int> set;
		set.resize(size_map);

		vector<int> check_size = { 0,0,0,0 };
		for (int i = 0; i < set.size(); i++)
		{
			if (i < num_terrain_cells[0])
			{
				set[i] = 0;
				check_size[0]++;
			}
			else if (i >= num_terrain_cells[0] && i < num_terrain_cells[0] + num_terrain_cells[1])
			{
				set[i] = 1;
				check_size[1]++;

			}
			else if (i >= num_terrain_cells[0] + num_terrain_cells[1] && i < num_terrain_cells[0] + num_terrain_cells[1] + num_terrain_cells[2])
			{
				set[i] = 2;
				check_size[2]++;

			}
			else
			{
				set[i] = 3;
				check_size[3]++;

			}
		}
		/*for (int i = 0; i < 4; i++)
		{
			cout << i << "." << check_size[i] << endl;
		}*/

		//        for (int i = 0; i < 2500; i++)
		//        {
		//            if(i%50==0)
		//                cout << endl;
		//            cout << set[i] << " ";
		//
		//        }
		cout << endl;
		srand(time(0));

		for (int i = 0; i < size_map; i++)
		{
			
			if (rand() % 2 == 0)
			{
				int x = i;

				int x_ran = rand() % size_map;

				swap(x, x_ran, set);
			}
		}

		for (int i = 0; i < size_map; i++)
		{
			int y = i % dim;
			int x = i / dim;

			(map_env[y][x]).terrain_type = set[i];
		}
		insert_target();
		target.Set_terrain(map_env[target.ReturnY()][target.ReturnX()].terrain_type);
		//cout << "targets terrain type- "<<target.Return_terrain() << endl;
	}
	void swap(int x, int x_ran, vector<int>& set)
	{
		int temp = set[x];
		set[x] = set[x_ran];
		set[x_ran] = temp;
	}

	void insert_target()
	{

		int p1, p2;
		srand(time(0));
		p1 = 0 + rand() % (dim);
		p2 = 0 + rand() % (dim);
		//cout << "target at coordinates "<<p1 << " , " << p2 << endl;
		target.SetX(p2);
		target.SetY(p1);
	
	}

	void change_target()
	{
		int y = target.ReturnY();
		int x = target.ReturnX();

		vector<point> neighbor;
		if (y - 1 >= 0)
		{
			point p(y - 1, x, map_env[y - 1][x].terrain_type);
			neighbor.push_back(p);
		}
		if (x - 1 >= 0)
		{
			point p(y, x - 1, map_env[y][x-1].terrain_type);
			neighbor.push_back(p);
		}
		if (y + 1 < dim)
		{
			point p(y + 1, x, map_env[y + 1][x].terrain_type);
			neighbor.push_back(p);
		}
		if (x + 1 < dim)
		{
			point p(y, x + 1, map_env[y][x+1].terrain_type);
			neighbor.push_back(p);
		}

		int size = neighbor.size();
		//int k = 0 + rand() % neighbor.size();
		random_device rd;
		uniform_int_distribution<int> distribution(0, size-1);
		mt19937 engine(rd()); // Mersenne twister MT19937

		int k = distribution(engine);
		target = neighbor[k];
	}

	void print_board()
	{
		cout << "Printing Map:" << endl << endl;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				cout << map_env[i][j].terrain_type << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	void print_probability_board()
	{
		cout << "Printing Probability Map:" << endl << endl;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				cout << map_prob[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "actions taken till now using rule 1 " << action_rule_1 << endl;
	}

	void print_probability_board_found()
	{
		cout << "Printing Probability Map for finding in cell:" << endl << endl;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				cout << map_prob_found[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "actions taken till now using rule 2 " << action_rule_2 << endl;
	}

	void print_probability_board_distance()
	{
		cout << "Printing Probability Map for distance metric:" << endl << endl;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				cout << map_prob_distance[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "actions taken till now using rule distance " << action_rule_distance << endl;
	}

	bool search_cell(int p1, int p2)
	{
		bool result = true;

		action_rule_1++;
		action_rule_2++;
		action_rule_distance++;


		if (target.ReturnY() != p1 || target.ReturnX() != p2)
		{
			result = false;
			return result;
		}
		
		//cout << "terrain is "<<map_env[p1][p2].Return_terrain() << endl;
		
		//srand(time(0));
		//bool TrueFalse = (rand() % 100) < 75;
		

		//cout << "target at "<<target.ReturnY() << target.ReturnX() << endl;
		
		random_device rd;
		uniform_int_distribution<int> distribution(1, 100);
		mt19937 engine(rd()); // Mersenne twister MT19937

		int value = distribution(engine);
		if (value <= prob_target_not_found[map_env[p1][p2].Return_terrain()])
			result = false;
		
		return result;
	}

	void update_belief_rule_1(int p1, int p2)
	{
		vector<double> actual_prob_target_not_found = { 0.1,0.3,0.7,0.9 };
		double denominator;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				denominator = (map_prob[i][j] * (actual_prob_target_not_found[map_env[p1][p2].Return_terrain()]) ) + (1 - map_prob[i][j]);
				
				if (i == p1 && j == p2)
					map_prob[i][j] = ( map_prob[i][j] * (actual_prob_target_not_found[map_env[p1][p2].Return_terrain()]) ) / denominator;

				else
					map_prob[i][j] = map_prob[i][j] / denominator;
			}
	
		}
	}

	void update_belief_rule_2(int p1, int p2)
	{
		vector<double> actual_prob_target_found = { 0.9,0.7,0.3,0.1 };
		double denominator;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				map_prob_found[i][j]= actual_prob_target_found[map_env[p1][p2].Return_terrain()] * map_prob[i][j];
			}

		}
	}

	void update_belief_rule_distance(int p1, int p2)
	{
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				map_prob_distance[i][j] = map_prob[i][j] * (1.0/ ( abs(p1 - i) + abs(p2 - j) + 1) );
			}

		}
	}

	point find_next_cell_rule_1(point &current_cell, int terrain)
	{
		double max = 0.0;
		point next_cell;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				if (map_prob[i][j] > max && map_env[i][j].terrain_type != terrain)
				{
					max = map_prob[i][j];
					next_cell.SetY(i);
					next_cell.SetX(j);
				}
					
			}
		}
		action_rule_1+=get_manhattan_distance(current_cell, next_cell);
		return next_cell;		
	}
	point find_next_cell_rule_1()
	{
		double max = 0.0;
		point next_cell;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				if (map_prob[i][j] > max )
				{
					max = map_prob[i][j];
					next_cell.SetY(i);
					next_cell.SetX(j);
				}

			}
		}

		return next_cell;
	}

	point find_next_cell_rule_2(point& current_cell)
	{
		double max = 0.0;
		point next_cell;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				if (map_prob_found[i][j] > max)
				{
					max = map_prob_found[i][j];
					next_cell.SetY(i);
					next_cell.SetX(j);
				}

			}
		}
		action_rule_2 += get_manhattan_distance(current_cell, next_cell);
		return next_cell;
	}

	point find_next_cell_rule_distance(point& current_cell, int terrain)
	{
		double max = 0.0;
		point next_cell;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{

				if (map_prob_distance[i][j] > max && map_env[i][j].terrain_type != terrain)
				{
					max = map_prob_distance[i][j];
					next_cell.SetY(i);
					next_cell.SetX(j);
				}

			}
		}

		action_rule_distance += get_manhattan_distance(current_cell, next_cell);
		return next_cell;
	}



	int get_manhattan_distance(point p1, point p2)
	{
		return abs( p1.ReturnX()-p2.ReturnX() ) + abs( p1.ReturnY() - p2.ReturnY() );
	}

	int get_action_rule_1()
	{
		return action_rule_1;
	}

	int get_action_rule_2()
	{
		return action_rule_2;
	}

	int get_action_rule_distance()
	{
		return action_rule_distance;
	}

	int get_target_type()
	{
		return target.Return_terrain();
	}

	point return_target()
	{
		return target;
	}
private:

	vector<vector<point>>  map_env;
	vector<vector<double>> map_prob;
	vector<vector<double>> map_prob_found;
	vector<vector<double>> map_prob_distance;
	vector<double> prob_target_not_found = { 10,30,70,90 };
	int dim;
	point target;
	int action_rule_1 = 0, action_rule_2 = 0, action_rule_distance = 0;
};


#endif /* map_h */