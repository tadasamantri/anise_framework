#include "interface.h"
#include "mawilabelsnode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CMawiLabelsNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CMawiLabelsNode(config);
}
}

