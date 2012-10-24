#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include "Point.h"
#include <QWidget>
#include <QString>
#include <vector>
#include "Path.h"
#include <iostream>
#include <deque>
#include <QtGui>

#include <stdio.h>                      /* Standard input/output definitions */
#include <string.h>                     /* String function definitions */
#include <unistd.h>                     /* UNIX standard function definitions */
//#include <fcntl.h>                    /* File control definitions */
#include <errno.h>                      /* Error number definitions */
//#include <termios.h>                  /* POSIX terminal control definitions */
#include "Point.h"                     /* The points class, used to gather all touched points */
#include <list>
#include <sstream>
#include <dirent.h>                     /* To access the file system */
#include "defines.h"
#include <poppler/qt4/poppler-qt4.h>    /* Poppler library, used for PDF rendering */
#include "Keyboard.h"
#include "loadFileWindow.h"
#include "ColourPalette.h"
#include "SizeChooser.h"
#include "menu.h"

#include <QMutex>

#define TEST

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);
    virtual ~ScribbleArea();

    //bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    //bool isModified()const ;
    QColor penColor() const;
    int penWidth() const;

    //void setReadWriteMode(int mode);
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

public slots:

private slots:
    void myUpdate();
    void review();
    void updatePathsOnPage();

protected:
    void paintEvent(QPaintEvent *event);
    //void resizeEvent(QResizeEvent *event);

private:
    //void resizeImage(QImage *image, const QSize &newSize);
    void loadNewDocument();
    void saveFile();
    void updatePageContent();
    void drawLineTo(const Point endPoint);
    void drawPathsOnPage(QPainter& painter, int page);
    void drawPathsOnPageForSave(QPainter& painter, int page);
    void updatePDF();
    int loadPDF_withPagesCount(std::string fileName);
    void changePage(int page);
    void cleanRedoVector();
    void cleanPathsOnCurentPageVector();
    void displayOtherWindows(QPainter& painter);
    bool drawnPathsAnalyzer(int start, int item, int end);
    
    
    //void sendPacket();

    std::string DOC_PATH;
    bool modified;
    bool reviewNeeded;
    bool pathUpdateNeeded;
    bool scribbling;
    bool updateImage;

    enum modes
    {
        WRITE, ERASE, MENU_PRESS, LOAD, SAVE_AS, COLOUR, SIZE_WRITE, SIZE_ERASE
    };

    enum
    {
        MENU_BOTTON_H = 40,
        MENU_BOTTON_W = 47,
        MENU_BOTTON_SPACING = 17
    };

    int mMode;
    int oldWriteEraseMode;
    int oldWriteEraseModeSize;
    int myPenSize;
    int myEraserSize;
    int buttonPressed;
    int currentPage;
    int totalPages;

    QImage image;
    QImage imageCopy;
    QColor myPenColor;
    QString saveFileName;

    Point lastPoint;

    QMutex pathsLock;
    QMutex lockForTempPath;

    std::vector< std::vector<Path*> > pathsOnPage;
    std::vector< std::vector<Path*> > redoVector;
    std::vector<int> Paths_IDs;

    QMutex lockForImage; //Not sure if it is needed but just in case;

    Point* oldPoint; //Used for menu button pressing
    //QPainter* painter;
    Path * mTempPath;
    QTimer *updateTimer;
    QTimer *reviewTimer;
    QTimer *updatePathVector;

    Keyboard* keyboard;
    LoadFileWindow* loadWindow;
    ColourPalette * colourPalette;
    SizeChooser *sizeChooserWrite;
    SizeChooser *sizeChooserErase;
    Menu * menu;

    Poppler::Document* document;
    Poppler::Page* pdfPage;
};

#endif