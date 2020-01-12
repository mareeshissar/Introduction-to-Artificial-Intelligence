//
//  main.cpp
//  assignment3_AI
//
//  
//  
//

#include <iostream>
# include "map.h"

int main(int argc, const char* argv[]) {

	bool target_found=false;
	int p1, p2,d=4;
	map map1;
	map1.print_board();
	map1.print_probability_board();

	srand(983);
	int count = 0;
	int step = 1;
	while (target_found == false)
	{
		if (count == 0)
		{
			p1 = 0 + rand() % (d);
			p2 = 0 + rand() % (d);
			count++;
		}
		
		else
		{
			//comment the lines below while using rule 2
			p1 = map1.find_next_cell_rule_1().ReturnY();
			p2 = map1.find_next_cell_rule_1().ReturnX();

			//comment the lines below while using rule 1
			//p1 = map1.find_next_cell_rule_2().ReturnY();
			//p2 = map1.find_next_cell_rule_2().ReturnX();
		}
		
		cout << p1 << " , " << p2<<endl;
		target_found=map1.search_cell(p1, p2);
		
		if (target_found == true)
			break;

		map1.update_belief_rule_1(p1, p2);
		map1.print_probability_board(); 

		//uncomment the lines below to get results using rule 2
		//map1.update_belief_rule_2(p1, p2);
		//map1.print_probability_board_found();
		
		step++;
	}
	cout << "number of steps " << step << endl;

    return 0;
}