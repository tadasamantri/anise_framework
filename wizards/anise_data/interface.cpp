#include "interface.h"
#include "%ProjectName:l%data.h"

extern "C"
{
    CData *maker()
    {
        return new C%ProjectName:c%Data();
    }
}
