/* 
 * File:   ScribbleArea.h
 * Author: scribble
 *
 * Created on October 25, 2012, 2:17 PM
 */

#ifndef SCRIBBLEAREA_H
#define	SCRIBBLEAREA_H

#include <GL/glu.h>
#include "Color.h"
#include "Path.h"
#include "Point.h"
#include <boost/thread/mutex.hpp>

class ScribbleArea {
public:
    ScribbleArea();
    ScribbleArea(const ScribbleArea& orig);
    virtual ~ScribbleArea();
    void Draw();
    
    void setPenColor(Color *newColor);
    void setPenWidth(float newWidth);
    
    Color* getPenColor();
    float getPenSize();
   
    void nextPage();
    void previousPage();
    void undo();
    void redo();
    void write();
    void erase();
    void clearAll();
    
    void screenPressEvent(Point* point);
    void screenMoveEvent(Point* point);
    void screenReleaseEvent(/*Points *point*/);
    
private:

     enum modes
    {
        WRITE, ERASE, MENU_PRESS, LOAD, SAVE_AS, COLOUR, SIZE_WRITE, SIZE_ERASE
    };
    
    Color* penColor;
    float penSize;
    
    int mMode;
    bool scribbling;
    
    std::vector<int> Paths_IDs;
    Path  *mTempPath;
    
    Point lastPoint;
    
    boost::mutex pathsLock;
    boost::mutex lockForTempPath;

    int currentPage;
    std::vector<std::vector<Path*> > pathsOnPage;
};

#endif	/* SCRIBBLEAREA_H */

