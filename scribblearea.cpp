//#include <QtGui>

#include <QtCore/qnamespace.h>

#include "scribblearea.h"

/*! Default constructor
 * \param *parent A pointer to a QWidget parent object
 *
 * Initializes the ScribbleArea object
 */
ScribbleArea::ScribbleArea(QWidget *parent)
: QWidget(parent), DOC_PATH(PATH), modified(false), reviewNeeded(false), pathUpdateNeeded(false), scribbling(false), updateImage(true), mMode(WRITE), oldWriteEraseModeSize(WRITE), myPenSize(1), myEraserSize(10), buttonPressed(Menu::NONE), currentPage(0), myPenColor(Qt::black), oldPoint(NULL), mTempPath(NULL), document(NULL), pdfPage(NULL)
{
    setAttribute(Qt::WA_StaticContents);

    //load pdf and make sure it contains at least 1 page
    loadNewDocument();
    //painter = new QPainter(&image);
    //painter->setRenderHint(QPainter::Antialiasing);

    //Not sure why I need to end it here, but if I do not do it I get an error message (even though the program works normally)
    // painter->end();

    //Initializing all the extra windows
    keyboard = new Keyboard();
    loadWindow = new LoadFileWindow();
    colourPalette = new ColourPalette();
    sizeChooserWrite = new SizeChooser(myPenSize);
    sizeChooserErase = new SizeChooser(myEraserSize);
    menu = new Menu();

    updatePageContent();
    //Periodical function calls; usually needed for cleanups
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
    updateTimer->start(SCREEN_UPDATE_TIMER);

    reviewTimer = new QTimer(this);
    connect(reviewTimer, SIGNAL(timeout()), this, SLOT(review()));
    reviewTimer->start(REVIEW_TIMER);

    updatePathVector = new QTimer(this);
    connect(updatePathVector, SIGNAL(timeout()), this, SLOT(updatePathsOnPage()));
    updatePathVector->start(UPDATE_PATHS_VECTOR);

    updateImage = true;
    sendPacket();
}

/*! Default destructor
 */
ScribbleArea::~ScribbleArea()
{
    delete oldPoint;
    //delete painter;
    delete mTempPath;

    for ( int i = 0; i < totalPages; i++ )
    {
        currentPage = i;
        cleanRedoVector();
        cleanPathsOnCurentPageVector();
    }
    delete document;
    delete pdfPage;

    delete updateTimer;
    delete reviewTimer;
    delete updatePathVector;

    delete keyboard;
    delete loadWindow;
    delete colourPalette;
    delete sizeChooserWrite;
    delete sizeChooserErase;
    delete menu;
}

/*! Save file
 * This function save the content of the bitmap into a PDF file. The file name corresponds to DOC_PATH and if the file exists it will be replaced by the current bitmap content
 * 
 * \TODO Check if file exists, if it does prompt for replacement (Yes/Cancel)
 */
void ScribbleArea::saveFile()
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    //first save in tmp file, once save is complete, rename....
    printer.setOutputFileName("pdf/printtmp.pdf");
    printer.setPageSize(QPrinter::Letter);
    QPainter painter(&printer);

    for ( int page = 0; page < totalPages; page++ )
    {
        changePage(page);
        updatePDF();
        QRectF dirtyRect(-33, -33, 819, 1092); //Those number were found with try and error method, need to correspond to the values of file opening
        painter.drawImage(dirtyRect, image);
        painter.setRenderHint(QPainter::Antialiasing);
        drawPathsOnPageForSave(painter, page);
        if ( page != totalPages - 1 )
        {
            printer.newPage();
        }
    }

    //rename file to real name
    rename("pdf/printtmp.pdf", DOC_PATH.c_str());

#ifdef TEST
    std::cout << "Saved as: " << DOC_PATH << std::endl;
#endif

    changePage(currentPage);
    updatePDF();
}

/*! Load new document
 * This function load a new PDF document corresponding to the DOC_PATH filename
 * 
 */
void ScribbleArea::loadNewDocument()
{
    for ( currentPage = 0; currentPage < ( int ) pathsOnPage.size(); currentPage++ )
    {
        cleanPathsOnCurentPageVector();
        cleanRedoVector();
    }

    //Need to reset current page
    currentPage = 0;

    int nPage = loadPDF_withPagesCount(DOC_PATH);
    if ( nPage < 1 )
    {
        std::cout << "Cannot open document" << std::endl;
        exit(CANNOT_OPEN_DOC);
    }
    totalPages = nPage;

    pathsOnPage.resize(totalPages);
    redoVector.resize(totalPages);
    Paths_IDs.resize(totalPages);

    //Set all path IDs to 0
    for ( int i = 0; i < totalPages; i++ )
    {
        Paths_IDs[i] = 0;
    }
}

/*! Screen update function
 *
 * This function is called periodically, each SCREEN_UPDATE milliseconds (in the range of 30 milliseconds, defined in define.h).
 * It will call the update() function only only if the screen content has changed since last update.
 */
void ScribbleArea::myUpdate()
{
    if ( modified )
    {
        update();
        modified = false;
    }
}

/*! Update page content from a PDF
 *
 * This function updates the viewed content from a PDF file.
 */
void ScribbleArea::updatePDF()
{

    // Generate a QImage of the rendered page

    //APPARENTLY THERE IS A MEMORY LEAK SOMEPLACE HERE
    /*
    ==11665== 28 (24 direct, 4 indirect) bytes in 1 blocks are definitely lost in loss record 277 of 678
    ==11665==    at 0x400737F: operator new(unsigned int) (vg_replace_malloc.c:255)
    ==11665==    by 0x3F2850B: Splash::Splash(SplashBitmap*, int, SplashScreenParams*) (in /usr/lib/libpoppler.so.5.0.0)
    ==11665==    by 0x3E7038F: SplashOutputDev::SplashOutputDev(SplashColorMode, int, int, unsigned char*, int, int) (in /usr/lib/libpoppler.so.5.0.0)
    ==11665==    by 0x40451A8: Poppler::Page::renderToImage(double, double, int, int, int, int, Poppler::Page::Rotation) const (in /usr/lib/libpoppler-qt4.so.3.2.0)
    ==11665==    by 0x804C04F: ScribbleArea::updatePDF() (scribblearea.cpp:86)
    ==11665==    by 0x804C295: ScribbleArea::loadPDF_withPagesCount() (scribblearea.cpp:121)
    ==11665==    by 0x804BC13: ScribbleArea::ScribbleArea(QWidget*) (scribblearea.cpp:15)
    ==11665==    by 0x8051078: MainWindow::MainWindow() (mainwindow.cpp:12)
    ==11665==    by 0x8052828: main (main.cpp:17)
    ==11665==
    {
        <insert_a_suppression_name_here>
        Memcheck:Leak
        fun:_Znwj
        fun:_ZN6SplashC1EP12SplashBitmapiP18SplashScreenParams
        fun:_ZN15SplashOutputDevC1E15SplashColorModeiiPhii
        fun:_ZNK7Poppler4Page13renderToImageEddiiiiNS0_8RotationE
        fun:_ZN12ScribbleArea9updatePDFEv
        fun:_ZN12ScribbleArea22loadPDF_withPagesCountEv
        fun:_ZN12ScribbleAreaC1EP7QWidget
        fun:_ZN10MainWindowC1Ev
        fun:main
    }
     */
    QImage loadedImage(pdfPage->renderToImage(PDF_RESOLUTION, PDF_RESOLUTION, /*72.0, 72.0,*/ -1, -1, WIDTH, HEIGHT, /*-1, -1,*/ Poppler::Page::Rotate0)); //pdfPage->renderToImage(PDF_RESOLUTION, PDF_RESOLUTION - 10, 0, -80, WIDTH, HEIGHT, Poppler::Page::Rotate0)); // =  //All default values with the exception of rotation: Poppler::Page::Rotate270
    //QImage loadedImage(pdfPage->renderToImage(100, 100, -1, -1, -1, -1, /*-1, -1,*/ Poppler::Page::Rotate0));
    if ( loadedImage.isNull() )
    {
        std::cout << "Cannot convert from PDF to image" << std::endl;
        exit(PDF_TO_IMAGE_FAILED);
    }

    lockForImage.lock();

    image = loadedImage;
    imageCopy = image.copy();

    lockForImage.unlock();
}

/*! Load the PDF and return the number of pages
 *
 * \return <i>Number of pages</i> - The number of pages in the PDF document
 *
 * This function loads the PDF document making it available to the scribbleArea and returns the number of pages the document contains.
 *
 * This function should be <b>called only once</b> since it creates a pointer to the document and calling this function will create memory leaks unless the document is deleted first
 */
int ScribbleArea::loadPDF_withPagesCount(std::string fileName)
{
    QString filename = QString(fileName.c_str()); //DOC_PATH);
    document = Poppler::Document::load(filename);
    if ( !document || document->isLocked() || document == 0 )
    {
        perror("Cannot open document");
        exit(CANNOT_OPEN_DOC);
    }
    document->setRenderHint(Poppler::Document::TextAntialiasing);
    document->setRenderHint(Poppler::Document::Antialiasing);

    changePage(currentPage);
    updatePDF();
    return document->numPages();
}

/* ! Save image
 *
 * \param &fileName The filename under which the image should be saved
 * \param *fileFormat The file format in which the image should be saved
 *
 * This function save the current viewed image as a image file.
 * /
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    lockForImage.lock();
    QImage visibleImage = image;
    lockForImage.unlock();
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat))
    {
        //modified = false;
        return true;
    }
    else
    {
        return false;
    }
}*/

/*! Set Pen Color
 *
 * \param &newColor A reference to a QColor object
 *
 * This function sets the pen color
 */
void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

/*! Set Pen Width
 *
 * \param newWidth An integer representing the new width of the pen
 *
 * This function set the width of the pen to be used.
 */
void ScribbleArea::setPenWidth(int newWidth)
{
    myPenSize = newWidth;
}

/*! Paint event
 *
 * \param *event A QPaintEvent representing a paint event
 *
 * This function renders the screen whenever a QPaintEvent occurs.
 */
void ScribbleArea::paintEvent(QPaintEvent * /* event */)
{
    lockForImage.lock();
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), image);
    lockForImage.unlock();
}

/* ! Resize event
 *
 * \param *event A QResizeEvent representing the resize event
 *
 * This function is called each time the window is resized.
 * /
void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
    }
    QWidget::resizeEvent(event);
}*/

/*! Draw line between two points
 * 
 * \param &endPoint A Point representing the point to which a line needs to be drawn
 *
 * This function will draw a line between a previous point, which needs to be initialized on Screen Touch Event, and the current point.
 */
void ScribbleArea::drawLineTo(const Point endPoint)
{
    lockForImage.lock();

    //qDebug("1");
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    if ( mMode == WRITE )
    {
        painter.setPen(QPen(myPenColor, myPenSize, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    }
    else if ( mMode == ERASE )
    {
        painter.setPen(QPen(Qt::white, myEraserSize, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    }
    painter.drawLine(lastPoint, endPoint);

    //int rad = (myPenWidth / 2) + 2;
    //update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;

    lockForImage.unlock();
}

/* ! Resize a image to the required parameters
 *
 * \param *image A pointer to a QImage
 * \param &newSize A reference to a QSize
 *
 * This function will take the passed image and resize it correspondingly to the QSize give
 * /
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
    {
        return;
    }

    / *
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter->drawImage(QPoint(0, 0), *image);
 *image = newImage;
 * /
}*/

/*! Screen Press Event
 *
 * \param *point A pointer to a Point object
 *
 * This function initializes the lastPoint and enables scribbling in Write mode, or tries to delete a path on which the point passes through in Erase mode
 */
void ScribbleArea::screenPressEvent(Point* point)
{
    //if point is NULL return, nothing to do
    if ( point == NULL )
    {
        return;
    }

    //if in SAVE_AS mode and touches are in the keyboard area
    if ( mMode == SAVE_AS && point->y() > 690 )
    {
        char a = keyboard->readKeyboard(point);

        //If backspace was press, erase last character written
        if ( a == '\b' )
        {
            if ( !saveFileName.isEmpty() )
            {
                saveFileName.remove(saveFileName.size() - 1, saveFileName.size());
            }
        }
            //If enter was pressed, try saving the file
        else if ( a == '\r' )
        {
            //Save file
            std::string fileName = "pdf/";
            fileName += saveFileName.toStdString();
            fileName += ".pdf";
            DOC_PATH = fileName;
            std::cout << "DOC_PATH: " << DOC_PATH << std::endl;
            saveFile();
            std::cout << "Saving...." << std::endl;
            mMode = WRITE;
        }
            //If the character return is not NULL add it to the potential save as filename
        else if ( a != NULL )
        {
            saveFileName += a;
        }

        delete point;
        point = NULL;
    }
        //The point corresponds to a menu press, trying to retrieve the button pressed
    else if ( point->y() < 45 )
    {
        //if mode is write or erase save it to restore the state
        if ( mMode != MENU_PRESS )//== WRITE || mMode == ERASE || mMode == LOAD)
        {
            oldWriteEraseMode = this->mMode;
        }
        mMode = MENU_PRESS;
        buttonPressed = menu->readMenu(point);

        updatePageContent();

        delete point;
        point = NULL;

    }
        //If mode is LOAD, trying to load the user chosen file
    else if ( mMode == LOAD )
    {
        std::string name = loadWindow->readLoadFileWindow(point);
        if ( !name.empty() && name != "Cancel" && name != "." && name != ".." )
        {
            std::cout << "got filename: " << name.c_str() << std::endl;
            std::string mFileName = "pdf/";
            mFileName += name.c_str();

            std::cout << "file to open " << mFileName << std::endl;
            DOC_PATH = ( char* ) mFileName.c_str();

            loadNewDocument();
            mMode = WRITE;
        }
        else if ( name == "Cancel" )
        {
            std::cout << "canceling...." << std::endl;
            mMode = WRITE;

        }
        delete point;
        point = NULL;
    }
        //If mode is write, initialize the writing sequence
    else if ( this->mMode == WRITE )
    {
        cleanRedoVector();
        lastPoint.setX(point->x());
        lastPoint.setY(point->y());
        scribbling = true;

        lockForTempPath.lock();
        mTempPath = new Path(point, this->mMode, this->myPenColor, this->myPenSize, Paths_IDs[currentPage]++);
        lockForTempPath.unlock();
    }
        //If mode is erase, initialize the erasing sequence
    else if ( mMode == ERASE )
    {
        cleanRedoVector();
        lastPoint.setX(point->x());
        lastPoint.setY(point->y());
        scribbling = true;

        lockForTempPath.lock();
        mTempPath = new Path(point, WRITE, Qt::white, myEraserSize, Paths_IDs[currentPage]++);
        lockForTempPath.unlock();
    }
        //If mode is colour, read the colour palette until user hit OK
    else if ( mMode == COLOUR )
    {
        int choice = colourPalette->readColour(point);
        if ( choice == ColourPalette::OK )
        {
            this->myPenColor.setNamedColor(colourPalette->getColour().c_str());
            mMode = WRITE;
        }
        else if ( choice == ColourPalette::CANCEL )
        {
            mMode = WRITE;
        }

        delete point;
        point = NULL;
    }
        //If mode is SIZE_WRITE, read the pen size from the user until user hits OK
    else if ( mMode == SIZE_WRITE )
    {
        int choice = sizeChooserWrite->readSize(point);
        if ( choice == SizeChooser::OK )
        {
            myPenSize = sizeChooserWrite->getSize();
            mMode = WRITE;
        }
        else if ( choice == SizeChooser::CANCEL )
        {
            mMode = WRITE;
        }

        delete point;
        point = NULL;
    }
        //If mode is SIZE_ERASE, read the eraser size from the user until user hits OK
    else if ( mMode == SIZE_ERASE )
    {
        int choice = sizeChooserErase->readSize(point);
        if ( choice == SizeChooser::OK )
        {
            myEraserSize = sizeChooserErase->getSize();
            mMode = ERASE;

        }
        else if ( choice == SizeChooser::CANCEL )
        {
            mMode = ERASE;
        }

        delete point;
        point = NULL;

    }
        //any other point needs to be delete
    else
    {
        delete point;
        point = NULL;
    }

    //Update page content only in the mode was not write or erase
    if ( mMode != WRITE || mMode != ERASE )
    {

        updatePageContent();
    }
    modified = true;
}

/*! Screen Move Event
 *
 * \param *point A pointer to a Point object
 *
 * This function draws a line between a last point and the new point in Write mode or tries to delete a path on which the point passes through in Erase mode
 */
void ScribbleArea::screenMoveEvent(Point* point)
{
    if ( point == NULL )
    {
        return;
    }
    if ( scribbling == true )
    {
        if ( point->y() < 45 )
        {
            screenReleaseEvent();
        }
        else
        {
            pathsLock.lock();
            drawLineTo(*point);
            lockForTempPath.lock();
            mTempPath->addPoint(point);
            lockForTempPath.unlock();
            pathsLock.unlock();
            modified = true;
        }
    }
    else if ( mMode == MENU_PRESS )
    {
        delete oldPoint;
        oldPoint = point;
    }
        //Here we can add more else if to enhance user experience by changing the color of the pressed button.
    else
    {
        delete point;
        point = NULL;
    }
}

/*! Screen Release Event
 *
 * This function disables scribbling and informs the ScribbleArea that nothing is touching the screen anymore
 */
void ScribbleArea::screenReleaseEvent(/*Points *point*/)
{
    if ( scribbling == true )
    {
        scribbling = false;
        pathsLock.lock();
        lockForTempPath.lock();
        if ( mTempPath->getPointsCount() < 3 )
        {
            delete mTempPath;
        }
        else
        {
            pathsOnPage[currentPage].push_back(mTempPath);
            //std::cout << "Point count: " << mTempPath->getPointsCount() << std::endl;
        }

        mTempPath = NULL;

        lockForTempPath.unlock();
        reviewNeeded = true;
        modified = true;
        //updatePageContent();
        //mTempPath->getArea()->printMatrix();
        pathsLock.unlock();
        //std::cout<<"Release"<<std::endl;
    }
    else if ( mMode == MENU_PRESS )
    {
        mMode = oldWriteEraseMode;

        if ( oldPoint != NULL && buttonPressed == menu->readMenu(oldPoint) )//determineButtonPressed(oldPoint))
        {
            switch ( buttonPressed )
            {
                case Menu::PREVIOUS_B:
                    std::cout << "Previous" << std::endl;
                    previousPage();
                    break;

                case Menu::UNDO_B:
                    std::cout << "Undo" << std::endl;
                    undo();
                    break;

                case Menu::REDO_B:
                    std::cout << "Redo" << std::endl;
                    redo();
                    break;

                case Menu::WRITE_B:
                    std::cout << "Write" << std::endl;
                    write();
                    break;

                case Menu::ERASE_B:
                    std::cout << "Erase" << std::endl;
                    erase();
                    break;

                case Menu::CLEARALL_B:
                    std::cout << "Clear all" << std::endl;
                    clearAll();
                    break;

                case Menu::NEXT_B:
                    std::cout << "Next" << std::endl;
                    nextPage();
                    break;

                case Menu::LOAD_B:
                    std::cout << "Load" << std::endl;
                    if ( mMode == LOAD )
                    {
                        mMode = WRITE;
                    }
                    else
                    {
                        mMode = LOAD;
                        loadWindow->updateFolder();
                    }
                    break;

                case Menu::SAVE_B:
                    std::cout << "Save" << std::endl;
                    saveFile();
                    break;

                case Menu::SAVE_AS_B:
                    std::cout << "Save As" << std::endl;
                    if ( mMode == SAVE_AS )
                    {
                        mMode = WRITE;
                    }
                    else
                    {
                        saveFileName = "";
                        mMode = SAVE_AS;
                    }
                    break;

                case Menu::COLOUR_B:
                    std::cout << "Colour" << std::endl;
                    if ( mMode == COLOUR )
                    {
                        mMode = WRITE;
                    }
                    else
                    {
                        mMode = COLOUR;
                    }
                    break;

                case Menu::SIZE_B:
                    std::cout << "Size" << std::endl;
                    if ( mMode == SIZE_WRITE )
                    {
                        //showKeyboard = false;
                        mMode = WRITE;
                    }
                    else if ( mMode == SIZE_ERASE )
                    {
                        //showKeyboard = true;

                        mMode = ERASE;
                    }
                    else if ( mMode == WRITE )
                    {
                        mMode = SIZE_WRITE;
                    }
                    else if ( mMode == ERASE )
                    {
                        mMode = SIZE_ERASE;
                    }
                    break;

                default:
                    std::cout << "default" << std::endl;
                    break;
            }
        }

        buttonPressed = Menu::NONE;
        updatePageContent();
        delete oldPoint;
        oldPoint = NULL;
    }

    //updatePageContent();
    modified = true;
}

/*! Clean Redo Vector (Delete all objects)
 *
 * This function iterates through the Redo vector, compares all paths to the paths in PathOnPages vector while deleting only the points that are only present in the Redo vector.
 * Otherwise the vector of points is cleared and then the Path object deleted
 */
void ScribbleArea::cleanRedoVector()
{
    int redoVecSize = ( int ) redoVector[currentPage].size();

    //For each Path in redoVector, find if the same path ID is present on the current page of PathsOnPage
    //If present then clear the points vector and then delete the Path object
    //Otherwise delete delete Path without cleaning it which will delete all Point objects
    for ( int i = 0; i < redoVecSize; i++ )
    {
        delete redoVector[currentPage][i];
        redoVector[currentPage][i] = NULL;
    }

    redoVector[currentPage].clear();
}

/*! Clean PathOnPageVector (Delete all objects)
 *
 * This functions cleans all the path object present in the vector. Since the Point objects are shared it makes sure that it is deleted only once
 */
void ScribbleArea::cleanPathsOnCurentPageVector()
{
    pathsLock.lock();
    int vectorSize = pathsOnPage[currentPage].size();

    for ( int i = 0; i < vectorSize; i++ )
    {
        delete pathsOnPage[currentPage][i];
        pathsOnPage[currentPage][i] = NULL;
    }

    pathsOnPage[currentPage].clear();
    pathsLock.unlock();

    //delete[] deleted;
}

/*! Next Page
 *
 * This function tries, if available, to access the next page of the PDF
 */
void ScribbleArea::nextPage()
{
    //Image that will be loaded needs to be the next one
    if ( currentPage + 1 < totalPages )
    {
        currentPage++;

        changePage(currentPage);
        updatePDF();
        pathsLock.lock();
        updatePageContent();
        pathsLock.unlock();
        modified = true;
    }
}

/*! Previous Page
 *
 * This function tries, if available, to access the previous page of the PDF
 */
void ScribbleArea::previousPage()
{
    //Image that will be loaded needs to be the previous one
    if ( currentPage - 1 >= 0 )
    {
        currentPage--;

        changePage(currentPage);
        updatePDF();
        pathsLock.lock();
        updatePageContent();
        pathsLock.unlock();
        modified = true;
    }
}

/*! Change Page
 *
 * This function load the new page of the PDF. The page loaded correspond to the integer in the currentPage variable.
 */
void ScribbleArea::changePage(int page)
{
    delete pdfPage;
    pdfPage = document->page(page);
    if ( pdfPage == 0 )
    {
        std::cout << "Failed accessing a page of the exam" << std::endl;
        exit(CANNOT_ACCESS_PDF_PAGE);
    }
}

/*! Update current page content
 *
 * This function redraws the content of the current page. The PDF page is first loaded and then all the enabled paths
 */
void ScribbleArea::updatePageContent()
{
    //painter->setRenderHint(QPainter::Antialiasing);
    lockForImage.lock();
    image = imageCopy.copy();
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    Point _lastPoint;
    Point _newPoint;

    drawPathsOnPage(painter, currentPage);

    lockForTempPath.lock();
    if ( mTempPath != NULL )
    {
        painter.setPen(QPen(mTempPath->getPenColor(), mTempPath->getPenWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for ( uint i = 0; i < mTempPath->getPath().size(); i++ )
        {
            _newPoint = *( mTempPath->getPath()[i] );
            if ( i == 0 )
            {
                _lastPoint = _newPoint;
                //std::cout<<"Last point: "<<_lastPoint.x()<<" "<<_lastPoint.y()<<std::endl;
            }
            else
            {
                painter.drawLine(_lastPoint, _newPoint);
                _lastPoint = _newPoint;
            }
        }
    }
    lockForTempPath.unlock();

    displayOtherWindows(painter);
    lockForImage.unlock();
}

/** Display all that needs to be displayed on the screen. Ie: Menu, load window, save window.... 
 * 
 * @param painter The painter on which the windows can be drawn
 */
void ScribbleArea::displayOtherWindows(QPainter& painter)
{
    menu->displayMenu(painter, buttonPressed);

    if ( mMode == SAVE_AS )
    {
        keyboard->displayKeyboard(painter);
        keyboard->displayText(painter, QString(saveFileName));
    }
    else if ( mMode == LOAD )
    {
        loadWindow->displayLoadFileWindow(painter);
    }
    else if ( mMode == LOAD )
    {
        loadWindow->updateFileList(painter);
    }
    else if ( mMode == COLOUR )
    {
        colourPalette->displayColourPalette(painter);
    }
    else if ( mMode == SIZE_WRITE )
    {
        sizeChooserWrite->displaySizeChooser(painter);
    }
    else if ( mMode == SIZE_ERASE )
    {
        sizeChooserErase->displaySizeChooser(painter);
    }
}

/** Draw paths on a given page
 * 
 * This function will draw all the paths corresponding to the given page
 * 
 * @param painter The painter on which all the paths need to be drawn
 * @param page The page on from which all the paths will be drawn
 */
void ScribbleArea::drawPathsOnPage(QPainter& painter, int page)
{
    Point _lastPoint;
    Point newPoint;
    int totalIDs = Paths_IDs[page];
    if ( totalIDs > 0 )
    {
        for ( uint i = 0; i < pathsOnPage[page].size(); i++ )//(int i = pathsOnPage[currentPage].size() - 1; i >= 0; i--)
        {
            {
                if ( pathsOnPage[page][i] != NULL )
                {
                    //std::cout << "Path ID: " << pathsOnPage[currentPage][i]->getPathID() << " Enabled: " << pathsOnPage[currentPage][i]->isEnabled() << " at vector slot: " << i << " W0 / E1 " << pathsOnPage[currentPage][i]->getMode() << std::endl;
                    if ( pathsOnPage[page][i]->getMode() == WRITE && pathsOnPage[page][i]->isEnabled() )
                    {
                        painter.setPen(QPen(pathsOnPage[page][i]->getPenColor(), pathsOnPage[page][i]->getPenWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                        for ( uint j = 0; j < pathsOnPage[page][i]->getPath().size(); j++ )
                        {
                            newPoint = *( pathsOnPage[page][i]->getPath()[j] );
                            if ( j == 0 )
                            {
                                _lastPoint = newPoint;
                            }
                            else
                            {
                                painter.drawLine(_lastPoint, newPoint);
                                _lastPoint = newPoint;
                            }
                        }
                    }
                }
            }
        }
    }
}

/** Draw paths on a given page for saving (differentiates from a regular draw because the points are modified to fit a PDF page
 * 
 * This function will draw all the paths corresponding to the given page
 * 
 * @param painter The painter on which all the paths need to be drawn
 * @param page The page on from which all the paths will be drawn
 */
void ScribbleArea::drawPathsOnPageForSave(QPainter& painter, int page)
{
    Point _lastPoint;
    Point _newPoint;
    int totalIDs = Paths_IDs[page];
    if ( totalIDs > 0 )
    {
        for ( uint i = 0; i < pathsOnPage[page].size(); i++ )//(int i = pathsOnPage[currentPage].size() - 1; i >= 0; i--)
        {
            {
                if ( pathsOnPage[page][i] != NULL )
                {
                    //std::cout << "Path ID: " << pathsOnPage[currentPage][i]->getPathID() << " Enabled: " << pathsOnPage[currentPage][i]->isEnabled() << " at vector slot: " << i << " W0 / E1 " << pathsOnPage[currentPage][i]->getMode() << std::endl;
                    if ( pathsOnPage[page][i]->getMode() == WRITE && pathsOnPage[page][i]->isEnabled() )
                    {
                        painter.setPen(QPen(pathsOnPage[page][i]->getPenColor(), pathsOnPage[page][i]->getPenWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

                        for ( uint j = 0; j < pathsOnPage[page][i]->getPath().size(); j++ )
                        {
                            _newPoint = *( pathsOnPage[page][i]->getPath()[j] );
                            _newPoint.adjustForSaving();
                            if ( j == 0 )
                            {
                                _lastPoint = _newPoint;
                            }
                            else
                            {
                                painter.drawLine(_lastPoint, _newPoint);
                                _lastPoint = _newPoint;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*! Undo
 *
 * This function allows the user to undo the last actions. Presently, there is no limit of now many undo can be performed, meaning the user can press undo until there is nothing present on the screen
 */
void ScribbleArea::undo()
{
    pathsLock.lock();
    if ( !pathsOnPage[currentPage].empty() )
    {
        for ( int i = ( int ) pathsOnPage[currentPage].size() - 1; i >= 0; i-- )
        {
            if ( pathsOnPage[currentPage][i] != NULL )
            {
                redoVector[currentPage].push_back(pathsOnPage[currentPage][i]);

                int id = pathsOnPage[currentPage][i]->getPathID();
                pathsOnPage[currentPage][i] = NULL; //pop_back();

                for ( int j = i - 1; j >= 0; j-- )
                {
                    if ( pathsOnPage[currentPage][j] != NULL && id == pathsOnPage[currentPage][j]->getPathID() )
                    {
                        pathsOnPage[currentPage][j]->enablePath();
                        break;
                    }
                }
                break;
            }
        }

        //updatePageContent();
        modified = true;
    }
    pathsLock.unlock();
}

/*! Redo
 *
 * This function allows the user to redo the last undone actions. This action is only available if the last action(s) is an undo, otherwise this function will have no effect
 */
void ScribbleArea::redo()
{
    pathsLock.lock();
    if ( !redoVector[currentPage].empty() )
    {
        int id = redoVector[currentPage].back()->getPathID();
        for ( int i = pathsOnPage[currentPage].size() - 1; i >= 0; i-- )
        {
            if ( pathsOnPage[currentPage][i] != NULL && id == pathsOnPage[currentPage][i]->getPathID() )
            {
                //std::cout << "Redone Path " << id << std::endl;
                pathsOnPage[currentPage][i]->disablePath();
                break;
            }
        }

        pathsOnPage[currentPage].push_back(redoVector[currentPage].back());
        redoVector[currentPage].pop_back();

        //updatePageContent();
        modified = true;
    }
    pathsLock.unlock();
}

/*! Set write mode
 *
 * This function set the mode to write, allowing the user to write on top of the PDF
 */
void ScribbleArea::write()
{
    mMode = WRITE;
    modified = true;
}

/*! Set erase mode
 *
 * This function set the mode to erase, allowing the user to erase anything that has been written on top of the PDF, leaving the PDF intact
 */
void ScribbleArea::erase()
{
    mMode = ERASE;
    modified = true;
}

/*! Clear all
 *
 * This function clears the current page from all writing. This action <b>cannot</b> be undone.
 */
void ScribbleArea::clearAll()
{
    cleanRedoVector();
    cleanPathsOnCurentPageVector();

    Paths_IDs[currentPage] = 0;

    //updatePDF();
    lockForImage.lock();
    image = imageCopy.copy();
    lockForImage.unlock();
    //updatePageContent();
    modified = true;
}

/*! Get Pen Color
 *
 * \return <i>myPenColor</i> - A QColor representing the current pen color in use
 *
 * This function returns the pen color currently in use
 */
QColor ScribbleArea::penColor() const
{
    return myPenColor;
}

/*! Get Pen Width
 *
 * \return <i>myPenWidth</i> - An integer representing the width of the pen in use
 *
 * This function returns the pen width currently in use
 */
int ScribbleArea::penWidth() const
{
    return myPenSize;
}

/*! Review function
 *
 * This function goes through all the paths on the current page and verifies if it should be there or no
 */
void ScribbleArea::review()
{
    pathsLock.lock();

    int option = 2;

    if ( this->mMode == WRITE && reviewNeeded && pathsOnPage[currentPage].size() > 6 )//4)//6)
    {
        //std::cout<<"reviewing..."<<std::endl;
        reviewNeeded = false;
        int start = ( ( ( int ) pathsOnPage[currentPage].size() - REVIEW_BUFFER ) > 0 ) ? ( pathsOnPage[currentPage].size() - REVIEW_BUFFER ) : 0;
        for ( int i = start; i < ( int ) pathsOnPage[currentPage].size(); i++ )
        {
            if ( pathsOnPage[currentPage][i] != NULL )
            {
                if ( option == 0 )
                {
                    if ( ( i - 6 ) >= 0 )
                    {
                        if ( drawnPathsAnalyzer(i - 6, i, i) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 5 ) >= 0 )
                    {
                        int end = ( i + 1 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 1;
                        if ( drawnPathsAnalyzer(i - 5, i, end) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 4 ) >= 0 )
                    {
                        int end = ( i + 2 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 2;
                        if ( drawnPathsAnalyzer(i - 4, i, end) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 3 ) >= 0 )
                    {
                        int end = ( i + 3 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 3;
                        if ( drawnPathsAnalyzer(i - 3, i, end) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 2 ) >= 0 )
                    {
                        int end = ( i + 4 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 4;
                        if ( drawnPathsAnalyzer(i - 2, i, end) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 1 ) >= 0 )
                    {
                        int end = ( i + 5 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 5;
                        if ( drawnPathsAnalyzer(i - 1, i, end) )
                        {
                            continue;
                        }
                    }

                    int end = ( i + 6 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 6;
                    if ( drawnPathsAnalyzer(i, i, end) )
                    {
                        continue;
                    }
                }
                else if ( option == 1 )
                {

                    if ( ( i - 4 ) >= 0 )
                    {
                        if ( drawnPathsAnalyzer(i - 4, i, i) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 3 ) >= 0 )
                    {
                        int end = ( i + 1 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 1;
                        if ( drawnPathsAnalyzer(i - 3, i, end) )
                        {
                            continue;
                        }
                    }


                    if ( ( i - 2 ) >= 0 )
                    {
                        int end = ( i + 2 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 2;
                        if ( drawnPathsAnalyzer(i - 2, i, end) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 1 ) >= 0 )
                    {
                        int end = ( i + 3 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 3;
                        if ( drawnPathsAnalyzer(i - 1, i, end) )
                        {
                            continue;
                        }
                    }

                    int end = ( i + 4 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 4;
                    if ( drawnPathsAnalyzer(i, i, end) )
                    {
                        continue;
                    }
                }
                else if ( option == 2 )
                {
                    if ( ( i - 3 ) >= 0 )
                    {
                        if ( drawnPathsAnalyzer(i - 3, i, i) )
                        {
                            continue;
                        }
                    }

                    if ( ( i - 2 ) >= 0 )
                    {
                        int end = ( i + 1 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 1;
                        if ( drawnPathsAnalyzer(i - 2, i, end) )
                        {
                            continue;
                        }
                    }
                    if ( ( i - 1 ) >= 0 )
                    {
                        int end = ( i + 2 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 2;
                        if ( drawnPathsAnalyzer(i - 1, i, end) )
                        {
                            continue;
                        }
                    }

                    int end = ( i + 3 >= ( int ) pathsOnPage[currentPage].size() ) ? ( int ) pathsOnPage[currentPage].size() - 1 : i + 3;
                    if ( drawnPathsAnalyzer(i, i, end) )
                    {
                        continue;
                    }
                }
            }
        }
    }

    pathsLock.unlock();
}

/*! Update the PathsOnPage vector
 *
 * This function removes all the NULL entries from the pathsOnPage[currentPage] vector. It should be called periodically so that the size of the vector is kept at a minimum
 */
void ScribbleArea::updatePathsOnPage()
{
    if ( pathUpdateNeeded )
    {
        pathsLock.lock();
        std::vector<Path*> pathsOnPageCopy;

        for ( uint i = 0; i < pathsOnPage[currentPage].size(); i++ )
        {
            if ( pathsOnPage[currentPage][i] != NULL )
            {
                pathsOnPageCopy.push_back(pathsOnPage[currentPage][i]);
            }
        }
        pathsOnPage[currentPage].clear();
        pathsOnPage[currentPage] = pathsOnPageCopy;
        pathsLock.unlock();
        pathUpdateNeeded = false;
    }
}

bool ScribbleArea::drawnPathsAnalyzer(int start, int item, int end)
{
    //return true;
    //    std::cout << "Start: " << start << " item: " << item << " end: " << end << " Size: " << pathsOnPage[currentPage].size() << std::endl;
    ScreenAreas* a = new ScreenAreas();

    int access = 0;
    for ( int i = start; i < end; i++ )
    {
        if ( i != item && pathsOnPage[currentPage][i] != NULL )
        {
            //std::cout << "i " << i << std::endl;
            ScreenAreas::orAreas(a, pathsOnPage[currentPage][i]->getArea());
            access++;
        }
    }

    if ( access > 2 && pathsOnPage[currentPage][item]->getLength() < 1.5 * MINIMUM_PATH && !ScreenAreas::overlap(a, pathsOnPage[currentPage][item]->getArea()) )
    {
        delete pathsOnPage[currentPage][item];
        pathsOnPage[currentPage][item] = NULL;
        // pathsOnPage[currentPage][item]->setPenColor(Qt::green);
        //updatePageContent();
        modified = true;

        std::cout << "Path deleted" << std::endl;
        pathUpdateNeeded = true;
        delete a;
        return true;
    }
    delete a;
    return false;

}

