#ifndef Entities
#define Entities

//includes
#include <vector>
#include <array>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <ctime>
#include <chrono>
#include <climits>
#include <cmath>
#include <random>
//#include "direct.h" //Windows mkdir function
#include <sys/stat.h> //Linux mkdir function
#include <sys/types.h> //Linux mkdir errors and function
#include <sstream>
#include <fstream>
#include <algorithm> //for sort function
#include <filesystem>
#include <thread>

namespace ENTITY
{
    //--- Motion_path ---
    //Last update : 04/06/2023
    //Creat Date : 04/06/2023
    //Description : Stores the path of specimens destiny.
    struct Motion_path
    {
        unsigned int fragment_destiny = 0;
        unsigned int population_destiny = 0;
        unsigned int group_destiny = 0;
        unsigned int source_fragment = 0;
        unsigned int source_population = 0;
        unsigned int source_group = 0;
        unsigned int source_index = 0;
        bool emigrant = false;
        Motion_path(int fragment_destiny, int population_destiny, int group_destiny, int source_fragment, int source_population, int source_group, int source_index, bool emigrant);
    };
    //--- End Motion_path ---

    //--- Class Especime ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Stores all the necessary processes and variables of the individuals.
    class Specimen
    {
    public:
        //Variables
        unsigned long long int identification = 0;
        unsigned long long int last_group_identification = 0;
        unsigned long long int specie_identification = 0;
        unsigned long long int maternal_identification = 0;
        unsigned long long int kinship_labels[2];
        unsigned int current_age = 0;
        unsigned int life_expectancy = 0;
        unsigned int parental_care_duration = 0;
        unsigned char gender = ' ';
        bool is_alpha = false;
        bool was_alpha = false;
        bool taking_care = false;
        bool erase = false;
        bool reproduce = false;
        bool able_to_mate = false;
        std::vector<std::string> bitstrings;
        std::vector<ENTITY::Specimen> offsprings;
        //Methods
        Specimen(int Specie_identification); //Random cronstructor
        Specimen(int index_fragment, int index_population, int index_group, int female_index, int male_index); //Not random cronstructor
        Specimen(const ENTITY::Specimen& original);//Copy constructer
        Specimen(ENTITY::Specimen *original);//Copy constructer
        void Calculate_life_expectancy();
        void Crossing_over(std::vector<std::string> maternal_bitstrings, std::vector<std::string> paternal_bitstrings);
        void Apply_mutations(unsigned long long int *maternal_kinship_labels, unsigned long long int *paternal_kinship_labels);
        void Relocate_specimens(ENTITY::Motion_path destination);
        void Age(int index_fragment, int index_population, int index_group);
        void Update_parental_care(int index_fragment, int index_population, int index_group);      
        void Disperse(int index_fragment, int index_population, int index_group, int index_specimen);
        void Reproduce(int index_fragment, int index_population, int index_group, int index_specimen);
        void Indiscriminate_verhulst(int index_fragment, int index_population, int index_group, int index_specimen);
        void Able_to_mate();
        void Act(int index_fragment, int index_population, int index_group, int index_specimen);//All the behavior routine
        void Store_data(int index_fragment, int index_population);
        bool Migration_verification(int Verhulst);
        bool Disperse_verification();
        int  Find_sexual_partner(int index_fragment, int index_population, int index_group);
        std::string Generate_random_bit_strings();
        unsigned char Random_gender();

    };
    //--- End Class Especime ---

    //--- Struct Individuals ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Stores the males and females specimens.
    struct Individuals
    {
        //Variables
        std::vector<ENTITY::Specimen> males;
        std::vector<ENTITY::Specimen> females;
    };
    //--- End Struct Individuals ---

    //--- Struct Alpha ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Stores current group alpha information
    struct Alpha
    {
        //Variables
        int index = 0;
        unsigned int duration = 1;
        bool has_alpha = false;
    };
    //--- End Struct Alpha ---

    //--- Struct Groups ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Stores the specimens, alpha information and the total number of individuals.
    struct Group
    {
        //Variables
        unsigned long long int identification;
        unsigned long long int group_origin;
        unsigned int abundance;
        ENTITY::Alpha alpha;
        ENTITY::Individuals individuals;
        std::vector<ENTITY::Specimen> males_to_insert;
        std::vector<ENTITY::Specimen> females_to_insert;
        std::vector<int> target_groups;
        //Methods
        Group(int group_identification, int group_origin_identification);//Creates an empty group
        Group(int number_of_individuals, int Specie_identification, int group_identification);//Creates a group with random specimens
        void Set_target_groups(int size_groups, int index_group);
        void Shuffle_vectors(unsigned char source_vector);
        void Erase_specimens();
        void Insert_specimens();
        void Sort_specimens(bool sort_male, bool sort_female);
        void SET_Alpha();
        void Update_alpha(int Specie_identification);
        void Update_abundance();
        void Update();
        int Count_available_females();
    };
    //--- End Struct Groups ---

    //--- Struct Emigrants ---
    //Last update : 12/01/2022
    //Creat Date : 12/01/2022
    //Description : Stores the individuals that have migrated.
    struct Emigrants
    {
        //Variables
        ENTITY::Individuals elements;
    };
    //--- End Struct Emigrants ---

    //--- Struct TABLES ---
    //Last update : 17/05/2023
    //Creat Date : 21/10/2021
    //Description : Stores the data for analysis.
    struct POPULATION_TABLES
    {
        //K
        std::vector<long long int> Carrying_capacity;
        //N
        std::vector<long long int> Females;
        std::vector<long long int> Males;
        //T
        std::vector<long long int> Threshold;
        std::vector<std::vector<long long int>> Threshold_by_Ages;
        //Groups
        std::vector<long long int> Groups;
        //Kinship labels
        std::vector<long long int> kinship_labels;
        //Death
        std::vector<long long int> Senescence_Death;
        std::vector<long long int> Verhulst_Death;
        std::vector<std::vector<long long int>> Death_By_Age;
        //Births
        std::vector<long long int> Birth;
        //Chances of reproducing
        std::vector<long long int> Reproductives_Chances;
        //Age
        std::vector<std::vector<long long int>> Ages_By_Ages;
        //Alpha
        std::vector<long long int> Alpha_Reign;
        //Disperse
        std::vector<long long int> Disperse;
        //Migration
        std::vector<long long int> Migration_entries;
        std::vector<long long int> Migration_exits;
        //Verhulst
        std::vector<double> Verhulst;
        //Verhulst Bias
        std::vector<double> V_Bias;
        //Reserve memory
        void reserve_memory(int number_time_steps, int cleaning_duration, int specimen_max_age);
        void reset(int specimen_max_age); //Reset = Fill the vectors with 0
    };
    //--- End Struct POPULATION_TABLES ---

    //--- Class Population ---
    //Last update : 23/03/2022
    //Creat Date : 21/10/2021
    //Description : Stores the total amount of specimens and groups
    class Population
    {
        //Variables
    public:
        unsigned long long int Specie_identification = 0;
        unsigned long long int death_by_senescence = 0;
        unsigned long long int death_by_Verhulst = 0;
        unsigned long long int dispersion_exits = 0;
        unsigned long long int migration_exits = 0;
        unsigned long long int migration_entries = 0;
        unsigned long long int available_females = 0; 
        unsigned long long int reproductives_chances = 0;
        unsigned long long int births = 0;
        unsigned long long int population_treshold = 0;
        unsigned long long int females_amount = 0;
        unsigned long long int males_amount = 0;
        unsigned long long int alpha_changes = 0;
        unsigned int carrying_capacity = 0;
        unsigned int carrying_capacity_with_variation = 0;
        unsigned int abundance = 0;
        double verhulst = 0;
        double VERHULST_BIAS_VARIANT = 0;
        ENTITY::Emigrants emigrants;
        ENTITY::POPULATION_TABLES data_table;
        std::vector<ENTITY::Group> groups;
        std::vector<int> all_kinship_labels;
        //Method
        void Set_carrying_capacity(double individuals_per_square_kilometers, double current_area);
        void Update(int index_fragment);
        void Start_Population(double initial_density, int maximum_individuals_per_group);
        void Select_breeders(int breeders_quantity);
        void Apply_variations(int index_fragment);
        void Set_groups_targets();
        void Delete_empty_groups();
        void Split_groups();
        void Sort_specimens();
        void Set_verhulst();
        void Store_data();
        void Update_groups();
        void Clear_groups();
        void Count_available_females();
        void Update_abundance();
        void Update_Alpha();
        void Exodus();
        int Count_kinship_labels();
        int Random_breeding_group();
        unsigned int Calculate_reproduction_chances(int empty_spaces, int available_females, int specie_identification);
        Population(int initial_density, int index_Specie_List, double current_area, bool start_population);
    };
    //--- End Class Population ---

    //--- Struct FRAGMENT_TABLES ---
    //Last update : 27/09/2022
    //Creat Date : 27/09/2022
    //Description : Stores the data for analysis.
    struct FRAGMENT_TABLES
    {
        //Area
        std::vector<double> Area;
        //Perimeter
        std::vector<double> Perimeter;
        //Reserve memory
        void reserve_memory(int number_time_steps, int cleaning_duration);
        void reset(); //Reset = Fill the vectors with 0
    };
    //--- End Struct FRAGMENT_TABLES ---
    
    //--- Struct FRAGMENTATION ---
    //Last update : 08/03/2022
    //Creat Date : 08/03/2022
    //Description : Stores the data for fragmentation.
    struct FRAGMENTATION
    {
        //Variables
        unsigned int fragmentation_time_step = 0;
        unsigned int number_of_fragmented_areas = 0;
        int source_fragment_identification = -1;
        bool fragment_environment = false;
    };
    
    //--- End Struct FRAGMENTATION ---

    //--- Struct VARIATION_CATEGORIES ---
    //Last update : 23/03/2022
    //Creat Date : 23/03/2022
    //Description : Stores data to vary the value of a variable.
    struct VARIATION_CATEGORIES
    {
        //Variables
        unsigned int start_time_step = 0;
        unsigned int duration = 0;
        unsigned int end_time_step = 0;
    };
    
    //--- End Struct VARIATION_CATEGORIES ---

    //--- Struct VARIATION ---
    //Last update : 23/03/2022
    //Creat Date : 23/03/2022
    //Description : Stores data to vary the value of a variable.
    struct VARIATION
    {
        //Variables
        double variance = 0;
        std::string type = "";
        VARIATION_CATEGORIES descending;
        VARIATION_CATEGORIES growing;
    };
    
    //--- End Struct VARIATION ---

    //--- Class Fragment ---
    //Last update : 23/03/2022
    //Creat Date : 21/10/2021
    //Description : Stores all necessary processes and variables of landscape's fragments.
    struct Fragment
    {
        //Variables
        unsigned long long int identification = 0;
        double original_area = 0;
        double original_perimeter = 0;
        double current_area = 0;
        double current_perimeter = 0;
        double initial_density = 0;
        bool reset_values_after_variation = false;
        ENTITY::FRAGMENTATION Fragmentation_variables;
        ENTITY::VARIATION Area_variation;
        ENTITY::VARIATION Perimeter_variation;
        ENTITY::VARIATION Carrying_capacity_variation;
        ENTITY::VARIATION Verhulst_variation;
        std::vector<ENTITY::Population> populations;
        std::vector<ENTITY::Fragment> new_fragments;
        //fragment tables
        FRAGMENT_TABLES fragments_tables;
        void Apply_variations();
        void Fragmentation(); 
        void Populate_fragmented_areas();
        void Store_data();
        void Update();
    };

    //--- End Class Fragment ---

    //--- Struct Specie ---
    //Last update : 23/03/2022
    //Creat Date : 21/10/2021
    //Description : Stores specie's information
    struct Specie
    {
        //Variables
        unsigned long long int last_specimen_identification = 0;
        unsigned long long int last_kinship_label = 0;
        unsigned long long int identification = 0;
        unsigned int maximum_age = 0;
        unsigned int maturity_age = 0;
        unsigned int disperse_age = 0;
        unsigned int mutation_threshold = 0;
        unsigned int bit_strings_size = 0;
        unsigned int number_of_dominant_positions = 0;
        unsigned int alphas_reign_time = 0;
        unsigned int number_of_offsprings = 0;
        unsigned int duration_of_parental_care = 0;
        unsigned int maximum_number_of_individuals_per_group = 0;
        unsigned int Penalty_for_one_equal_kinship_label = 0;
        unsigned int Penalty_for_two_identical_kinship_label = 0;
        unsigned int Penalty_Penna_Model = 0;
        double individuals_per_square_kilometers = 0;
        double alpha_reproductive_success = 0;
        double female_reproductive_failure = 0;
        double disperse_rate = 0;
        bool has_alpha = false;
        bool has_parental_care = false;
        bool organize_in_groups = false;
        bool has_asexual_reproduction = false;
        bool has_sexual_reprodution = false;
        bool male_disperses = false;
        bool female_disperses = false;
        bool male_migrates = false;
        bool female_migrates =false;
        bool affected_by_carrying_capacity_variation = false;
        bool affected_by_Verhulst_variation = false;
        std::string name = " ";
    };

    //--- End Specie ---

}

namespace TOOLS
{
    //--- Struct Random ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Stores and create random numbers.
    struct RANDOM_NUMBERS
    {
        //Variables
        double number = 0;
        std::vector<double> numbers;
        //Methods
        void CREATE_RANDOM_NUMBERS(int random_number_amount, int maximum_range);
        void CLEAR();
    };
    //--- End Struct Random---

    //--- Struct Progression bar ---
    //Last update : 21/10/2021
    //Creat Date : 21/10/2021
    //Description : Progression bar variables and methods.
    struct PROGRESS_BAR
    {
        //Variables
        unsigned int progress_bar_lenght = 10;
        //Methods
        void Update(int sample, int dataset, int total_sample, int total_datasets);
    };

    //--- End Progression bar ---
}

namespace MODEL_SETTINGS
{
    //--- Struct GLOBAL_VARIABLES ---
    //Last update : 23/03/2022
    //Creat Date : 21/10/2021
    //Description : Stores the main variables of the model.
    struct GLOBAL_VARIABLES
    {
        //Varibles
        unsigned int number_of_time_steps = 0;
        unsigned int sample_quantity = 0;
        unsigned int datasets = 0;
        unsigned int CLEANING_DURATION = 0;
        unsigned int current_time_step = 0;
        unsigned int current_dataset = 0;
        unsigned long long int last_group_identification = 0;
        double ORIGINAL_VERHULST_BIAS = 0;
        bool INDISCRIMINATE_VERHULST = false;
        bool MIGRATE = false;
        bool DISPERSE = false;
        bool CLEANING = false;
        std::string OUTPUT_DIRECTORY = "/Outputs/";
        std::string FOLDER_NAME = "Simulation";
        //OBJs
        TOOLS::RANDOM_NUMBERS _aleatory;
        TOOLS::PROGRESS_BAR Progress_bar;
        std::vector<ENTITY::Fragment> fragments;
        std::vector<std::string> settings_data;
        std::vector<std::string> fragments_data;
        std::vector<std::string> species_data;
        std::vector<ENTITY::Specie> SPECIES_LIST;
        //Matrix
        std::vector<std::string> crossing_cost_matrix_data;
        std::vector<std::vector<std::string>> crossing_cost_matrix;
        std::vector<std::string> crossing_cost_column; 
    };
    //--- End Struct GLOBAL_VARIABLES ---
}
 extern MODEL_SETTINGS::GLOBAL_VARIABLES Settings;
#endif