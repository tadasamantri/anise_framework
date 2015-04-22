#ifndef MAWIEXTRACTORNODE_H
#define MAWIEXTRACTORNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "tabledata/tabledata.h"
#include <QObject>
#include <QString>


class CMawiExtractorNode: public CNode
{
  Q_OBJECT

  private:
    // Data Structures
    QSharedPointer<const CTableData> m_flows_table;
    QSharedPointer<const CTableData> m_labels_table;

  public:
    // Constructor
    explicit CMawiExtractorNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual bool data(QString gate_name, const CConstDataPointer &data);
    // Create new table for the extracted flows copying the header of the
    // ... supplied table.
    QSharedPointer<CTableData> createTable(
            const QSharedPointer<const CTableData> &old_table);
    // Extract flows from 'old_flows' into 'new_flows'. Return true if
    // ... the extraction was successfull.
    bool extractFlows(const QSharedPointer<const CTableData> &old_flows,
                      const QSharedPointer<const CTableData> &labels,
                      const QSharedPointer<CTableData> &new_flows);
    // Obtain the correct string representation of an IP address from
    // ... a possibly split IP.
    QString buildIp(const QList<QVariant> &row,
                    qint32 i_ip, qint32 i_ip3, qint32 i_ip2,
                    qint32 i_ip1, qint32 i_ip0);
};

#endif // MAWIEXTRACTORNODE_H

