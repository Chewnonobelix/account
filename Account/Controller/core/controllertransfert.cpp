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
    Entry in(m_entry), out(m_entry);

    out.setAccount(m_outcomeAccount);
    out.setType("outcome");
    setCurrentAccount(out.account());
    addEntry(out);

    in.setType("income");
    in.setAccount(m_incomeAccount);
    setCurrentAccount(in.account());
    addEntry(in);

    emit s_finish();
}

void ControllerTransfert::onDateChanged(QDate d)
{
    m_entry.setDate(d);
    auto i = m_entry.info();
    i.setEstimated(d > QDate::currentDate());
    m_entry.setInfo(i);
}

void ControllerTransfert::onTitleChanged(QString t)
{
    auto i = m_entry.info();
    i.setTitle(t);
    m_entry.setInfo(i);
}

void ControllerTransfert::onValueChanged(double v)
{
    m_entry.setValue(v);
}
