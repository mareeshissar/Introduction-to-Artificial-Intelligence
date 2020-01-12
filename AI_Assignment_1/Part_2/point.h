//
//  point.h
//  AI_assignment1
//
//  Created by Kartik Rattan on 9/22/19.
//  Copyright © 2019 Kartik Rattan. All rights reserved.
//

#ifndef point_h
#define point_h

class point
{
public:
    point()                     //initiate a default point with -100
    {
        y=-100;
        x=-100;
    }
    point(int a,int b)
    {
        y = a;
        x = b;
    }
    double priority;    //priority variable
    int return_x()
    {
        return x;
    }
    int return_y()
    {
        return y;
    }
    double return_priority()
    {
        return priority;
    }
    void print_point()
    {
        cout << "("<<y<<","<<x<<")";
    }
    bool operator <(const point& a) const               //define operator < for queue
    {
        return ((this->priority) > a.priority);
    }
    bool operator >(const point& a) const               //define operator > for queue
    {
        return ((this->priority) > a.priority);
    }
    struct Comparepoints {                                      //define compartor for STLs
        bool operator()(point const& p1, point const& p2)
        {
            return p1.priority < p2.priority;
        }
    };
private:
    int x;              //x-cord
    int y;              //y-cord
};

#endif /* point_h */
