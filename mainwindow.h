
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QString>
#include "defines.h"
#include "inputData.h"

#include "scribblearea.h"

#include "stdio.h"

#include <queue>

#include <boost/thread/thread.hpp> 

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

protected:
    //void closeEvent(QCloseEvent *event);


private slots:
    //void open();
    //void save();
    void penColor();
    void penWidth();
    // void about();

    //Added
    void previous();
    void undo();
    void redo();
    void next();
    void write();
    void erase();
    void clearAll();

private:
    //bool saveFile(const QByteArray &fileFormat);

    //Added
    void createButtons();
    QGraphicsView /*QWidget*/ *window;
    QHBoxLayout *layout;

    QPushButton* preB;
    QPushButton* undoB;
    QPushButton* redoB;
    QPushButton* nextB;
    QPushButton* writeB;
    QPushButton* eraseB;
    //QPushButton* penColorB;
    //QPushButton* penWidthB;
    QPushButton* clearAllB;


    ScribbleArea *scribbleArea;
    inputData * input;

    //static const char VERSION[] = "Exam v0.1";

    /*
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
     * */
};

#endif