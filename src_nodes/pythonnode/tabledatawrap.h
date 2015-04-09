#ifndef CTABLEDATAWRAP_H
#define CTABLEDATAWRAP_H

#include "tabledata/tabledata.h"
#include <QObject>


class CTableDataWrap : public QObject
{
    Q_OBJECT

  public:
    CTableDataWrap(void* table_data);

  public slots:
    qint32 rowCount();
    QList<QVariant> row(qint32 i_row);

  private:
    CTableData* m_table_data;
};

#endif // CTABLEDATAWRAP_H
