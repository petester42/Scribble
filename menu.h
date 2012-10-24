/* 
 * File:   menu.h
 * Author: greg
 *
 * Created on July 26, 2012, 3:05 PM
 */

#ifndef MENU_H
#define	MENU_H

#include "defines.h"
#include "QPainter"
#include "Point.h"

class Menu
{
public:
    Menu();
    virtual ~Menu();
    void displayMenu(QPainter& painter, int buttonPressed);
    int readMenu(Point * point);

    enum
    {
        NONE,
        PREVIOUS_B,
        UNDO_B,
        REDO_B,
        WRITE_B,
        ERASE_B,
        SIZE_B,
        COLOUR_B,
        CLEARALL_B,
        LOAD_B,
        SAVE_B,
        SAVE_AS_B,
        NEXT_B
    };
private:
    
    std::string font;
    int fontSize;

    enum
    {
        MENU_BOTTON_H = 40,
        MENU_BOTTON_W = 47,
        MENU_BOTTON_SPACING = 17
    };
};

#endif	/* MENU_H */

