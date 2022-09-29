#include "graph.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Graph g;
    g.show();
    return a.exec();
}
