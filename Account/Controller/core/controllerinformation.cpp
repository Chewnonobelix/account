#include "controllerinformation.h"

int ControllerInformation::exec()
{
    if (m_entry.hasMetadata("frequency")) {
        emit frequencyVisible(m_entry.hasMetadata("frequency"));

        if (m_entry.hasMetadata("frequency")) {
            QUuid f = m_entry.metaData<QUuid>("frequency");
            int g = m_entry.metaData<int>("freqGroup");
            auto freqs = m_db->selectFrequency();
            int page = m_currentPage;
            page--;
            for(auto it: freqs)
            {
                if(it.id() == f)
                {
                    auto le = it.listEntries(g);
                    QVariantList model;
                    emit maxPageChanged(le.size() / 100 + 1);
                    emit currentPageChanged(1);
                    for (int i = (page * 100); i < le.size() && i < (page + 1) * 100; i++)
                        model << le[i];

                    emit pageModel(QVariant::fromValue(model));
                }
            }
        }
    }

    return 0;
}

void ControllerInformation::view(QUuid id)
{
    m_entry = AbstractController::entry(id);

    QStringList catList /*= m_db->selectCategory().values(m_entry.type())*/;
    catList << "";
    emit this->catList(catList);

    emit view(QVariant::fromValue(m_entry));

    exec();
}

void ControllerInformation::titleChange(QString title)
{
    m_entry.setTitle(title);

    updateEntry(m_entry);
}

void ControllerInformation::valueChange(double value)
{
    m_entry.setValue(value);

    updateEntry(m_entry);
    emit s_update(m_entry.id());
}

void ControllerInformation::categoryChange(QString cat)
{
    QString old = m_entry.category();
    m_entry.setCategory(cat);

    updateEntry(m_entry);
    emit s_changeCat(old, m_entry.id());
}

void ControllerInformation::addNewCategory(QString cat)
{
    //    m_db->addCategory(cat, m_entry.type());
    categoryChange(cat);
    view(m_entry.id());
    emit s_exec();
    
}

void ControllerInformation::pageChange()
{
    exec();
}

void ControllerInformation::supportChange(int support)
{
    m_entry.setSupport((Account::SupportEnum)support);
    m_db->updateEntry(m_entry);
    emit s_exec();
}
