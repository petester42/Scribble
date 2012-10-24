/* 
 * File:   Points.h
 * Author: greg
 *
 * Created on January 14, 2012, 3:03 PM
 */

#ifndef POINTS_H
#define	POINTS_H

#include "defines.h"
#include "QPoint"
#include <iostream>

class Point : public QPoint
{
public:
    explicit Point(int column, int row, int X, int Y);
    //Point(int column, int row, int X, int Y);
    Point(const Point& orig);
    Point();
    virtual ~Point();

    //Getters
    int getColumn() const;
    int getRow() const;
    void adjustForSaving();

private:
    int column;
    int row;
};

#endif	/* POINTS_H */


