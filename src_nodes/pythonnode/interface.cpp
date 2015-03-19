#include "interface.h"
#include "pythonnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CPythonNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CPythonNode(config);
}
}

