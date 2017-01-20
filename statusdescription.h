#ifndef STATUSDESCRIPTION_H
#define STATUSDESCRIPTION_H


#include "visa.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE         512
#endif

class StatusDescription
{
public:
    StatusDescription();
    ViStatus statusDesc(ViObject vi, ViStatus status, ViChar desc[]);

private:

};

#endif // STATUSDESCRIPTION_H
