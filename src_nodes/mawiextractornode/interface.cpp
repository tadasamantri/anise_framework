#include "interface.h"
#include "mawiextractornode.h"

extern "C"
{
void configure(CNodeConfig &config)
{
    CMawiExtractorNode::configure(config);
}

CNode *maker(const CNodeConfig &config)
{
    return new CMawiExtractorNode(config);
}
}

