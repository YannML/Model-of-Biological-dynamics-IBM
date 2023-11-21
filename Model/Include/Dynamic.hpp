#ifndef Dynamic
#define Dynamic

//includes
//#include "File_Management.hpp"
#include "Entities.hpp"
#include "File_Management.hpp"
namespace ENVIRONMENTs
{
    //--- VIRTUAL ENVIRONMENT'S FUNCTIONS ---
            
            //--- Function START_DYNAMICS ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : Start the model and increases sampling.
            void START_DYNAMICS();
            //--- End START_DYNAMICS ---

            //--- Function POPULATE_ENVIRONMENT ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : Randomly create specimens.
            void POPULATE_FRAGMENTS();
            //--- End POPULATE_ENVIRONMENT ---

            //--- Function PASS_TIME ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : Run the model without dispersal, migration and indiscriminate verhulst 
            //for a specified period of time in the ./Configuration/settings.csv file. 
            void CLEANING();
            //--- End PASS_TIME ---

            //--- Function MIGRATE  ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : Transfer individuals to another fragment, based on Verhulst
            void MIGRATE();
            //--- End MIGRATE ---

            //--- Function PASS_TIME ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : Increases time.
            void PASS_TIME();
            //--- End PASS_TIME ---

            //--- Function RESET_DATA ---
            //Last update : 21/10/2021
            //Creat Date : 21/10/2021
            //Description : reset control variables to 0 and or clean vectors.
            void RESET_DATA();
            //--- End RESET_DATA ---

    //--- End VIRTUAL ENVIRONMENT'S FUNCTIONS ---
}

namespace SPECIMENs
{
    //--- SPECIMENS'S FUNCTIONS ---

        //--- Function ACT ---
        //Last update : 30/06/2023
        //Creat Date : 30/06/2023
        //Description : All the specimen behavior
        void ACT(int index_fragment, int index_population, int index_group);
        //--- End ACT ---

    //--- End SPECIMENS'S FUNCTIONS ---
}

#endif