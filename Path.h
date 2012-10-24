/*!  
 * File:   Path.h
 * Author: greg
 *
 * Created on March 3, 2012, 11:55 AM
 */

#ifndef PATH_H
#define	PATH_H

#include <vector>
#include "Point.h" 
#include "QColor"
#include "defines.h"
#include "ScreenAreas.h"

#include <iostream>

class Path
{
public:
    Path(Point* point, int mode, QColor color, int width, int ID, bool final = false);
    Path(const Path& orig);
    virtual ~Path();
    void addPoint(Point* point);
    std::vector<Point*> getPath();
    int getPenWidth();
    QColor getPenColor() const;
    int getMode() const;
    void setEraseMode();
    void setWriteMode();

    int getPathID() const;
    void disablePath();
    void enablePath();
    bool isEnabled() const;

    int getLength();
    ScreenAreas* getArea();
    
    void setPenColor(QColor color);
    
    int getPointsCount();

private:
    int sqrt(const int x);

    int WriteEraseMode;
    int myPenWidth;
    QColor myPenColor;
    std::vector<Point *> mPointsVector;
    int ID;
    bool active;
    /*! Used to determine if the path is really what the user wanted to write or not*/
    bool final;
    int totalLength;
    int mX;
    int mY;

    ScreenAreas *area;

    enum mode
    {
        WRITE, ERASE
    };
};

#endif	/* PATH_H */

