//
// Point.h
//
// Jack Murdock
//

#ifndef __POINT_H_
#define __POINT_H_

#include <iostream>
using namespace std;


class Point {
private:
    int xval, yval;
public:
    // Constructor uses default arguments to allow calling with zero, one,
    // or two values.
    Point(int x = -1, int y = -1) {
        xval = x;
        yval = y;
    }
    
    // Extractors.
    const int x() { return xval; }
    const int y() { return yval; }
    
    // Distance to another point.  Pythagorean thm.
    int dist(Point other) {
        int xd = xval - other.xval;
        int yd = yval - other.yval;
        return sqrt(xd*xd + yd*yd);
    }
    
    // Add or subtract two points.
    Point operator+(Point& b)
    {
        return Point(xval + b.xval, yval + b.yval);
    }
    Point operator-(Point& b)
    {
        return Point(xval - b.xval, yval - b.yval);
    }
    
    // Move the existing point.
    void move(int a, int b)
    {
        xval += a;
        yval += b;
    }
    
    friend ostream &operator<<(ostream &output, const Point &p)
    {
        output << "(" << p.xval << "," << p.yval << ")";
        return output;
    }
};

#endif