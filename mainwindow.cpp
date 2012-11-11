#include <QtGui>
#include <QtCore/qnamespace.h>

#include "mainwindow.h"

void testing()
{
    std::cout<<"Main window threa"<<std::endl;
}

/*! Default constructor
 * 
 * This the default constructor. It created the ScribbleArea, the inputData thread, sets the window outline and creates all tge buttons available to the user. 
 */
MainWindow::MainWindow():window(NULL), layout(NULL), preB(NULL),  undoB(NULL),  redoB(NULL), nextB(NULL),  writeB(NULL),  eraseB(NULL), clearAllB(NULL),scribbleArea(NULL), input(NULL)
{

    scribbleArea = new ScribbleArea();
    setCentralWidget(scribbleArea);
    input = new inputData(scribbleArea);
    input->start();

    //createButtons();

    setWindowTitle(tr(VERSION));
    //resize(HEIGHT, WIDTH);
    resize(WIDTH, HEIGHT);
    
    //ScribbleArea t2;
    //boost::thread  t(testing);//, &t2));
}

/*! Default destructor
 * 
 * This is the default destructor. It stops the thread from running and deletes the ScribbleArea and the inputData object. 
 * 
 * <b>** Need to find a way to terminate the thread. The loop in run() needs to be stopped somehow while it is waiting for an input.</b>
 * - Possible solution would be to simulate a touch on the screen
 */
MainWindow::~MainWindow()
{
    //Need to find a clever way to stop this thread: the input from the USB are blocking and therefore wait() will wait until we "de-block" it by touching the screen. 
    input->stop();

    input->terminate(); //quit();
    input->wait();
    delete scribbleArea;
    delete input;

    delete preB;
    delete undoB;
    delete redoB;
    delete nextB;
    delete writeB;
    delete eraseB;
    //delete penColorB;
    //delete penWidthB;
    delete clearAllB;

    delete layout;
    delete window;
    //
}

/*
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
 */

/*
void MainWindow::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}
 */

/*
void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}
 */

/*! Set pen color
 * 
 * This function allows the user to choose for a large number of pen colors. The user chooses the color by clicking on the color of choice and then OK
 * 
 * ** Depending on the implementation of the touch screen this might need to be changed
 */
void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

/*! Set pen width
 * 
 * This function allows the user to change the width of the pen by choosing (writing) an integer between 1 and 50
 * 
 * ** Depending on the implementation of the touch screen this might need to be changed
 */
void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInteger(this, tr("Scribble"),
            tr("Select pen width:"),
            scribbleArea->penWidth(),
            1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

/*! Previous Page
 * 
 * This function changes the page on the ScribbleArea to the previous page
 */
void MainWindow::previous()
{
    scribbleArea->previousPage();
}

/*! Next page
 * 
 * This function changes the page on the ScribbleArea to the next page
 */
void MainWindow::next()
{
    scribbleArea->nextPage();
}

/*! Undo last action
 * 
 * This function allows the user to undo the last actions. 
 */
void MainWindow::undo()
{
    scribbleArea->undo();
}

/*! Redo what has been undone
 * 
 * This function allows the user to redo whatever was undone. This action is only available if the previous action was an undo, otherwise nothing will occur. 
 */
void MainWindow::redo()
{
    scribbleArea->redo();
}

/*! Set to Write mode
 * 
 * This function enables the write mode allowing the user to write 
 */
void MainWindow::write()
{
    scribbleArea->write();
}

/*! Set to Erase mode
 * 
 * This function enables the erase mode allowing the user to erase 
 */
void MainWindow::erase()
{
    scribbleArea->erase();
}

/*! Clear all (on current page)
 * 
 * Allows the user to clear the screen completely. This action <b>cannot</b> be undone.
 */
void MainWindow::clearAll()
{
    QMessageBox msgBox;
    msgBox.addButton(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setText("Clear all cannot be undone. Are you sure you want to clear all?");

    int selection = msgBox.exec();
    if (selection == QMessageBox::Yes)
    {
        scribbleArea->clearAll();
        //qDebug() << "yes";
    }
    else if (selection == QMessageBox::No)
    {
        //qDebug() << "no";
    }

}

/*! Create buttons 
 * 
 * This function creates all the buttons needed by the user and connects them to the appropriate functions
 */
void MainWindow::createButtons()
{
    
    window = new QGraphicsView(this); // QWidget(this);
    window->resize(WIDTH /* * 9 / 10*/, MENUWIDTH);

    //Creating the required buttons
    preB = new QPushButton("Previous", this);
    preB->resize(80, 80); //,102);
    undoB = new QPushButton("Undo", this);
    redoB = new QPushButton("Redo", this);
    nextB = new QPushButton("Next", this);
    writeB = new QPushButton("Write", this);
    eraseB = new QPushButton("Erase", this);
    //penColorB = new QPushButton("Pen Color", this);
    //penWidthB = new QPushButton("Pen Width", this);
    clearAllB = new QPushButton("Clear all", this);

    //connecting buttons to actions
    connect(preB, SIGNAL(clicked()), this, SLOT(previous()));
    connect(undoB, SIGNAL(clicked()), this, SLOT(undo()));
    connect(redoB, SIGNAL(clicked()), this, SLOT(redo()));
    connect(nextB, SIGNAL(clicked()), this, SLOT(next()));
    connect(writeB, SIGNAL(clicked()), this, SLOT(write()));
    connect(eraseB, SIGNAL(clicked()), this, SLOT(erase()));
    //connect(penColorB, SIGNAL(clicked()), this, SLOT(penColor()));
    //connect(penWidthB, SIGNAL(clicked()), this, SLOT(penWidth()));
    connect(clearAllB, SIGNAL(clicked()), this, SLOT(clearAll()));

    //Creating a box layout and placing all the buttons in it
    layout = new QHBoxLayout; 
    layout->addWidget(preB);
    layout->addWidget(undoB);
    layout->addWidget(redoB);
    layout->addWidget(writeB);
    layout->addWidget(eraseB);
    layout->addWidget(clearAllB);
    //layout->addWidget(penColorB);
    //layout->addWidget(penWidthB);
    layout->addWidget(nextB);

    window->setBackgroundRole(QPalette::Shadow);
    //displaying buttons
    window->setLayout(layout);
    
    window->show();
}

/*! Save page content
 * 
 * This function is NOT is use, however, in the future, it will save the content of all the pages and save them as an image or PDF.
 * /
bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
            initialPath,
            tr("%1 Files (*.%2);;All Files (*)")
            .arg(QString(fileFormat.toUpper()))
            .arg(QString(fileFormat)));
    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return scribbleArea->saveImage(fileName, fileFormat);
    }
}*/