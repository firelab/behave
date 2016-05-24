#ifndef CROWNENUMS_H
#define CROWNENUMS_H

struct CrownModuleActivationMode
{
    enum CrownModuleActivationModeEnum
    {
        INACTIVE = 0,    // Crown module is turned off in behave runs                   
        ACTIVE = 1      // Crown module is turned on in behave runs                 
    };
};

#endif // CROWNENUMS_H
