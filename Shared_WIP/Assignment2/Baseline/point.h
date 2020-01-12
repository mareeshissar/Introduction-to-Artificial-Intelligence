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
        }

        int ReturnX()
        {
                return x;
        }
        int ReturnY()
        {
                return y;
        }

private:
        int x;              //x-coordinate
        int y;              //y-coordinate

};

#endif /* point_h */

