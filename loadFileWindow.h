/* 
 * File:   loadFileWindow.h
 * Author: greg
 *
 * Created on July 16, 2012, 1:13 PM
 */

#ifndef LOADFILEWINDOW_H
#define	LOADFILEWINDOW_H

#include "QPainter"
#include "defines.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <list>
#include <string>
#include <iostream>
#include "Point.h"
#include <vector>
#include <iostream>
#include <algorithm>

class LoadFileWindow
{
public:
    LoadFileWindow();
    virtual ~LoadFileWindow();
    void displayLoadFileWindow(QPainter& painter);
    
    std::string readLoadFileWindow(Point* point);
    void updateFileList(QPainter& painter);
    void updateFolder();
private:
   int getDir(std::string dir, std::vector<std::string> &files);
   void updateFiles(QPainter& painter);
   void scrollUp();
   void scrollDown();

    enum
    {
        BUTTON_W = 410,
        BUTTON_H = 50
    };
    
    std::vector<std::string> files;
    
    uint start;
    bool updateNeeded;
    int selected;

};

#endif	/* LOADFILEWINDOW_H */

