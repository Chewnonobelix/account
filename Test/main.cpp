#include <QApplication>
#include <QTest>
#include <QDebug>

#include "testinformation.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    int status = 0;

    TestInformation ti;

    status |= QTest::qExec(&ti, argc, argv);

    return status;
}
