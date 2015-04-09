#ifndef CFLOWSDATAWRAP_H
#define CFLOWSDATAWRAP_H

#include "tcpstreamsdata/tcpstreamsdata.h"
#include "tcpstreamsdata/tcpstream.h"
#include <QObject>


class CFlowsDataWrap : public QObject
{
  Q_OBJECT

  public:
    explicit CFlowsDataWrap(const void* flows_data, QObject *parent = 0);

  public slots:
    qint32 openStreamsCount() const { return m_flows_data->openStreamsCount(); }
    qint32 closedStreamsCount() const { return m_flows_data->closedStreamsCount(); }
    inline QList<CTcpStream*> openStreams() const;
    inline QList<CTcpStream*> closedStreams() const;

  private:
    const CTcpStreamsData* m_flows_data;
};


QList<CTcpStream*> CFlowsDataWrap::openStreams() const
{
    return m_flows_data->getOpenStreams();
}

QList<CTcpStream*> CFlowsDataWrap::closedStreams() const
{
    return m_flows_data->getClosedStreams();
}

#endif // CFLOWSDATAWRAP_H
