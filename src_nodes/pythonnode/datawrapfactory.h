#ifndef CDATAWRAPFACTORY_H
#define CDATAWRAPFACTORY_H

#include <PythonQtCppWrapperFactory.h>
#include <QObject>


// Class that knows how to wrap pointers with other QObjects that can
// ... work with the pointers of the desired Data structure.
class CDataWrapFactory : public PythonQtCppWrapperFactory
{
  public:
    virtual QObject* create(const QByteArray& classname, void* ptr);
};

#endif // CDATAWRAPFACTORY_H
