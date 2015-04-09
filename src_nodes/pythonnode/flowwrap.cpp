#include "flowwrap.h"

CFlowWrap::CFlowWrap(void *ptr, QObject *parent)
    : QObject(parent)
    , m_flow(static_cast<CTcpStream*>(ptr))
{

}
