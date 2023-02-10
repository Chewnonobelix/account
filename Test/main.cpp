#include <QApplication>
#include <QTest>
#include <QDebug>
#include <QRandomGenerator>
#include <QtConcurrent/QtConcurrent>
#include "testcategory.h"
#include "testdebt.h"
#include "testentry.h"
#include "testfrequency.h"
#include "testinformation.h"

#include "Controller/core/maincontroller.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    int status = 0;

    TestCategory tc;
    status |= QTest::qExec(&tc, argc, argv);

    TestInformation ti;
    status |= QTest::qExec(&ti, argc, argv);

    TestEntry te;
    status |= QTest::qExec(&te, argc, argv);


    TestFrequency tf;
    status |= QTest::qExec(&tf, argc, argv);

    TestDebt td;
    status |= QTest::qExec(&td, argc, argv);

    auto t = QtConcurrent::run([](){
        ControllerDB json;
        json.init();
        auto today = QDate::currentDate();
        auto it = today.addYears(-2);
        auto end = today.addYears(2);
        json.setCurrentAccount(today.toString("dd-MM-yyyy"));
        QRandomGenerator seed;
        for(; it < end; it = it.addDays(1))
        {
            auto nb = seed.bounded(0, 10);

            for(auto i = 0; i < nb; i ++) {
                auto type = seed.bounded(0, 2) == 0 ? Account::TypeEnum::Income : Account::TypeEnum::Outcome;
                Entry e;
                e.setDate(it);
                e.setTitle(QString("%1_%2").arg(it.toString("dd-MM-yyyy")).arg(i));
                e.setValue(seed.bounded(0, 1000000)/1000.);
                e.setType(type);
                json.addEntry(e);

                qDebug()<<it<<i;
            }
        }
    });

    t.waitForFinished();
    return status;
}
