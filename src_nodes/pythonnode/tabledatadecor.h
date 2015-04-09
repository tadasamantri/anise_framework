#ifndef TABLEDECOR_H
#define TABLEDECOR_H

#include <QObject>
#include "tabledata/tabledata.h"

class CTableDataDecor : public QObject
{
  Q_OBJECT

  public:
    CTableDataDecor(QObject* parent = 0): QObject(parent) {}

  public slots:
    CTableData* new_CTableData() { return new CTableData(); }
    void delete_CTableData(CTableData* data) { delete data; }

    // Access methods for the CTableData object
    qint32 getRowCount(const CTableData* data) { return data->rowCount(); }
};

#endif // TABLEDECOR_H
