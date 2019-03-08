#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QRandomGenerator>
#include "Controller/maincontroller.h"
#include "Model/budget.h"

//#include "Test/xmltest.h"

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
        qDebug()<<"Test days"<<itDate;

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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qDebug()<<"ControllerDB"<<qRegisterMetaType<ControllerDB>()<<sizeof(int*);
    qDebug()<<"ControllerXML"<<qRegisterMetaType<ControllerXML>();
    qDebug()<<"ControllerXMLMulti"<<qRegisterMetaType<ControllerXMLMulti>();


//    XmlTest test1;

    if(argc > 1 && QString(argv[1]) == "--test")
    {
//        testCharge();
        testBudget();
    }

    MainController mc;

    mc.exec();

    return app.exec();
}
