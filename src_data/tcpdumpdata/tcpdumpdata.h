#ifndef TCPDUMPDATA_H
#define TCPDUMPDATA_H

#include "tcpdumppacket.h"
#include "data/data.h"
#include "node/node.h"
#include <QList>
#include <QByteArray>
#include <QSharedPointer>


class CTcpDumpData: public CData
{
  private:
    // Structures and variables.
    QByteArray m_magic_word;
    bool m_little_endian;
    QList<QSharedPointer<CTcpDumpPacket>> m_packets;
    CNode *m_reporting_node;

  public:
    explicit CTcpDumpData();
    virtual ~CTcpDumpData();
    virtual CDataPointer clone() const { return CDataPointer(); }
    // Set and unset the Node that will be used to report the progress of the
    // ... parsing.
    void setNodeReporter(CNode *node);
    void unsetNodeReporter();
    // The progress we are reporting using 'm_reporting_node'.
    void nodeReport(qint8 percentage);
    // Parse a byte array into several packets. Extract the tcp dump magic word too.
    bool parse(const QByteArray &blob);
    // How many packets are available.
    qint32 availablePackets() const;
    QSharedPointer<const CTcpDumpPacket> getPacket(int i) const;
    // If the IP checksum of the specified packet is correct, or if the packet
    // ... is not an IP packet return true.
    bool validIp(QSharedPointer<CTcpDumpPacket> packet);
    // Return true if the IP packet is not fragmented.
    bool defrag(QSharedPointer<CTcpDumpPacket> packet);

  private:
    quint32 parseHeader(const QByteArray &blob);
    // Parse a TCP packet and return how many bytes were used in the parsing.
    quint32 parsePackets(const QByteArray &blob, quint32 offset);
    // Return 4 bytes as a single number taking into account endianess.
    quint32 get4Bytes(const QByteArray &blob, quint32 offset);
    // Parse the protocol layer.
    void parseIpProtocol(QSharedPointer<CTcpDumpPacket> packet);
};

#endif // TCPDUMPDATA_H
