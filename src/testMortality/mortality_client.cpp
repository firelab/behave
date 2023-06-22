#include "mortality.h"

#include<algorithm>
#include<cmath>
#include<fstream>
#include<iostream>
#include<locale>
#include<sstream>
#include<string>
#include<vector>

class readFile {
public:
    std::vector<string> vHeader;
    std::vector<string> vPlotIds;
    std::vector<string> vFofemProbs;
    std::vector<std::vector<string>> vData;

    explicit readFile (const string& fName);
    int getDataTypeIndex(const string& str);
    int getDataIndex(const string& str);

protected:
    std::fstream file;
    string lineDetails;
    int itemCount = 0;
    string header;
    string headerItem;
    string dataItem;
    string dataItems;

    std::stringstream ss;
    std::vector<string> vDataItems;

    static string cleanString (string text) {
        for( std::string::iterator iter = text.begin() ; iter != text.end() ; )
        {
            if( !std::isprint(*iter) ) iter = text.erase(iter) ;  // erase non-printable characters
            else ++iter ;  // not erased, increment iterator
        }

        // Remove leading and trailing whitespace
        auto it_r = std::find_if(text.rbegin(), text.rend(),
                               [](char c) {
                                   return !std::isspace<char>(c, std::locale::classic());
                               });
        text.erase(it_r.base(), text.end());

        auto it_l = std::find_if(text.begin(), text.end(),
                               [](char c) {
                                   return !std::isspace<char>(c, std::locale::classic());
                               });
        text.erase(text.begin(), it_l);


        return text;
    }
};

int readFile::getDataIndex(const string& pid) {
    auto it = find(vPlotIds.begin(), vPlotIds.end(), pid);

    // If element was found
    if (it != vPlotIds.end())
    {
        // calculating the index of K
        int index = it - vPlotIds.begin();
        return index;
    }
    else {
        // If the element is not
        // present in the vector
        std::cout << "-1" << std::endl;
        return -1;
    }
}

int readFile::getDataTypeIndex(const string& pid) {
    auto it = find(vHeader.begin(), vHeader.end(), pid);

    // If element was found
    if (it != vHeader.end())
    {
        // calculating the index of K
        int index = it - vHeader.begin();
//        std::cout << index << std::endl;
        return index;
    }
    else {
        // If the element is not
        // present in the vector
//        std::cout << "-1" << std::endl;
        return -1;
    }
}


readFile::readFile(const string& fName) { // Constructor with parameters
    file.open(fName, std::ios::in);

    /* Check that the file can be found and is accessible */
    if( !file.is_open()) {

        std::cout << "File "<< fName <<" not found." << std::endl;
        exit(-1);
    }

    // Get the index names from the header row of file
    getline (file, header);
    ss << header;

    while (getline (ss, headerItem, ','))
    {
        vHeader.push_back (cleanString((headerItem)));
    };

    while (getline(file, dataItems))
    {
        // Reset the string stream to allow new data line to be passed to it
        ss.str("");
        ss.clear();

        // Convert the data line to a string stream so that it can be parsed
        ss << cleanString(dataItems);

        while (getline (ss, dataItem, ','))
        {
            vDataItems.push_back (cleanString(dataItem));
        };

        // Only create Vectors of FOFEM Probabilities if the output file is being read
        if (getDataTypeIndex("MortAvg percent") != -1)
        {
            vPlotIds.push_back(vDataItems[getDataTypeIndex("PlotId")]);
            vFofemProbs.push_back(vDataItems[getDataTypeIndex("MortAvg percent")]);
        }

        // Make vDataItems the same size as vHeader
        for (int i = (int) vDataItems.size(); i < vHeader.size(); i++)
        {
            vDataItems.emplace_back((""));
        }

        vData.push_back(vDataItems);
        vDataItems.clear();
    }

    file.close();
}


int main()
{
    constexpr int ok = 1;
    int rc = 0;
    SpeciesMasterTable smt;
    Mortality mortality(smt);
    Mortality mortalityCopy(mortality); // test copy constructor

    EquationType equationType;
    string speciesCode;

    double probalilityOfMortality = 0;
    double p = 0;
    std::fstream outFile;
    int runid = 0;

    int fsx;
    int FlLe_ScHtx;
    int TreeExpansionFactorx;
    int Diameterx;
    int TreeHeightx;
    int CrownRatiox;
    int CrownScorchPx;
    int CKRx;
    int BeetleDamagex;

    vector<SpeciesMasterTableRecord> speciesInRegion;
  
    vector<bool> requiredFieldVector;

    vector<std::string> fieldNameStrings =
    {
        "region",
        "flame_length_or_scorch_height_switch",
        "flame_length_or_scorch_height_value",
        "equation_type",
        "dbh",
        "tree_height",
        "crown_ratio",
        "crown_damage",
        "cambium_kill_rating",
        "beetle_damage",
        "bole_char_height",
        "fire_severity"
    };

    readFile myFileInput("/home/tdog/workspace/projects/git_projects/behave/src/testMortality/FOFEM Mortality Test.tre");
    readFile myFileOutput("/home/tdog/workspace/projects/git_projects/behave/src/testMortality/FOFEM Mortality Test-Out.csv");

    fsx = myFileInput.getDataTypeIndex("FS");
    FlLe_ScHtx = myFileInput.getDataTypeIndex("FlLe/ScHt");
    TreeExpansionFactorx = myFileInput.getDataTypeIndex("TreeExpansionFactor");
    Diameterx = myFileInput.getDataTypeIndex("Diameter");
    TreeHeightx = myFileInput.getDataTypeIndex("TreeHeight");
    CrownRatiox = myFileInput.getDataTypeIndex("CrownRatio");
    CrownScorchPx = myFileInput.getDataTypeIndex("CrownScorch%");
    CKRx = myFileInput.getDataTypeIndex("CKR");
    BeetleDamagex = myFileInput.getDataTypeIndex("BeetleDamage");

    RegionCode region = RegionCode::south_east;
    mortality.setRegion(region);

    /* Not Sure if this block is needed */
//    speciesInRegion = mortality.getSpeciesRecordVectorForRegion(region);
//    speciesInRegion = mortality.getSpeciesRecordVectorForRegionAndEquationType(region, equationType);
//    int numSpeciesRecords = mortality.getNumberOfRecordsInSpeciesTable();
//    int speciesIndex = mortality.getSpeciesTableIndexFromSpeciesCode(speciesCode);
//    bool isInRegion = mortality.checkIsInRegionAtSpeciesTableIndex(speciesIndex, region);

    outFile.open("/home/tdog/workspace/projects/git_projects/behave/src/testMortality/resultsProbMort.csv", std::ios::out);
    outFile << "RunId,";
    for (const auto &e : myFileInput.vHeader) outFile << e << ",";
    outFile << "BehaveProbability,"  << "FOFEMProbability," << "AbsoluteDifference" << std::endl;

    // Iterate through vector to calculate probabilities of mortality
    for (auto & element : myFileInput.vData) {
        int idx = myFileInput.getDataTypeIndex("EquationType");
        string plotID = element[myFileInput.getDataTypeIndex("PlotId")];
        
        string fs = element[fsx];
        string FlLe_ScHt = element[FlLe_ScHtx];
        string TreeExpansionFactor = element[TreeExpansionFactorx];
        string Diameter = element[Diameterx];
        string TreeHeight = element[TreeHeightx];
        string CrownRatio = element[CrownRatiox];
        string CrownScorchP = element[CrownScorchPx];
        string CKR = element[CKRx];
        string BeetleDamage = element[BeetleDamagex];

        if (element[idx] == "CRNSCH")
        {
            mortality.setEquationType(EquationType::crown_scorch);
            equationType = EquationType::crown_scorch;
        }
        else if (element[idx] == "CRCABE")
        {
            mortality.setEquationType(EquationType::crown_damage);
            equationType = EquationType::crown_damage;
        }
        else if (element[idx] == "BOLCHR")
        {
            mortality.setEquationType(EquationType::bole_char);
            equationType = EquationType::bole_char;
        }
        else
        {
            mortality.setEquationType(EquationType::not_set);
            equationType = EquationType::not_set;
        }

        speciesCode = element[myFileInput.getDataTypeIndex("TreeSpecies")];
        mortality.setSpeciesCode(speciesCode);
        rc = mortality.updateInputsForSpeciesCodeAndEquationType(speciesCode, equationType);

        if(rc == ok)
        {
            if (!fs.empty() ){
                if (fs == "S")
                    mortality.setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch::scorch_height);
                else if (fs == "F")
                    mortality.setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch::flame_length);
                else {
                    mortality.setFlameLengthOrScorchHeightSwitch(FlameLengthOrScorchHeightSwitch::flame_length);
                    mortality.setFlameLengthOrScorchHeightValue(4, LengthUnits::Feet);
                }
            }

            if (!FlLe_ScHt.empty() )
                mortality.setFlameLengthOrScorchHeightValue(stod(FlLe_ScHt), LengthUnits::Feet);

            if (!TreeExpansionFactor.empty() )
             mortality.setTreeDensityPerUnitArea(stod(TreeExpansionFactor), AreaUnits::Acres);

            if (!Diameter.empty() )
                mortality.setDBH(stod(Diameter), LengthUnits::Inches);

            if (!TreeHeight.empty() )
                mortality.setTreeHeight(stod(TreeHeight), LengthUnits::Feet);

            if (!CrownRatio.empty() )
                mortality.setCrownRatio(stod(CrownRatio) / 100); // input as a fraction from 0.0 to 1.0

            if (!CrownScorchP.empty() )
                mortality.setCrownDamage(stod(CrownScorchP));

            if (!CKR.empty() )
                mortality.setCambiumKillRating(stod(CKR));
            
            if(!BeetleDamage.empty())
            {
                std::transform(BeetleDamage.begin(), BeetleDamage.end(), BeetleDamage.begin(), ::toupper);
                if ( BeetleDamage == "YES")
                    mortality.setBeetleDamage(BeetleDamage::yes);
                else if ( BeetleDamage == "NO")
                    mortality.setBeetleDamage(BeetleDamage::no);
                else
                    mortality.setBeetleDamage(BeetleDamage::not_set);
            }

            if (!element[myFileInput.getDataTypeIndex("BoleCharHeight")].empty())
                mortality.setBoleCharHeight(stod(element[myFileInput.getDataTypeIndex("BoleCharHeight")]), LengthUnits::Feet);


            requiredFieldVector = mortality.getRequiredFieldVector();

            for(int i = (int)RequiredFieldNames::dbh; i < requiredFieldVector.size(); i++)
            {
                if(requiredFieldVector[i] == true)
                {
                    RequiredFieldNames currentRequiredFieldName = static_cast<RequiredFieldNames>(i);
                    if((currentRequiredFieldName == RequiredFieldNames::dbh) && mortality.getDBH(LengthUnits::Inches) < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::tree_height) && mortality.getTreeHeight(LengthUnits::Feet) < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::crown_ratio) && mortality.getCrownRatio() < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::crown_damage) && mortality.getCrownDamage() < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::cambium_kill_rating) && mortality.getCambiumKillRating() < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << " - " << plotID <<  "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::beetle_damage) && mortality.getBeetleDamage() == BeetleDamage::not_set)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << " - " << plotID <<  "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::bole_char_height) && mortality.getBoleCharHeight(LengthUnits::Feet) < 0)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                    if((currentRequiredFieldName == RequiredFieldNames::fire_severity) && mortality.getFireSeverity() == FireSeverity::not_set)
                    {
                        std::cout << "Error, missing required input " << fieldNameStrings[i] << "\n";
                    }
                }
            }

            probalilityOfMortality = mortality.calculateMortality(ProbabilityUnits::Percent);

            // Write out input data for this current plotid to results file
            outFile << ++runid << ",";
            for (const auto &e : element) outFile << e << ",";
            outFile << (int) probalilityOfMortality << ",";

            // Find the FOFEM Probability for the current plot ID and include in results output file
            double fofemProb = stod(myFileOutput.vFofemProbs[myFileOutput.getDataIndex(plotID)]);

            outFile << myFileOutput.vFofemProbs[myFileOutput.getDataIndex(plotID)] << ",";
            outFile << fabs((fofemProb - probalilityOfMortality)) << ",";
            outFile << std::endl;

//            std::cout << "Probability of mortality: " << probalilityOfMortality <<"%\n";
        }
        else
        {
            std::cout << "Error, species not found\n";
        }
    }

    outFile.close();


    return 0;
}

/* TODO: ERASE Before commiting to repo */
/* Code Remnants for future reference - Thomas DeVera */
//        for (items i;
//            (file >> i.plotid) //.ignore(std::numeric_limits<std::streamsize>::max(), ',')
//            && getline(file, i.plotid, ',')
//            && getline(file, i.monstatus, ',');)
//            {
//                for( std::string::iterator iter = i.plotid.begin() ; iter != i.plotid.end() ; )
//                {
//                    if( !std::isprint(*iter) ) iter = i.plotid.erase(iter) ;  // erase non-printable characters
//                    else ++iter ;  // not erased, increment iterator
//                }
//                is.push_back(i);
//            }

//    string pid = "CS-ABBA-5-25";
//    auto result = std::find_if(
//            is.begin(),
//            is.end(),
//            [&pid](items const& c) {
//                return c.plotid == pid;
//            }
//    );

//    if (result != is.end()) {
//        std::cout<< "Plot ID found at index: " << result - is.begin() <<std::endl;
//    }