//includes
#include "Include/File_Management.hpp"

//--- Get_Input_Data ---
void INPUT_DATA::Get_Input_Data()
{
    try
    {
        std::ifstream inFile;
        inFile.open("./Configuration/config.csv");

        std::string line, word, value;
        std::istringstream iss;
        int data_category = 0;

        while (!inFile.eof()) {
            // read line by line from the file
            getline(inFile, line);
            if (inFile.good())
            {
                bool save_char_in_data_category = false;
                bool save_actual_char = false;
                bool open_single_quotes = false;
                
                // read word by word from the line
                iss.clear(); // clear out state
                iss.str(line);
                while (iss.good()) {
                    iss >> word;
                    
                    if(word == "SETTINGS")
                    {
                        data_category = 0;
                    }
                    else if(word == "LANDSCAPE")
                    {
                        data_category = 1;
                    }
                    else if(word == "SPECIES")
                    {
                        data_category = 2;
                    }
                    else if(word == "CROSSING_COST_MATRIX")
                    {
                        data_category = 3;
                    }
                    else if(word == "*")
                    {
                        switch (data_category)
                        {
                            case 0://SETTINGS DATA
                            {
                                Settings.settings_data.reserve(sizeof(value));
                                Settings.settings_data.emplace_back(value);
                                break;
                            }
                            case 1://LANDSCAPE DATA
                            {
                                Settings.fragments_data.reserve(sizeof(value));
                                Settings.fragments_data.emplace_back(value);
                                break;
                            }
                            case 2://ESPECIE DATA
                            {
                                Settings.species_data.reserve(sizeof(value));
                                Settings.species_data.emplace_back(value);
                                break;
                            }
                            case 3://CROSSING COST MATRIX
                            {
                                Settings.crossing_cost_matrix_data.reserve(sizeof(value));
                                Settings.crossing_cost_matrix_data.emplace_back(value);
                                break;
                            }
                        }//End switch data_category
                    }
                    else
                    {
                        //Does Nothing
                    }

                    // read char by char from the word
                    for (char character : word)
                    {
                        //find delimiters    
                        switch (character)
                        {
                            case '\'':
                                {
                                    
                                    if(open_single_quotes == false){
                                        open_single_quotes = true;
                                        save_actual_char = false;
                                    }
                                    else
                                    {
                                        open_single_quotes = false;
                                        value += ";";
                                    }

                                    save_actual_char = false;
                                    break;
                                }
                            case '*':
                                {
                                    value = "";
                                    break;   
                                }
                            default :
                                {
                                    save_actual_char = true;
                                    break;
                                }
                        }
                        //End find delimiters

                        //Save character in value
                        if(save_actual_char == true)
                        {
                            if(open_single_quotes == true)
                            {
                                value += character;
                            }
                        }//End Save char in string
                    }//char by char
                }//Word by word	
            }//Line by line
        }//File read

        //Close file
        inFile.close();
    }
    catch(const std::exception& e)
    {
        std::cout<<"Error!: Get_Input_Data() | File_Management.cpp"<<'\n';
        std::cout<<"Excepetion: "<< e.what() <<'\n';
    }
}
//--- End Get_Input_Data ---

//--- OUTPUT ---
void OUTPUT_DATA::OUTPUT()
{
    //Create the folder
    std::string PATH = Settings.OUTPUT_DIRECTORY + "/Dataset_"+std::to_string(Settings.current_dataset);
    mkdir(PATH.c_str(),0777);
    
    std::string file_name = " ";
    std::vector<std::string> output;
    int index = 0;
    std::string data = " ";
    
    int index_fragment = 0;
    while (index_fragment < Settings.fragments.size())
    {
        std::string fragment_identification = std::to_string(Settings.fragments[index_fragment].identification);
        
        //Fragment Data
        {
            //Header
            data = "Time Step,Area,Perimeter";
            output.reserve(sizeof(data));
            output.emplace_back(data);
            index = 0;
            while (index < Settings.fragments[index_fragment].fragments_tables.Area.size())
            {
                data = std::to_string(index)
                +","+std::to_string(Settings.fragments[index_fragment].fragments_tables.Area[index])
                +","+std::to_string(Settings.fragments[index_fragment].fragments_tables.Perimeter[index]);
                output.reserve(sizeof(data));
                output.emplace_back(data);
                index++;
            }
            file_name = PATH+"/_F"+fragment_identification+".csv";
            std::ofstream output_file(file_name);
            std::ostream_iterator<std::string> output_iterator(output_file, "\n");
            copy(output.begin(), output.end(), output_iterator);
            output_file.close();
            output.clear();
        }

        int index_population = 0;
        while (index_population < Settings.fragments[index_fragment].populations.size())
        {
            int specie_identification = Settings.fragments[index_fragment].populations[index_population].Specie_identification;
            std::string Specie_Identification = std::to_string(specie_identification);
            file_name = " ";
            output.clear();
            index = 0;
            data = " ";
        
            //Populations Data Per Year
            {
                //Header
                data = "Time_steps,Carrying_capacity,Females,Males,Threshold,Groups,kinship_labels,Senescence_Death,Verhulst_Death,Birth,Reproductives_Chances,Alpha_Reign,Disperse,Migration_entries,Migration_exits,Verhulst,V_Bias";
                
                std::string header ="Counts_at_Age";
                int index_header = 0;
                while(index_header <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                {
                    data += "," + header + std::to_string(index_header); 
                    index_header++;
                }
                header ="Death_at_Age";
                index_header = 0;
                while(index_header <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                {
                    data += "," + header + std::to_string(index_header); 
                    index_header++;
                }
                header ="Threshold_at_Age";
                index_header = 0;
                while(index_header <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                {
                    data += "," + header + std::to_string(index_header); 
                    index_header++;
                }
                output.reserve(sizeof(data));
                output.emplace_back(data);
                
                index = 0;
                int sample_duration = Settings.number_of_time_steps + Settings.CLEANING_DURATION;
                while (index < sample_duration)
                {
                    data = std::to_string(index)+","
                    +std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Carrying_capacity[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Females[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Males[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Threshold[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Groups[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.kinship_labels[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Senescence_Death[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Verhulst_Death[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Birth[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Reproductives_Chances[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Alpha_Reign[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Disperse[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Migration_entries[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Migration_exits[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Verhulst[index])
                    +","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.V_Bias[index])
                    ;
                    
                    int at_age = 0;
                    while(at_age <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                    {
                        data+=","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Ages_By_Ages[index][at_age]);
                        at_age++;
                    }
                    at_age = 0;
                    while(at_age <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                    {
                        data+=","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Death_By_Age[index][at_age]);
                        at_age++;
                    }
                    at_age = 0;
                    while(at_age <= Settings.SPECIES_LIST[specie_identification].maximum_age)
                    {
                        data+=","+std::to_string(Settings.fragments[index_fragment].populations[index_population].data_table.Threshold_by_Ages[index][at_age]);
                        at_age++;
                    }
                    output.reserve(sizeof(data));
                    output.emplace_back(data);
                    index++;
                }
                file_name = PATH+"/_F"+fragment_identification+"_S"+Specie_Identification+"_Pop.csv";
                std::ofstream output_file(file_name);
                std::ostream_iterator<std::string> output_iterator(output_file, "\n");
                copy(output.begin(), output.end(), output_iterator);
                output_file.close();
                output.clear();
            }

            index_population++;
        }//End tables loop
        index_fragment++;
    }//End fragment loop
}
//--- End OUTPUT ---

void OUTPUT_DATA::CREATE_OUTPUT_DIRECTORY()
{
    //_mkdir(Settings.OUTPUT_DIRECTORY.c_str()); //Windows mkdir
    mkdir(Settings.OUTPUT_DIRECTORY.c_str(),0777);//The permissions 0777 stands for read, write and execute
    Settings.OUTPUT_DIRECTORY += "/Datasets";
    mkdir(Settings.OUTPUT_DIRECTORY.c_str(),0777);
    //Copy the config file to the output directory
    std::filesystem::copy("./Configuration/config.csv",Settings.OUTPUT_DIRECTORY.c_str());
}