/* 
 * File:   PalmRejection.h
 * Author: greg
 *
 * Created on May 23, 2012, 12:39 PM
 */

#ifndef PALMREJECTION_H
#define	PALMREJECTION_H

#include "Point.h"
#include <pthread.h>
#include <iostream>
#include <vector>
#include "ScribbleArea.h"
#include "QQueue"
#include "ScreenAreas.h"

///////////////////////////

class PalmRejection : public QWidget {
    Q_OBJECT// : public QThread

public:
    PalmRejection(ScribbleArea* scribble);
    virtual ~PalmRejection();
    void eventTouch(QQueue<Point* > *mPointsQueue);
    void eventMove(QQueue<Point* > *mPointsQueue);
    void eventRelease(/*Points *point*/);

private slots:
    void resetPalm();

private:
    int mod(int, const int x = ANALYZE_BUFFER);
    int abs(int);
    float sqrt(int x);

    void analyzeNewSetOfPoints(); //this is done to reject possible "duplicates" of points caused on area edges
    void findPen();
    void findNextPoint();
    void updatePalmMatrix(ushort slot, ushort ignore = -1);
    void updatePosition();
    void flushPointBuffer();
    bool firstTryFindingPenAndPalm();
    void compact_pointToAnalyze();

    float getDistance(int, int);

    ScribbleArea* scribble;
    bool stopRequest;
    ushort position;

    std::vector <std::vector < Point *> > pointToAnalyze;
    std::vector <Point *> possiblePenPoints;

    ushort sampling;
    bool penPresent;

    ScreenAreas mPalm;
    ScreenAreas mPen;

    QTimer *palmResetTimer;

    //used as reference points
    int mX;
    int mY;
};

#endif	/* PALMREJECTION_H */

