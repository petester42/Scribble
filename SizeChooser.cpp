/* 
 * File:   SizeChooser.cpp
 * Author: greg
 * 
 * Created on July 25, 2012, 12:17 PM
 */

#include "SizeChooser.h"

SizeChooser::SizeChooser(int size) : penSize(size)
{
}

SizeChooser::~SizeChooser()
{
}

/** Display the size chooser window
 * 
 * @param painter The painter on which the size chooser window can be drawn on
 */
void SizeChooser::displaySizeChooser(QPainter &painter)
{
    painter.setBrush(QBrush("#FFF8D6"));
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawRect(WIDTH / 3, HEIGHT / 13, 3 * SIZE_W + 3 * SIZE_S, 3 * SIZE_H + 5 * SIZE_S);

    painter.setBrush(QBrush("#FFF8C6"));
    painter.drawRect(WIDTH / 3 + SIZE_S, HEIGHT / 13 + SIZE_S, 2 * SIZE_W, 2 * SIZE_H + SIZE_S);
    std::ostringstream convert;
    convert << penSize;
    painter.drawText(QRect(WIDTH / 3 + SIZE_S, HEIGHT / 13 + SIZE_S, 2 * SIZE_W, 2 * SIZE_H + SIZE_S), Qt::AlignCenter, convert.str().c_str());

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawRect(WIDTH / 3 + 2 * SIZE_S + 2 * SIZE_W, HEIGHT / 13 + SIZE_S, SIZE_W, SIZE_H);
    painter.drawText(QRect(WIDTH / 3 + 2 * SIZE_S + 2 * SIZE_W, HEIGHT / 13 + SIZE_S, SIZE_W, SIZE_H), Qt::AlignCenter, "Up");

    painter.drawRect(WIDTH / 3 + 2 * SIZE_S + 2 * SIZE_W, HEIGHT / 13 + 2 * SIZE_S + SIZE_H, SIZE_W, SIZE_H);
    painter.drawText(QRect(WIDTH / 3 + 2 * SIZE_S + 2 * SIZE_W, HEIGHT / 13 + 2 * SIZE_S + SIZE_H, SIZE_W, SIZE_H), Qt::AlignCenter, "Down");

    painter.drawRect(WIDTH / 3 + SIZE_S, HEIGHT / 13 + 4 * SIZE_S + 2 * SIZE_H, 1.5 * SIZE_W, SIZE_H);
    painter.drawText(QRect(WIDTH / 3 + SIZE_S, HEIGHT / 13 + 4 * SIZE_S + 2 * SIZE_H, 1.5 * SIZE_W, SIZE_H), Qt::AlignCenter, "OK");

    painter.drawRect(WIDTH / 3 + 2 * SIZE_S + 1.5 * SIZE_W, HEIGHT / 13 + 4 * SIZE_S + 2 * SIZE_H, 1.5 * SIZE_W, SIZE_H);
    painter.drawText(QRect(WIDTH / 3 + 2 * SIZE_S + 1.5 * SIZE_W, HEIGHT / 13 + 4 * SIZE_S + 2 * SIZE_H, 1.5 * SIZE_W, SIZE_H), Qt::AlignCenter, "Cancel");

    painter.drawLine(WIDTH / 3, HEIGHT / 13 + 3 * SIZE_S + 2 * SIZE_H, WIDTH / 3 + 3 * SIZE_H + 5 * SIZE_S, HEIGHT / 13 + 3 * SIZE_S + 2 * SIZE_H);
}

/**Read the chosen size
 * 
 * @param point The point that corresponds to a size chooser button
 * @return Always NONE, since the user has to read the chosen size by himself, usually after the OK button has been pressed
 */
int SizeChooser::readSize(Point* point)
{
    if (point->y() < HEIGHT / 13 + 0.5 * SIZE_S)
    {
        return NONE;
    }
    else if (point->y() < HEIGHT / 13 + 1.5 * SIZE_S + 1 * SIZE_H)
    {
        return row1(point);
    }
    else if (point->y() < HEIGHT / 13 + 2.5 * SIZE_S + 2 * SIZE_H)
    {
        return row2(point);
    }
    else if (point->y() < HEIGHT / 13 + 3.5 * SIZE_S + 2 * SIZE_H)
    {
        return NONE;
    }
    else if (point->y() < HEIGHT / 13 + 4.5 * SIZE_S + 3 * SIZE_H)
    {
        return row3(point);
    }
    else
    {
        return NONE;
    }

}

/**Read pen size
 * 
 * @return An integer corresponding to a pen size
 */
int SizeChooser::getSize()
{
    return penSize;
}

/** Read from row 1
 * 
 * Row 1 has only 1 possible button: Size up
 * 
 * @param point The point corresponding to a button of the size chooser on row 1
 * @return Always NONE, since the user has to read the chosen size by himself, usually after the OK button has been pressed, 
 */
int SizeChooser::row1(Point* point)
{
    if (point->x() < WIDTH / 3 + 1.5 * SIZE_S + 2 * SIZE_W)
    {
        return NONE;
    }
    else if (point->x() < WIDTH / 3 + 2.5 * SIZE_S + 3 * SIZE_W)
    {
        if (penSize + 1 <= MAX_PEN_SIZE)
        {
            penSize++;
        }
        return NONE;
    }
    else
    {
        return NONE;
    }
}

/** Read from row 2
 * 
 * Row 2 has only 1 possible button: Size down
 * 
 * @param point The point corresponding to a button of the size chooser on row 2
 * @return Always NONE, since the user has to read the chosen size by himself, usually after the OK button has been pressed, 
 */
int SizeChooser::row2(Point* point)
{
    if (point->x() < WIDTH / 3 + 1.5 * SIZE_S + 2 * SIZE_W)
    {
        return NONE;
    }
    else if (point->x() < WIDTH / 3 + 2.5 * SIZE_S + 3 * SIZE_W)
    {
        if (penSize - 1 >= MIN_PEN_SIZE)
        {
            penSize--;
        }
        return NONE;
    }
    else
    {
        return NONE;
    }
}

/** Read from row 3
 * 
 * Row 3 has only 2 possible buttons: OK and Cancel
 * 
 * @param point The point corresponding to a button of the size chooser on row 3
 * @return OK or Cancel, depending which button the user hit, or NONE if no button was hit 
 */
int SizeChooser::row3(Point* point)
{
    if (point->x() < WIDTH / 3 + 0.5 * SIZE_S)
    {
        return NONE;
    }
    else if(point->x()<WIDTH / 3 + 1.5 * SIZE_S+1.5 * SIZE_W)
    {
        //std::cout<<"OK"<<std::endl;
        return OK;
    }
    else if(point->x()<WIDTH / 3 + 2.5 * SIZE_S+3 * SIZE_W)
    {
        //std::cout<<"Cancel"<<std::endl;
        return CANCEL;
    }
    else
    {
        return NONE;
    }

}