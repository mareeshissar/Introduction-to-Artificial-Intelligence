#pragma once
//
//  point.h
//  AI_assignment_3
//
//
//

#ifndef point_h
#define point_h

#include <iostream>
using namespace std;

class point
{
public:
	int terrain_type;
	point()                     //initiate with default value of -100
	{
		y = -100;
		x = -100;
		terrain_type = -100;
	}
	point(int a, int b, int c)
	{
		y = a;
		x = b;
		terrain_type = c;
	}

	int ReturnX()
	{
		return x;
	}
	int ReturnY()
	{
		return y;
	}
	void SetX(int value)
	{
		x = value;
	}
	void SetY(int value)
	{
		y=value;
	}
	int Return_terrain()
	{
		return terrain_type;
	}
	void print()
	{
		cout << "(" << y << "," << x << ")";
	}

private:
	int x;              //x-coordinate
	int y;              //y-coordinate


};

#endif /* point_h */