#include <QtTest>
#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

#include "../Account/Controller/controllerxmlmulti.h"

// add necessary includes here

class XmlTest : public QObject
{
    Q_OBJECT
private:
    ControllerXMLMulti m_xml;
    bool ret;
    int m_timer;
    QRandomGenerator rdn;

public:
    XmlTest();
    ~XmlTest();

protected:
    void timerEvent(QTimerEvent *);

private slots:
    void initTestCase();
    void testAccounts();
    void testEntries();
    void testCharge();
    void cleanupTestCase();
};

XmlTest::XmlTest()
{

}

XmlTest::~XmlTest()
{

}

void XmlTest::timerEvent(QTimerEvent *)
{
    m_xml.close();
}

void XmlTest::initTestCase()
{
    QVERIFY(m_xml.init());
    ret = true;
    m_timer = startTimer(60*1000*0.5);
    rdn.bounded(10000.0);

}

void XmlTest::testEntries()
{

    QString accountName = m_xml.selectAccount()[rdn.generate()%m_xml.selectAccount().size()];
    auto entries = m_xml.selectEntry(accountName);

    bool ret = true;
    QDate date = QDate::currentDate();
    static int j = 0;
    date = date.addDays(j);
    j++;
    for(int i = 0; i < 100; i ++)
    {
        Entry e;
        e.setDate(date);
        e.setAccount(accountName);
        Information info;
        info.setTitle("entry " + QString::number(rdn.generate()));
        info.setEstimated(date > QDate::currentDate());
        e.setValue(rdn.generateDouble());
        e.setInfo(info);
        e.setType(rdn.generate()%2 ? "income":"outcome");

        ret &= m_xml.addEntry(e);
    }

    int toRemove = rdn.generate() % 20;

    auto entries3 = m_xml.selectEntry(accountName);


    QCOMPARE(entries3.size(), entries.size() + 100);
    //    ret &= (entries.size() + 100) == entries3.size();
    //    qDebug()<<"Add result"<<ret<<entries3.size();

    for(int i = 0; i < toRemove; i++)
    {
        auto entries2 = m_xml.selectEntry(accountName);
        ret &= m_xml.removeEntry(entries2[rdn.generate()%entries2.size()]);
    }

    auto entries2 = m_xml.selectEntry(accountName);

    ret = entries2.size() == (entries.size() + (100-toRemove));

    QCOMPARE(entries2.size(), entries.size() + (100-toRemove));
}

void XmlTest::testAccounts()
{
    int totalAccount = m_xml.selectAccount().size();

    auto list = m_xml.selectAccount();
    bool ret = true;
    int first = 0;

    for(auto it: list)
    {
        int ints = (it.split("_").last().toInt());
        first =  ints > first ? ints : first ;
    }

    for(int i = 0; i < 81; i ++)
    {
        QVERIFY(ret);
        QString accountName = "account_" + QString::number(i+first+1);
        Entry e;
        Information info;
        info.setTitle("initial");
        e.setAccount(accountName);
        e.setType("income");
        e.setValue(rdn.generateDouble());
        e.setInfo(info);

        ret &= m_xml.addEntry(e);
        qDebug()<<ret<<i<<(i+first)<<first;
    }

    auto list2 =  m_xml.selectAccount();
    int totalAccount2 = m_xml.selectAccount().size();


    QCOMPARE(totalAccount+81, totalAccount2);
    ret &=(totalAccount+81 == totalAccount2);
    qDebug()<<"Add account "<<ret;
    qDebug()<<"Remove"<<m_xml.selectAccount().last();
    QVERIFY(m_xml.removeAccount(m_xml.selectAccount().last()));

    //    return ret && (totalAccount+1 == m_xml.selectAccount().size());
}

void XmlTest::testCharge()
{
    int i = 1;
    QDate cDate = QDate::currentDate();
    QDate itDate = cDate.addYears(-30);
    QString accountName = "charge_"+cDate.toString("dd-MM-yyyy");
    bool ret = true;

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
        itDate = itDate.addDays(1);
    }
    m_xml.close();

    QVERIFY(ret);
}


void XmlTest::cleanupTestCase()
{
    killTimer(m_timer);
}

QTEST_APPLESS_MAIN(XmlTest)

#include "tst_xmltest.moc"
