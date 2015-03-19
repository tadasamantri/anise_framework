#include "%ProjectName:l%data.h"
#include <QDebug>


//------------------------------------------------------------------------------
// Constructor and Destructor

C%ProjectName:c%Data::C%ProjectName:c%Data()
    : CData()
{

}


//------------------------------------------------------------------------------
// Public Functions

CDataPointer C%ProjectName:c%Data::clone() const
{
    qWarning() << "Cloning not implemented.";
    return CDataPointer();
}


//------------------------------------------------------------------------------
// Private Functions
