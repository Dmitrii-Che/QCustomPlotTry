#include "chdvplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
CHDVPlot w;
w.show();

return a.exec();
}
