#ifndef METADATA_H
#define METADATA_H

#include <QMap>
#include <QString>
#include <QVariant>
class MetaData
{
private:
    QMap<QString, QVariant> m_metaData;

public:
    MetaData();
    MetaData(const MetaData&);
    ~MetaData();

    bool hasMetadata() const;
    bool hasMetadata(QString) const;
    QStringList metaDataList() const;
    void removeMetaData(QString);
    template<class T>
    void setMetadata(QString name, T val)
    {
        m_metaData[name] = val;
    }

    template<class T>
    T metaData(QString name) const
    {
        return m_metaData[name].value<T>();
    }

    MetaData& operator = (const MetaData&);

};

#endif // METADATA_H
