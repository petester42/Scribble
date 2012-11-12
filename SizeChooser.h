/* 
 * File:   SizeChooser.h
 * Author: greg
 *
 * Created on July 25, 2012, 12:17 PM
 */

#ifndef SIZECHOOSER_H
#define	SIZECHOOSER_H
#include "QPainter"
#include "Point.h"
#include "Defines.h"
#include <sstream>

class SizeChooser
{
public:
    SizeChooser(int size);
    virtual ~SizeChooser();
    void displaySizeChooser(QPainter &painter);
    int readSize(Point* point);
    int getSize();

    enum
    {
        OK, CANCEL, NONE
    };
private:
    int penSize;
    int row1(Point* point);
    int row2(Point* point);
    int row3(Point* point);

    enum
    {
        SIZE_W = 50,
        SIZE_H = 40,
        SIZE_S = 15,
        MAX_PEN_SIZE = 30,
        MIN_PEN_SIZE = 1
    };
};

#endif	/* SIZECHOOSER_H */

