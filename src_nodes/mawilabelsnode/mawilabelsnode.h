#ifndef MAWILABELSNODE_H
#define MAWILABELSNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "tabledata/tabledata.h"
#include <QObject>
#include <QString>
#include <QSharedPointer>


class CMawiLabelsNode: public CNode
{
    Q_OBJECT

  private:
    // Data Structures
  QSharedPointer<CTableData> m_table_data;

  public:
    // Constructor
    explicit CMawiLabelsNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual bool data(QString gate_name, const CConstDataPointer &data);

  private:
    QSharedPointer<CTableData> createTable();
    bool parseMawiXml(const QByteArray& bytes, QSharedPointer<CTableData> &table_data);
};

#endif // MAWILABELSNODE_H

