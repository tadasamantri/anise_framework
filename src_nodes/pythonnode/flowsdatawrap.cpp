#include "flowsdatawrap.h"

CFlowsDataWrap::CFlowsDataWrap(const void *flows_data, QObject *parent)
    : QObject(parent)
    , m_flows_data(static_cast<const CTcpStreamsData*>(flows_data))
{

}
