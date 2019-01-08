#include <QtTest>
#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

#include "../Account/Controller/controllerxml.h"

// add necessary includes here

class XmlTest : public QObject
{
    Q_OBJECT
private:
    ControllerXML m_xml;
    bool ret;
public:
    XmlTest();
    ~XmlTest();

private slots:
    void initTestCase();
    void testAccounts();
    void testEntries();
    void testCharge();
};

XmlTest::XmlTest()
{

}

XmlTest::~XmlTest()
{

}

void XmlTest::initTestCase()
{
    QVERIFY(m_xml.init());
    ret = true;
}

void XmlTest::testEntries()
{
    QRandomGenerator rdn;
    rdn.bounded(10000.0);

    QString accountName = m_xml.selectAccount()[rdn.generate()%m_xml.selectAccount().size()];
    auto entries = m_xml.selectEntry(accountName);

    bool ret = true;
    for(int i = 0; i < 100; i ++)
    {
        Entry e;
        e.setDate(QDate::currentDate());
        e.setAccount(accountName);
        Information info;
        info.setTitle("entry " + QString::number(rdn.generate()));
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
    QRandomGenerator rdn;
    rdn.bounded(10000.0);
    auto list = m_xml.selectAccount();
    bool ret = true;
    for(int i = 0; i < 2; i ++)
    {
        QString accountName = "account " + QString::number(i+totalAccount);
        Entry e;
        Information info;
        info.setTitle("initial");
        e.setAccount(accountName);
        e.setType("income");
        e.setValue(rdn.generateDouble());
        e.setInfo(info);

        ret &= m_xml.addEntry(e);
    }

    auto list2 =  m_xml.selectAccount();
    int totalAccount2 = m_xml.selectAccount().size();

    ret &=(totalAccount+2 == totalAccount2);
    qDebug()<<"Add account "<<ret;
    qDebug()<<"remove account"<<m_xml.removeAccount(m_xml.selectAccount().last());

//    return ret && (totalAccount+1 == m_xml.selectAccount().size());
}

void XmlTest::testCharge()
{
    QTime begin;
    begin.start();
    int i = 1;
    while(i < 100)
    {
        testEntries();
        qDebug()<<(ret)<<i;
        i++;
    }
}

QTEST_APPLESS_MAIN(XmlTest)

#include "tst_xmltest.moc"
