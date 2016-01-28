#include <iostream>
#include <iomanip>
#include "behave.h"
#include <time.h>
#include <cmath>

#ifdef CLI
#include <fstream>
#include <iterator>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
namespace po = boost::program_options;

/**
 * Function used to verify if an option has been set (not defaulted)
 * @param vm variable map
 * @param optn required option
 */
void verify_option_set(const po::variables_map& vm, const char* optn)
{
    if(!vm.count(optn) && !vm[optn].defaulted())
    {
        std::cout<<(std::string("Option '") + optn + "' was not set.\n")<<std::endl;
    }
}

// Additional command line parser which interprets '@something' as a
// option "response_file" with the value "something"
std::pair<std::string, std::string> at_option_parser(std::string const&s)
{
    if ('@' == s[0])
        return std::make_pair(std::string("response_file"), s.substr(1));
    else
        return std::pair<std::string, std::string>();
}
#endif //CLI

#ifdef CLI
int main(int argc, char *argv[])
#else
int main()
#endif
{	
    // Surface Fire Inputs;
    int fuelModelNumber = 0;
    double moisture1h = 0.0;
    double moisture10h = 0.0;
    double moisture100h = 0.0;
    double moistureLiveHerb = 0.0;
    double moistureLiveWood = 0.0;
    double windspeed = 0.0;
    double windDirection = 0;
    double slope = 0.0;
    double slopeAspect = 0.0;
    double directionOfMaxSpread = 0;
    double flameLength = 0; 
    double directionOfInterest = 0;
    double spreadRate = 0;

#ifdef CLI
    // Declare a group of options that will be
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
                    ("help", "produce help message")
                    ("config_file", po::value<std::string>(),
                            "configuration file ('config_file' flag not required)")
                    ("response_file", po::value<std::string>(),
                             "response file (can be specified with '@name', also)")
                            ;
    // Declare a group of options that will be
    // allowed both on command line and in
    // config file
    po::options_description config("Simulation options");
    config.add_options()
            ("fuel_model_number", po::value<int>()->required(), "Fuel model number")
            ("moisture_1hr", po::value<double>()->required(), "1-hr fuel moisture")
            ("moisture_10hr", po::value<double>()->required(), "10-hr fuel moisture")
            ("moisture_100hr", po::value<double>()->required(), "100-hr fuel moisture")
            ("moisture_live_herb", po::value<double>()->required(), "Live herbacious fuel moisture")
            ("moisture_live_wood", po::value<double>()->required(), "Live woody fuel moisture")
            ("wind_speed", po::value<double>()->required(), "Mid-flame wind speed")
            ("wind_direction", po::value<double>()->required(), "Mid-flame wind direction")
            ("slope", po::value<double>()->required(), "slope")
            ("aspect", po::value<double>()->required(), "aspect")
            ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::options_description config_file_options;
    config_file_options.add(config);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("config_file", -1);

    po::variables_map vm;

    po::parsed_options opts_command = po::command_line_parser(argc, argv).
                    options(cmdline_options).extra_parser(at_option_parser).positional(p).run();

    store(opts_command, vm);

    if( argc == 1 )
    {
        std::cout << visible << "\n";
        return -1;
    }

    if (vm.count("config_file")) {
        std::ifstream ifs(vm["config_file"].as<std::string>().c_str());
        if (!ifs)
        {
            std::cout << "cannot open config file: " << vm["config_file"].as<std::string>() << "\n";
            return -1;
        }
        else
        {
            po::parsed_options opts_config = parse_config_file(ifs, config_file_options);

            store(opts_config, vm);
        }
    }

    if (vm.count("help")) {
        std::cout << visible << "\n";
        return 0;
    }

//        if (vm.count("version")) {
//            cout << "Behave version: " << VERSION << "\n";
//            cout << "Git version: " << GIT_VERSION << "\n";
//            cout << "Release date: " << RELEASE_DATE << "\n";
//            return 0;
//        }

    if (vm.count("response_file")) {
        // Load the file and tokenize it
        std::ifstream ifs(vm["response_file"].as<std::string>().c_str());
        if (!ifs) {
            std::cout << "Could not open the response file\n";
            return -1;
        }
        // Read the whole file into a string
        std::stringstream ss;
        ss << ifs.rdbuf();
        // Split the file content
        boost::char_separator<char> sep(" \n\r");
        std::string sstr = ss.str();
        boost::tokenizer<boost::char_separator<char> > tok(sstr, sep);
        std::vector<std::string> args;
        copy(tok.begin(), tok.end(), std::back_inserter(args));

        for(unsigned int i=0; i< args.size(); i++)
            std::cout << args[i] << std::endl;

        po::parsed_options opts_resp = po::command_line_parser(args).options(cmdline_options).run();

        // Parse the file and store the options
        store(opts_resp, vm);
    }

    notify(vm);
     
    fuelModelNumber = vm["fuel_model_number"].as<int>();
    moisture1h = vm["moisture_1hr"].as<double>();
    moisture10h = vm["moisture_10hr"].as<double>();
    moisture100h = vm["moisture_100hr"].as<double>();
    moistureLiveHerb = vm["moisture_live_herb"].as<double>();
    moistureLiveWood = vm["moisture_live_wood"].as<double>();
    windspeed = vm["wind_speed"].as<double>();
    windDirection = vm["wind_direction"].as<double>();
    slope = vm["slope"].as<double>();
    slopeAspect = vm["aspect"].as<double>();

	printf("Spread_rate=%lf", spreadRate);

#endif //CLI        

    Behave behave;

	// Setting the wind and spread angle input mode (default is upslope)
	//behavePlus.setWindAndSpreadAnglesRelativeToUpslope();
	behave.setWindAndSpreadAnglesRelativeToNorth();

	// Checking  wind and spread angle input mode
	// The method of determining angle or slope input mode is admittedly bit clunky
	std::cout << "Wind and spread direction are in degrees clockwise relative to ";
	if (behave.isWindAndSpreadAngleRelativeToUpslope())
	{
		std::cout << "upslope" << std::endl << std::endl;
	}
	if (behave.isWindAndSpreadAngleRelativeToNorth())
	{
		std::cout << "compass north" << std::endl << std::endl;
	}

	// Setting the slope input mode (default is percent)
	//behavePlus.setSlopeInputToPercent();
	behave.setSlopeInputToDegrees();

	// Checking the slope input mode
	// The method of determining slope input mode is admittedly bit clunky
	std::wcout << "Slope input mode is ";
	if (behave.isSlopeInPercent())
	{
		std::cout << "percent" << std::endl << std::endl;
	}
	if (behave.isSlopeInDegrees())
	{
		std::cout << "degrees" << std::endl << std::endl;
	}

	spreadRate = behave.calculateSurfaceFireForwardSpreadRate();

#ifndef CLI
	for (int i = 101; i <= 109; i++)
	{
		fuelModelNumber = i; 
		moisture1h = 0.06;
		moisture10h = 0.07;
		moisture100h = 0.08;
		moistureLiveHerb = 0.60;
		moistureLiveWood = 0.90;
		windspeed = 3;
		windDirection = 90;
		slope = 50;
		slopeAspect = 50;
		behave.updateSurfaceInputs(fuelModelNumber, moisture1h, moisture10h, moisture100h, moistureLiveHerb, moistureLiveWood, windspeed, windDirection, slope, slopeAspect);
		directionOfInterest = 0;

		spreadRate = behave.calculateSurfaceFireForwardSpreadRate();
		//spreadRate = behave.calculateSurfaceFireForwardSpreadRate(directionOfInterest);

		directionOfMaxSpread = behave.getDirectionOfMaxSpread();
        std::cout << "Direction of maximum spread is for fuel model " << i << " is " << std::setprecision(0) << std::fixed << round(directionOfMaxSpread)
			<< " degrees" << std::endl;
		spreadRate = floor(spreadRate * 10 + 0.5) / 10;
		// std::cout << "The direction of interest is " << directionOfInterest << " degrees" << std::endl;
		std::cout << "Spread rate in direction of interest " << directionOfInterest << " degrees for fuel model " << i << " is " << std::setprecision(1) << std::fixed
			<< spreadRate << " ch/hr" << std::endl;
		flameLength = behave.getFlameLength();
		flameLength = floor(flameLength * 10 + 0.5) / 10;
		std::cout << "Flame length for fuel model " << i << " is "<< flameLength << " ft" << std::endl << std::endl;
	}
	// Used for debug
	std::cout << "Press Enter to continue";
	std::cin.get();
#endif // CLI

	return 0;
}
