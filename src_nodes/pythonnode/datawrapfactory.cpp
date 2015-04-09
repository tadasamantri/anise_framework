#include "datawrapfactory.h"
#include "tabledatawrap.h"
#include "flowsdatawrap.h"
#include "flowwrap.h"
#include <QDebug>
#include <typeinfo>


QObject *CDataWrapFactory::create(const QByteArray &classname, void *ptr)
{
    if(classname == "CTableData") {
        return new CTableDataWrap(ptr);
    }
    else if(classname == "CTcpStreamsData") {
        return new CFlowsDataWrap(ptr);
    }
    else if(classname == "CTcpStream") {
        return new CFlowWrap(ptr);
    }

    return nullptr;
}
