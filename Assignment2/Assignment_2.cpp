//
//  board_formation.h
//  AI_assignment_2
//
//  Created by Mareesh Kumar Issar on 10/04/19.
// 
//

#include "point.h"
#include "board_formation.h"

#include <iostream>

int main()
{
	int d, n;

	cout << "Enter the dimension:";
	cin >> d;
	cout << "Enter the number of mines:";
	cin >> n;
	Board b1(d, n);
	b1.PrintActualBoard();
	b1.PrintUserBoard();
	srand(23);
	for (int i = 0; i < 5; i++)         //swap mine with an open position
	{
		int p1;
		int p2;
		
		p1 = 0 + rand() % (d);
		p2 = 0 + rand() % (d);
		
		cout << i + 1 << ". " << "(" << p1 << "," << p2 << ")" << endl;               //printing the open position to swap
		b1.CheckCell(p1, p2);
		b1.PrintUserBoard();
	}
	
	return 0;

}
