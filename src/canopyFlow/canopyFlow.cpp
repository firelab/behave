#include "canopyFlow.h"

canopyFlow::canopyFlow()
{
    C = NULL;
    usuh = 0.0;
    uzc = NULL;
    uzcs = NULL;
    doh = 0.0;
    z0oh = 0.0;
    one_doh = 0.0;
    K = 0.4;
    c1 = 0.38;
    c3 = 15.0;
    STRSC = 0.5;
    rough = 1.07;
    logRough = log(rough);
    measuredDataExists = false;
    n_measured = 0;
    measuredHeight = NULL;
    measuredSpeed = NULL;
    measuredShear = NULL;
}

canopyFlow::canopyFlow(canopyFlow &rhs)
{
    if(rhs.C)
        make_canopy(rhs.C);
    else
        C = NULL;
    usuh = rhs.usuh;
    if(rhs.uzc)
    {
        uzc = new double[C->numNodes];
        memcpy(uzc, rhs.uzc, sizeof(double) * C->numNodes);
    }else{
        uzc = NULL;
    }
    if(rhs.uzcs)
    {
        uzcs = new double[C->numNodes];
        memcpy(uzcs, rhs.uzcs, sizeof(double) * C->numNodes);
    }else{
        uzcs = NULL;
    }
    doh = rhs.doh;
    z0oh = rhs.z0oh;
    one_doh = rhs.one_doh;
    K = rhs.K;
    c1 = rhs.c1;
    c3 = rhs.c3;
    STRSC = rhs.STRSC;
    rough = rhs.rough;
    logRough = rhs.logRough;
    measuredDataExists = rhs.measuredDataExists;
    n_measured = rhs.n_measured;
    if(rhs.measuredHeight)
    {
        measuredHeight = new double[n_measured];
        memcpy(measuredHeight, rhs.measuredHeight, sizeof(double) * n_measured);
    }else{
        measuredHeight = NULL;
    }
    if(rhs.measuredSpeed)
    {
        measuredSpeed = new double[n_measured];
        memcpy(measuredSpeed, rhs.measuredSpeed, sizeof(double) * n_measured);
    }else{
        measuredSpeed = NULL;
    }
    if(rhs.measuredShear)
    {
        measuredShear = new double[n_measured];
        memcpy(measuredShear, rhs.measuredShear, sizeof(double) * n_measured);
    }else{
        measuredShear = NULL;
    }
}

canopyFlow &canopyFlow::operator=(const canopyFlow &rhs)
{
    if(&rhs != this)
    {
        if(rhs.C)
            make_canopy(rhs.C);
        else
            C = NULL;
        usuh = rhs.usuh;
        if(rhs.uzc)
        {
            delete[] uzc;
            uzc = new double[C->numNodes];
            memcpy(uzc, rhs.uzc, sizeof(double) * C->numNodes);
        }else{
            uzc = NULL;
        }
        if(rhs.uzcs)
        {
            delete[] uzcs;
            uzcs = new double[C->numNodes];
            memcpy(uzcs, rhs.uzcs, sizeof(double) * C->numNodes);
        }else{
            uzcs = NULL;
        }
        doh = rhs.doh;
        z0oh = rhs.z0oh;
        one_doh = rhs.one_doh;
        K = rhs.K;
        c1 = rhs.c1;
        c3 = rhs.c3;
        STRSC = rhs.STRSC;
        rough = rhs.rough;
        logRough = rhs.logRough;
        measuredDataExists = rhs.measuredDataExists;
        n_measured = rhs.n_measured;
        if(rhs.measuredHeight)
        {
            delete[] measuredHeight;
            measuredHeight = new double[n_measured];
            memcpy(measuredHeight, rhs.measuredHeight, sizeof(double) * n_measured);
        }else{
            measuredHeight = NULL;
        }
        if(rhs.measuredSpeed)
        {
            delete[] measuredSpeed;
            measuredSpeed = new double[n_measured];
            memcpy(measuredSpeed, rhs.measuredSpeed, sizeof(double) * n_measured);
        }else{
            measuredSpeed = NULL;
        }
        if(rhs.measuredShear)
        {
            delete[] measuredShear;
            measuredShear = new double[n_measured];
            memcpy(measuredShear, rhs.measuredShear, sizeof(double) * n_measured);
        }else{
            measuredShear = NULL;
        }
    }
    return *this;
}

canopyFlow::~canopyFlow()
{
    delete C;
    C = NULL;
    delete[] uzc;
    uzc = NULL;
    delete[] uzcs;
    uzcs = NULL;
    delete[] measuredHeight;
    measuredHeight = NULL;
    delete[] measuredSpeed;
    measuredSpeed = NULL;
    delete[] measuredShear;
    measuredShear = NULL;
}

void canopyFlow::readData(std::string filename)
{
    measuredDataExists = true;

    dataFile = filename;
    FILE* f = fopen(filename.c_str(), "r");
    if(f == NULL)
        throw std::runtime_error("Cannot open measured data file.\n");

    char buff[1024];
    char* p;
    //int count;

    double ummd, uwmd;

    while(fgets(buff, 1024, f) != NULL)
    {
        p = buff;
        while(isspace(*p))
            p++;
        if((*p == '\0') || (*p == '%'))
            continue;

        sscanf(buff, "n = %d", &n_measured);
        sscanf(buff, "ummd = %lf", &ummd);
        sscanf(buff, "uwmd = %lf", &uwmd);
    }

    measuredHeight = new double[n_measured];
    measuredSpeed = new double[n_measured];
    measuredShear = new double[n_measured];

    fseek(f, 0, SEEK_SET);
    while(fgets(buff, 1024, f) != NULL)
    {
        p = buff;
        while(isspace(*p))
            p++;
        if((*p == '\0') || (*p == '%'))
            continue;

        if(strncmp(p, "zzm", 3) == 0)
        {
            p = strchr(p, '=');
            p++;
            for(int i=0; i<n_measured; i++)
            {
                while(isspace(*p))
                    p++;
                sscanf(p, "%lf", &measuredHeight[i]);
                while(!isspace(*p))
                    p++;
            }
        }
        if(strncmp(p, "umm ", 4) == 0)
        {
            p = strchr(p, '=');
            p++;
            for(int i=0; i<n_measured; i++)
            {
                while(isspace(*p))
                    p++;
                sscanf(p, "%lf", &measuredSpeed[i]);
                while(!isspace(*p))
                    p++;
            }
        }
        if(strncmp(p, "uwm ", 4) == 0)
        {
            p = strchr(p, '=');
            p++;
            for(int i=0; i<n_measured; i++)
            {
                while(isspace(*p))
                    p++;
                sscanf(p, "%lf", &measuredShear[i]);
                while(!isspace(*p))
                    p++;
            }
        }
    }

    for(int i=0; i<n_measured; i++)
    {
        measuredSpeed[i] /= ummd;
        measuredShear[i] /= uwmd;
    }
}

void canopyFlow::plot()
{
    double* y = new double[C->numNodes];
    for(int i=0; i<C->numNodes; i++)
    {
        y[i] = i*C->cellsize;
    }

    double* x = new double[C->numNodes];
    for(int i=0; i<C->numNodes; i++)
    {
        x[i] = uzcs[i];
    }

    //PLFLT xmin =0, ymin=0, xmax=1, ymax=1;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    ////plsdev("wxwidgets"); // sets the plot device to WX Widget which
    //// allows for viewing and saving the plot to a file
    //// Note that saving postscript from within widgets is buggy.
    //// other useful values in place of wxwidgets:
    //// xwin - X-window display to screen
    //// ps - postscript file
    //// psc - color postscript file
    //// Or just comment out line to get a list of choices
    ////plsdev("psc");
    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("uzcs.pdf");// sets the names of the output file

    //// Parse and process command line arguments.
    ////pls->parseopts( &argc, argv, PL_PARSE_FULL ); // device and other options
    //// can be set from the command-line:
    //// -dev devname        sets the output device to "devname"
    //// -o output_file      sets the output file name to output_file                // -h                  gives a list of all possible options


    //pls->init();           // start plplot object
    //pls->env(xmin, xmax, ymin, ymax, just, axis );
    ////Setup window size
    //// - just=0 sets axis so they scale indepedently
    //// - axis=0 draw axis box, ticks, and numeric labels
    ////   see "man plenv" for details
    //pls->lab( "uzcs", "z/h", "uzcs plot");

    //// Plot the data points - (num_points, x, y. plot_symbol)
    ////  - plot_symbol=9 sets a circle with a dot in the
    //// middle for the plot symbol - see "man plpoin"
    ////pls->poin( numNodes, (PLFLT*) x,(PLFLT*) y, 9 );
    //pls->line( C->numNodes, (PLFLT*) x,(PLFLT*) y);

    //delete pls; // close plot

    delete x;
    x = NULL;
    delete y;
    y = NULL;
}

void canopyFlow::plotWind(double inputSpeed, double inputHeight)
{

    double* measured_windSpeed;
    double* measured_z;
    if(measuredDataExists)
    {
        measured_z = new double[n_measured];
        for(int i=0; i<n_measured; i++)
            measured_z[i] = measuredHeight[i]*C->canopyHeight;

        measured_windSpeed = new double[n_measured];
        for(int i=0; i<n_measured; i++)
            measured_windSpeed[i] = measuredSpeed[i];
    }

    int cells = 10000;
    double cellsize = inputHeight/cells;

    double* z = new double[cells];
    for(int i=0; i<cells; i++)
        z[i] = i*cellsize;

    double* windSpeed = new double[cells];
    for(int i=0; i<cells; i++)
        windSpeed[i] = get_windspeed(inputSpeed, inputHeight, i*cellsize);

    double* zCanopy = new double[C->numNodes];   //set up canopy density z axis
    for(int i=0; i<C->numNodes; i++)
        zCanopy[i] = i * C->cellsize * C->canopyHeight;

    //PLFLT xmin =0, ymin=0, xmax=windSpeed[cells-1], ymax=z[cells-1];

    double maxHaz = C->haz[0];
    for(int i=0; i<C->numNodes; i++)
    {
        if(C->haz[i]>maxHaz)
            maxHaz = C->haz[i];
    }

    double maxWind = windSpeed[0];
    for(int i=0; i<cells; i++)
    {
        if(windSpeed[i]>maxWind)
            maxWind = windSpeed[i];
    }

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    ////plsdev("wxwidgets"); // sets the plot device to WX Widget which
    //// allows for viewing and saving the plot to a file
    //// Note that saving postscript from within widgets is buggy.
    //// other useful values in place of wxwidgets:
    //// xwin - X-window display to screen
    //// ps - postscript file
    //// psc - color postscript file
    //// Or just comment out line to get a list of choices
    ////plsdev("psc");
    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("windProfile.pdf");// sets the names of the output file

    //// Parse and process command line arguments.
    ////pls->parseopts( &argc, argv, PL_PARSE_FULL ); // device and other options
    //// can be set from the command-line:
    //// -dev devname        sets the output device to "devname"
    //// -o output_file      sets the output file name to output_file                // -h                  gives a list of all possible options

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);    //first change the color pallet0 first color(#1) to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->env(xmin, xmax, ymin, ymax, just, axis );
    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    ////pls->vsta();
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bnst", 0.0, 0, "bcnst", 0.0, 0 );
    //Setup window size
    // - just=0 sets axis so they scale indepedently
    // - axis=0 draw axis box, ticks, and numeric labels
    //   see "man plenv" for details

    //pls->lab( "wind speed", "height", "wind speed plot");

    // Plot the data points - (num_points, x, y. plot_symbol)
    //  - plot_symbol=9 sets a circle with a dot in the
    // middle for the plot symbol - see "man plpoin"
    //pls->poin( numNodes, (PLFLT*) x,(PLFLT*) y, 9 );

    //plot fill area for canopy
    double* xFill = new double[4];
    double* yFill = new double[4];
    xFill[0] = 0;
    yFill[0] = 0;
    xFill[1] = 0;
    yFill[1] = C->canopyHeight;
    xFill[2] = maxWind;
    yFill[2] = C->canopyHeight;
    xFill[3] = maxWind;
    yFill[3] = 0;
    //pls->scol0a( 0, 0, 255, 0, 0.5);
    //pls->col0( 0 );
    ////plscol0(1, 0, 255, 0);    //first change the color pallet0 first color(#1)
    ////plcol0(1);              //now change our font color to be the color #1
    //pls->fill(4, xFill, yFill);

    //plscol0(1, 0, 0, 0);    //first change the color pallet0 first color(#1)
    //plcol0(1);              //now change our font color to be the color #1
    //pls->line(cells, (PLFLT*) windSpeed,(PLFLT*) z);   //plot wind speed
    //plscol0(2, 1, 0, 0);    //first change the color pallet0 first color(#1) to be black (0,0,0)
    //plcol0(2);              //now change our font color to be the color #1
    //pls->line(cells, (PLFLT*) uzcs,(PLFLT*) z);   //plot ustar^2

    ////pls->ssym(0.0, 2.5);
    //if(measuredDataExists)
    //    pls->poin(n_measured, (PLFLT*) measured_windSpeed,(PLFLT*) measured_z, 22);   //plot measured wind speed

    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "Canopy Wind Flow" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "wind speed" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "height" );
    ////pls->wind(0.0, max, 0.0, zCanopy[C->numNodes-1]);  //reset window to haz coordinates
    //plscol0(1, 14, 143, 14);    //first change the color pallet0 first color(#1) to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1
    //pls->wind(0.0, maxHaz, ymin, ymax);  //reset window to haz coordinates
    //pls->box("cmstv", 0.0, 0, "", 0.0, 0);
    //pls->line(C->numNodes, (PLFLT*) C->haz,(PLFLT*) zCanopy);   //plot haz
    //pls->mtex( "t", 3.0, 0.5, 0.5, "Non-dimensional leaf area density" );

    //delete pls; // close plot

    delete windSpeed;
    windSpeed = NULL;
    delete z;
    z = NULL;
}

void canopyFlow::plotDimensionalWind(double inputSpeed, double inputHeight)
{
    // NOPs for parameters to silence C4100
    if (inputHeight == 0)
    {

    }
    if (inputSpeed == 0)
    {

    }

//    double* measured_windSpeed;
//    double* measured_shear;
//    double* measured_z;
//    if(measuredDataExists)
//    {
//        measured_z = new double[n_measured];
//        for(int i=0; i<n_measured; i++)
//            measured_z[i] = measuredHeight[i]*C->canopyHeight;
//
//        measured_windSpeed = new double[n_measured];
//        for(int i=0; i<n_measured; i++)
//            measured_windSpeed[i] = measuredSpeed[i] * log(Iz0) * get_uhuH(inputSpeed, inputHeight);    //multiply by log() and uhuH to scale data to the input speed and direction
//        measured_shear = new double[n_measured];
//        for(int i=0; i<n_measured; i++)
//            measured_shear[i] = measuredShear[i] * inputSpeed;    //multiply by log() and uhuH to scale data to the input speed and direction
//    }
//
//
//    int plotNodes = C->numNodes + (inputHeight - C->canopyHeight) / (C->cellsize * C->canopyHeight);
//
//    double* z = new double[plotNodes];
//    for(int i=0; i<plotNodes; i++){
//        z[i] = i*C->cellsize * C->canopyHeight;
//        //printf("%lf\n", z[i]);
//    }
//
//    double* windSpeed = new double[plotNodes];
//    for(int i=0; i<plotNodes; i++){
//        windSpeed[i] = get_windspeed(inputSpeed, inputHeight, z[i]);
////        if(i<10)
////            printf("i=%d\twind=%lf\n", i, windSpeed[i]);
////        if(i>9996 && i<11000)
////            printf("i=%d\twind=%lf\n", i, windSpeed[i]);
////        if(i>16090)
////            printf("i=%d\twind=%lf\n", i, windSpeed[i]);
//    }
//
//    double* windShear = new double[C->numNodes];
//    for(int i=0; i<C->numNodes; i++)
//        windShear[i] = uzcs[i] * inputSpeed;
//
//    double* zCanopy = new double[C->numNodes];   //set up canopy density z axis
//    for(int i=0; i<C->numNodes; i++)
//        zCanopy[i] = i * C->cellsize * C->canopyHeight;
//
//    //PLFLT xmin =0, ymin=0, xmax=windSpeed[plotNodes-1], ymax=z[plotNodes-1];
//
//    double maxHaz = C->haz[0];
//    for(int i=0; i<C->numNodes; i++)
//    {
//        if(C->haz[i]>maxHaz)
//            maxHaz = C->haz[i];
//    }
//
//    double maxWind = windSpeed[0];
//    for(int i=0; i<plotNodes; i++)
//    {
//        if(windSpeed[i]>maxWind)
//            maxWind = windSpeed[i];
//    }
//
//    //PLINT just=0, axis=0;
//    //plstream *pls;
//
//    //// plplot initialization
//
//    //pls = new plstream();  // declare plplot object
//
//    ////plsdev("wxwidgets"); // sets the plot device to WX Widget which
//    //// allows for viewing and saving the plot to a file
//    //// Note that saving postscript from within widgets is buggy.
//    //// other useful values in place of wxwidgets:
//    //// xwin - X-window display to screen
//    //// ps - postscript file
//    //// psc - color postscript file
//    //// Or just comment out line to get a list of choices
//    ////plsdev("psc");
//    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
//    ////red on black default
//    //plsfnam("windProfile.pdf");// sets the names of the output file
//
//    //// Parse and process command line arguments.
//    ////pls->parseopts( &argc, argv, PL_PARSE_FULL ); // device and other options
//    //// can be set from the command-line:
//    //// -dev devname        sets the output device to "devname"
//    //// -o output_file      sets the output file name to output_file                // -h                  gives a list of all possible options
//
//    //plscolbg(255,255,255);  //change background color
//
//    //pls->init();           // start plplot object
//    //plscol0(1, 0, 0, 0);    //first change the color pallet0 first color(#1) to be black (0,0,0)
//    //plscol0(2, 255, 0, 0);    //change the color pallet0 color#2 to be red
//    //plscol0(3, 14, 143, 14);    //change the color pallet0 color(#3) to be dark green (14,143,14)
//    //plcol0(1);              //now change our font color to be the color #1
//
//    ////pls->env(xmin, xmax, ymin, ymax, just, axis );
//    //pls->adv( 0 );
//    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
//    ////pls->vsta();
//    //pls->wind( xmin, xmax, ymin, ymax);
//    //pls->box( "bnst", 0.0, 0, "bcnst", 0.0, 0 );
//    ////Setup window size
//    // - just=0 sets axis so they scale indepedently
//    // - axis=0 draw axis box, ticks, and numeric labels
//    //   see "man plenv" for details
//
//    //pls->lab( "wind speed", "height", "wind speed plot");
//
//    // Plot the data points - (num_points, x, y. plot_symbol)
//    //  - plot_symbol=9 sets a circle with a dot in the
//    // middle for the plot symbol - see "man plpoin"
//    //pls->poin( numNodes, (PLFLT*) x,(PLFLT*) y, 9 );
//
//    //plot fill area for canopy    
//    //plcol0(3);              //now change our font color to be the color #3
//    //pls->wind(0.0, maxHaz, ymin, ymax);  //reset window to haz coordinates
//    //pls->box("cmstv", 0.0, 0, "", 0.0, 0);
//
//    //double* xFill;
//    //double* yFill;
//    //PLINT haz_num = C->numNodes + 2;
//    //xFill = new double[haz_num];
//    //yFill = new double[haz_num];
//    //xFill[0] = 0.0;
//    //yFill[0] = 0.0;
//    //for(int i=1; i<(haz_num-1); i++)
//    //{
//    //    xFill[i] = C->haz[i-1];
//    //    yFill[i] = zCanopy[i-1];
//    //}
//    //xFill[haz_num-1] = 0.0;
//    //yFill[haz_num-1] = yFill[haz_num-2];
//    //pls->fill(haz_num, (PLFLT*) xFill, (PLFLT*) yFill);
//    //pls->mtex( "t", 3.0, 0.5, 0.5, "Non-dimensional leaf area density" );
//
//
//
//    //pls->wind( xmin, xmax, ymin, ymax);
//    //plcol0(1);              //now change our font color to be the color #1
//    //pls->line(plotNodes, (PLFLT*) windSpeed,(PLFLT*) z);   //plot wind speed
//    //plcol0(2);              //now change our font color to be the color #2
//    //pls->line(C->numNodes, (PLFLT*) windShear,(PLFLT*) z);   //plot ustar^2
//
//    //pls->ssym(0.0, 2.5);
//    //if(measuredDataExists)
//    //{
//    //    plcol0(1);              //now change our font color to be the color #1
//    //    pls->poin(n_measured, (PLFLT*) measured_windSpeed,(PLFLT*) measured_z, 22);   //plot measured wind speed
//
//    //    plcol0(2);              //now change our font color to be the color #2
//    //    pls->poin(n_measured, (PLFLT*) measured_shear,(PLFLT*) measured_z, 22);   //plot measured shear (ustar^2?)
//
//    //    std::size_t found = dataFile.find_last_of("/\\");
//    //    plcol0(1);              //now change our font color to be the color #1
//    //    pls->ptex( xmax, 0.7, 1.0, 0.0, 1, dataFile.substr(found+1).c_str());   //print out data filename on graph
//    //}
//
//
//    //pls->schr(0, 1.6);  //change font size
//    //pls->mtex( "t", 4.0, 0.5, 0.5, "Canopy Wind Flow" );
//    //pls->schr(0, 1.0);  //change font size
//    //pls->mtex( "b", 3.0, 0.5, 0.5, "wind speed" );
//    //pls->mtex( "l", 3.0, 0.5, 0.5, "height" );
//    ////pls->wind(0.0, max, 0.0, zCanopy[C->numNodes-1]);  //reset window to haz coordinates
//
//
//    //plcol0(0);
//
//    //// Draw a legend
//    //PLINT        nlegend = 3;
//    //const char   *text[3], *symbols[3];
//    //PLINT        opt_array[3];
//    //PLINT        text_colors[3];
//    //PLINT        line_colors[3];
//    //PLINT        line_styles[3];
//    //PLFLT        line_widths[3];
//    //PLINT        symbol_numbers[3], symbol_colors[3];
//    //PLFLT        symbol_scales[3];
//    //PLFLT        legend_width, legend_height;
//
//    //// First legend entry.
//    //opt_array[0]   = PL_LEGEND_LINE;
//    //text_colors[0] = 1;
//    //text[0]        = "wind speed";
//    //line_colors[0] = 1;
//    //line_styles[0] = 1;
//    //line_widths[0] = 1.;
//    //symbols[0] = "";
//
//    //// Second legend entry.
//    //opt_array[1]      = PL_LEGEND_LINE;
//    //text_colors[1]    = 2;
//    //text[1]           = "reynold's stress";
//    //line_colors[1]    = 2;
//    //line_styles[1]    = 1;
//    //line_widths[1]    = 1.;
//    ////symbol_colors[1]  = 3;
//    ////symbol_scales[1]  = 1.;
//    ////symbol_numbers[1] = 4;
//    //symbols[1]        = "";
//    //// from the above opt_arrays we can completely ignore everything
//    //// to do with boxes.
//
//    //// Third legend entry.
//    //opt_array[2]   = PL_LEGEND_LINE;
//    //text_colors[2] = 3;
//    //text[2]        = "leaf area density";
//    //line_colors[2] = 3;
//    //line_styles[2] = 1;
//    //line_widths[2] = 1.;
//    //symbols[2] = "";
//
//    //pls->legend(&legend_width, &legend_height,
//    //            PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX, PL_POSITION_LEFT | PL_POSITION_TOP,
//    //            0.03, 0.03, 0.1, 0,
//    //            1, 1, 1, 1,
//    //            nlegend, opt_array,
//    //            1.0, 1.0, 2.0,
//    //            1., text_colors, (const char **) text,
//    //            NULL, NULL, NULL, NULL,
//    //            line_colors, line_styles, line_widths,
//    //            symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );
//
//    //delete pls; // close plot
//
//
//    if(measuredDataExists)
//    {
//        delete measured_windSpeed;
//        measured_windSpeed = NULL;
//        delete measured_shear;
//        measured_shear = NULL;
//        delete measured_z;
//        measured_z = NULL;
//    }
//    delete windSpeed;
//    windSpeed = NULL;
//    delete windShear;
//    windShear = NULL;
//    delete zCanopy;
//    zCanopy = NULL;
//    delete z;
//    z = NULL;
//    //delete xFill;
//    //xFill = NULL;
//    //delete yFill;
//    //yFill = NULL;
}

void canopyFlow::plotWAFvsCdLAI(double inputHeight, double midFlameHeight, double lowLAI, double highLAI, int profileType)
{
    int plotNodes = 1000;
    double* cdLAI = new double[plotNodes];
    double* WAFarrayMidFlame = new double[plotNodes];
    double* WAFarrayIntegral = new double[plotNodes];

    double LAIStepSize = (highLAI-lowLAI) / (plotNodes-1);

    for(int i=0; i<plotNodes; i++){
        C->leafAreaIndex = lowLAI + i * LAIStepSize;
        cdLAI[i] = C->leafAreaIndex*C->dragCoefAth;
        cdLAI[i] = log10(cdLAI[i]);
        C->initialize();
        computeWind();
        if(profileType == 0)    //sheltered
        {
            WAFarrayMidFlame[i] = get_windAdjustmentFactorShelteredMidFlame(inputHeight, midFlameHeight);
            WAFarrayIntegral[i] = get_windAdjustmentFactorShelteredIntegral(inputHeight, 2.0 * midFlameHeight);

        }else if(profileType == 1)  //unsheltered
        {
            WAFarrayIntegral[i] = get_windAdjustmentFactorUnshelteredIntegral(inputHeight, 2.0 * midFlameHeight);
        }else
            printf("\n\nError determining profileType\n");
    }

    //PLFLT xmin = cdLAI[0], ymin = 0.0, xmax = cdLAI[plotNodes-1], ymax = 1.0;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("WAF.pdf");// sets the names of the output file

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);        //change the color pallet0 color #1 to be black (0,0,0)
    //plscol0(2, 255, 0, 0);      //change the color pallet0 color #2 to be red (255,0,0)
    //plscol0(3, 255, 255, 255);  //change the color pallet0 color #3 to be white (255,255,255)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bcnstl", 0.0, 0, "bcnst", 0.0, 0 );

    //if(profileType == 0)    //sheltered
    //{
    //    pls->line(plotNodes, (PLFLT*) cdLAI,(PLFLT*) WAFarrayIntegral);   //plot WAF integral method
    //    plcol0(2);              //now change our font color to be the color #2
    //    pls->line(plotNodes, (PLFLT*) cdLAI,(PLFLT*) WAFarrayMidFlame);   //plot WAF midflame method
    //    plcol0(1);              //now change our font color to be the color #1
    //}else if(profileType == 1)  //unsheltered
    //{
    //    pls->line(plotNodes, (PLFLT*) cdLAI,(PLFLT*) WAFarrayIntegral);   //plot WAF integral method
    //}else
    //    printf("\n\nError determining profileType\n");


    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "Wind Adjustment Factor" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "Cd*LAI" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "WAF" );



    //// Draw a legend
    //PLINT        nlegend = 2;
    //const char   *text[2], *symbols[2];
    //PLINT        opt_array[2];
    //PLINT        text_colors[2];
    //PLINT        line_colors[2];
    //PLINT        line_styles[2];
    //PLFLT        line_widths[2];
    //PLINT        symbol_numbers[2], symbol_colors[2];
    //PLFLT        symbol_scales[2];
    //PLFLT        legend_width, legend_height;

    //// First legend entry.
    //opt_array[0]   = PL_LEGEND_LINE;
    //text_colors[0] = 1;
    //text[0]        = "integral method";
    //line_colors[0] = 1;
    //line_styles[0] = 1;
    //line_widths[0] = 1.;
    //symbols[0] = "";

    //// Second legend entry.
    //opt_array[1]      = PL_LEGEND_LINE;
    //text_colors[1]    = 2;
    //text[1]           = "midflame method";
    //line_colors[1]    = 2;
    //line_styles[1]    = 1;
    //line_widths[1]    = 1.;
    ////symbol_colors[1]  = 3;
    ////symbol_scales[1]  = 1.;
    ////symbol_numbers[1] = 4;
    //symbols[1]        = "";
    //// from the above opt_arrays we can completely ignore everything
    //// to do with boxes.

    //plscol0a( 15, 32, 32, 32, 0.70 );

    //if(profileType == 0)    //sheltered
    //{
    //    pls->legend(&legend_width, &legend_height,
    //                PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX, 0,
    //                0.03, 0.03, 0.1, 3,
    //                1, 1, 1, 1,
    //                nlegend, opt_array,
    //                1.0, 1.0, 2.0,
    //                1., text_colors, (const char **) text,
    //                NULL, NULL, NULL, NULL,
    //                line_colors, line_styles, line_widths,
    //                symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    //}else if(profileType == 1)  //unsheltered
    //{
    //    nlegend = nlegend-1;
    //    pls->legend(&legend_width, &legend_height,
    //                PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX, 0,
    //                0.03, 0.03, 0.1, 3,
    //                1, 1, 1, 1,
    //                nlegend, opt_array,
    //                1.0, 1.0, 2.0,
    //                1., text_colors, (const char **) text,
    //                NULL, NULL, NULL, NULL,
    //                line_colors, line_styles, line_widths,
    //                symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    //}else
    //    printf("\n\nError determining profileType\n");

    //pls->legend(&legend_width, &legend_height,
    //            PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX, 0,
    //            0.03, 0.03, 0.1, 3,
    //            1, 1, 1, 1,
    //            nlegend, opt_array,
    //            1.0, 1.0, 2.0,
    //            1., text_colors, (const char **) text,
    //            NULL, NULL, NULL, NULL,
    //            line_colors, line_styles, line_widths,
    //            symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    //delete pls; // close plot

    delete cdLAI;
    cdLAI = NULL;
    delete WAFarrayMidFlame;
    WAFarrayMidFlame = NULL;
    delete WAFarrayIntegral;
    WAFarrayIntegral = NULL;
}

void canopyFlow::plotz0ohvsCdLAI(double inputHeight, double lowLAI, double highLAI)
{
    // NOPs for parameters to silence C4100
    if (inputHeight == 0)
    {

    }

    int plotNodes = 1000;
    double* cdLAI = new double[plotNodes];
    double* z0ohArray = new double[plotNodes];

    double LAIStepSize = (highLAI-lowLAI) / (plotNodes-1);

    for(int i=0; i<plotNodes; i++){
        C->leafAreaIndex = lowLAI + i * LAIStepSize;
        cdLAI[i] = C->leafAreaIndex*C->dragCoefAth;
        cdLAI[i] = log10(cdLAI[i]);
        C->initialize();
        computeWind();
        z0ohArray[i] = z0oh;
    }

    double max_z0oh = 0.0;
    double min_z0oh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(z0ohArray[i] > max_z0oh)
            max_z0oh = z0ohArray[i];
        if(z0ohArray[i] < min_z0oh)
            min_z0oh = z0ohArray[i];
    }

    max_z0oh = 0.2;     //Just arbitrarily set this for the plot axis since that's what Bill did

    //PLFLT xmin = cdLAI[0], ymin = min_z0oh, xmax = cdLAI[plotNodes-1], ymax = max_z0oh;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("z0oh.pdf");// sets the names of the output file

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);        //change the color pallet0 color #1 to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bcnstl", 0.0, 0, "bcnst", 0.0, 0 );

    //pls->line(plotNodes, (PLFLT*) cdLAI,(PLFLT*) z0ohArray);   //plot line

    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "z0oh" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "Cd*LAI" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "z0oh" );

    //delete pls; // close plot

    delete cdLAI;
    cdLAI = NULL;
    delete z0ohArray;
    z0ohArray = NULL;
}

void canopyFlow::plotdohvsCdLAI(double inputHeight, double lowLAI, double highLAI)
{
    // NOPs for parameters to silence C4100
    if (inputHeight == 0)
    {

    }

    int plotNodes = 10000;
    double* cdLAI = new double[plotNodes];
    double* dohArray = new double[plotNodes];

    double LAIStepSize = (highLAI-lowLAI) / (plotNodes-1);

    for(int i=0; i<plotNodes; i++){
        C->leafAreaIndex = lowLAI + i * LAIStepSize;
        cdLAI[i] = C->leafAreaIndex*C->dragCoefAth;
        cdLAI[i] = log10(cdLAI[i]);
        C->initialize();
        computeWind();
        dohArray[i] = doh;
    }

    double max_doh = 0.0;
    double min_doh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(dohArray[i] > max_doh)
            max_doh = dohArray[i];
        if(dohArray[i] < min_doh)
            min_doh = dohArray[i];
    }

    max_doh = 1.0;     //Just arbitrarily set this for the plot axis since that's what Bill did

    //PLFLT xmin = cdLAI[0], ymin = min_doh, xmax = cdLAI[plotNodes-1], ymax = max_doh;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("doh.pdf");// sets the names of the output file

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);        //change the color pallet0 color #1 to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bcnstl", 0.0, 0, "bcnst", 0.0, 0 );

    //pls->line(plotNodes, (PLFLT*) cdLAI,(PLFLT*) dohArray);   //plot line

    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "doh" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "Cd*LAI" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "doh" );

    //delete pls; // close plot

    delete cdLAI;
    cdLAI = NULL;
    delete dohArray;
    dohArray = NULL;
}

void canopyFlow::plotz0ohvsone_doh(double inputHeight, double lowLAI, double highLAI)
{
    // NOPs for parameters to silence C4100
    if (inputHeight == 0)
    {

    }

    int plotNodes = 1000;
    double* z0ohArray = new double[plotNodes];
    double* one_dohArray = new double[plotNodes];

    double LAIStepSize = (highLAI-lowLAI) / (plotNodes-1);

    for(int i=0; i<plotNodes; i++){
        C->leafAreaIndex = lowLAI + i * LAIStepSize;
        C->initialize();
        computeWind();
        z0ohArray[i] = z0oh;
        one_dohArray[i] = one_doh;
    }

    double max_z0oh = 0.0;
    double min_z0oh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(z0ohArray[i] > max_z0oh)
            max_z0oh = z0ohArray[i];
        if(z0ohArray[i] < min_z0oh)
            min_z0oh = z0ohArray[i];
    }

    double max_one_doh = 0.0;
    double min_one_doh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(one_dohArray[i] > max_one_doh)
            max_one_doh = one_dohArray[i];
        if(one_dohArray[i] < min_one_doh)
            min_one_doh = one_dohArray[i];
    }

    //PLFLT xmin = min_one_doh, ymin = min_z0oh, xmax = max_one_doh, ymax = max_z0oh;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("z0oh_vs_one_doh.pdf");// sets the names of the output file

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);        //change the color pallet0 color #1 to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bcnst", 0.0, 0, "bcnst", 0.0, 0 );

    //pls->line(plotNodes, (PLFLT*) one_dohArray,(PLFLT*) z0ohArray);   //plot line

    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "z0oh_vs_one_doh" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "one_doh" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "z0oh" );

    //delete pls; // close plot

    delete z0ohArray;
    z0ohArray = NULL;
    delete one_dohArray;
    one_dohArray = NULL;
}

void canopyFlow::plotz0ohvsdoh(double inputHeight, double lowLAI, double highLAI)
{
    // NOPs for parameters to silence C4100
    if (inputHeight == 0)
    {

    }

    int plotNodes = 1000;
    double* z0ohArray = new double[plotNodes];
    double* dohArray = new double[plotNodes];

    double LAIStepSize = (highLAI-lowLAI) / (plotNodes-1);

    for(int i=0; i<plotNodes; i++){
        C->leafAreaIndex = lowLAI + i * LAIStepSize;
        C->initialize();
        computeWind();
        z0ohArray[i] = z0oh;
        dohArray[i] = doh;
    }

    double max_z0oh = 0.0;
    double min_z0oh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(z0ohArray[i] > max_z0oh)
            max_z0oh = z0ohArray[i];
        if(z0ohArray[i] < min_z0oh)
            min_z0oh = z0ohArray[i];
    }

    double max_doh = 0.0;
    double min_doh = 10000000000.0;

    for(int i=0; i<plotNodes; i++){
        if(dohArray[i] > max_doh)
            max_doh = dohArray[i];
        if(dohArray[i] < min_doh)
            min_doh = dohArray[i];
    }

    //PLFLT xmin = min_doh, ymin = min_z0oh, xmax = max_doh, ymax = max_z0oh;

    //PLINT just=0, axis=0;
    //plstream *pls;

    //// plplot initialization

    //pls = new plstream();  // declare plplot object

    //plsdev("pdf"); //cairo uses the same color scheme as on screen - black on
    ////red on black default
    //plsfnam("z0oh_vs_doh.pdf");// sets the names of the output file

    //plscolbg(255,255,255);  //change background color

    //pls->init();           // start plplot object
    //plscol0(1, 0, 0, 0);        //change the color pallet0 color #1 to be black (0,0,0)
    //plcol0(1);              //now change our font color to be the color #1

    //pls->adv( 0 );
    //pls->vpor( 0.15, 0.85, 0.1, 0.8 );
    //pls->wind( xmin, xmax, ymin, ymax);
    //pls->box( "bcnst", 0.0, 0, "bcnst", 0.0, 0 );

    //pls->line(plotNodes, (PLFLT*) dohArray,(PLFLT*) z0ohArray);   //plot line

    //pls->schr(0, 1.6);  //change font size
    //pls->mtex( "t", 4.0, 0.5, 0.5, "z0oh_vs_doh" );
    //pls->schr(0, 1.0);  //change font size
    //pls->mtex( "b", 3.0, 0.5, 0.5, "doh" );
    //pls->mtex( "l", 3.0, 0.5, 0.5, "z0oh" );

    //delete pls; // close plot

    delete z0ohArray;
    z0ohArray = NULL;
    delete dohArray;
    dohArray = NULL;
}

void canopyFlow::make_canopy(canopy::eCanopyType t)
{
    if(t == canopy::normal_distribution)
        C = new canopy_normal_distribution;
    else if(t == canopy::triangle)
        C = new canopy_triangle_distribution;
    else if(t == canopy::Massman)
        C = new massman_distribution;
    else if(t == canopy::measured)
        C = new measured_distribution;
    else
        throw std::logic_error("ERROR: Cannot determine canopy type in canopyFlow::make_canopy().\n");
}

void canopyFlow::make_canopy(canopy* X)
{
    delete C;
    if(X->distributionType == canopy::normal_distribution)
        C = new canopy_normal_distribution(*((canopy_normal_distribution*)X));
    else if(X->distributionType == canopy::triangle)
        C = new canopy_triangle_distribution(*((canopy_triangle_distribution*)X));
    else if(X->distributionType == canopy::Massman)
        C = new massman_distribution(*((massman_distribution*)X));
    else if(X->distributionType == canopy::measured)
        C = new measured_distribution(*((measured_distribution*)X));
    else
        throw std::logic_error("ERROR: Cannot determine canopy type in canopyFlow::make_canopy().\n");
}

void canopyFlow::computeWind()
{
    //C->initialize();
    usuh = c1 - (c1 + K / log(C->z0gh)) * exp(-c3 *C->zetah);
    double Csurf = 2.0 * usuh * usuh;
    double nexp = C->zetah / Csurf;

    double* nzet = new double[C->numNodes];
    uzc =  new double[C->numNodes];
    uzcs =  new double[C->numNodes];

    double max_uzc = 0.0;
    double qc = 0.60;
    double qb = 2.0 / (1 - exp(-1.0));
    double qa = 4.02 - qb;
    double qstar = qa + qb * exp(-qc * nexp);
    for(int i=0; i<C->numNodes; i++)
    {
        nzet[i] = nexp * C->zetaz[i];
        if(i*C->cellsize <= C->z0gh)
            uzc[i] = 0.0;
        else
            uzc[i] = log(i*C->cellsize / C->z0gh) * cosh(nzet[i]) / cosh(nexp);
        uzcs[i] = cosh(qstar * nzet[i]) / cosh(qstar * nexp);
        if(uzc[i] > max_uzc)
            max_uzc = uzc[i];
    }

    delete nzet;

    for(int i=0; i<C->numNodes; i++)
        uzc[i] /= max_uzc;

    //uzcs[0] = 0.0;
    //for(int i=1;i<C->numNodes;i++) //calculate Reynolds stress using cumulative trapazoid
    //    uzcs[i] = uzcs[i-1] + (C->hacpz[i-1]*uzc[i-1]*uzc[i-1] + C->hacpz[i]*uzc[i]*uzc[i]) * 0.5;

    //for(int i=1;i<C->numNodes;i++)
    //    uzcs[i] /= uzcs[C->numNodes-1];

    //compute lowest inflection point
//    int begin = (int) 2.0 * (C->z0gh/C->cellsize);
//    double difU;
//    double difUOld = uzc[begin] - 2.0*uzc[begin-1] + uzc[begin-2];   //start by computing the value of the second derivative at node = begin-1
//    int end;
//    for(int i=begin; i<C->numNodes-1; i++)
//    {
//        difU = uzc[i+1] - 2.0*uzc[i] + uzc[i-1];
//        if((difU*difUOld) < 0.0)   //if we've switched sign, break
//        {
//            end = i;
//            break;
//        }
//        difUOld = difU;
//    }

//    //now set the stress below the lowest inflection point to the value at the inflection point
//    for(int i=0; i<end; i++)
//        uzcs[i] = uzcs[end-1];




    //MASSMAN'S OLD METHOD
//    one_doh = 0.0;
//    for(int i=0; i<C->numNodes; i++)   //integrate using extended Simpson's rule
//    {
//        if(i%2 == 0)    //if even numbers
//            one_doh += uzcs[i];
//        else            //if odd numbers
//            one_doh += 2.0 * uzcs[i];
//    }

//    one_doh = one_doh - 0.5 * (uzcs[0] + uzcs[C->numNodes-1]);
//    one_doh *= C->cellsize * 2.0/3.0;
//    doh = 1.0 - one_doh;



    //MASSMAN'S NEW METHOD (SHAW AND PEREIRA 1982)
    double denom=0.0;
    double numer=0.0;
    for(int i=0; i<C->numNodes; i++) //integrate using extended Simpson's rule
    {
        if(i%2 == 0) //if even numbers
        {
            denom += uzcs[i];
            numer += uzcs[i]*i*C->cellsize;
        }
        else //if odd numbers
        {
            denom += 2.0 * uzcs[i];
            numer += 2.0 * uzcs[i]*i*C->cellsize;
        }
    }
    denom = denom - 0.5 * (uzcs[0] + uzcs[C->numNodes-1]);
    numer = numer - 0.5 * (uzcs[0]*0.0*C->cellsize + uzcs[C->numNodes-1]*(C->numNodes-1)*C->cellsize);
    denom*= C->cellsize * 2.0/3.0;
    numer*=C->cellsize*2.0/3.0;
    doh = (numer/denom)*(1- uzcs[0]);
    one_doh = 1-doh;





    z0oh = rough * one_doh * exp(-K/usuh);
    printf("doh = %lf\nz0oh = %lf\n", doh, z0oh);

    Iz0 = one_doh / z0oh;
}

/**
*@brief Computes wind adjustment factor using the mid flame height method for the sheltered case.
*@param inputHeight Height of input windspeed measured from the GROUND.
*@param midFlameHeight Midflame height measured from the GROUND.
*@return returns the wind adjustment factor, if negative the computation was in error
*/
double canopyFlow::get_windAdjustmentFactorShelteredMidFlame(double inputHeight, double midFlameHeight)
{
    double WAF;
    if(midFlameHeight < C->z0g)
    {
        WAF = 0.0;
    }else if(2.0 * midFlameHeight <= C->canopyHeight)    //below canopy (sheltered case)
    {
        WAF = log(Iz0) / log((inputHeight - C->canopyHeight)/(C->canopyHeight * z0oh) + Iz0);
        WAF *= uzc[(int)(midFlameHeight/(C->canopyHeight*C->cellsize) + 0.5)];
    }else{  //flame tip is above canopy top, which is not valid for this function (use unsheltered function)
        WAF = -1.0; //return negative number, meaning invalid
    }

    return WAF;
}

/**
*@brief Computes wind adjustment factor using the integration method for the sheltered case.
*@param inputHeight Height of input windspeed measured from the GROUND.
*@param flameHeight Height of flame measured from the GROUND.
*@return returns the wind adjustment factor, if negative the computation was in error
*/
double canopyFlow::get_windAdjustmentFactorShelteredIntegral(double inputHeight, double flameHeight)
{
    double WAF;
    double inter1 = 0.0;
    if(flameHeight < C->z0g)
    {
        WAF = 0.0;
    }else if(flameHeight <= C->canopyHeight)    //below canopy (sheltered case)
    {
       WAF = log(Iz0) / log((inputHeight - C->canopyHeight)/(C->canopyHeight * z0oh) + Iz0);
       int flameHeightIndex = (int)(flameHeight/(C->canopyHeight*C->cellsize) + 0.5);
       for(int i=0; i<=flameHeightIndex; i++)   //integrate using extended Simpson's rule
       {
           if(i%2 == 0)    //if even numbers
               inter1 += uzc[i];
           else            //if odd numbers
               inter1 += 2.0 * uzc[i];
       }

       inter1 = inter1 - 0.5 * (uzc[0] + uzc[flameHeightIndex]);
       inter1 *= C->cellsize * 2.0/3.0 * C->canopyHeight / flameHeight;

        WAF = WAF * inter1;
    }else{  //midFlameHeight is above canopy top, which is not valid for this function (use unsheltered function)
        WAF = -1.0; //return negative number, meaning invalid
    }

    return WAF;
}

/**
*@brief Computes wind adjustment factor using the integration method for the unsheltered case.
*@param inputHeight Height of input windspeed measured from the GROUND.
*@param flameHeight Height of flame measured from the GROUND.
*@return returns the wind adjustment factor, if negative the computation was in error
*/
double canopyFlow::get_windAdjustmentFactorUnshelteredIntegral(double inputHeight, double flameHeight)
{
    double WAF;
    if(flameHeight < C->z0g)
    {
        WAF = -1.0;  //This is an error if this happens.
    }else if(flameHeight <= C->canopyHeight)    //below canopy (sheltered case)
    {
       WAF = -1.0;  //This is an error, probably should use sheltered case.
    }else{
        double delta = flameHeight - C->canopyHeight;
        double Id = C->canopyHeight * one_doh / delta;
        //WAF = log(delta / (C->canopyHeight * z0oh) + Iz0) - 1.0 + Id * log(1.0/Id + 1.0) / log(inputHeight/(C->canopyHeight * z0oh) + Iz0);
        WAF = (log(delta / (C->canopyHeight * z0oh) + Iz0) - 1.0 + Id * log(1.0/Id + 1.0)) / log((inputHeight - C->canopyHeight)/(C->canopyHeight * z0oh) + Iz0);
    }
    return WAF;
}

double canopyFlow::get_windspeed(double inputSpeed, double inputHeight, double desiredHeight)
{
    //double uCanopyHeight = K * inputSpeed / (usuh * (log((inputHeight/C->canopyHeight - doh) / z0oh) + logRough));
    //double uCanopyHeight = inputSpeed * log(Iz0) / log(inputHeight/(C->canopyHeight * z0oh) + Iz0);
    //printf("uCanopyHeight = %lf\n", uCanopyHeight);
    //double uCanopyHeight = inputSpeed * log((1.0 - doh) / z0oh) / log((inputHeight/C->canopyHeight - doh) / z0oh);
    //double uhuH = log(Iz0) / log(inputHeight/(C->canopyHeight * z0oh) + Iz0);
    //double uhuH = 1.0 / log((inputHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0);





//    double uhuH = 1.0 / log((inputHeight) / (z0oh * C->canopyHeight) + Iz0);
//    if(desiredHeight <= C->z0g)  //if below ground roughness height
//    {
//        return 0.0;
//    }else if(desiredHeight <= C->canopyHeight)  //below canopy
//    {
//        //return uCanopyHeight * uzc[(int)(desiredHeight/(C->canopyHeight*C->cellsize))];
//        return uzc[(int)(desiredHeight/(C->canopyHeight*C->cellsize) + 0.5)] * inputSpeed * uhuH;
//    }else   //above canopy height
//    {
//        //return uCanopyHeight * usuh * log((desiredHeight/C->canopyHeight - doh) / (z0oh + C->z0g/C->canopyHeight) / K;
//        //return inputSpeed * log((desiredHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0) / log((inputHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0);
//        return inputSpeed * log((desiredHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0) / log((inputHeight) / (z0oh * C->canopyHeight) + Iz0);
//    }

    if(!uzc)    //if computeWind hasn't been called yet, call it to set up uzc, etc.
        computeWind();

    double uhuH = get_uhuH(inputSpeed, inputHeight);
    if(desiredHeight <= C->z0g)  //if below ground roughness height
    {
        return 0.0;
    }else if(desiredHeight <= C->canopyHeight)  //below canopy
    {
        return uzc[(int)(desiredHeight/(C->canopyHeight*C->cellsize) + 0.5)] * log(Iz0) * uhuH;
    }else   //above canopy height
    {
        return uhuH * log((desiredHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0);
    }
}

double canopyFlow::get_uhuH(double inputSpeed, double inputHeight)
{
    return inputSpeed / log((inputHeight - C->canopyHeight) / (z0oh * C->canopyHeight) + Iz0);
}
