#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QRandomGenerator>
#include "Controller/maincontroller.h"
#include "Controller/controllerbudget.h"
#include "Controller/controllerfrequency.h"
#include "Controller/controllercommon.h"
#include "Model/budget.h"

void testCharge()
{
    ControllerXMLMulti m_xml;
    QRandomGenerator rdn;
    int i = 1;
    QDate cDate = QDate::currentDate();
    QDate itDate = cDate.addYears(-30);
    QString accountName = "charge_"+cDate.toString("dd-MM-yyyy");
    bool ret = true;
    rdn.bounded(10000.0);

    while(itDate < cDate.addYears(30))
    {
        for(i = 0; i < 5; i++)
        {
            Entry e;
            e.setDate(itDate);
            e.setAccount(accountName);
            Information info;
            info.setTitle("entry " + QString::number(rdn.generate()));
            info.setEstimated(itDate > QDate::currentDate());
            e.setValue(rdn.generateDouble()*10);
            e.setInfo(info);
            e.setType(rdn.generate()%2 ? "income":"outcome");

            ret &= m_xml.addEntry(e);
        }
        m_xml.close();
        itDate = itDate.addDays(1);
    }
}

void testBudget()
{
    Entry e1, e2;
    e1.setId(1); e2.setId(2);
    e1.setValue(2); e2.setValue(3);
    e1.setType("outcome"); e2.setType("outcome");
    Budget b;

    b<<e1<<e1<<e2;
    b>>e1>>e2>>e2;
}

int main(int argc, char *argv[])
{
    using namespace Account;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    app.setApplicationName("Account");
    app.setOrganizationName("Chewnonobelix Inc");
//    XmlTest test1;

    if(argc > 1 && app.arguments().contains("--test"))
    {
//        testCharge();
        testBudget();
    }

    int index = app.arguments().indexOf("--s");
    int type = index > -1 ? app.arguments()[index + 1].toInt() : 0;
    MainController mc(type);

    mc.exec();

    return app.exec();
}
