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
	}
	Point(int a, int b)
	{
		y = a;
		x = b;
	}
	
	int ReturnX()
	{
		return x;
	}
	int ReturnY()
	{
		return y;
	}
	void SetVisited()
	{
		visited=true;
	}
	bool GetVisited()
	{
		return visited;
	}
	
	//bool operator <(const Point& a) const               //define operator < for queue
	//{
	//	return ((this->priority) > a.priority);
	//}
	//bool operator >(const Point& a) const               //define operator > for queue
	//{
	//	return ((this->priority) > a.priority);
	//}
	//struct Comparepoints {                                      //define compartor for STLs
	//	bool operator()(Point const& p1, Point const& p2)
	//	{
	//		return p1.priority < p2.priority;
	//	}
	//};
private:
	int x;              //x-coordinate
	int y;              //y-coordinate
	bool visited = false;					//checks whether cell was visited before
};

#endif /* point_h */