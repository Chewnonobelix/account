#include "controllerinformation.h"

int ControllerInformation::exec()
{
    return 0;
}

void ControllerInformation::view(QUuid id)
{
    if(id == m_entry.id())
        return;

    m_entry = AbstractController::entry(id);

    emit view(QVariant::fromValue(m_entry));


    if (m_entry.hasMetadata("frequency")) {
        emit frequencyVisible(m_entry.hasMetadata("frequency"));
        QUuid f = m_entry.metaData<QUuid>("frequency");
        int g = m_entry.metaData<int>("freqGroup");

        int page = m_currentPage;
        page--;

        QVariantList model, le;
        QList<Entry> les = db()->selectEntry().values();
        std::for_each(les.begin(), les.end(), [&le, f, g](Entry e) {
            if (e.metaData<QUuid>("frequency") == f && e.metaData<int>("freqGroup") == g)
                le << QVariant::fromValue(e);
        });

        emit maxPageChanged(le.size() / 100 + 1);
        emit currentPageChanged(1);
        for (int i = (page * 100); i < le.size() && i < (page + 1) * 100; i++)
            model << le[i];

        emit pageModel(QVariant::fromValue(model));
    }


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
    auto list = m_db->selectCategory()[m_entry.type()];
    Category c = list[QUuid::fromString(cat)];

    m_entry.setCategory(c);

    updateEntry(m_entry);
}

void ControllerInformation::addNewCategory(QString cat)
{
    auto type = m_entry.type();
    auto cats = db()->selectCategory()[type].values();
    auto f = std::find_if(cats.begin(), cats.end(), [cat, type](Category c) {
        return c.name() == cat && c.type() == type;
    });

    if (f != cats.end())
        return;

    Category c;
    c.setName(cat);
    c.setType(m_entry.type());
    m_db->addCategory(c);

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
    m_entry.setSupport((Account::SupportEnum) support);
    m_db->updateEntry(m_entry);
    emit s_exec();
}
