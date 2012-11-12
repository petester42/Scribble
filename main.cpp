#include <QApplication>
#include "Mainwindow.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "Point.h"
#include "ScreenAreas.h"  /* The points class, used to gather all touched points */

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv, true);
    MainWindow window;
    window.show();

    return app.exec();
}
