//
//  main.cpp
//  assignment3_AI
//
//  
//  
//

#include <iostream>
#include "map.h"

int main(int argc, const char* argv[]) {

	vector<int> rule_1;
	vector<int> rule_distance;
	vector<int> target_type_count(4, 0);
	vector<double> terrain_type_average_rule_1(4, 0.0);
	vector<double> terrain_type_average_rule_distance(4, 0.0);

	double rule_1_avg=0.0;
	double rule_distance_avg=0.0;

	int num_iter=10;
	for (int i = 0; i < num_iter; i++)
	{
		cout << "Iteration " << i + 1 << endl;
		bool target_found = false;
		int p1, p2, d = 40;
		map map1, map2;
		//map1.print_board();

		target_type_count[map1.get_target_type()]++;
		//map1.print_probability_board();

		map2 = map1;

		srand(983);
		int count = 0;
		int step = 1;
		point current_cell, temp_cell;
		while (target_found == false)
		{
			if (count == 0)
			{
				p1 = 0 + rand() % (d);
				p2 = 0 + rand() % (d);
				current_cell.SetY(p1);
				current_cell.SetX(p2);

				count++;
			}


			else
			{
				//comment the lines below while using rule 2
				//temp_cell = map1.find_next_cell_rule_1(current_cell);
				//p1 = temp_cell.ReturnY();
				//p2 = temp_cell.ReturnX();

				//comment the lines below while using rule 1
				//temp_cell = map1.find_next_cell_rule_2(current_cell);
				//p1 = temp_cell.ReturnY();
				//p2 = temp_cell.ReturnX();


				temp_cell = map1.find_next_cell_rule_distance(current_cell);
				p1 = temp_cell.ReturnY();
				p2 = temp_cell.ReturnX();

				current_cell.SetY(p1);
				current_cell.SetX(p2);

			}

			//cout << p1 << " , " << p2<<endl;
			target_found = map1.search_cell(p1, p2);

			if (target_found == true)
				break;

			//map1.update_belief_rule_1(p1, p2);
			//map1.print_probability_board(); 

			//uncomment the lines below to get results using rule 2
			//map1.update_belief_rule_2(p1, p2);
			//map1.print_probability_board_found();

			//part 4 ans
			map1.update_belief_rule_1(p1, p2);
			map1.update_belief_rule_distance(p1, p2);


			step++;
		}
		//map1.print_probability_board_distance();
	    rule_distance.push_back(map1.get_action_rule_distance());
		terrain_type_average_rule_distance[map1.get_target_type()]+= map1.get_action_rule_distance();

		//cout << "number of steps for rule distance " << step << " for iteration " << i+1 << endl;

		//map2.print_board();
		//map2.print_probability_board();

		count = 0;
		step = 1;
		target_found = false;
		srand(983);
		while (target_found == false)
		{
			if (count == 0)
			{
				p1 = 0 + rand() % (d);
				p2 = 0 + rand() % (d);
				current_cell.SetY(p1);
				current_cell.SetX(p2);

				count++;
			}


			else
			{
				//comment the lines below while using rule 2
				//temp_cell = map1.find_next_cell_rule_1(current_cell);
				//p1 = temp_cell.ReturnY();
				//p2 = temp_cell.ReturnX();

				//comment the lines below while using rule 1
				//temp_cell = map1.find_next_cell_rule_2(current_cell);
				//p1 = temp_cell.ReturnY();
				//p2 = temp_cell.ReturnX();


				temp_cell = map2.find_next_cell_rule_1(current_cell);
				p1 = temp_cell.ReturnY();
				p2 = temp_cell.ReturnX();

				current_cell.SetY(p1);
				current_cell.SetX(p2);

			}

			//cout << p1 << " , " << p2 << endl;
			target_found = map2.search_cell(p1, p2);

			if (target_found == true)
				break;

			//map1.update_belief_rule_1(p1, p2);
			//map1.print_probability_board(); 

			//uncomment the lines below to get results using rule 2
			//map1.update_belief_rule_2(p1, p2);
			//map1.print_probability_board_found();

			//part 4 ans
			map2.update_belief_rule_1(p1, p2);
			step++;
		}
		//map2.print_probability_board();
		rule_1.push_back(map2.get_action_rule_1());
		terrain_type_average_rule_1[map2.get_target_type()]+= map2.get_action_rule_1();

		//cout << "number of steps for rule 1 " << step <<" for iteration "<<i+1<< endl;
	}
	
	for (int i = 0; i < rule_1.size(); i++)
	{
		rule_1_avg += rule_1[i];
	}
	cout << "Average number of actions using rule 1- " << rule_1_avg / num_iter << endl;

	for (int i = 0; i < rule_distance.size(); i++)
	{
		rule_distance_avg += rule_distance[i];
	}
	cout << "Average number of actions using rule distance- " << rule_distance_avg / num_iter << endl;
	
	for (int i = 0; i < target_type_count.size(); i++)
	{
		cout << "Target of type " << i << " has count " << target_type_count[i] << endl;
	}

	for (int i = 0; i < terrain_type_average_rule_1.size(); i++)
	{
		cout << "Average number of actions (rule 1) for target of type " << i << " is " << terrain_type_average_rule_1[i]/num_iter << endl;
	}
	for (int i = 0; i < terrain_type_average_rule_distance.size(); i++)
	{
		cout << "Average number of actions (rule distance) for target of type " << i << " is " << terrain_type_average_rule_distance[i] / num_iter << endl;
	}
	

    return 0;
}