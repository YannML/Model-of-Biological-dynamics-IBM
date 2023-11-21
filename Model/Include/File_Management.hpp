#ifndef File_Management
#define File_Management

//includes
//#include "Entities.hpp"
#include "Entities.hpp"
namespace INPUT_DATA
{
    //--- Function Get_Input_Data---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : read the .csv file containing the settings metadata.
    void Get_Input_Data();

    //--- End Get_Input_Data ---
}

namespace OUTPUT_DATA
{
    //--- Function OUTPUT ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Output the tables of each fragment's populations.
    void OUTPUT();

    //--- End OUTPUT ---

    //--- Function CREATE_OUTPUT_DIRECTORY ---
    //Last update : 17/10/2022
    //Creat Date : 17/10/2022
    //Description : Creates the output directory.
    void CREATE_OUTPUT_DIRECTORY();

    //--- End CREATE_OUTPUT_DIRECTORY ---
}

#endif