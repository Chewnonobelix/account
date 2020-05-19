#include "metadata.h"

MetaData::MetaData()
{

}

MetaData::MetaData(const MetaData& md): m_metaData(md.m_metaData)
{

}
MetaData::~MetaData()
{

}

bool MetaData::hasMetadata() const
{
    return !m_metaData.isEmpty();
}

bool MetaData::hasMetadata(QString name) const
{
    return m_metaData.contains(name);
}

QStringList MetaData::metaDataList() const
{
    return m_metaData.keys();
}

MetaData& MetaData::operator = (const MetaData& md)
{
    m_metaData = md.m_metaData;

    return *this;
}

void MetaData::removeMetaData(QString key)
{
    m_metaData.remove(key);
}
