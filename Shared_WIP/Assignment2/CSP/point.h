#pragma once
//
//  point.h
//  AI_assignment_2
//
//  Created by Mareesh Kumar Issar on 10/04/19.
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
		isHiddenNeigbor = false;
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

	void SetIsHiddenNeighbor(int val)
	{
		isHiddenNeigbor = val;
	}
	
	int GetIsHiddenNeighbor()
	{
		return isHiddenNeigbor == 1;
	}
	void SetPointClueValue(int v)
	{
		pointClueValue = v;
	}

	int GetPointClueValue()
	{
		return pointClueValue;
	}


private:
	int x;              //x-coordinate
	int y;              //y-coordinate
	int isHiddenNeigbor;			//for use in CSP
	int pointClueValue;			//value of the clue
};

#endif /* point_h */