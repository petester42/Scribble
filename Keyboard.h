/* 
 * File:   Keyboard.h
 * Author: greg
 *
 * Created on July 11, 2012, 10:14 AM
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#include "QPainter"
#include "Defines.h"
#include <QWidget>
#include "Point.h"
#include <iostream>
#include "QTextEdit"

//enable for all testing messages
//#define TEST_ENABLED

class Keyboard
{
public:
    Keyboard();
    virtual ~Keyboard();
    char readKeyboard(Point* point);
    void displayKeyboard(QPainter& painter);
    void displayText(QPainter& painter,QString string);

private:
    /*
    void displayRowQWE(QPainter &painter);
    void displayRowASD(QPainter &painter);
    void displayRowZXC(QPainter &painter);
    void displayRow123(QPainter &painter);
     */

    char analyzeRow123(int x);
    char analyzeRowQWE(int x);
    char analyzeRowASD(int x);
    char analyzeRowZXC(int x);
    char analyzeRowSpace(int x);

    enum
    {
        BUTTON_W = 43,
        BUTTON_H = 43
    };
    QPainter painter;
};

#endif	/* KEYBOARD_H */

