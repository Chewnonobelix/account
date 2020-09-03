#include "controllertransfert.h"

ControllerTransfert::ControllerTransfert(): AbstractController()
{

}

ControllerTransfert::~ControllerTransfert()
{}

int ControllerTransfert::exec()
{
    emit accountListChanged(m_db->selectAccount());
    emit openChanged();

    return 0;
}

void ControllerTransfert::set(QObject * view)
{
    m_view = view;
}


void ControllerTransfert::accept()
{
    QObject *to, *from, *val, *info, *dateField;
    QString accountOut, accountIn, title; QDate date;
    double transfertVal;
    to = m_view->findChild<QObject*>("toCombo");
    from = m_view->findChild<QObject*>("fromCombo");
    val = m_view->findChild<QObject*>("spinVal");
    info = m_view->findChild<QObject*>("fieldInfo");
    dateField = m_view->findChild<QObject*>("dateField");

    accountOut = from->property("currentText").toString();
    accountIn = to->property("currentText").toString();
    title = info->property("text").toString();
    transfertVal = val->property("value").toInt() / 100.0;
    date = dateField->property("selectedDate").toDate();

    Entry in, out;

    out.setAccount(accountOut);
    out.setDate(date);
    out.setValue(transfertVal);
    out.setType("outcome");
    Information inf;
    inf.setTitle(title);
    inf.setEstimated(out.date() > QDate::currentDate());
    out.setInfo(inf);

    in = out;
    in.setType("income");
    in.setAccount(accountIn);
    setCurrentAccount(in.account());
    addEntry(in);
    setCurrentAccount(out.account());
    addEntry(out);

    emit s_finish();
}
