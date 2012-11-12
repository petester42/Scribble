/* 
 * File:   menu.cpp
 * Author: greg
 * 
 * Created on July 26, 2012, 3:05 PM
 */

#include "Menu.h"

Menu::Menu() : font("Arial"), fontSize(8)
{
}

Menu::~Menu()
{
}

/**Display menu function draws the menu for the user. The menu includes 7 buttons (for now)
 * 
 * @param painter
 * @param buttonPressed
 * 
 * <ul>
 * <li>Previous
 * <li>Undo
 * <li>Redo
 * <li>Write
 * <li>Erase
 * <li>Clear All
 * <li>Load
 * <li>Save
 * <li>Next
 * </ul> 
 */
void Menu::displayMenu(QPainter& painter, int buttonPressed)
{
    painter.setPen(Qt::black);
    painter.setFont(QFont(font.c_str(), fontSize, QFont::Bold));
    painter.setBrush(QBrush("#FFF8D6"));
    painter.drawRect(0, 0, WIDTH, 50);

    if (buttonPressed == PREVIOUS_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }

    painter.drawRect(.5 * MENU_BOTTON_SPACING + 0 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    QRect rect = QRect(0, MENU_BOTTON_SPACING, 1 * MENU_BOTTON_SPACING + 1 * MENU_BOTTON_W, MENU_BOTTON_SPACING);
    painter.drawText(rect, Qt::AlignCenter,
            "Previous");

    if (buttonPressed == UNDO_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(1.5 * MENU_BOTTON_SPACING + 1 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Undo");

    if (buttonPressed == REDO_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(2.5 * MENU_BOTTON_SPACING + 2 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Redo");

    if (buttonPressed == WRITE_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(3.5 * MENU_BOTTON_SPACING + 3 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Write");

    if (buttonPressed == ERASE_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(4.5 * MENU_BOTTON_SPACING + 4 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Erase");

    if (buttonPressed == SIZE_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(5.5 * MENU_BOTTON_SPACING + 5 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Size");

    if (buttonPressed == COLOUR_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(6.5 * MENU_BOTTON_SPACING + 6 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Color");

    if (buttonPressed == CLEARALL_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(7.5 * MENU_BOTTON_SPACING + 7 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Clear all");

    if (buttonPressed == LOAD_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(8.5 * MENU_BOTTON_SPACING + 8 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Load");

    if (buttonPressed == SAVE_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(9.5 * MENU_BOTTON_SPACING + 9 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Save");

    if (buttonPressed == SAVE_AS_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(10.5 * MENU_BOTTON_SPACING + 10 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Save As");

    if (buttonPressed == NEXT_B)
    {
        painter.setBrush(QBrush("#4863A0"));
    }
    else
    {
        painter.setBrush(QBrush("#FFF8C6"));
    }
    rect.adjust(0, 0, 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING, 0);
    painter.drawRect(11.5 * MENU_BOTTON_SPACING + 11 * MENU_BOTTON_W, 5, MENU_BOTTON_W, MENU_BOTTON_H);
    painter.drawText(rect, Qt::AlignCenter,
            "Next");
}

/** Read menu
 * 
 * @param point The point that corresponds to a menu button
 * @return The menu button that has been pressed, or NONE if none was pressed
 */
int Menu::readMenu(Point* point)
{
    if (point->x() < 1 * MENU_BOTTON_W + 1 * MENU_BOTTON_SPACING)
    {
        return PREVIOUS_B;
    }
    else if (point->x() < 2 * MENU_BOTTON_W + 2 * MENU_BOTTON_SPACING)
    {
        return UNDO_B;
    }
    else if (point->x() < 3 * MENU_BOTTON_W + 3 * MENU_BOTTON_SPACING)
    {
        return REDO_B;
    }
    else if (point->x() < 4 * MENU_BOTTON_W + 4 * MENU_BOTTON_SPACING)
    {
        return WRITE_B;
    }
    else if (point->x() < 5 * MENU_BOTTON_W + 5 * MENU_BOTTON_SPACING)
    {
        return ERASE_B;
    }
    else if (point->x() < 6 * MENU_BOTTON_W + 6 * MENU_BOTTON_SPACING)
    {
        return SIZE_B;
    }
    else if (point->x() < 7 * MENU_BOTTON_W + 7 * MENU_BOTTON_SPACING)
    {
        return COLOUR_B;
    }
    else if (point->x() < 8 * MENU_BOTTON_W + 8 * MENU_BOTTON_SPACING)
    {
        return CLEARALL_B;
    }
    else if (point->x() < 9 * MENU_BOTTON_W + 9 * MENU_BOTTON_SPACING)
    {
        return LOAD_B;
    }
    else if (point->x() < 10 * MENU_BOTTON_W + 10 * MENU_BOTTON_SPACING)
    {
        return SAVE_B;
    }
    else if (point->x() < 11 * MENU_BOTTON_W + 11 * MENU_BOTTON_SPACING)
    {
        return SAVE_AS_B;
    }
    else if (point->x() < 12 * MENU_BOTTON_W + 12 * MENU_BOTTON_SPACING)
    {
        return NEXT_B;
    }

    return NONE;
}

