/* 
 * File:   ChooseColour.h
 * Author: greg
 *
 * Created on July 23, 2012, 2:17 PM
 */

#ifndef COLORPALETTE_H
#define	COLORPALETTE_H

#include "Defines.h"
#include "QPainter"
#include "Point.h"

class ColourPalette
{
public:
    ColourPalette();
    virtual ~ColourPalette();
    void displayColourPalette(QPainter &painter);
    int readColour(Point* point);
    std::string getColour();

    enum
    {
        OK, CANCEL, NONE
    };
private:
    void initializeColors();
    int row1(Point* point);
    int row2(Point* point);
    int row3(Point* point);
    int row4(Point* point);
    int row5(Point* point);
    std::string colors[3][4];

    int width;
    std::string currentColour;

    enum
    {
        COLOUR_W = 40,
        COLOUR_H = 40,
        COLOUR_S = 15
    };

};

#endif	/* COLORPALETTE_H */

