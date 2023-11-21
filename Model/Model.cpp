//includes
#include "Include/Model.hpp"

//INITIALIZE
void INITIALIZE()
{
    //Clock
    now = time(NULL);
    nowLocal = *localtime(&now);
    date_and_time = std::to_string(nowLocal.tm_mday) + "_" + std::to_string(nowLocal.tm_mon + 1) + "_" + std::to_string(nowLocal.tm_year + 1900) + " " + std::to_string(nowLocal.tm_hour) + "-" + std::to_string(nowLocal.tm_min);
    model_start_time = std::chrono::high_resolution_clock::now();

    //Get input
    try
    {
        INPUT_DATA::Get_Input_Data();
    }
    catch (const std::exception &e)
    {
        std::cout << " Error!\n";
        std::cout << "Error!: getting input data -- Get_Input_Data () | Model.cpp " << '\n';
        std::cout << "Excepetion: " << e.what() << '\n';
    }
    
    //Initial settings
    {
        //settings data
        try
        {
            std::vector<std::string> settings_data_separated_values;
            std::string settings_values = Settings.settings_data[0];
            size_t delimiter_position = 0;
            std::string value;
            while ((delimiter_position = settings_values.find(';')) != std::string::npos)
            {
                value = settings_values.substr(0, delimiter_position);
                settings_data_separated_values.reserve(sizeof(value));
                settings_data_separated_values.emplace_back(value);
                settings_values.erase(0, delimiter_position + 1);
            }

            Settings.number_of_time_steps = std::stoi(settings_data_separated_values[0]);
            Settings.sample_quantity = std::stoi(settings_data_separated_values[1]);
            Settings.datasets = std::stoi(settings_data_separated_values[2]);
            settings_data_separated_values[3] == "true" ? Settings.INDISCRIMINATE_VERHULST = true : Settings.INDISCRIMINATE_VERHULST = false;
            Settings.ORIGINAL_VERHULST_BIAS = std::stod(settings_data_separated_values[4]);
            settings_data_separated_values[5] == "true" ? Settings.MIGRATE = true : Settings.MIGRATE = false;
            settings_data_separated_values[6] == "true" ? Settings.CLEANING = true : Settings.CLEANING = false;
            Settings.CLEANING_DURATION = std::stoi(settings_data_separated_values[7]);
            Settings.OUTPUT_DIRECTORY = settings_data_separated_values[8]+"_"+ date_and_time;
            
            settings_data_separated_values.clear();
            Settings.settings_data.clear();
        }
        catch (const std::exception &e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: Setting initial settings -- settings data | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }

        //species data
        try
        {
            ENTITY::Specie sp;
            std::vector<std::string> specie_separate_values;

            for (int index = 0; index < Settings.species_data.size(); index++)
            {
                std::string specie_values = Settings.species_data[index];
                size_t delimiter_position = 0;
                std::string value;

                while ((delimiter_position = specie_values.find(';')) != std::string::npos)
                {
                    value = specie_values.substr(0, delimiter_position);
                    specie_separate_values.reserve(sizeof(value));
                    specie_separate_values.emplace_back(value);
                    specie_values.erase(0, delimiter_position + 1);
                }
                
                sp.name = specie_separate_values[0];
                sp.identification = std::stoi(specie_separate_values[1]);
                sp.maximum_age = std::stoi(specie_separate_values[2]);
                sp.maturity_age = std::stoi(specie_separate_values[3]);
                sp.disperse_age = std::stoi(specie_separate_values[4]);
                sp.mutation_threshold = std::stoi(specie_separate_values[5]);
                sp.bit_strings_size = std::stoi(specie_separate_values[6]);
                sp.number_of_dominant_positions = std::stoi(specie_separate_values[7]);
                sp.alphas_reign_time = std::stoi(specie_separate_values[8]);
                sp.number_of_offsprings = std::stoi(specie_separate_values[9]);
                sp.duration_of_parental_care = std::stoi(specie_separate_values[10]);
                sp.maximum_number_of_individuals_per_group = std::stoi(specie_separate_values[11]);
                sp.Penalty_for_one_equal_kinship_label = std::stoi(specie_separate_values[12]);
                sp.Penalty_for_two_identical_kinship_label = std::stoi(specie_separate_values[13]);
                sp.Penalty_Penna_Model = std::stoi(specie_separate_values[14]);
                sp.individuals_per_square_kilometers = std::stod(specie_separate_values[15]);
                sp.female_reproductive_failure = std::stod(specie_separate_values[16]);
                sp.alpha_reproductive_success = std::stod(specie_separate_values[17]);
                sp.disperse_rate = std::stod(specie_separate_values[18]);
                specie_separate_values[19] == "true" ? sp.has_asexual_reproduction = true : sp.has_asexual_reproduction = false;
                specie_separate_values[20] == "true" ? sp.has_sexual_reprodution = true : sp.has_sexual_reprodution = false;
                specie_separate_values[21] == "true" ? sp.male_disperses = true : sp.male_disperses = false;
                specie_separate_values[22] == "true" ? sp.female_disperses = true : sp.female_disperses = false;
                specie_separate_values[23] == "true" ? sp.male_migrates = true : sp.male_migrates = false;
                specie_separate_values[24] == "true" ? sp.female_migrates = true : sp.female_migrates = false;
                specie_separate_values[25] == "true" ? sp.affected_by_carrying_capacity_variation = true : sp.affected_by_carrying_capacity_variation = false;
                specie_separate_values[26] == "true" ? sp.affected_by_Verhulst_variation = true : sp.affected_by_Verhulst_variation = false;

                //Disperse activation
                sp.disperse_rate <= 0.0 ? Settings.DISPERSE = false : Settings.DISPERSE = true;
                sp.alpha_reproductive_success <= 0.0 ? sp.has_alpha = false : sp.has_alpha = true;
                sp.duration_of_parental_care < 1 ? sp.has_parental_care = false : sp.has_parental_care = true;
                sp.maximum_number_of_individuals_per_group <= 1 ? sp.organize_in_groups = false : sp.organize_in_groups = true;

                Settings.SPECIES_LIST.reserve(sizeof(sp));
                Settings.SPECIES_LIST.emplace_back(sp);
                
                specie_separate_values.clear();
            }
            
            Settings.species_data.clear();
        }
        catch (const std::exception &e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: Setting initial settings -- species data | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }
        
        //fragments data
        try
        {
            ENTITY::Fragment fragment;
            std::vector<std::string> fragment_separate_values;

            for (int index = 0; index < Settings.fragments_data.size(); index++)
            {
                std::string fragment_values = Settings.fragments_data[index];
                
                size_t delimiter_position = 0;
                std::string value;

                while ((delimiter_position = fragment_values.find(';')) != std::string::npos)
                {
                    value = fragment_values.substr(0, delimiter_position);
                    fragment_separate_values.reserve(sizeof(value));
                    fragment_separate_values.emplace_back(value);
                    fragment_values.erase(0, delimiter_position + 1);
                }

                fragment.identification = std::stoi(fragment_separate_values[0]);
                fragment.original_area = std::stod(fragment_separate_values[1]);
                fragment.original_perimeter = std::stod(fragment_separate_values[2]);
                fragment.current_area = std::stod(fragment_separate_values[1]);
                fragment.current_perimeter = std::stod(fragment_separate_values[2]);
                fragment.initial_density = std::stod(fragment_separate_values[3]);
                fragment_separate_values[4] == "true" ? fragment.Fragmentation_variables.fragment_environment = true : fragment.Fragmentation_variables.fragment_environment = false;
                fragment.Fragmentation_variables.number_of_fragmented_areas = std::stoi(fragment_separate_values[5]);
                fragment.Fragmentation_variables.fragmentation_time_step = std::stoi(fragment_separate_values[6]);
                //Area variation data
                fragment.Area_variation.type = fragment_separate_values[7];
                fragment.Area_variation.variance = std::stod(fragment_separate_values[8]);
                fragment.Area_variation.growing.start_time_step = std::stoi(fragment_separate_values[9]);
                fragment.Area_variation.growing.duration = std::stoi(fragment_separate_values[10]);
                fragment.Area_variation.descending.start_time_step = std::stoi(fragment_separate_values[11]);
                fragment.Area_variation.descending.duration = std::stoi(fragment_separate_values[12]);
                    //Calculate end time step
                    fragment.Area_variation.growing.end_time_step = fragment.Area_variation.growing.start_time_step + fragment.Area_variation.growing.duration;
                    fragment.Area_variation.descending.end_time_step = fragment.Area_variation.descending.start_time_step + fragment.Area_variation.descending.duration;
                //Perimeter variation data
                fragment.Perimeter_variation.type = fragment_separate_values[13];
                fragment.Perimeter_variation.variance = std::stod(fragment_separate_values[14]);
                fragment.Perimeter_variation.growing.start_time_step = std::stoi(fragment_separate_values[15]);
                fragment.Perimeter_variation.growing.duration = std::stoi(fragment_separate_values[16]);
                fragment.Perimeter_variation.descending.start_time_step = std::stoi(fragment_separate_values[17]);
                fragment.Perimeter_variation.descending.duration = std::stoi(fragment_separate_values[18]);
                    //Calculate end time step
                    fragment.Perimeter_variation.growing.end_time_step = fragment.Perimeter_variation.growing.start_time_step + fragment.Perimeter_variation.growing.duration;
                    fragment.Perimeter_variation.descending.end_time_step = fragment.Perimeter_variation.descending.start_time_step + fragment.Perimeter_variation.descending.duration;
                //Carrying capacity variation data
                fragment.Carrying_capacity_variation.type = fragment_separate_values[19];
                fragment.Carrying_capacity_variation.variance = std::stod(fragment_separate_values[20]);
                fragment.Carrying_capacity_variation.growing.start_time_step = std::stoi(fragment_separate_values[21]);
                fragment.Carrying_capacity_variation.growing.duration = std::stoi(fragment_separate_values[22]);
                fragment.Carrying_capacity_variation.descending.start_time_step = std::stoi(fragment_separate_values[23]);
                fragment.Carrying_capacity_variation.descending.duration = std::stoi(fragment_separate_values[24]);
                    //Calculate end time step
                    fragment.Carrying_capacity_variation.growing.end_time_step = fragment.Carrying_capacity_variation.growing.start_time_step + fragment.Carrying_capacity_variation.growing.duration;
                    fragment.Carrying_capacity_variation.descending.end_time_step = fragment.Carrying_capacity_variation.descending.start_time_step + fragment.Carrying_capacity_variation.descending.duration;
                //Verhulst variation data
                fragment.Verhulst_variation.type = fragment_separate_values[25];
                fragment.Verhulst_variation.variance = std::stod(fragment_separate_values[26]);
                fragment.Verhulst_variation.growing.start_time_step = std::stoi(fragment_separate_values[27]);
                fragment.Verhulst_variation.growing.duration = std::stoi(fragment_separate_values[28]);
                fragment.Verhulst_variation.descending.start_time_step = std::stoi(fragment_separate_values[29]);
                fragment.Verhulst_variation.descending.duration = std::stoi(fragment_separate_values[30]);
                    //Calculate end time step
                    fragment.Verhulst_variation.growing.end_time_step = fragment.Verhulst_variation.growing.start_time_step + fragment.Verhulst_variation.growing.duration;
                    fragment.Verhulst_variation.descending.end_time_step = fragment.Verhulst_variation.descending.start_time_step + fragment.Verhulst_variation.descending.duration;
                fragment_separate_values[31] == "true" ? fragment.reset_values_after_variation = true : fragment.reset_values_after_variation = false;
                
                //reserve memory for the table
                fragment.fragments_tables.reserve_memory(Settings.number_of_time_steps,Settings.CLEANING_DURATION);

                Settings.fragments.reserve(sizeof(fragment));
                Settings.fragments.emplace_back(fragment);

                fragment_separate_values.clear();
            }

            Settings.fragments_data.clear();
        }
        catch (const std::exception &e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: Setting initial settings -- fragment data | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }

        //crossing cost matrix data
        try
        {
            std::vector<std::string> settings_data_separated_values;
            std::string settings_values = Settings.crossing_cost_matrix_data[0];
            size_t delimiter_position = 0;
            std::string value;
            while ((delimiter_position = settings_values.find(';')) != std::string::npos)
            {
                value = settings_values.substr(0, delimiter_position);
                settings_data_separated_values.reserve(sizeof(value));
                settings_data_separated_values.emplace_back(value);
                settings_values.erase(0, delimiter_position + 1);
            }

            int number_of_lines = Settings.fragments.size();
            int line = 0;
            while (line < number_of_lines)
            {
                int cost_value = 0;
                int number_of_columns = 1;
                while(cost_value < settings_data_separated_values.size())
                {
                    Settings.crossing_cost_column.reserve(sizeof(settings_data_separated_values[cost_value]));
                    Settings.crossing_cost_column.emplace_back(settings_data_separated_values[cost_value]);

                    if(number_of_columns == Settings.fragments.size())
                    {
                        Settings.crossing_cost_matrix.reserve(sizeof(Settings.crossing_cost_column));
                        Settings.crossing_cost_matrix.emplace_back(Settings.crossing_cost_column);
                        Settings.crossing_cost_column.clear();
                        number_of_columns = 0;
                        line++;    
                    }
                    number_of_columns++;
                    cost_value++;
                }
            }
      
            settings_data_separated_values.clear();
            Settings.crossing_cost_matrix_data.clear();

        }
        catch (const std::exception &e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: Setting initial settings -- crossing cost matrix | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }

        OUTPUT_DATA::CREATE_OUTPUT_DIRECTORY();

    }
    
    //Create Empty Populations
    for(int index_fragment = 0; index_fragment < Settings.fragments.size(); ++index_fragment)
    {
        Settings.fragments[index_fragment].populations.reserve(sizeof(Settings.SPECIES_LIST));
        //Species amount
        for(int specie = 0; specie < Settings.SPECIES_LIST.size(); ++specie)
        {
            ENTITY::Population population(Settings.fragments[index_fragment].initial_density, specie, Settings.fragments[index_fragment].current_area, false);
            Settings.fragments[index_fragment].populations.emplace_back(population);
        }
    } 

    //Start progress bar
    std::cout << "\nModel progress:\n";
    Settings.Progress_bar.Update(0,0,Settings.sample_quantity,Settings.datasets);
    //End of initial settings
}
//End INITIALIZE

int main()
{
    //INITIALIZE
    { 
        try
        {
            INITIALIZE();
        }
        catch(const std::exception& e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: INITIALIZE() | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }
    }
    //End INITIALIZE
    //Start dynamics
    {
        try
        {
           ENVIRONMENTs::START_DYNAMICS();
        }
        catch (const std::exception &e)
        {
            std::cout << " Error!\n";
            std::cout << "Error!: START_DYNAMICS() | Model.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }
    }
    //End of dynamics
    
    //Calculates model processing time
    model_end_time = std::chrono::high_resolution_clock::now();
    model_duration_seconds = model_end_time - model_start_time;
    std::cout<<"\n";
    std::cout << "Model processing time: " << model_duration_seconds.count() << " seconds.\n\n";
}