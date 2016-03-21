

#include "measured_distribution.h"

measured_distribution::measured_distribution() : canopy()
{
    distributionType = measured;
    fodist = NULL;
    n_fodist = 0;
    zh = NULL;
    n_zh = 0;
}

measured_distribution::measured_distribution(std::string filename)
{
    distributionType = measured;

    FILE* f = fopen(filename.c_str(), "r");
    if(f == NULL)
        throw std::runtime_error("Cannot open file.\n");

    char buff[1024];
    char* p;
    //int count;

    while(fgets(buff, 1024, f) != NULL)
    {
        p = buff;
        while(isspace(*p))
            p++;
        if((*p == '\0') || (*p == '%'))
            continue;

        sscanf(buff, "cdh = %lf", &dragCoefAth);
        sscanf(buff, "h = %lf", &canopyHeight);
        sscanf(buff, "m = %d", &n_fodist);
    }

    fodist = new double[n_fodist];
    zh =  new double[n_fodist + 1];

    fseek(f, 0, SEEK_SET);
    while(fgets(buff, 1024, f) != NULL)
    {
        p = buff;
        while(isspace(*p))
            p++;
        if((*p == '\0') || (*p == '%'))
            continue;

        if(strncmp(p, "fodist", 6) == 0)
        {
            p = strchr(p, '=');
            p++;
            for(int i=0; i<n_fodist; i++)
            {
                while(isspace(*p))
                    p++;
                sscanf(p, "%lf", &fodist[i]);
                while(!isspace(*p))
                    p++;
            }
        }

        if(strncmp(p, "zh", 2) == 0)
        {
            p = strstr(p, "z0h");
            p = p+3;
            zh[0] = z0g / canopyHeight;
            for(int i=1; i<n_fodist+1; i++)
            {
                while(isspace(*p))
                    p++;
                sscanf(p, "%lf", &zh[i]);
                while(!isspace(*p))
                    p++;
            }
        }
    }

    //Compute leaf area index
    leafAreaIndex = 0.0;
    for(int i=0; i<n_fodist; i++)
        leafAreaIndex += fodist[i] * (zh[i+1] - zh[i]) * canopyHeight;


}

measured_distribution::measured_distribution(measured_distribution &rhs) : canopy(rhs)
{
    distributionType = rhs.distributionType;
    n_fodist = rhs.n_fodist;
    if(rhs.fodist)
    {
        fodist = new double[n_fodist];
        memcpy(fodist, rhs.fodist, sizeof(double) * n_fodist);
    }else{
        fodist = NULL;
    }
    n_zh = rhs.n_zh;
    if(rhs.zh)
    {
        zh = new double[n_zh];
        memcpy(zh, rhs.zh, sizeof(double) * n_zh);
    }else{
        zh = NULL;
    }

}

measured_distribution &measured_distribution::operator=(const measured_distribution &rhs)
{
    if(&rhs != this)
    {
        canopy::operator=(rhs);
        n_fodist = rhs.n_fodist;
        if(rhs.fodist)
        {
            delete[] fodist;
            fodist = new double[n_fodist];
            memcpy(fodist, rhs.fodist, sizeof(double) * n_fodist);
        }else{
            fodist = NULL;
        }
        n_zh = rhs.n_zh;
        if(rhs.zh)
        {
            delete[] zh;
            zh = new double[n_zh];
            memcpy(zh, rhs.zh, sizeof(double) * n_zh);
        }else{
            zh = NULL;
        }
    }
    return *this;
}

measured_distribution::~measured_distribution()
{
    delete[] fodist;
    fodist = NULL;
    delete[] zh;
    zh = NULL;
}

void measured_distribution::compute_haz()
{
    double integHazn = 0.0;
    //double zheight;

    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //double intermediate = cellsize*2.0 / 3.0;
    double intermediate = 2.0 / 3.0;
    //---------------FIX THIS------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int cellIndex = (int)(zh[0]/cellsize + 0.5);   //round here using +0.5 and int!
    for(int i=0; i<=cellIndex; i++)
        haz[i] = 0.0;
    int cellIndexOld;
    for(int j=0; j<n_fodist; j++)   //loop over fodist
    {
        cellIndexOld = cellIndex;
        cellIndex = (int)(zh[j + 1] / cellsize + 0.5);   //round here using +0.5 and int!
        for(int i=cellIndexOld+1; i<=cellIndex; i++)
        {
            haz[i] = fodist[j];
            if(i%2 == 0)    //if even numbers
                integHazn += haz[i];
            else            //if odd numbers
                integHazn += 2.0 * haz[i];
        }
    }

    integHazn = integHazn - 0.5 * (haz[0] + haz[numNodes-1]);
    integHazn *= intermediate;

    for(int i=0; i<numNodes; i++)
        haz[i] = leafAreaIndex * haz[i] /integHazn;

}


