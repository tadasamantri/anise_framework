#include "tabledatawrap.h"
#include <QDebug>


CTableDataWrap::CTableDataWrap(void *table_data)
    : m_table_data(static_cast<CTableData*>(table_data))
{

}

qint32 CTableDataWrap::rowCount()
{
    return m_table_data->rowCount();
}

QList<QVariant> CTableDataWrap::row(qint32 i_row)
{
    return m_table_data->getRow(i_row);
}
