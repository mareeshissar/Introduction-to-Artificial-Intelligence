#pragma once
//
//  point.h
//  AI_assignment_2
//
//
//

#ifndef point_h
#define point_h

#include <iostream>

class Point
{
public:
	Point()                     //initiate with default value of -100
	{
		y = -100;
		x = -100;
	}

	Point(int a, int b)
	{
		y = a;
		x = b;	
		isHiddenNeigbor = false;
	}
	
	int ReturnX()
	{
		return x;
	}
	int ReturnY()
	{
		return y;
	}

	void SetIsHiddenNeighbor()
	{
		isHiddenNeigbor = true;
	}
	
	bool GetIsHiddenNeighbor()
	{
		return isHiddenNeigbor == true;
	}

private:
	int x;              //x-coordinate
	int y;              //y-coordinate
	bool isHiddenNeigbor;			//for use in CSP
};

#endif /* point_h */

