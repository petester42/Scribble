/* 
 * File:   loadFileWindow.cpp
 * Author: greg
 * 
 * Created on July 16, 2012, 1:13 PM
 */

#include "LoadFileWindow.h"

LoadFileWindow::LoadFileWindow() : start(0), selected(-1)
{
    updateFolder();
}

LoadFileWindow::~LoadFileWindow()
{
}

void LoadFileWindow::updateFolder()
{
    //Get list of files in folder PDF
    std::string dir = std::string("pdf/");
    files.clear();
    getDir(dir, files);

    std::sort(files.begin(), files.end());
}

void LoadFileWindow::displayLoadFileWindow(QPainter &painter)
{
    painter.setBrush(QBrush("#FFF8C6"));
    painter.drawRect(WIDTH / 5, HEIGHT / 5, 3 * WIDTH / 5, 12 * BUTTON_H);
    //painter.setBrush(QBrush("#FFF8C6"));

    QRect rect = QRect(WIDTH / 5, HEIGHT / 5, BUTTON_W, BUTTON_H);
    painter.drawRect(WIDTH / 5, HEIGHT / 5, BUTTON_W, BUTTON_H);
    painter.drawText(rect, Qt::AlignCenter, "Select file to open: ");

    //OK
    painter.drawRect(WIDTH / 5, HEIGHT / 5 + 11 * BUTTON_H, BUTTON_W / 2, BUTTON_H);
    painter.drawText(QRect(WIDTH / 5, HEIGHT / 5 + 11 * BUTTON_H, BUTTON_W / 2, BUTTON_H), Qt::AlignCenter, "OK");

    //Cancel
    painter.drawRect(WIDTH / 5 + BUTTON_W / 2, HEIGHT / 5 + 11 * BUTTON_H, BUTTON_W / 2, BUTTON_H);
    painter.drawText(QRect(WIDTH / 5 + BUTTON_W / 2, HEIGHT / 5 + 11 * BUTTON_H, BUTTON_W / 2, BUTTON_H), Qt::AlignCenter, "Cancel");

    //UP
    painter.drawRect(WIDTH / 5 + BUTTON_W, HEIGHT / 5, WIDTH - (2 * WIDTH / 5 + BUTTON_W), 6 * BUTTON_H);
    painter.drawText(QRect(WIDTH / 5 + BUTTON_W, HEIGHT / 5, WIDTH - (2 * WIDTH / 5 + BUTTON_W), 6 * BUTTON_H), Qt::AlignCenter, "Up");

    //DOWN
    painter.drawRect(WIDTH / 5 + BUTTON_W, HEIGHT / 5 + 6 * BUTTON_H, WIDTH - (2 * WIDTH / 5 + BUTTON_W), 6 * BUTTON_H);
    painter.drawText(QRect(WIDTH / 5 + BUTTON_W, HEIGHT / 5 + 6 * BUTTON_H, WIDTH - (2 * WIDTH / 5 + BUTTON_W), 6 * BUTTON_H), Qt::AlignCenter, "Down");

    //start = 3;
    updateFiles(painter);
}

/*function... might want it in some class?*/
int LoadFileWindow::getDir(std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL)
    {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void LoadFileWindow::updateFiles(QPainter &painter)
{

    //files.sort();
    painter.setBrush(QBrush("#FFF8D6"));
    int position = 0;
    for (uint fileFromList = start; fileFromList < files.size(); fileFromList++)//std::list<std::string>::iterator it = files.begin(); it != files.end(); it++)
    {
        bool modified = false;
        if (position == selected)
        {
            painter.setBrush(QBrush("#4863A0"));
            modified = true;
        }

        painter.drawRect(WIDTH / 5, HEIGHT / 5 + (1 + position) * BUTTON_H, BUTTON_W, BUTTON_H);
        painter.drawText(QRect(WIDTH / 5 + 20, HEIGHT / 5 + (1.35 + position) * BUTTON_H, BUTTON_W, BUTTON_H), Qt::AlignLeft, files[fileFromList].c_str());

        position++;

        if (modified)
        {
            painter.setBrush(QBrush("#FFF8D6"));
        }
        if (fileFromList-start == 9)
        {
            break;
        }
    }
}

std::string LoadFileWindow::readLoadFileWindow(Point* point)
{
    if (point->x() < (WIDTH / 5) || point->x()>(WIDTH - WIDTH / 5) || point->y()<(HEIGHT / 5) || point->y()>(HEIGHT / 5 + 12 * BUTTON_H))
    {
        std::cout << "Out of bound" << std::endl;
        return "";
    }
    else if (point->x()>(WIDTH / 5 + BUTTON_W) && point->x()<(WIDTH - WIDTH / 5))
    {
        if (point->y()>(HEIGHT / 5) && point->y()<(HEIGHT / 5 + 6 * BUTTON_H))
        {
            std::cout << "UP" << std::endl;
            scrollUp();
        }
        else
        {
            std::cout << "Down" << std::endl;
            scrollDown();
        }

        return "";
    }
    else if (point->y()>(HEIGHT / 5 + 11 * BUTTON_H))
    {
        int finalChoice = selected;
        selected = -1;
        if (point->x() < WIDTH / 5 + BUTTON_W / 2)
        {
            std::cout << "OK" << std::endl;
            if (finalChoice >= 0 && finalChoice < (int)files.size())
            {
                return files[finalChoice+start];
            }

            return "";
        }
        else
        {
            std::cout << "Cancel" << std::endl;
            return "Cancel";
        }
    }
    else
    {
        if (point->y()<(HEIGHT / 5 + 1 * BUTTON_H))
        {
            std::cout << "no good, menu" << std::endl;
            return "";
        }

        selected = -1;
        if (point->y()<(HEIGHT / 5 + 2 * BUTTON_H))
        {
            std::cout << "place 0" << std::endl;
            selected = 0;
        }
        else if (point->y()<(HEIGHT / 5 + 3 * BUTTON_H))
        {
            std::cout << "place 1" << std::endl;
            selected = 1;
        }
        else if (point->y()<(HEIGHT / 5 + 4 * BUTTON_H))
        {
            std::cout << "place 2" << std::endl;
            selected = 2;
        }
        else if (point->y()<(HEIGHT / 5 + 5 * BUTTON_H))
        {
            std::cout << "place 3" << std::endl;
            selected = 3;
        }
        else if (point->y()<(HEIGHT / 5 + 6 * BUTTON_H))
        {
            std::cout << "place 4" << std::endl;
            selected = 4;
        }
        else if (point->y()<(HEIGHT / 5 + 7 * BUTTON_H))
        {
            std::cout << "place 5" << std::endl;
            selected = 5;
        }
        else if (point->y()<(HEIGHT / 5 + 8 * BUTTON_H))
        {
            std::cout << "place 6" << std::endl;
            selected = 6;
        }
        else if (point->y()<(HEIGHT / 5 + 9 * BUTTON_H))
        {
            std::cout << "place 7" << std::endl;
            selected = 7;
        }
        else if (point->y()<(HEIGHT / 5 + 10 * BUTTON_H))
        {
            std::cout << "place 8" << std::endl;
            selected = 8;
        }
        else
        {
            std::cout << "place 9" << std::endl;
            selected = 9;
        }
        updateNeeded = true;
        return "";
    }
}

void LoadFileWindow::updateFileList(QPainter &painter)
{
    if (updateNeeded)
    {
        updateFiles(painter);
        updateNeeded = false;
    }
}

void LoadFileWindow::scrollUp()
{
    //Can we scroll up
    //Yes: change start -1
    if (start > 0)
    {
        start--;
        selected++;
        updateNeeded = true;
    }
}

void LoadFileWindow::scrollDown()
{
    //Can we scroll down
    //YES: change start +1
    if (start + 9 < files.size())
    {
        start++;
        selected--;
        updateNeeded = true;
    }
}