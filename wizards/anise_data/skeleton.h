#ifndef %ProjectName:u%DATA_H
#define %ProjectName:u%DATA_H

#include "data/data.h"

class C%ProjectName:c%Data: public CData
{
  public:
    explicit C%ProjectName:c%Data();
    virtual CDataPointer clone() const;
};

#endif // %ProjectName:u%DATA_H
