#ifndef CFLOWWRAP_H
#define CFLOWWRAP_H

#include "tcpstreamsdata/tcpstream.h"
#include <QObject>

class CFlowWrap : public QObject
{
  Q_OBJECT

  public:
    explicit CFlowWrap(void* ptr, QObject *parent = 0);

  public slots:
    quint32 src() const { return m_flow->source_addr; }
    quint32 dst() const { return m_flow->destination_addr; }
    double start_time() const { return m_flow->start_time; }
    double finish_time() const { return m_flow->finish_time; }

  private:
    CTcpStream* m_flow;
};

#endif // CFLOWWRAP_H
