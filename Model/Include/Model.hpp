#ifndef Model
#define Model

//includes
//#include "Dynamic.hpp"
#include "Entities.hpp"
#include "File_Management.hpp"
#include "Dynamic.hpp"
//Variables
 time_t now;
 struct tm nowLocal;
 std::string date_and_time;
 std::chrono::time_point<std::chrono::high_resolution_clock> model_start_time, model_end_time;
 std::chrono::duration<float> model_duration_seconds;

//--- Function INITIALIZE ---
//Last update : 21/10/2021
//Creat Date : 21/10/2021
//Description : Create the system variables according to the settings 
//in the ./App/Configuration/settings.cvs file. And prepares the fragments 
//for the dynamics. 
void INITIALIZE();

//--- End Function INITIALIZE ---  

#endif