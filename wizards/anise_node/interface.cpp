#include "interface.h"
#include "%ProjectName:l%node.h"

extern "C"
{
    void configure(CNodeConfig &config)
    {
        C%ProjectName:c%Node::configure(config);
    }

    CNode *maker(const CNodeConfig &config)
    {
        return new C%ProjectName:c%Node(config);
    }
}
