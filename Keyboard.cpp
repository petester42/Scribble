/* 
 * File:   Keyboard.cpp
 * Author: greg
 * 
 * Created on July 11, 2012, 10:14 AM
 */

#include "Keyboard.h"

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

/** Display (draw) keyboard on the given painter
 * 
 * @param painter The painter on which we can draw the keyboard
 */
void Keyboard::displayKeyboard(QPainter& painter)
{
    painter.setBrush(QBrush("#FFF8D6"));
    painter.drawRect(0, HEIGHT - 7.75 * BUTTON_H, WIDTH, 7.75 * BUTTON_H);

    painter.setBrush(QBrush("#FFF8C6"));

    //Bottom line, includes letters: Z X C V B N M
    painter.drawRect(2 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    QRect rect = QRect(0, HEIGHT - 2.7 * BUTTON_H, 5 * BUTTON_W, 20);
    painter.drawText(rect, Qt::AlignCenter,
            "Z");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(3.5 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "X");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(5 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "C");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(6.5 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "V");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(8 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "B");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(9.5 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "N");

    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawRect(11 * BUTTON_W, HEIGHT - 3 * BUTTON_H, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "M");

    //Middle line, includes letters: A S D F G H J K L Enter
    painter.drawRect(1.5 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, -3 * BUTTON_W, -19 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "A");

    painter.drawRect(3 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "S");

    painter.drawRect(4.5 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "D");

    painter.drawRect(6 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "F");

    painter.drawRect(7.5 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "G");

    painter.drawRect(9 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "H");

    painter.drawRect(10.5 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "J");

    painter.drawRect(12 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "K");

    painter.drawRect(13.5 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "L");

    painter.drawRect(15 * BUTTON_W, HEIGHT - 4.5 * BUTTON_H, 2.5 * BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 4.5 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "Enter");

    //First line, includes letters: Q W E R T Y U I O P

    painter.drawRect(1 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, -3 * BUTTON_W, -29.5 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "Q");

    painter.drawRect(2.5 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "W");

    painter.drawRect(4 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "E");

    painter.drawRect(5.5 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "R");

    painter.drawRect(7 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "T");

    painter.drawRect(8.5 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "Y");

    painter.drawRect(10 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "U");

    painter.drawRect(11.5 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "I");

    painter.drawRect(13 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "O");

    painter.drawRect(14.5 * BUTTON_W, HEIGHT - 6 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "P");

    //Numbers and backspace
    painter.drawRect(0.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, -3 * BUTTON_W, -28 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "1");

    painter.drawRect(2 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "2");

    painter.drawRect(3.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "3");

    painter.drawRect(5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "4");

    painter.drawRect(6.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "5");

    painter.drawRect(8 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "6");

    painter.drawRect(9.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "7");

    painter.drawRect(11 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "8");

    painter.drawRect(12.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "9");

    painter.drawRect(14 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "0");

    painter.drawRect(15.5 * BUTTON_W, HEIGHT - 7.5 * BUTTON_H, 2 * BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 4 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "Backspace");

    //space bar    
    painter.drawRect(3.5 * BUTTON_W, HEIGHT - 1.6 * BUTTON_H, 7 * BUTTON_W, BUTTON_H);
    rect.adjust(0, 0, 3 * BUTTON_W, 0);
    painter.drawText(rect, Qt::AlignCenter,
            "");
}

/** Display the written SAVE_AS file name
 * 
 * @param painter The painter with which we can draw
 * @param string The filename that has been written until now by the user
 */
void Keyboard::displayText(QPainter& painter, QString string)
{
    painter.setBrush(QBrush("#FFF8D6"));
    painter.drawRect(2 * BUTTON_W, HEIGHT - 9.25 * BUTTON_H, WIDTH - 4 * BUTTON_W, 1.25 * BUTTON_H);
    QRect rect = QRect(2 * BUTTON_W, HEIGHT - 9.25 * BUTTON_H, WIDTH - 4 * BUTTON_W, 1.25 * BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            string);

}

/** Read character from keyboard
 * 
 * This function relates the received point to a keyboard button
 * 
 * @param point The point corresponding to the screen touch
 * @return The corresponding character or NULL if none
 */
char Keyboard::readKeyboard(Point *point)
{
    if (point->y() < HEIGHT - 6.25 * BUTTON_H)
    {
        return analyzeRow123(point->x());
    }
    else if (point->y() < HEIGHT - 4.75 * BUTTON_H)
    {
        return analyzeRowQWE(point->x());
    }
    else if (point->y() < HEIGHT - 3.25 * BUTTON_H)
    {
        return analyzeRowASD(point->x());
    }
    else if (point->y() < HEIGHT - 1.75 * BUTTON_H)
    {
        return analyzeRowZXC(point->x());
    }
    else
    {
        return analyzeRowSpace(point->x());
    }
}

/** Analyze the row of 123... buttons
 * 
 * @param x The x coordinate corresponding to the 123 row
 * @return The corresponding character to the x coordinate or NULL
 */
char Keyboard::analyzeRow123(int x)
{
    if (x < 0.25 * BUTTON_W)
    {
        return NULL; //NULL;
    }
    else if (x < 1.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "1" << std::endl;
#endif
        return '1';
    }
    else if (x < 3.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "2" << std::endl;
#endif
        return '2';
    }
    else if (x < 4.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "3" << std::endl;
#endif
        return '3';
    }
    else if (x < 6.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "4" << std::endl;
#endif
        return '4';
    }
    else if (x < 7.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "5" << std::endl;
#endif
        return '5';
    }
    else if (x < 9.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "6" << std::endl;
#endif
        return '6';
    }
    else if (x < 10.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "7" << std::endl;
#endif
        return '7';
    }
    else if (x < 12.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "8" << std::endl;
#endif
        return '8';
    }
    else if (x < 13.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "9" << std::endl;
#endif
        return '9';
    }
    else if (x < 15.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "0" << std::endl;
#endif
        return '0';
    }
    else
    {
#ifdef TEST_ENABLED
        std::cout << "Backspace" << std::endl;
#endif
        return '\b';
    }
}

/** Analyze the row of QWE... buttons
 * 
 * @param x The x coordinate corresponding to the QWE row
 * @return The corresponding character to the x coordinate or NULL
 */
char Keyboard::analyzeRowQWE(int x)
{
    if (x < 0.75 * BUTTON_W || x > 15.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << " no good " << std::endl;
#endif
        return NULL; //-1;
    }
    else if (x < 2.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "Q" << std::endl;
#endif
        return 'Q';
    }
    else if (x < 3.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "W" << std::endl;
#endif
        return 'W';
    }
    else if (x < 5.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "E" << std::endl;
#endif
        return 'E';
    }
    else if (x < 6.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "R" << std::endl;
#endif
        return 'R';
    }
    else if (x < 8.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "T" << std::endl;
#endif
        return 'T';
    }
    else if (x < 9.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "Y" << std::endl;
#endif
        return 'Y';
    }
    else if (x < 11.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "U" << std::endl;
#endif
        return 'U';
    }
    else if (x < 12.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "I" << std::endl;
#endif
        return 'I';
    }
    else if (x < 14.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "O" << std::endl;
#endif
        return 'O';
    }
    else
    {
#ifdef TEST_ENABLED
        std::cout << "P" << std::endl;
#endif
        return 'P';
    }
}

/** Analyze the row of ASD... buttons
 * 
 * @param x The x coordinate corresponding to the ASD row
 * @return The corresponding character to the x coordinate or NULL
 */
char Keyboard::analyzeRowASD(int x)
{
    if (x < 1.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << " no good " << std::endl;
#endif
        return NULL; //-1;
    }
    else if (x < 2.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "A" << std::endl;
#endif
        return 'A';
    }
    else if (x < 4.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "S" << std::endl;
#endif
        return 'S';
    }
    else if (x < 5.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "D" << std::endl;
#endif
        return 'D';
    }
    else if (x < 7.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "F" << std::endl;
#endif
        return 'F';
    }
    else if (x < 8.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "G" << std::endl;
#endif
        return 'G';
    }
    else if (x < 10.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "H" << std::endl;
#endif
        return 'H';
    }
    else if (x < 11.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "J" << std::endl;
#endif
        return 'J';
    }
    else if (x < 13.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "K" << std::endl;
#endif
        return 'K';
    }
    else if (x < 14.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "L" << std::endl;
#endif
        return 'L';
    }
    else
    {
#ifdef TEST_ENABLED
        std::cout << "Enter" << std::endl;
#endif
        return '\r';
    }
}

/** Analyze the row of ZXC... buttons
 * 
 * @param x The x coordinate corresponding to the ZXC row
 * @return The corresponding character to the x coordinate or NULL
 */
char Keyboard::analyzeRowZXC(int x)
{

    if (x < 1.75 * BUTTON_W || x > 12.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << " no good " << std::endl;
#endif
        return NULL; //-1;
    }
    else if (x < 3.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "Z" << std::endl;
#endif
        return 'Z';
    }
    else if (x < 4.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "X" << std::endl;
#endif
        return 'X';
    }
    else if (x < 6.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "C" << std::endl;
#endif
        return 'C';
    }
    else if (x < 7.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "V" << std::endl;
#endif
        return 'V';
    }
    else if (x < 9.25 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "B" << std::endl;
#endif
        return 'B';
    }
    else if (x < 10.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << "N" << std::endl;
#endif
        return 'N';
    }
    else
    {
#ifdef TEST_ENABLED
        std::cout << "M" << std::endl;
#endif
        return 'M';
    }
}

/** Analyze Row Space
 * 
 * Verify of the point touched correspond to the space button
 * 
 * @param x The x coordinates of the touched point
 * @return A space character or NULL
 */
char Keyboard::analyzeRowSpace(int x)
{
    if (x < 3.25 * BUTTON_W || x > 10.75 * BUTTON_W)
    {
#ifdef TEST_ENABLED
        std::cout << " no good " << std::endl;
#endif
        return NULL; //-1;
    }
    else
    {
#ifdef TEST_ENABLED
        std::cout << "Space" << std::endl;
#endif
        return ' ';
    }
}