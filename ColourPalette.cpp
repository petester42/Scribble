/* 
 * File:   ChooseColour.cpp
 * Author: greg
 * 
 * Created on July 23, 2012, 2:17 PM
 */

#include "ColourPalette.h"

ColourPalette::ColourPalette() : currentColour("#000000")
{
    initializeColors();
    width = (3 * WIDTH / 5 - 3 * COLOUR_S) / 3;
}

ColourPalette::~ColourPalette()
{
}

/** Display the colour palette
 * 
 * This function display the color palette
 * 
 * @param painter The painter on which we can draw the colour palette
 */
void ColourPalette::displayColourPalette(QPainter &painter)
{
    painter.setBrush(QBrush("#FFF8C6"));
    painter.drawRect(WIDTH / 5, HEIGHT / 5, 3 * WIDTH / 5, 6 * COLOUR_H + 5 * COLOUR_S);
    //painter.setBrush(QBrush("#FFF8C6"));

    //Row 1
    painter.setBrush(QBrush(colors[0][0].c_str()));
    painter.drawRect(WIDTH / 5 + 0 * width + COLOUR_S / 2, HEIGHT / 5 + COLOUR_S / 2, width, COLOUR_H);

    painter.setBrush(QBrush(colors[1][0].c_str()));
    painter.drawRect(WIDTH / 5 + 1 * width + 3 * COLOUR_S / 2, HEIGHT / 5 + COLOUR_S / 2, width, COLOUR_H);

    painter.setBrush(QBrush(colors[2][0].c_str()));
    painter.drawRect(WIDTH / 5 + 2 * width + 5 * COLOUR_S / 2, HEIGHT / 5 + COLOUR_S / 2, width, COLOUR_H);

    //Row 2
    painter.setBrush(QBrush(colors[0][1].c_str()));
    painter.drawRect(WIDTH / 5 + 0 * width + COLOUR_S / 2, HEIGHT / 5 + 3 * COLOUR_S / 2 + COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[1][1].c_str()));
    painter.drawRect(WIDTH / 5 + 1 * width + 3 * COLOUR_S / 2, HEIGHT / 5 + 3 * COLOUR_S / 2 + COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[2][1].c_str()));
    painter.drawRect(WIDTH / 5 + 2 * width + 5 * COLOUR_S / 2, HEIGHT / 5 + 3 * COLOUR_S / 2 + COLOUR_H, width, COLOUR_H);

    //Row 3
    painter.setBrush(QBrush(colors[0][2].c_str()));
    painter.drawRect(WIDTH / 5 + 0 * width + COLOUR_S / 2, HEIGHT / 5 + 5 * COLOUR_S / 2 + 2 * COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[1][2].c_str()));
    painter.drawRect(WIDTH / 5 + 1 * width + 3 * COLOUR_S / 2, HEIGHT / 5 + 5 * COLOUR_S / 2 + 2 * COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[2][2].c_str()));
    painter.drawRect(WIDTH / 5 + 2 * width + 5 * COLOUR_S / 2, HEIGHT / 5 + 5 * COLOUR_S / 2 + 2 * COLOUR_H, width, COLOUR_H);

    //Row 4
    painter.setBrush(QBrush(colors[0][3].c_str()));
    painter.drawRect(WIDTH / 5 + 0 * width + COLOUR_S / 2, HEIGHT / 5 + 7 * COLOUR_S / 2 + 3 * COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[1][3].c_str()));
    painter.drawRect(WIDTH / 5 + 1 * width + 3 * COLOUR_S / 2, HEIGHT / 5 + 7 * COLOUR_S / 2 + 3 * COLOUR_H, width, COLOUR_H);

    painter.setBrush(QBrush(colors[2][3].c_str()));
    painter.drawRect(WIDTH / 5 + 2 * width + 5 * COLOUR_S / 2, HEIGHT / 5 + 7 * COLOUR_S / 2 + 3 * COLOUR_H, width, COLOUR_H);

    //Seperation line
    painter.drawLine(WIDTH / 5, HEIGHT / 5 + 9 * COLOUR_S / 2 + 4 * COLOUR_H, WIDTH / 5 + 3 * WIDTH / 5, HEIGHT / 5 + 9 * COLOUR_S / 2 + 4 * COLOUR_H);

    //current colour
    painter.setBrush(QBrush(currentColour.c_str()));
    painter.drawRect(WIDTH / 5 + COLOUR_S / 2, HEIGHT / 5 + 11 * COLOUR_S / 2 + 4 * COLOUR_H, width, 1.5 * COLOUR_H);

    QRect rect = QRect(WIDTH / 5 + 3 * COLOUR_S / 2 + 1 * width, HEIGHT / 5 + 12 * COLOUR_S / 2 + 4 * COLOUR_H, width / 2, COLOUR_H);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setBrush(QBrush("#FFF8D6"));
    painter.drawRect(WIDTH / 5 + 3 * COLOUR_S / 2 + 1 * width, HEIGHT / 5 + 12 * COLOUR_S / 2 + 4 * COLOUR_H, width / 2, COLOUR_H);
    painter.drawText(rect, Qt::AlignCenter, "OK");

    painter.drawRect(WIDTH / 5 + 5 * COLOUR_S / 2 + 1.5 * width, HEIGHT / 5 + 12 * COLOUR_S / 2 + 4 * COLOUR_H, 3 * width / 2, COLOUR_H);
    rect.adjust(2.25 * width, 0, 0, 0);
    painter.drawText(rect, Qt::AlignCenter, "Cancel");
}

/** Read the colour from the colour palette
 * 
 * @param point The point that will be matched to a colour from the colour palette
 * @return The integer corresponding to the chosen colour, or NONE if nothing has been chosen
 */
int ColourPalette::readColour(Point* point)
{
    if (point->y()<(HEIGHT / 5 + COLOUR_S / 2))
    {
        //std::cout << "No good" << std::endl;
        return NONE;
    }
    else if (point->y() <(HEIGHT / 5 + 3 * COLOUR_S / 2 + COLOUR_H))
    {
        //std::cout << "Row 1" << std::endl;
        return row1(point);

    }
    else if (point->y() < (HEIGHT / 5 + 5 * COLOUR_S / 2 + 2 * COLOUR_H))
    {
       // std::cout << "Row 2" << std::endl;
        return row2(point);

    }
    else if (point->y()<(HEIGHT / 5 + 7 * COLOUR_S / 2 + 3 * COLOUR_H))
    {
        //std::cout << "Row 3" << std::endl;
        return row3(point);
    }
    else if (point->y()<(HEIGHT / 5 + 9 * COLOUR_S / 2 + 4 * COLOUR_H))
    {
        //std::cout << "Row 4" << std::endl;
        return row4(point);
    }
    else if (point->y()<(HEIGHT / 5 + 10 * COLOUR_S / 2 + 4.5 * COLOUR_H))
    {
        //std::cout << "In between OK and colour, no good" << std::endl;
        return NONE;
    }
    else if (point->y()<(HEIGHT / 5 + 12 * COLOUR_S / 2 + 5 * COLOUR_H))
    {
        //std::cout << "OK" << std::endl;
        return row5(point);
    }
    else
    {
        //std::cout << "No good, to low" << std::endl;
        return NONE;
    }
}

/**Read colours from row 1
 * 
 * @param point The point that corresponds to row 1 of the colours palette
 * @return Always NONE, since this will only change the chosen colours but it will only be activated when the OK button is pressed 
 */
int ColourPalette::row1(Point* point)
{
    if (point->x()<(WIDTH / 5 + 0 * width + 0 * COLOUR_S))
    {
        //std::cout << "-1" << std::endl;
        return NONE;
    }
    else if (point->x()<(WIDTH / 5 + 1 * width + 1 * COLOUR_S))
    {
        //std::cout << "1" << std::endl;
        currentColour= colors[0][0];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 2 * width + 2 * COLOUR_S)
    {
        //std::cout << "2" << std::endl;
        currentColour= colors[1][0];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 3 * width + 3 * COLOUR_S)
    {
        //std::cout << "3" << std::endl;
        currentColour= colors[2][0];
        return NONE;
    }
    else
    {
        //std::cout << "9" << std::endl;
        return NONE;
    }
}

/**Read colours from row 2
 * 
 * @param point The point that corresponds to row 2 of the colours palette
 * @return Always NONE, since this will only change the chosen colours but it will only be activated when the OK button is pressed 
 */
int ColourPalette::row2(Point* point)
{
    if (point->x()<(WIDTH / 5 + 0 * width + 0 * COLOUR_S))
    {
        //std::cout << "-1" << std::endl;
        return NONE;
    }
    else if (point->x()<(WIDTH / 5 + 1 * width + 1 * COLOUR_S))
    {
        //std::cout << "1" << std::endl;
        currentColour= colors[0][1];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 2 * width + 2 * COLOUR_S)
    {
        //std::cout << "2" << std::endl;
        currentColour= colors[1][1];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 3 * width + 3 * COLOUR_S)
    {
        //std::cout << "3" << std::endl;
        currentColour= colors[2][1];
        return NONE;
    }
    else
    {
        //std::cout << "9" << std::endl;
        return NONE;
    }
}

/**Read colours from row 3
 * 
 * @param point The point that corresponds to row 3 of the colours palette
 * @return Always NONE, since this will only change the chosen colours but it will only be activated when the OK button is pressed 
 */
int ColourPalette::row3(Point* point)
{
    if (point->x()<(WIDTH / 5 + 0 * width + 0 * COLOUR_S))
    {
        //std::cout << "-1" << std::endl;
        return NONE;
    }
    else if (point->x()<(WIDTH / 5 + 1 * width + 1 * COLOUR_S))
    {
        //std::cout << "1" << std::endl;
        currentColour= colors[0][2];
        return NONE;

    }
    else if (point->x() < WIDTH / 5 + 2 * width + 2 * COLOUR_S)
    {
       // std::cout << "2" << std::endl;
        currentColour= colors[1][2];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 3 * width + 3 * COLOUR_S)
    {
        //std::cout << "3" << std::endl;
        currentColour= colors[2][2];
        return NONE;
    }
    else
    {
        //std::cout << "9" << std::endl;
        return NONE;
    }
}

/**Read colours from row 4
 * 
 * @param point The point that corresponds to row 4 of the colours palette
 * @return Always NONE, since this will only change the chosen colours but it will only be activated when the OK button is pressed 
 */
int ColourPalette::row4(Point* point)
{
    if (point->x()<(WIDTH / 5 + 0 * width + 0 * COLOUR_S))
    {
        //std::cout << "-1" << std::endl;
        return NONE;;
    }
    else if (point->x()<(WIDTH / 5 + 1 * width + 1 * COLOUR_S))
    {
        //std::cout << "1" << std::endl;
        currentColour= colors[0][3];
        return NONE;;
    }
    else if (point->x() < WIDTH / 5 + 2 * width + 2 * COLOUR_S)
    {
        //std::cout << "2" << std::endl;
        currentColour= colors[1][3];
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 3 * width + 3 * COLOUR_S)
    {
        //std::cout << "3" << std::endl;
        currentColour= colors[2][3];
        return NONE;
    }
    else
    {
        //std::cout << "9" << std::endl;
        return NONE;
    }
}

/**Read from row 5
 * 
 * @param point The point that corresponds to row 5 of the colours palette
 * @return OK or Cancel depending on what button the user pressed, or NONE if no button was pressed
 */
int ColourPalette::row5(Point* point)
{
    if (point->x()<(WIDTH / 5 + 1 * width + 1 * COLOUR_S))
    {
        //std::cout<<"-1"<<std::endl;
        return NONE;
    }
    else if (point->x() < WIDTH / 5 + 2 * COLOUR_S + 1.5 * width)
    {
        //std::cout << "OK" << std::endl;
        return OK;
    }
    else if (point->x() < WIDTH / 5 + 3 * width + 3 * COLOUR_S)
    {
        //std::cout << "Cancel" << std::endl;
        return CANCEL;
    }
    else
    {
        //std::cout<<"9"<<std::endl;
        return NONE;
    }
}

/** Initialize the colours 2D matrix
 * 
 */
void ColourPalette::initializeColors()
{
    //Row 1
    colors[0][0]="#000000"; //black
    colors[1][0]="#585858"; //grey
    colors[2][0]="#FF0000"; //red
    
    //Row 2
    colors[0][1]="#0101DF"; //dark blue
    colors[1][1]="#0080FF"; //blue
    colors[2][1]="#00FFFF"; //light blue
    
    //Row 3
    colors[0][2]="#DF01A5"; //purple
    colors[1][2]="#FF8000"; //orange
    colors[2][2]="#FFFF00"; //yellow
    
    //Row 4
    colors[0][3]="#173B0B"; //dark green
    colors[1][3]="#31B404"; //green
    colors[2][3]="#9FF781"; //light green
}

/**Get the colour the user has chosen
 * 
 * This function should be called after the OK button was pressed. It will return the colour the user has chosen
 * 
 * @return The colour the user has chosen (pressed on)
 */
std::string ColourPalette::getColour()
{
    return currentColour;
}