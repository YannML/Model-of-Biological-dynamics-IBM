// includes
#include "Include/Entities.hpp"

//--- RANDOM_NUMBERS
// CLEAR
void TOOLS::RANDOM_NUMBERS::CLEAR()
{
    this->number = 0;
    this->numbers.clear();
}

// CREATE_RANDOM_NUMBERS
void TOOLS::RANDOM_NUMBERS::CREATE_RANDOM_NUMBERS(int random_number_amount, int maximum_range)
{

    this->CLEAR();
    double random_number = 0;
    static std::uniform_real_distribution<> dis(0, 1);

    int numbers_to_create = random_number_amount;
    this->numbers.reserve(numbers_to_create);
    while (numbers_to_create > 0)
    {
        static std::default_random_engine e;
        random_number = dis(e) * maximum_range;
        this->numbers.emplace_back(random_number);
        numbers_to_create--;
    }
    this->number = random_number;
}

//--- End RANDOM_NUMBERS

//--- Struct Motion_path
ENTITY::Motion_path::Motion_path(int fragment_destiny, int population_destiny, int group_destiny, int source_fragment, int source_population, int source_group, int source_index, bool emigrant)
{
    this->fragment_destiny = fragment_destiny;
    this->population_destiny = population_destiny;
    this->group_destiny = group_destiny;
    this->source_fragment = source_fragment;
    this->source_population = source_population;
    this->source_group = source_group;
    this->source_index = source_index;
    this->emigrant = emigrant;
}
// --- End Motion_path

//--- Tables
// Create tables
void ENTITY::FRAGMENT_TABLES::reset()
{
    std::fill(this->Area.begin(), this->Area.end(), 0);
    std::fill(this->Area.begin(), this->Area.end(), 0);
}
void ENTITY::FRAGMENT_TABLES::reserve_memory(int number_time_steps, int cleaning_duration)
{
    // Calculate sample rows
    int sample_duration = number_time_steps + cleaning_duration;
    // Reserving data & filling vectors
    this->Area.resize(sample_duration);
    this->Perimeter.resize(sample_duration);
    // Reset = Fill the vectors with 0
    this->reset();
}
void ENTITY::POPULATION_TABLES::reset(int specimen_max_age)
{
    // Filling vectors
    //  K
    std::fill(this->Carrying_capacity.begin(), this->Carrying_capacity.end(), 0);
    // N
    std::fill(this->Females.begin(), this->Females.end(), 0);
    std::fill(this->Males.begin(), this->Males.end(), 0);
    // T
    std::fill(this->Threshold.begin(), this->Threshold.end(), 0);
    // Groups
    std::fill(this->Groups.begin(), this->Groups.end(), 0);
    // Kinship labels
    std::fill(this->kinship_labels.begin(), this->kinship_labels.end(), 0);
    // Death
    std::fill(this->Senescence_Death.begin(), this->Senescence_Death.end(), 0);
    std::fill(this->Verhulst_Death.begin(), this->Verhulst_Death.end(), 0);
    // Chances of reproducing
    std::fill(this->Reproductives_Chances.begin(), this->Reproductives_Chances.end(), 0);
    // Births
    std::fill(this->Birth.begin(), this->Birth.end(), 0);
    // Alpha
    std::fill(this->Alpha_Reign.begin(), this->Alpha_Reign.end(), 0);
    // Disperse
    std::fill(this->Disperse.begin(), this->Disperse.end(), 0);
    // Migration
    std::fill(this->Migration_entries.begin(), this->Migration_entries.end(), 0);
    std::fill(this->Migration_exits.begin(), this->Migration_exits.end(), 0);
    // Verhulst
    std::fill(this->Verhulst.begin(), this->Verhulst.end(), 0);
    // Verhulst Bias
    std::fill(this->V_Bias.begin(), this->V_Bias.end(), 0);

    // Vector for data base on age
    std::vector<long long int> empty_vector_rows_by_age(specimen_max_age + 1, 0);
    // Age
    std::fill(this->Ages_By_Ages.begin(), this->Ages_By_Ages.end(), empty_vector_rows_by_age);
    // Death
    std::fill(this->Death_By_Age.begin(), this->Death_By_Age.end(), empty_vector_rows_by_age);
    // T
    std::fill(this->Threshold_by_Ages.begin(), this->Threshold_by_Ages.end(), empty_vector_rows_by_age);
}
void ENTITY::POPULATION_TABLES::reserve_memory(int number_time_steps, int cleaning_duration, int specimen_max_age)
{
    // Calculate sample rows
    int sample_duration = number_time_steps + cleaning_duration;

    // Reserving data
    //  K
    this->Carrying_capacity.resize(sample_duration);
    // N
    this->Females.resize(sample_duration);
    this->Males.resize(sample_duration);
    // T
    this->Threshold.resize(sample_duration);
    this->Threshold_by_Ages.resize(sample_duration);
    // Groups
    this->Groups.resize(sample_duration);
    // Kinship labels
    this->kinship_labels.resize(sample_duration);
    // Death
    this->Senescence_Death.resize(sample_duration);
    this->Verhulst_Death.resize(sample_duration);
    this->Death_By_Age.resize(sample_duration);
    // Chances of reproducing
    this->Reproductives_Chances.resize(sample_duration);
    // Births
    this->Birth.resize(sample_duration);
    // Age
    this->Ages_By_Ages.resize(sample_duration);
    // Alpha
    this->Alpha_Reign.resize(sample_duration);
    // Disperse
    this->Disperse.resize(sample_duration);
    // Migration
    this->Migration_entries.resize(sample_duration);
    this->Migration_exits.resize(sample_duration);
    // Verhulst
    this->Verhulst.resize(sample_duration);
    // Verhulst Bias
    this->V_Bias.resize(sample_duration);

    // Reset = Fill the vectors with 0
    this->reset(specimen_max_age);
}
//--- End Tables

//--- Class Specimen ---
// Main class
ENTITY::Specimen::Specimen(int Specie_identification)
{
    this->specie_identification = Specie_identification;
    
    if (Settings.SPECIES_LIST[Specie_identification].last_specimen_identification == LLONG_MAX)
    {
        Settings.SPECIES_LIST[Specie_identification].last_specimen_identification = 0;
    }
    
    this->identification = ++Settings.SPECIES_LIST[this->specie_identification].last_specimen_identification;
    // Generate random bitstring
    this->bitstrings.reserve(2);
    this->bitstrings.emplace_back(Generate_random_bit_strings());
    this->bitstrings.emplace_back(Generate_random_bit_strings());

    // Calculate life expectancy
    Calculate_life_expectancy();
    // Random age
    Settings._aleatory.CREATE_RANDOM_NUMBERS(1, this->life_expectancy);
    this->current_age = (int)Settings._aleatory.number;
    // kinship labels
    if (Settings.SPECIES_LIST[this->specie_identification].last_kinship_label >= LLONG_MAX)
    {
        Settings.SPECIES_LIST[this->specie_identification].last_kinship_label = 0;
    }

    this->kinship_labels[0] = ++Settings.SPECIES_LIST[this->specie_identification].last_kinship_label;
    this->kinship_labels[1] = ++Settings.SPECIES_LIST[this->specie_identification].last_kinship_label;
    // Gender
    this->gender = Random_gender();
    this->erase = false;
}

ENTITY::Specimen::Specimen(int index_fragment, int index_population, int index_group, int female_index, int male_index)
{
    int Specie_identification = Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[female_index].specie_identification;

    if (Settings.SPECIES_LIST[Specie_identification].last_specimen_identification == LLONG_MAX)
    {
        Settings.SPECIES_LIST[Specie_identification].last_specimen_identification = 0;
    }

    // Identifications
    this->identification = ++Settings.SPECIES_LIST[Settings.fragments[index_fragment].populations[index_population].Specie_identification].last_specimen_identification;
    this->specie_identification = Specie_identification;
    this->maternal_identification = Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[female_index].identification;
    this->last_group_identification = Settings.fragments[index_fragment].populations[index_population].groups[index_group].identification;

    // Gender
    this->gender = Random_gender();

    // Kinship
    unsigned long long int *maternal_kinship_labels = Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[female_index].kinship_labels;
    unsigned long long int *paternal_kinship_labels = Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males[male_index].kinship_labels;

    this->kinship_labels[0] = maternal_kinship_labels[0];
    this->kinship_labels[1] = paternal_kinship_labels[0];

    // Crossing over
    Crossing_over(
        Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[female_index].bitstrings,
        Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males[male_index].bitstrings);
    // Apply mutations
    Apply_mutations(maternal_kinship_labels, paternal_kinship_labels);

    // Age
    this->Calculate_life_expectancy();
    this->current_age = 0;

    //Store data
    this->Store_data(index_fragment,index_population);
    // Erase tag
    this->erase = false;
}

ENTITY::Specimen::Specimen(const ENTITY::Specimen &original)
{
    this->identification = original.identification;
    this->last_group_identification = original.last_group_identification;
    this->specie_identification = original.specie_identification;
    this->maternal_identification = original.maternal_identification;
    this->kinship_labels[0] = original.kinship_labels[0];
    this->kinship_labels[1] = original.kinship_labels[1];
    this->current_age = original.current_age;
    this->life_expectancy = original.life_expectancy;
    this->parental_care_duration = original.parental_care_duration;
    this->gender = original.gender;
    this->is_alpha = original.is_alpha;
    this->was_alpha = original.was_alpha;
    this->taking_care = original.taking_care;
    this->erase = original.erase;
    this->able_to_mate = original.able_to_mate;
    if(original.bitstrings.size() > 0)
    {   
        this->bitstrings.reserve(sizeof(original.bitstrings));
        this->bitstrings = original.bitstrings;
    }
    if(original.offsprings.size() > 0)
    {
        this->offsprings.reserve(sizeof(original.offsprings));
        this->offsprings = original.offsprings;
    }
}

ENTITY::Specimen::Specimen(ENTITY::Specimen *original)
{
    this->identification = original->identification;
    this->last_group_identification = original->last_group_identification;
    this->specie_identification = original->specie_identification;
    this->maternal_identification = original->maternal_identification;
    this->kinship_labels[0] = original->kinship_labels[0];
    this->kinship_labels[1] = original->kinship_labels[1];
    this->current_age = original->current_age;
    this->life_expectancy = original->life_expectancy;
    this->parental_care_duration = original->parental_care_duration;
    this->gender = original->gender;
    this->is_alpha = original->is_alpha;
    this->was_alpha = original->was_alpha;
    this->taking_care = original->taking_care;
    this->erase = original->erase;
    this->able_to_mate = original->able_to_mate;
    if(original->bitstrings.size() > 0)
    {
        this->bitstrings.reserve(sizeof(original->bitstrings));
        this->bitstrings = original->bitstrings;
    }
    if(original->offsprings.size() > 0)
    {
        this->offsprings.reserve(sizeof(original->offsprings));
        this->offsprings = original->offsprings;
    }
}

void ENTITY::Specimen::Age(int index_fragment, int index_population, int index_group)
{
    int deaths = 0;
    this->current_age++;
    // Parental care update
    if (this->taking_care == true)
    {
        Update_parental_care(index_fragment, index_population, index_group);
    }
    // Life Expectancy
    if (this->current_age >= this->life_expectancy)
    {
        this->erase = true;
        // Count Deaths by Senescence
        deaths++;
        if (this->taking_care == true)
        {
            deaths += this->offsprings.size();
            for (int index = 0; index < this->offsprings.size(); ++index)
            {
                // Death by age
                Settings.fragments[index_fragment].populations[index_population].data_table.Death_By_Age[Settings.current_time_step][offsprings[index].current_age]++;
            }
        }
        // Death by age
        Settings.fragments[index_fragment].populations[index_population].data_table.Death_By_Age[Settings.current_time_step][this->current_age]++;
        Settings.fragments[index_fragment].populations[index_population].death_by_senescence += deaths;
    }
}

void ENTITY::Specimen::Update_parental_care(int index_fragment, int index_population, int index_group)
{
    // Increase offspring age
    for (ENTITY::Specimen &offspring : this->offsprings)
    {
        offspring.current_age++;
        offspring.Store_data(index_fragment,index_population);
    }
    // Increase duration of parental care
    this->parental_care_duration++;
    // Verifies if parental care has come to an end
    if (this->parental_care_duration >= Settings.SPECIES_LIST[this->specie_identification].duration_of_parental_care)
    {
        this->taking_care = false;
        for (ENTITY::Specimen &offspring : this->offsprings)
        {
            ENTITY::Specimen specimen_copy(offspring);
            if(specimen_copy.gender == 'M')
            {
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].males_to_insert.reserve(sizeof(specimen_copy));
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].males_to_insert.emplace_back(specimen_copy);
            }else
            {
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].females_to_insert.reserve(sizeof(specimen_copy));
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].females_to_insert.emplace_back(specimen_copy);
            }
        }
        this->offsprings.clear();
        this->parental_care_duration = 0;
    }
}

void ENTITY::Specimen::Calculate_life_expectancy()
{

    // Random dominat positions
    Settings._aleatory.CREATE_RANDOM_NUMBERS(Settings.SPECIES_LIST[this->specie_identification].number_of_dominant_positions, Settings.SPECIES_LIST[this->specie_identification].bit_strings_size);
    std::vector<int> dominant_positions;
    dominant_positions.reserve(Settings.SPECIES_LIST[this->specie_identification].number_of_dominant_positions);
    int index = 0;
    while (index < Settings.SPECIES_LIST[this->specie_identification].number_of_dominant_positions)
    {
        dominant_positions.emplace_back((int)Settings._aleatory.numbers[index]);
        index++;
    }

    // Count mutations in the bit strings
    char mutation = '1';
    std::string bitstring_one = this->bitstrings[0];
    std::string bitstring_two = this->bitstrings[1];
    int number_of_mutations_found = 0;
    int last_mutation_position = -1;

    int index_bitstrings = 0;
    while (index_bitstrings < Settings.SPECIES_LIST[this->specie_identification].bit_strings_size)
    {
        if (number_of_mutations_found == Settings.SPECIES_LIST[this->specie_identification].mutation_threshold)
        {
            break;
        }

        for (int i = 0; i < dominant_positions.size(); i++)
        {
            if (index_bitstrings == dominant_positions[i] && index_bitstrings != last_mutation_position)
            {
                if (bitstring_one[index_bitstrings] == mutation || bitstring_two[index_bitstrings] == mutation)
                {
                    number_of_mutations_found++;
                    last_mutation_position = index_bitstrings;
                }
            }
        }

        if (index_bitstrings != last_mutation_position)
        {
            if (bitstring_one[index_bitstrings] == mutation && bitstring_two[index_bitstrings] == mutation)
            {
                number_of_mutations_found++;
                last_mutation_position = index_bitstrings;
            }
        }

        index_bitstrings++;
    }
    // Set life expectancy
    int expectancy = Settings.SPECIES_LIST[this->specie_identification].maximum_age;
    if (number_of_mutations_found >= Settings.SPECIES_LIST[this->specie_identification].mutation_threshold)
    {
        expectancy = last_mutation_position;
    }
    this->life_expectancy = expectancy;
}

void ENTITY::Specimen::Crossing_over(std::vector<std::string> maternal_bitstrings, std::vector<std::string> paternal_bitstrings)
{
    std::vector<std::string> maternal_cutoff_bitstring;
    std::vector<std::string> paternal_cutoff_bitstring;
    std::vector<std::string> first_string_rearranged;
    std::vector<std::string> second_string_rearranged;

    std::vector<int> cutoff_points;
    Settings._aleatory.CREATE_RANDOM_NUMBERS(2, Settings.SPECIES_LIST[this->specie_identification].bit_strings_size);
    cutoff_points.reserve(2);
    cutoff_points.emplace_back(Settings._aleatory.numbers[0]);
    cutoff_points.emplace_back(Settings._aleatory.numbers[1]);
    
    int bitstring_length = Settings.SPECIES_LIST[this->specie_identification].bit_strings_size;
    for(auto position : cutoff_points)
    {
        if(position <=0)
        {
            position = 1;
        }
        if(position >= bitstring_length)
        {
            position = bitstring_length - 1;
        }
    }
    // Cutting
    int numbers_to_end = bitstring_length - cutoff_points[0];
    maternal_cutoff_bitstring.push_back(maternal_bitstrings[0].substr(0, cutoff_points[0]));
    maternal_cutoff_bitstring.push_back(maternal_bitstrings[0].substr(cutoff_points[0], numbers_to_end));
    paternal_cutoff_bitstring.push_back(paternal_bitstrings[0].substr(0, cutoff_points[0]));
    paternal_cutoff_bitstring.push_back(paternal_bitstrings[0].substr(cutoff_points[0], numbers_to_end));
    numbers_to_end = bitstring_length - cutoff_points[1];
    maternal_cutoff_bitstring.push_back(maternal_bitstrings[1].substr(0, cutoff_points[1]));
    maternal_cutoff_bitstring.push_back(maternal_bitstrings[1].substr(cutoff_points[1], numbers_to_end));
    paternal_cutoff_bitstring.push_back(paternal_bitstrings[1].substr(0, cutoff_points[1]));
    paternal_cutoff_bitstring.push_back(paternal_bitstrings[1].substr(cutoff_points[1], numbers_to_end));

    first_string_rearranged.push_back(maternal_cutoff_bitstring[0] + paternal_cutoff_bitstring[1]);
    first_string_rearranged.push_back(maternal_cutoff_bitstring[1] + paternal_cutoff_bitstring[0]);
    first_string_rearranged.push_back(paternal_cutoff_bitstring[0] + maternal_cutoff_bitstring[1]);
    first_string_rearranged.push_back(paternal_cutoff_bitstring[1] + maternal_cutoff_bitstring[0]);

    second_string_rearranged.push_back(maternal_cutoff_bitstring[2] + paternal_cutoff_bitstring[3]);
    second_string_rearranged.push_back(maternal_cutoff_bitstring[3] + paternal_cutoff_bitstring[2]);
    second_string_rearranged.push_back(paternal_cutoff_bitstring[2] + maternal_cutoff_bitstring[3]);
    second_string_rearranged.push_back(paternal_cutoff_bitstring[3] + maternal_cutoff_bitstring[2]);
    // Randomly choose the bitstrings combinations
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    
    std::shuffle(std::begin(first_string_rearranged),std::end(first_string_rearranged), rng);
    std::shuffle(std::begin(second_string_rearranged),std::end(second_string_rearranged), rng);
    
    this->bitstrings.reserve(sizeof(first_string_rearranged[0]));
    this->bitstrings.emplace_back(first_string_rearranged[0]);
    this->bitstrings.reserve(sizeof(second_string_rearranged[0]));
    this->bitstrings.emplace_back(second_string_rearranged[0]);

    //std::cout<<"\n**New Specimen**"<<this->identification;
    //std::cout<<"\nCutOffPoints["<< cutoff_points[0]<<" | "<<cutoff_points[1]<<"]";
    //std::cout<<"\nMother_Bitstrings: "<< maternal_bitstrings[0]<<" | "<<maternal_bitstrings[1];
    //std::cout<<"\nFather_Bitstrings: "<< paternal_bitstrings[0]<<" | "<<paternal_bitstrings[1];
    //std::cout<<"\nFirst string possibilities : "<< first_string_rearranged[0]<<" | "<<first_string_rearranged[1]<<" | "<< first_string_rearranged[2]<<" | "<<first_string_rearranged[3];
    //std::cout<<"\nSecond string possibilities: "<< second_string_rearranged[0]<<" | "<<second_string_rearranged[1]<<" | "<< second_string_rearranged[2]<<" | "<<second_string_rearranged[3];
    //std::cout<<"\nOffspring_Bitstrings: "<< this->bitstrings[0]<<" | "<<this->bitstrings[1];
}

void ENTITY::Specimen::Apply_mutations(unsigned long long int *maternal_kinship_labels, unsigned long long int *paternal_kinship_labels)
{
    // similarity kinships check
    int similarity = 0;
    int new_mutations_amount = Settings.SPECIES_LIST[this->specie_identification].Penalty_Penna_Model;

    if (maternal_kinship_labels[0] == paternal_kinship_labels[0])
    {
        similarity++;
    }
    if (maternal_kinship_labels[1] == paternal_kinship_labels[1])
    {
        similarity++;
    }

    switch (similarity)
    {
    case 1:
    {
        new_mutations_amount = Settings.SPECIES_LIST[this->specie_identification].Penalty_for_one_equal_kinship_label + Settings.SPECIES_LIST[this->specie_identification].Penalty_Penna_Model;
        break;
    }
    case 2:
    {
        new_mutations_amount = Settings.SPECIES_LIST[this->specie_identification].Penalty_for_two_identical_kinship_label + Settings.SPECIES_LIST[this->specie_identification].Penalty_Penna_Model;
        break;
    }
    }

    // Apply mutations
    //std::cout<<"\nMutations Amount: "<< new_mutations_amount;
    int mutation = 0;
    while (mutation < new_mutations_amount)
    {
        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
        int index_string = 0;
        if (Settings._aleatory.number >= 0.5)
        {
            index_string = 1;
        }
       
        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, Settings.SPECIES_LIST[this->specie_identification].bit_strings_size);
        std::string specimen_string = this->bitstrings[index_string];
        //std::cout<<"\nRandom BitString: "<< index_string<<" Mutation Position: "<<(int)Settings._aleatory.number;
        specimen_string[(int)Settings._aleatory.number] = '1';
        this->bitstrings[index_string] = specimen_string;

        mutation++;
    }
    //std::cout<<"\nOffspring_Bitstrings: "<< this->bitstrings[0]<<" | "<<this->bitstrings[1];
    //std::cout<<"\n ";
}

void ENTITY::Specimen::Relocate_specimens(ENTITY::Motion_path path)
{
    //Creates empty object
    ENTITY::Specimen specimen_copy = this;
    // Destination possibilities
    switch (this->gender)
    {
    case 'F':
        if (path.emigrant == true)//Migrate
        {
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].emigrants.elements.females.reserve(
                sizeof(specimen_copy)
            );
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].emigrants.elements.females.emplace_back(
                specimen_copy);
        }
        else // Disperse
            {
                Settings.fragments[path.fragment_destiny].populations[path.population_destiny].groups[path.group_destiny].females_to_insert.reserve(
                    sizeof(specimen_copy)
                );
                Settings.fragments[path.fragment_destiny].populations[path.population_destiny].groups[path.group_destiny].females_to_insert.emplace_back(
                    specimen_copy);
            }
        break;
    case 'M':
        if (path.emigrant == true) // Migrate
        {
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].emigrants.elements.males.reserve(
                sizeof(specimen_copy)
            );
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].emigrants.elements.males.emplace_back(
                specimen_copy);
        }else // Disperse
        {
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].groups[path.group_destiny].males_to_insert.reserve(
                sizeof(specimen_copy)
            );
            Settings.fragments[path.fragment_destiny].populations[path.population_destiny].groups[path.group_destiny].males_to_insert.emplace_back(
                specimen_copy);
        }
        break;
    }

    // The original is marked to be deleted
    this->erase = true;
}

std::string ENTITY::Specimen::Generate_random_bit_strings()
{
    // Random bit strings
    int bitstrings_size = Settings.SPECIES_LIST[this->specie_identification].bit_strings_size;
    Settings._aleatory.CREATE_RANDOM_NUMBERS(bitstrings_size, 1);

    int index_random = 0;
    std::string bitstring ="";
    while (index_random < bitstrings_size)
    {
        int random_number = 0;
        Settings._aleatory.numbers[index_random] > 0.5 ? random_number = 1 : random_number = 0;

        bitstring += std::to_string(random_number);
        index_random++;
    }
    return bitstring;
}

unsigned char ENTITY::Specimen::Random_gender()
{
    unsigned char gender;
    Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
    Settings._aleatory.number >= 0.5 ? gender = 'M' : gender = 'F';
    return gender;
}

bool ENTITY::Specimen::Disperse_verification()
{
    
        if (this->is_alpha == false)
        {
            if (this->current_age >= Settings.SPECIES_LIST[this->specie_identification].disperse_age)
            {
                switch (this->gender)
                {
                case 'M':
                    if (Settings.SPECIES_LIST[this->specie_identification].male_disperses == true)
                    {
                        // Try to disperse
                        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                        double random_rate = Settings._aleatory.number;
                        if (random_rate <= Settings.SPECIES_LIST[this->specie_identification].disperse_rate)
                        {
                            return true;
                        }
                    }
                    break;
                case 'F':
                    if (Settings.SPECIES_LIST[this->specie_identification].female_disperses == true)
                    {
                        // Try to disperse
                        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                        double random_rate = Settings._aleatory.number;
                        if (random_rate <= Settings.SPECIES_LIST[this->specie_identification].disperse_rate)
                        {
                            return true;
                        }
                    }
                    break;
                } // gender cases
            }     // is mature
        }         // it isn't alpha
    return false;
}

void ENTITY::Specimen::Disperse(int index_fragment, int index_population, int index_group, int index_specimen)
{
    // Disperse is on
    if (Settings.DISPERSE == true)
    {
        // To disperse more than 1 group is needed
        if (Settings.fragments[index_fragment].populations[index_population].groups.size() > 1)
        {
            if(this->erase == false)
            {
                // Disperse verification
                double can_disperse = this->Disperse_verification();
                if (can_disperse == true)
                {
                    // randomly choose a group by shuffling the target groups vector.
                    auto rd = std::random_device{};
                    auto rng = std::default_random_engine{rd()};
                    std::shuffle(std::begin(Settings.fragments[index_fragment].populations[index_population].groups[index_group].target_groups),
                                std::end(Settings.fragments[index_fragment].populations[index_population].groups[index_group].target_groups), rng);
                    int random_group = Settings.fragments[index_fragment].populations[index_population].groups[index_group].target_groups[0];
                    // Edit last group location
                    this->last_group_identification = Settings.fragments[index_fragment].populations[index_population].groups[random_group].identification;
                    ENTITY::Motion_path path(index_fragment, index_population, random_group, index_fragment, index_population, index_group, index_specimen, false);
                    this->Relocate_specimens(path);
                    // Increase population dispersion exits counter
                    Settings.fragments[index_fragment].populations[index_population].dispersion_exits++;
                } // Can disperse verification
            }
        }     // More than 1 group verification
    }
}

int ENTITY::Specimen::Find_sexual_partner(int index_fragment, int index_population, int index_group)
{
    //std::cout<<"\n Inside Find_sexual_partner Grupo: "<<index_group<<" TimeStep: "<<Settings.current_time_step;
    //Only one male
    if(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males.size() == 1)
    {
        //std::cout<<"\n vector size = 1";
        return 0;
    }
    
    //Alpha try to be the partner
    if (Settings.fragments[index_fragment].populations[index_population].groups[index_group].alpha.has_alpha == true)
    {
        double alpha_reproductive_success = Settings.SPECIES_LIST[this->specie_identification].alpha_reproductive_success;
        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
        double alpha_chance = Settings._aleatory.number;
        if (alpha_chance <= alpha_reproductive_success)
        {
            //std::cout<<"\n alpha: "<<Settings.fragments[index_fragment].populations[index_population].groups[index_group].alpha.index;
           return Settings.fragments[index_fragment].populations[index_population].groups[index_group].alpha.index;
        }
    }

    //Ordinary male will be the partner
    //auto first_unable_to_reproduce = [](auto specimen)
    //{
    //    return specimen.able_to_mate == false;
    //};
    //auto specimen_pointer  = std::find_if(std::begin(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males),
    //                    std::end(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males),
    //                    first_unable_to_reproduce);
    
    //int vector_size_limit;
    
    //if(specimen_pointer != std::end(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males))
    //{
    //    //Find unable to reproduce so all males before it can reproduce
    //    vector_size_limit = std::distance(std::begin(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males), specimen_pointer);
    //}
    //else
    //{   
        //Has no unable to reproduce all males can reproduce
    //    vector_size_limit = Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males.size();
    //}
    
    auto count = [](auto specimen)
    {
        return specimen.able_to_mate == true;
    };

    int breeders_amount  = std::count_if(std::begin(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males),
                std::end(Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males),
                count);

    //std::cout<<"\n vector_size_limit: "<<breeders_amount;
    //allocate male indexs
    std::vector<int>male_indexs(breeders_amount);
    // Fill list with all the male indexs
    std::iota(std::begin(male_indexs), std::end(male_indexs), 0);

    // Remove the alpha index
    male_indexs.erase(std::remove(male_indexs.begin(), male_indexs.end(), Settings.fragments[index_fragment].populations[index_population].groups[index_group].alpha.index), male_indexs.end());
                        
    // randomly choose a male by shuffling the male_indexs vector.
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(male_indexs),
                    std::end(male_indexs), rng);
        
    return male_indexs[0];
}

void ENTITY::Specimen::Reproduce(int index_fragment, int index_population, int index_group, int index_specimen)
{
    if (this->reproduce == true)
    {   
        //std::cout<<"\nReproduce()";
        int male_index = this->Find_sexual_partner(index_fragment,index_population, index_group);
        //std::cout<<"\n index: "<<male_index<<"\tsize: "<<Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males.size();
        //Creation of new specimens
        for (int offspring = 0; offspring < Settings.SPECIES_LIST[this->specie_identification].number_of_offsprings; ++offspring)
        {
            ENTITY::Specimen new_specimen(index_fragment, index_population, index_group, index_specimen, male_index);
            
            if(Settings.SPECIES_LIST[this->specie_identification].duration_of_parental_care > 0){
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[index_specimen].offsprings.reserve(sizeof(new_specimen));
                Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[index_specimen].offsprings.emplace_back(new_specimen);
                // Change females status
                this->taking_care = true;
                this->reproduce = false;
                this->able_to_mate = false;
            }
            else
            {
                if(new_specimen.gender =='M')
                {
                    Settings.fragments[index_fragment].populations[index_population].groups[index_group].males_to_insert.reserve(sizeof(new_specimen));
                    Settings.fragments[index_fragment].populations[index_population].groups[index_group].males_to_insert.emplace_back(new_specimen);
                }
                else
                {
                    Settings.fragments[index_fragment].populations[index_population].groups[index_group].females_to_insert.reserve(sizeof(new_specimen));
                    Settings.fragments[index_fragment].populations[index_population].groups[index_group].females_to_insert.emplace_back(new_specimen);
                }
                // Change females status
                this->taking_care = false;
                this->reproduce = false;
                this->able_to_mate = false;
            }

            // Increase births counter
            Settings.fragments[index_fragment].populations[index_population].births += Settings.SPECIES_LIST[this->specie_identification].number_of_offsprings;
        }
    }
}

bool ENTITY::Specimen::Migration_verification(int Verhulst)
{
    if (this->is_alpha == false)
    {
        if (Settings.MIGRATE == true)
        {
            if (this->gender == 'M')
            {
                if (Settings.SPECIES_LIST[this->specie_identification].male_migrates == true)
                {
                    // Try to migrate
                    Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                    if (Settings._aleatory.number < Verhulst)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (Settings.SPECIES_LIST[this->specie_identification].female_migrates == true)
                {
                    // Try to migrate
                    Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                    if (Settings._aleatory.number < Verhulst)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void ENTITY::Specimen::Indiscriminate_verhulst(int index_fragment, int index_population, int index_group, int index_specimen)
{
    if(this->erase == false)
    {
        // verifies if specimen can migrate
        bool can_migrate = this->Migration_verification(Settings.fragments[index_fragment].populations[index_population].verhulst);
        // Random nunbers to validate Verhulst
        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
        // Verifies if it will die
        if (Settings._aleatory.number >= Settings.fragments[index_fragment].populations[index_population].verhulst)
        {
            // Verifies if it will migrate
            if (can_migrate == true)
            {
                // Relocate individual to emigrants vector
                ENTITY::Motion_path path(index_fragment, index_population, index_group, index_fragment, index_population, index_group, index_specimen, true);
                this->Relocate_specimens(path);
                // Increase population migatrion exits counter
                Settings.fragments[index_fragment].populations[index_population].migration_exits++;
            }
            // Mark specimen to die
            this->erase = true;
            // Population death by Verhulst counter increase
            Settings.fragments[index_fragment].populations[index_population].death_by_Verhulst++;
            // Death by age
            Settings.fragments[index_fragment].populations[index_population].data_table.Death_By_Age[Settings.current_time_step][this->current_age]++;
        }
    }
}

void ENTITY::Specimen::Store_data(int index_fragment, int index_population)
{
    int time_step = Settings.current_time_step;

    
    switch (this->gender)
    {
    case 'M':
        Settings.fragments[index_fragment].populations[index_population].males_amount++;
        break;

    case 'F':
        Settings.fragments[index_fragment].populations[index_population].females_amount++;
        break;
    }

    // Parental control
    //if (this->taking_care == true)
    //{
    //    for (ENTITY::Specimen &offspring : this->offsprings)
    //    {
    //        offspring.Store_data(index_fragment, index_population);
    //    }
    //}
    // T
    Settings.fragments[index_fragment].populations[index_population].population_treshold += this->life_expectancy;
    Settings.fragments[index_fragment].populations[index_population].data_table.Threshold_by_Ages[time_step][this->life_expectancy]++;
    // Age
    Settings.fragments[index_fragment].populations[index_population].data_table.Ages_By_Ages[time_step][this->current_age]++;
    // Kinship labels
    Settings.fragments[index_fragment].populations[index_population].all_kinship_labels.reserve(2);
    Settings.fragments[index_fragment].populations[index_population].all_kinship_labels.emplace_back(this->kinship_labels[0]);
    Settings.fragments[index_fragment].populations[index_population].all_kinship_labels.emplace_back(this->kinship_labels[1]);
}

void ENTITY::Specimen::Able_to_mate()
{
    this->able_to_mate = false;
    //if (this->erase == false)
    //    {
            
            if (this->gender == 'M')
            {
                if(this->current_age >= Settings.SPECIES_LIST[this->specie_identification].maturity_age && this->taking_care == false)
                {
                    this->able_to_mate = true;
                }
            }
            else
            {
                if (this->current_age >= (Settings.SPECIES_LIST[this->specie_identification].maturity_age - 1) && this->taking_care == false)
                {
                    // Checks whether parental care will be completed
                    if ((this->current_age - this->life_expectancy) > Settings.SPECIES_LIST[this->specie_identification].duration_of_parental_care)
                    {
                        this->able_to_mate = true;
                    }
                }
            }
      //  }
}

void ENTITY::Specimen::Act(int index_fragment, int index_population, int index_group, int index_specimen)
{
    //  Age
    this->Age(index_fragment, index_population, index_group);
    this->Able_to_mate();
    this->Reproduce(index_fragment, index_population, index_group, index_specimen);
    // Verhulst / Migrate
    if (Settings.fragments[index_fragment].populations[index_population].verhulst > 0) // If smaller than evreybody will migrates. Thats function is outside the Specimen obj
    {
        this->Indiscriminate_verhulst(index_fragment, index_population, index_group, index_specimen);
    }
    //  Disperse
    this->Disperse(index_fragment, index_population, index_group, index_specimen);
    // Store specimen data
    this->Store_data(index_fragment, index_population);
}
//--- End Class Specimen ---

//--- Group ---
void ENTITY::Group::SET_Alpha()
{
    bool chose_an_alpha = false;
    int chosen_specimen_index = 0;
    int last_life_expectancy = 0;

    for (int index = 0; index < this->individuals.males.size(); ++index)
    {

        if (this->individuals.males[index].was_alpha == false)
        {
            if (this->individuals.males[index].life_expectancy > last_life_expectancy)
            {

                chosen_specimen_index = index;
                last_life_expectancy = this->individuals.males[index].life_expectancy;
                chose_an_alpha = true;
            }
        }
    }

    if (chose_an_alpha == true)
    {
        this->individuals.males[chosen_specimen_index].is_alpha = true;
        this->individuals.males[chosen_specimen_index].was_alpha = true;
        this->alpha.index = chosen_specimen_index;
        this->alpha.duration = 1;
        this->alpha.has_alpha = true;
    }
    else
    {
        this->alpha.index = -1;
        this->alpha.duration = 0;
        this->alpha.has_alpha = false;
    }
};

ENTITY::Group::Group(int group_identification, int group_origin_identification)
{
    this->identification = group_identification;
    this->group_origin = group_origin_identification;
    this->abundance = 0;
}

ENTITY::Group::Group(int number_of_individuals, int Specie_identification, int group_identification)
{

    this->identification = group_identification;
    this->group_origin = group_identification;
    this->abundance = number_of_individuals;

    while (number_of_individuals > 0)
    {

        ENTITY::Specimen specimen(Specie_identification);

        specimen.last_group_identification = this->identification;

        if (specimen.gender == 'M')
        {
            
            this->individuals.males.reserve(sizeof(specimen));
            this->individuals.males.emplace_back(specimen);
        }
        else
        {
            this->individuals.females.reserve(sizeof(specimen));
            this->individuals.females.emplace_back(specimen);
        }
        number_of_individuals--;
    }
}

void ENTITY::Group::Shuffle_vectors(unsigned char source_vector)
{
    int vector_size = 0;
    std::vector<ENTITY::Specimen> copy_vector;
    switch (source_vector)
    {
    case 'M':
        vector_size = this->individuals.males.size();
        Settings._aleatory.CREATE_RANDOM_NUMBERS(vector_size, vector_size);
        for (int index = 0; index < vector_size; ++index)
        {
            // Store the elements of the position
            copy_vector.reserve(sizeof(this->individuals.males[index]));
            copy_vector.emplace_back(this->individuals.males[index]);
            // Get the random index
            int random_index = (int)Settings._aleatory.numbers[index];
            // Replace the element with the random element
            this->individuals.males[index] = this->individuals.males[random_index];
            this->individuals.males[random_index] = copy_vector[0];
            copy_vector.clear();
        }
        break;
    case 'F':
        vector_size = this->individuals.females.size();
        Settings._aleatory.CREATE_RANDOM_NUMBERS(vector_size, vector_size);
        for (int index = 0; index < vector_size; ++index)
        {
            // Store the elements of the position
            copy_vector.reserve(sizeof(this->individuals.females[index]));
            copy_vector.emplace_back(this->individuals.females[index]);
            // Get the random index
            int random_index = (int)Settings._aleatory.numbers[index];
            // Replace the element with the random element
            this->individuals.females[index] = this->individuals.females[random_index];
            this->individuals.females[random_index] = copy_vector[0];
            copy_vector.clear();
        }
        break;
    }
}

void ENTITY::Group::Insert_specimens()
{
    if(this->males_to_insert.size() > 0){
        this->individuals.males.reserve(sizeof(this->males_to_insert));
        this->individuals.males.insert(this->individuals.males.end(), this->males_to_insert.begin(), this->males_to_insert.end());
        this->males_to_insert.clear();
    }
    
    if(this->females_to_insert.size() > 0){
        this->individuals.females.reserve(sizeof(this->females_to_insert));
        this->individuals.females.insert(this->individuals.females.end(), this->females_to_insert.begin(), this->females_to_insert.end());
        this->females_to_insert.clear();   
    }
}

void ENTITY::Group::Erase_specimens()
{

    this->individuals.males.erase(
        std::remove_if(
            this->individuals.males.begin(),
            this->individuals.males.end(),
            [](ENTITY::Specimen &specimen)
            { return specimen.erase; }),
        this->individuals.males.end());

    this->individuals.females.erase(
        std::remove_if(
            this->individuals.females.begin(),
            this->individuals.females.end(),
            [](ENTITY::Specimen &specimen)
            { return specimen.erase; }),
        this->individuals.females.end());
}

void ENTITY::Group::Sort_specimens(bool sort_male, bool sort_female)//Sort by viability of reproduction
{
    //Funtion that sort
    auto is_able_to_mate = [](ENTITY::Specimen &specimen){
                    return specimen.able_to_mate == true;
                };
    //Males
    if(sort_male == true)
    {
        std::partition(std::begin(this->individuals.males),
                        std::end(this->individuals.males),
                        is_able_to_mate);
    }
    //Females
    if(sort_female == true)
    {
        std::partition(std::begin(this->individuals.females),
                std::end(this->individuals.females),
                is_able_to_mate);
    }
}

void ENTITY::Group::Set_target_groups(int size_groups, int index_group)
{
    // Clear the vector
    if(this->target_groups.size() > 0)
    {
        this->target_groups.clear();
    }

    if(size_groups > 1)
    {
        // List of target groups to disperse to
        this->target_groups.resize(size_groups);
        // Fill list with all the groups indexs
        std::iota(std::begin(this->target_groups), std::end(this->target_groups), 0);
        // Remove the current index
        this->target_groups.erase(this->target_groups.begin()+index_group);
    }

}

void ENTITY::Group::Update_alpha(int Specie_identification)
{
    if(Settings.SPECIES_LIST[Specie_identification].has_alpha == true)
    {
        //Verifies if group has alpha
        bool alpha_found = false;
        
        auto find_alpha = [](auto specimen)
        {
            return specimen.is_alpha == true;
        };
        auto specimen_pointer  = std::find_if(std::begin(this->individuals.males),
                    std::end(this->individuals.males),
                    find_alpha);
        
        if(specimen_pointer != std::end(this->individuals.males))
        {
            alpha_found = true;
            this->alpha.index = std::distance(std::begin(this->individuals.males), specimen_pointer);
        }

        if (alpha_found == true)
        {
            this->alpha.duration++;

            if (this->alpha.duration > Settings.SPECIES_LIST[Specie_identification].alphas_reign_time)
            {
                // Not a alpha anymore
                this->individuals.males[this->alpha.index].is_alpha = false;
                // Group has no alpha
                this->alpha.has_alpha = false;
                // Find a new alpha
                this->SET_Alpha();
            }
        }
        else
        {
            // Find a new alpha
            this->SET_Alpha();
        }
    }
}

void ENTITY::Group::Update_abundance()
{
    // Sum vectors
    this->abundance =
        this->individuals.males.size() +
        this->individuals.females.size();
    // Count offsprings
    for (ENTITY::Specimen &specimen : this->individuals.females)
    {
        if (specimen.taking_care == true)
        {
            this->abundance += specimen.offsprings.size();
        }
    }
    //to do: loop male if it take care
}

void ENTITY::Group::Update()
{
    this->Erase_specimens();
    this->Insert_specimens();
    this->Update_abundance();
}

int ENTITY::Group::Count_available_females()
{
    auto count = [](auto specimen)
    {
        return specimen.able_to_mate == true;
    };

    int breeders_amount  = std::count_if(std::begin(this->individuals.females),
                std::end(this->individuals.females),
                count);
    return breeders_amount;
}
//--- End Group ---

//--- Class Population
void ENTITY::Population::Set_verhulst()
{

    double abundance_divided_by_support_capability = 0;
    // Carrying capacity with Variation
    if (this->carrying_capacity_with_variation > 0)
    {
        abundance_divided_by_support_capability = this->abundance / (double)this->carrying_capacity_with_variation;
        // Bias With Variation
        this->verhulst = 1 - (this->VERHULST_BIAS_VARIANT * abundance_divided_by_support_capability);
    }
    else
    {
        this->verhulst = 0;
    }
}

void ENTITY::Population::Set_carrying_capacity(double individuals_per_square_kilometers, double current_area)
{
    /*this->carrying_capacity =
    (int)
    (Settings.fragments[index_fragment].current_area /
    Settings.fragments[index_fragment].current_perimeter
    * Settings.SPECIES_LIST[this->Specie_identification].individuals_per_square_kilometers);*/

    this->carrying_capacity =
        (int)(current_area * individuals_per_square_kilometers);

    this->carrying_capacity_with_variation = this->carrying_capacity;
}

void ENTITY::Population::Select_breeders(int breeders_quantity)
{
    while (breeders_quantity > 0)
    {
        // Random Group choose
        int random_group = this->Random_breeding_group();
        // Reproduction | '-1' when have no mature males and available females to reproduce. return of Random_breeding_group() function.
        if (random_group == -1)
        {
            break;
        }
        else
        {
            //auto first_unable_to_reproduce = [](auto specimen)
            //{
            //    return specimen.able_to_mate == false;
            //};
            //auto specimen_pointer  = std::find_if(std::begin(this->groups[random_group].individuals.females),
            //            std::end(this->groups[random_group].individuals.females),
            //            first_unable_to_reproduce);
            //int breeders_amount = this->groups[random_group].individuals.females.size();
            //if(specimen_pointer != std::end(this->groups[random_group].individuals.females))
            //{
            //    breeders_amount = std::distance(std::begin(this->groups[random_group].individuals.females), specimen_pointer);
           //}
            auto reproducible = [](auto specimen)
            {
                return specimen.able_to_mate == true;
            };

            int breeders_amount  = std::count_if(std::begin(groups[random_group].individuals.females),
                        std::end(groups[random_group].individuals.females),
                        reproducible);
            // Random female
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, breeders_amount);
            int random_female = (int)Settings._aleatory.number;
            this->groups[random_group].individuals.females[random_female].reproduce = true;
            this->groups[random_group].individuals.females[random_female].able_to_mate = false;
            breeders_quantity--;
        }
    } // End loop reproduction
}

void ENTITY::Population::Start_Population(double initial_density, int maximum_individuals_per_group)
{
    //std::cout<<"\n grupos size()"<<this->groups.size();
    this->abundance = this->carrying_capacity * initial_density;
    bool create_group = true;
    int number_of_individuals = this->abundance;
    int amount_of_individuals = maximum_individuals_per_group;
    while (create_group == true)
    {
        if (number_of_individuals < maximum_individuals_per_group)
        {
            create_group = false;
            amount_of_individuals = number_of_individuals;
        }

        int group_identification = Settings.last_group_identification++;
        Group group(amount_of_individuals, this->Specie_identification, group_identification);
        this->groups.reserve(sizeof(group));
        this->groups.emplace_back(group);
        number_of_individuals -= maximum_individuals_per_group;
    } // End loop create groups
}

void ENTITY::Population::Apply_variations(int index_fragment)
{
    // Variables
    double variance, value, variable_sum_with_variation, maximum_value, minimum_value;
    bool descending, growing;
    int cases_of_variation;

    // Calculate the new carrying capacity
    int Specie_identification = this->Specie_identification;
    this->Set_carrying_capacity(Settings.SPECIES_LIST[Specie_identification].individuals_per_square_kilometers, Settings.fragments[index_fragment].current_area);

    // Vary K
    if (Settings.SPECIES_LIST[Specie_identification].affected_by_carrying_capacity_variation == true)
    {
        if (Settings.fragments[index_fragment].Carrying_capacity_variation.type != "NONE")
        {
            descending = false;
            growing = false;
            cases_of_variation = 0;
            // Check wich variation category is activated
            if (Settings.current_time_step >= Settings.fragments[index_fragment].Carrying_capacity_variation.growing.start_time_step && Settings.current_time_step <= Settings.fragments[index_fragment].Carrying_capacity_variation.growing.end_time_step)
            {
                growing = true;
            }
            if (Settings.current_time_step >= Settings.fragments[index_fragment].Carrying_capacity_variation.descending.start_time_step && Settings.current_time_step <= Settings.fragments[index_fragment].Carrying_capacity_variation.descending.end_time_step)
            {
                descending = true;
            }
            // Situations
            if (growing == true && descending == true)
            {
                cases_of_variation = 1;
            }
            else if (growing == true && descending == false)
            {
                cases_of_variation = 2;
            }
            else if (growing == false && descending == true)
            {
                cases_of_variation = 3;
            }
            else
            {
                cases_of_variation = 0;
            }
            // Generate variation
            if (growing == true || descending == true)
            {
                variance = Settings.fragments[index_fragment].Carrying_capacity_variation.variance * this->carrying_capacity / 100;
                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, round(variance));
                value = Settings._aleatory.number;
            }
            // Actions for each situation
            switch (cases_of_variation)
            {
            case 1: /*Random*/
            {
                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                int increase_or_decrease = round(Settings._aleatory.number);
                // Limited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "LIMITED")
                {
                    if (increase_or_decrease == 0)
                    {
                        this->carrying_capacity_with_variation = this->carrying_capacity + value;
                    }
                    else
                    {
                        this->carrying_capacity_with_variation = this->carrying_capacity - value;
                    }
                } // End limited
                // Unlimited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "UNLIMITED")
                {
                    if (increase_or_decrease == 0)
                    {
                        this->carrying_capacity_with_variation += value;
                    }
                    else
                    {
                        this->carrying_capacity_with_variation -= value;

                        if (this->carrying_capacity_with_variation <= 0)
                        {
                            this->carrying_capacity_with_variation = 0;
                        }
                    }
                } // End unlimited
                break;
            }
            case 2: /*growing*/
            {
                // Limited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "LIMITED")
                {
                    maximum_value = this->carrying_capacity + variance;
                    variable_sum_with_variation = this->carrying_capacity_with_variation + value;
                    if (variable_sum_with_variation <= maximum_value)
                    {
                        this->carrying_capacity_with_variation = variable_sum_with_variation;
                    }
                } // End limited

                // Unlimited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "UNLIMITED")
                {
                    this->carrying_capacity_with_variation += value;
                } // End unlimited
                break;
            }
            case 3: /*descending*/
            {
                // Limited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "LIMITED")
                {
                    minimum_value = this->carrying_capacity - variance;
                    variable_sum_with_variation = this->carrying_capacity_with_variation - value;
                    if (variable_sum_with_variation >= minimum_value)
                    {
                        this->carrying_capacity_with_variation = variable_sum_with_variation;
                    }
                } // End limited
                // Unlimited Variation
                if (Settings.fragments[index_fragment].Carrying_capacity_variation.type == "UNLIMITED")
                {
                    this->carrying_capacity_with_variation -= value;

                    if (this->carrying_capacity_with_variation <= 0)
                    {
                        this->carrying_capacity_with_variation = 0;
                    }
                } // End unlimited
                break;
            }
            default:
            {
                break;
            }
            } // End actions for each situation

            // Reset variation
            if (Settings.fragments[index_fragment].reset_values_after_variation == true)
            {
                if (Settings.current_time_step > Settings.fragments[index_fragment].Carrying_capacity_variation.growing.end_time_step && Settings.current_time_step > Settings.fragments[index_fragment].Carrying_capacity_variation.descending.end_time_step)
                {
                    this->carrying_capacity_with_variation = this->carrying_capacity;
                    Settings.fragments[index_fragment].Carrying_capacity_variation.type = "NONE";
                }
            }

        } // Variation verification
    }     // Specie Affected by carrying capacity variation check

    // Vary Verhulst Bias
    if (Settings.fragments[index_fragment].Verhulst_variation.type != "NONE")
    {
        descending = false;
        growing = false;
        cases_of_variation = 0;
        // Check wich variation category is activated
        if (Settings.current_time_step >= Settings.fragments[index_fragment].Verhulst_variation.growing.start_time_step && Settings.current_time_step <= Settings.fragments[index_fragment].Verhulst_variation.growing.end_time_step)
        {
            growing = true;
        }
        if (Settings.current_time_step >= Settings.fragments[index_fragment].Verhulst_variation.descending.start_time_step && Settings.current_time_step <= Settings.fragments[index_fragment].Verhulst_variation.descending.end_time_step)
        {
            descending = true;
        }
        // Situations
        if (growing == true && descending == true)
        {
            cases_of_variation = 1;
        }
        else if (growing == true && descending == false)
        {
            cases_of_variation = 2;
        }
        else if (growing == false && descending == true)
        {
            cases_of_variation = 3;
        }
        else
        {
            cases_of_variation = 0;
        }
        // Generate variation
        if (growing == true || descending == true)
        {
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, round(Settings.fragments[index_fragment].Verhulst_variation.variance));
            value = Settings._aleatory.number / 1000;
        }
        // Actions for each situation
        switch (cases_of_variation)
        {
        case 1: /*Random*/
        {
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
            int increase_or_decrease = round(Settings._aleatory.number);
            // Limited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "LIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->VERHULST_BIAS_VARIANT = Settings.ORIGINAL_VERHULST_BIAS + value;
                    if (this->VERHULST_BIAS_VARIANT >= 1)
                    {
                        this->VERHULST_BIAS_VARIANT = 1;
                    }
                }
                else
                {
                    this->VERHULST_BIAS_VARIANT = Settings.ORIGINAL_VERHULST_BIAS - value;
                    if (this->VERHULST_BIAS_VARIANT < Settings.ORIGINAL_VERHULST_BIAS)
                    {
                        this->VERHULST_BIAS_VARIANT = Settings.ORIGINAL_VERHULST_BIAS;
                    }
                }
            } // End limited
            // Unlimited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "UNLIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->VERHULST_BIAS_VARIANT += value;
                    if (this->VERHULST_BIAS_VARIANT >= 1)
                    {
                        this->VERHULST_BIAS_VARIANT = 1;
                    }
                }
                else
                {
                    this->VERHULST_BIAS_VARIANT -= value;

                    if (this->VERHULST_BIAS_VARIANT <= 0)
                    {
                        this->VERHULST_BIAS_VARIANT = 0;
                    }
                }
            } // End unlimited
            break;
        }
        case 2: /*growing*/
        {
            // Limited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "LIMITED")
            {
                variance = Settings.fragments[index_fragment].Verhulst_variation.variance / 1000;
                maximum_value = Settings.ORIGINAL_VERHULST_BIAS + variance;
                variable_sum_with_variation = this->VERHULST_BIAS_VARIANT + value;

                if (variable_sum_with_variation <= maximum_value)
                {
                    this->VERHULST_BIAS_VARIANT = variable_sum_with_variation;

                    if (this->VERHULST_BIAS_VARIANT >= 1)
                    {
                        this->VERHULST_BIAS_VARIANT = 1;
                    }
                }else
                {
                    this->VERHULST_BIAS_VARIANT = maximum_value;
                }
            } // End limited

            // Unlimited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "UNLIMITED")
            {
                this->VERHULST_BIAS_VARIANT += value;
                if (this->VERHULST_BIAS_VARIANT >= 1)
                {
                    this->VERHULST_BIAS_VARIANT = 1;
                }
            } // End unlimited
            break;
        }
        case 3: /*descending*/
        {
            // Limited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "LIMITED")
            {
                variance = Settings.fragments[index_fragment].Verhulst_variation.variance / 1000;
                minimum_value = Settings.ORIGINAL_VERHULST_BIAS - variance;
                variable_sum_with_variation = this->VERHULST_BIAS_VARIANT - value;
                if (variable_sum_with_variation >= minimum_value)
                {
                    this->VERHULST_BIAS_VARIANT = variable_sum_with_variation;
                }
                else
                {
                    this->VERHULST_BIAS_VARIANT = minimum_value;
                }
            } // End limited
            // Unlimited Variation
            if (Settings.fragments[index_fragment].Verhulst_variation.type == "UNLIMITED")
            {
                this->VERHULST_BIAS_VARIANT -= value;

                if (this->VERHULST_BIAS_VARIANT <= 0)
                {
                    this->VERHULST_BIAS_VARIANT = 0;
                }
            } // End unlimited
            break;
        }
        default:
        {
            break;
        }
        } // End actions for each situation

        // Reset variation
        if (Settings.fragments[index_fragment].reset_values_after_variation == true)
        {
            if (Settings.current_time_step > Settings.fragments[index_fragment].Verhulst_variation.growing.end_time_step && Settings.current_time_step > Settings.fragments[index_fragment].Verhulst_variation.descending.end_time_step)
            {
                this->VERHULST_BIAS_VARIANT = Settings.ORIGINAL_VERHULST_BIAS;
                Settings.fragments[index_fragment].Verhulst_variation.type = "NONE";
            }
        }

    } // Variation verification
}

void ENTITY::Population::Delete_empty_groups()
{
    int index_group = 0;
    while( index_group < this->groups.size())
    {
        bool empty_group = false;
        this->groups[index_group].abundance > 0 ? empty_group = false : empty_group = true;
        if (empty_group == true)
        {
            this->groups.erase(
                std::begin(this->groups) + index_group);
        }
        else
        {
            index_group++;
        }
    }
}

void ENTITY::Population::Split_groups()
{
    for (int index_group = 0; index_group < this->groups.size(); index_group++)
    {
        int Specie_identification = this->Specie_identification;
        // Group Max abundance
        if (this->groups[index_group].abundance >= Settings.SPECIES_LIST[Specie_identification].maximum_number_of_individuals_per_group)
        {
            // Split the group randomly in half
            do
            {
                if (Settings.last_group_identification == LLONG_MAX)
                {
                    Settings.last_group_identification = 0;
                }

                int group_identification = Settings.last_group_identification++;
                int origin_identification = this->groups[index_group].identification;

                // Empty group
                ENTITY::Group new_group(group_identification, origin_identification);

                // Randomize vectors
                // Males
                this->groups[index_group].Shuffle_vectors('M');
                // Females
                this->groups[index_group].Shuffle_vectors('F');

                // Copy Males vector
                int half_vector_male = int(this->groups[index_group].individuals.males.size() / 2);
                int half_vector_female = int(this->groups[index_group].individuals.females.size() / 2);
                
                new_group.individuals.males.reserve(half_vector_male);
                new_group.individuals.males.insert(std::begin(new_group.individuals.males),
                                                    this->groups[index_group].individuals.males.begin() + half_vector_male,
                                                    this->groups[index_group].individuals.males.end());
                // Copy Females vector
                new_group.individuals.females.reserve(half_vector_female);
                new_group.individuals.females.insert(std::begin(new_group.individuals.females),
                                                    this->groups[index_group].individuals.females.begin() + half_vector_female,
                                                    this->groups[index_group].individuals.females.end());

                // Delete in the original group the copied half
                // Males vector
                this->groups[index_group].individuals.males.erase(
                    this->groups[index_group].individuals.males.begin() + half_vector_male,
                    this->groups[index_group].individuals.males.end());
                // Females vector
                this->groups[index_group].individuals.females.erase(
                    this->groups[index_group].individuals.females.begin() + half_vector_female,
                    this->groups[index_group].individuals.females.end());

                // Store the new group
                this->groups.reserve(sizeof(new_group));
                this->groups.emplace_back(new_group);

                // Update group abundance
                this->groups[index_group].Update_abundance();
                // Break loop condition
                if (this->groups[index_group].abundance < Settings.SPECIES_LIST[Specie_identification].maximum_number_of_individuals_per_group)
                {
                    break;
                }

            } while (true);
            // End Split
        } // End Group abundance
    }
}

void ENTITY::Population::Sort_specimens()
{
   for (int index_group = 0; index_group < this->groups.size(); ++index_group)
    {
        this->groups[index_group].Sort_specimens(true,true);
    } 
}

ENTITY::Population::Population(int initial_density, int index_Specie_List, double current_area, bool start_population)
{

    this->Specie_identification = Settings.SPECIES_LIST[index_Specie_List].identification;
    this->Set_carrying_capacity(Settings.SPECIES_LIST[this->Specie_identification].individuals_per_square_kilometers, current_area);
    this->carrying_capacity_with_variation = this->carrying_capacity;
    this->abundance = 0;
    this->VERHULST_BIAS_VARIANT = Settings.ORIGINAL_VERHULST_BIAS;
    this->Set_verhulst();
    // Reserve Memory for tables
    this->data_table.reserve_memory(
        Settings.number_of_time_steps,
        Settings.CLEANING_DURATION,
        Settings.SPECIES_LIST[this->Specie_identification].maximum_age);
    // Start Inicial Population
    //if (start_population == true)
    //{
    //    this->Start_Population(initial_density, Settings.SPECIES_LIST[this->Specie_identification].maximum_number_of_individuals_per_group);
    //}
}

int ENTITY::Population::Random_breeding_group()
{
    
    std::vector<int> breeding_groups;
    for (int index = 0; index < this->groups.size(); ++index)
    {
        bool has_males_breeders = false;
        bool has_females_breeders = false;
        
        
        auto first_able_to_reproduce = [](auto specimen)
        {
            return specimen.able_to_mate == true;
        };
        //Verifies if have mature male
        auto specimen_pointer  = std::find_if(std::begin(this->groups[index].individuals.males),
                    std::end(this->groups[index].individuals.males),
                    first_able_to_reproduce);
        
        if(specimen_pointer != std::end(this->groups[index].individuals.males))
        {
            has_males_breeders = true;
        }
        //Verifies if have females able to reproduce
        specimen_pointer  = std::find_if(std::begin(this->groups[index].individuals.females),
                    std::end(this->groups[index].individuals.females),
                    first_able_to_reproduce);
        
        if(specimen_pointer != std::end(this->groups[index].individuals.females))
        {
            has_females_breeders = true;
        }

        //Store group index
        if (has_males_breeders == true && has_females_breeders == true)
        {
            breeding_groups.reserve(1);
            breeding_groups.emplace_back(index);
        }
    }
    // Dont have groups with reproductive specimens
    if (breeding_groups.size() == 0)
    {
        return -1;
    }

    Settings._aleatory.CREATE_RANDOM_NUMBERS(1, breeding_groups.size());
    return breeding_groups[(int)Settings._aleatory.number];
}

unsigned int ENTITY::Population::Calculate_reproduction_chances(int empty_spaces, int available_females, int specie_identification)
{
    double failure_rate = Settings.SPECIES_LIST[specie_identification].female_reproductive_failure;
    int attempts = (empty_spaces >= available_females) ? available_females : empty_spaces;
    int reproductions = 0;
    for (int chances = 0; chances < attempts; ++chances)
    {
        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
        if (Settings._aleatory.number > failure_rate)
        {
            reproductions++;
        }
    }
    return reproductions;
}

void ENTITY::Population::Update_groups()
{
    for (int index_group = 0; index_group < this->groups.size(); ++index_group)
    {
        this->groups[index_group].Update();
    }
}

void ENTITY::Population::Set_groups_targets()
{
    for (int index_group = 0; index_group < this->groups.size(); ++index_group)
    {
        this->groups[index_group].Set_target_groups(this->groups.size(), index_group);
    }
}

void ENTITY::Population::Update_abundance()
{
    this->abundance = 0;
    for (int index = 0; index < this->groups.size(); index++)
    {
        this->groups[index].Update_abundance();
        this->abundance += this->groups[index].abundance;
    }
}

void ENTITY::Population::Exodus()
{
    if (this->verhulst <= 0)
    {
        for (int index_group = 0; index_group < this->groups.size(); ++index_group)
        {
            // Females
            this->emigrants.elements.females.reserve(sizeof(this->groups[index_group].individuals.females));
            this->emigrants.elements.females.insert(std::end(this->emigrants.elements.females),
                                                    std::begin(this->groups[index_group].individuals.females),
                                                    std::end(this->groups[index_group].individuals.females));
            // Males
            this->emigrants.elements.males.reserve(sizeof(this->groups[index_group].individuals.males));
            this->emigrants.elements.males.insert(std::end(this->emigrants.elements.males),
                                                  std::begin(this->groups[index_group].individuals.males),
                                                  std::end(this->groups[index_group].individuals.males));
            // Delete all specimens
            this->groups[index_group].individuals.females.clear();
            this->groups[index_group].individuals.males.clear();
        }
    }
}

void ENTITY::Population::Update_Alpha()
{
    for (int index_group = 0; index_group < this->groups.size(); ++index_group)
    {
        this->groups[index_group].Update_alpha(this->Specie_identification);
    }
}

void ENTITY::Population::Update(int index_fragment)
{
    // Variation
    this->Apply_variations(index_fragment);
    // Upgrade groups
    this->Update_groups();
    // Delete empty groups
    this->Delete_empty_groups();
    // Split groups
    this->Split_groups();
    //Set groups targets
    this->Set_groups_targets();
    //Update alpha
    this->Update_Alpha();
    // Update abundance of population
    this->Update_abundance();
    //Sort vectors by specimens viability to reproduce
    this->Sort_specimens();
    // Count Available females in population
    this->Count_available_females();
    // Empty spaces
    int empty_spaces = this->carrying_capacity_with_variation - this->abundance;
    // Reproduction chances
    int breeders_quantity = this->Calculate_reproduction_chances(empty_spaces,this->available_females,this->Specie_identification);
    // Assign the reproductive chances
    this->reproductives_chances = breeders_quantity;
    // Select randomly females to reproduce
    this->Select_breeders(breeders_quantity);
    // Set Verhulst
    this->Set_verhulst();
}

void ENTITY::Population::Clear_groups()
{
    this->groups.clear();
}

void ENTITY::Population::Count_available_females()
{
    this->available_females = 0;
    for (auto &group : this->groups)
    {

        this->available_females += group.Count_available_females();
    }
}

int ENTITY::Population::Count_kinship_labels()
{
    std::sort(this->all_kinship_labels.begin(), this->all_kinship_labels.end());
    int unique_kinship_labels = std::unique(this->all_kinship_labels.begin(), this->all_kinship_labels.end()) - this->all_kinship_labels.begin();
    return unique_kinship_labels;
}

void ENTITY::Population::Store_data()
{
    int time_step = Settings.current_time_step;

    this->data_table.Alpha_Reign[time_step] += this->alpha_changes;
    this->data_table.Groups[time_step] += this->groups.size();
    this->data_table.Carrying_capacity[time_step] += this->carrying_capacity_with_variation;
    this->data_table.Senescence_Death[time_step] += this->death_by_senescence;
    this->data_table.Verhulst_Death[time_step] += this->death_by_Verhulst;
    this->data_table.Verhulst[time_step] += this->verhulst;
    this->data_table.V_Bias[time_step] += this->VERHULST_BIAS_VARIANT;
    this->data_table.Disperse[time_step] += this->dispersion_exits;
    this->data_table.Migration_exits[time_step] += this->migration_exits;
    this->data_table.Migration_entries[time_step] += this->migration_entries;
    this->data_table.Reproductives_Chances[time_step] += this->reproductives_chances;
    this->data_table.Birth[time_step] += this->births;
    this->data_table.kinship_labels[time_step] += this->Count_kinship_labels();
    
    //if(this->population_treshold > 0)
    //{
      //  this->data_table.Threshold[time_step] += (double)this->population_treshold / (double)(this->males_amount + this->females_amount);
    //}
    this->data_table.Threshold[time_step] += this->population_treshold;
    this->data_table.Males[time_step] += this->males_amount;
    this->data_table.Females[time_step] += this->females_amount;
    //this->data_table.Males[time_step] += (double)this->males_amount / (double)this->carrying_capacity_with_variation;
    //this->data_table.Females[time_step] += (double)this->females_amount / (double)this->carrying_capacity_with_variation;
    // Reset counter
    this->population_treshold = 0;
    this->males_amount = 0;
    this->females_amount = 0;
    this->alpha_changes = 0;
    this->death_by_senescence = 0;
    this->death_by_Verhulst = 0;
    this->dispersion_exits = 0;
    this->migration_exits = 0;
    this->migration_entries = 0;
    this->births = 0;
    this->all_kinship_labels.clear();
}
//--- End Class Population

//--- Struct Fragment
void ENTITY::Fragment::Apply_variations()
{
    // Variables
    double variance, value, variable_sum_with_variation, maximum_value, minimum_value;
    bool descending, growing;
    int cases_of_variation;

    // Vary the area
    if (this->Area_variation.type != "NONE")
    {
        descending = false;
        growing = false;
        cases_of_variation = 0;

        // Check wich variation category is activated
        if (Settings.current_time_step >= this->Area_variation.growing.start_time_step && Settings.current_time_step <= this->Area_variation.growing.end_time_step)
        {
            growing = true;
        }
        if (Settings.current_time_step >= this->Area_variation.descending.start_time_step && Settings.current_time_step <= this->Area_variation.descending.end_time_step)
        {
            descending = true;
        }

        // Situations
        if (growing == true && descending == true)
        {
            cases_of_variation = 1;
        }
        else if (growing == true && descending == false)
        {
            cases_of_variation = 2;
        }
        else if (growing == false && descending == true)
        {
            cases_of_variation = 3;
        }
        else
        {
            cases_of_variation = 0;
        }

        // Generate variation
        if (growing == true || descending == true)
        {
            variance = this->Area_variation.variance * this->original_area / 100;
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, round(variance));
            value = Settings._aleatory.number;
        }
        // Actions for each situation
        switch (cases_of_variation)
        {
        case 1: /*Random*/
        {
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
            int increase_or_decrease = round(Settings._aleatory.number);
            // Limited Variation
            if (this->Area_variation.type == "LIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->current_area = this->original_area + value;
                }
                else
                {
                    this->current_area = this->original_area - value;
                }
            } // End limited
            // Unlimited Variation
            if (this->Area_variation.type == "UNLIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->current_area += value;
                }
                else
                {
                    this->current_area -= value;

                    if (this->current_area <= 0)
                    {
                        this->current_area = 0;
                    }
                }
            } // End unlimited
            break;
        }
        case 2: /*growing*/
        {
            // Limited Variation
            if (this->Area_variation.type == "LIMITED")
            {
                maximum_value = this->original_area + variance;
                variable_sum_with_variation = this->current_area + value;
                if (variable_sum_with_variation <= maximum_value)
                {
                    this->current_area = variable_sum_with_variation;
                }
            } // End limited

            // Unlimited Variation
            if (this->Area_variation.type == "UNLIMITED")
            {
                this->current_area += value;
            } // End unlimited
            break;
        }
        case 3: /*descending*/
        {
            // Limited Variation
            if (this->Area_variation.type == "LIMITED")
            {
                minimum_value = this->original_area - variance;
                variable_sum_with_variation = this->current_area - value;
                if (variable_sum_with_variation >= minimum_value)
                {
                    this->current_area = variable_sum_with_variation;
                }
            } // End limited
            // Unlimited Variation
            if (this->Area_variation.type == "UNLIMITED")
            {
                this->current_area -= value;

                if (this->current_area <= 0)
                {
                    this->current_area = 0;
                }
            } // End unlimited
            break;
        }
        default:
        {
            break;
        }
        } // End actions for each situation

        // Reset variation
        if (this->reset_values_after_variation == true)
        {
            if (Settings.current_time_step > this->Area_variation.growing.end_time_step && Settings.current_time_step > this->Area_variation.descending.end_time_step)
            {
                this->current_area = this->original_area;
                this->Area_variation.type = "NONE";
            }
        }
    } // Variation verification

    // Vary the perimeter
    if (this->Perimeter_variation.type != "NONE")
    {
        descending = false;
        growing = false;
        cases_of_variation = 0;
        // Check wich variation category is activated
        if (Settings.current_time_step >= this->Perimeter_variation.growing.start_time_step && Settings.current_time_step <= this->Perimeter_variation.growing.end_time_step)
        {
            growing = true;
        }
        if (Settings.current_time_step >= this->Perimeter_variation.descending.start_time_step && Settings.current_time_step <= this->Perimeter_variation.descending.end_time_step)
        {
            descending = true;
        }
        // Situations
        if (growing == true && descending == true)
        {
            cases_of_variation = 1;
        }
        else if (growing == true && descending == false)
        {
            cases_of_variation = 2;
        }
        else if (growing == false && descending == true)
        {
            cases_of_variation = 3;
        }
        else
        {
            cases_of_variation = 0;
        }
        // Generate variation
        if (growing == true || descending == true)
        {
            variance = this->Perimeter_variation.variance * this->original_perimeter / 100;
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, round(variance));
            value = Settings._aleatory.number;
        }
        // Actions for each situation
        switch (cases_of_variation)
        {
        case 1: /*Random*/
        {
            Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
            int increase_or_decrease = round(Settings._aleatory.number);
            // Limited Variation
            if (this->Perimeter_variation.type == "LIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->current_perimeter = this->original_perimeter + value;
                }
                else
                {
                    this->current_perimeter = this->original_perimeter - value;
                }
            } // End limited
            // Unlimited Variation
            if (this->Perimeter_variation.type == "UNLIMITED")
            {
                if (increase_or_decrease == 0)
                {
                    this->current_perimeter += value;
                }
                else
                {
                    this->current_perimeter -= value;

                    if (this->current_area <= 0)
                    {
                        this->current_area = 0;
                    }
                }
            } // End unlimited
            break;
        }
        case 2: /*growing*/
        {
            // Limited Variation
            if (this->Perimeter_variation.type == "LIMITED")
            {
                maximum_value = this->original_perimeter + variance;
                variable_sum_with_variation = this->current_perimeter + value;
                if (variable_sum_with_variation <= maximum_value)
                {
                    this->current_perimeter = variable_sum_with_variation;
                }
            } // End limited

            // Unlimited Variation
            if (this->Perimeter_variation.type == "UNLIMITED")
            {
                this->current_perimeter += value;
            } // End unlimited
            break;
        }
        case 3: /*descending*/
        {
            // Limited Variation
            if (this->Perimeter_variation.type == "LIMITED")
            {
                minimum_value = this->original_perimeter - variance;
                variable_sum_with_variation = this->current_perimeter - value;
                if (variable_sum_with_variation >= minimum_value)
                {
                    this->current_perimeter = variable_sum_with_variation;
                }
            } // End limited
            // Unlimited Variation
            if (this->Perimeter_variation.type == "UNLIMITED")
            {
                this->current_perimeter -= value;

                if (this->current_area <= 0)
                {
                    this->current_area = 0;
                }
            } // End unlimited
            break;
        }
        default:
        {
            break;
        }
        } // End actions for each situation

        // Reset variation
        if (this->reset_values_after_variation == true)
        {
            if (Settings.current_time_step > this->Perimeter_variation.growing.end_time_step && Settings.current_time_step > this->Perimeter_variation.descending.end_time_step)
            {
                this->current_perimeter = this->original_perimeter;
                this->Perimeter_variation.type = "NONE";
            }
        }
    } // Variation verification
}

void ENTITY::Fragment::Populate_fragmented_areas()
{
    // Fragment was fragmented
    if (this->Fragmentation_variables.fragment_environment == true && this->Fragmentation_variables.fragmentation_time_step == Settings.current_time_step)
    {
        for (int index_population = 0; index_population < this->populations.size(); ++index_population)
        {
            int Specie_identification = this->populations[index_population].Specie_identification;

            // Loop untill has no specimens to migrate
            while (true)
            {
                // Random choice of a fragment
                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, this->new_fragments.size());
                int random_fragment = Settings._aleatory.number;
                // Random choice of a group
                if (this->new_fragments[random_fragment].populations[Specie_identification].groups.size() == 0)
                {
                    // Create an empty group for the random population
                    if (Settings.last_group_identification == LLONG_MAX)
                    {
                        Settings.last_group_identification = 0;
                    }
                    int group_identification = Settings.last_group_identification++;
                    ENTITY::Group new_group(group_identification, group_identification);
                    this->new_fragments[random_fragment].populations[Specie_identification].groups.reserve(sizeof(new_group));
                    this->new_fragments[random_fragment].populations[Specie_identification].groups.emplace_back(new_group);
                }
                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, this->new_fragments[random_fragment].populations[Specie_identification].groups.size());
                int random_group = Settings._aleatory.number;

                // Select the identification of the last group of the specimens
                int specimen_last_group;
                if (this->populations[index_population].emigrants.elements.males.size() > 0)
                {
                    specimen_last_group = this->populations[index_population].emigrants.elements.males[0].last_group_identification;
                }
                else if (this->populations[index_population].emigrants.elements.females.size() > 0)
                {
                    specimen_last_group = this->populations[index_population].emigrants.elements.females[0].last_group_identification;
                }
                else
                {
                    // Is empty you can not be in this loop anymore
                    break;
                }

                // Store elements from the same group
                ENTITY::Individuals specimens_from_same_group;
                for (ENTITY::Specimen &specimen : this->populations[index_population].emigrants.elements.males)
                {
                    if (specimen.last_group_identification == specimen_last_group)
                    {
                        specimens_from_same_group.males.reserve(sizeof(specimen));
                        specimens_from_same_group.males.emplace_back(specimen);
                        specimen.erase = true;
                    }
                }
                for (ENTITY::Specimen &specimen : this->populations[index_population].emigrants.elements.females)
                {
                    if (specimen.last_group_identification == specimen_last_group)
                    {
                        specimens_from_same_group.females.reserve(sizeof(specimen));
                        specimens_from_same_group.females.emplace_back(specimen);
                        specimen.erase = true;
                    }
                }

                // Copy the elements to the new fragment and group
                if (specimens_from_same_group.males.size() > 0)
                {
                    this->new_fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.males.insert(
                        this->new_fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.males.end(),
                        specimens_from_same_group.males.begin(),
                        specimens_from_same_group.males.end());
                }
                if (specimens_from_same_group.females.size() > 0)
                {
                    this->new_fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.females.insert(
                        this->new_fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.females.end(),
                        specimens_from_same_group.females.begin(),
                        specimens_from_same_group.females.end());
                }

                // Delete from the emigrants vector
                this->populations[index_population].emigrants.elements.males.erase(
                    std::remove_if(
                        this->populations[index_population].emigrants.elements.males.begin(),
                        this->populations[index_population].emigrants.elements.males.end(),
                        [](ENTITY::Specimen &specimen)
                        { return specimen.erase; }),
                    this->populations[index_population].emigrants.elements.males.end());
                this->populations[index_population].emigrants.elements.females.erase(
                    std::remove_if(
                        this->populations[index_population].emigrants.elements.females.begin(),
                        this->populations[index_population].emigrants.elements.females.end(),
                        [](ENTITY::Specimen &specimen)
                        { return specimen.erase; }),
                    this->populations[index_population].emigrants.elements.females.end());

                // Loop break condition
                int specimens_to_migrate = this->populations[index_population].emigrants.elements.males.size() +
                                           this->populations[index_population].emigrants.elements.females.size();
                // Is empty you can not be in this loop anymore
                if (specimens_to_migrate == 0)
                {
                    break;
                }
            } // Eternal loop
        }     // End loop population
    }         // End Migration is on check

    // Store the nenw fragments into to the Settings.fragments vector
    Settings.fragments.insert(Settings.fragments.end(), this->new_fragments.begin(), this->new_fragments.end());
    // Clear vector
    this->new_fragments.clear();
}

void ENTITY::Fragment::Fragmentation()
{
    int fragment_identification = Settings.fragments.size();
    // Fragmentation verification
    if (this->Fragmentation_variables.fragment_environment == true && this->Fragmentation_variables.fragmentation_time_step == Settings.current_time_step)
    {
        // Calculation of area & perimeter
        unsigned int number_of_fragmented_areas = this->Fragmentation_variables.number_of_fragmented_areas;
        double subareas_area = this->current_area / number_of_fragmented_areas;
        double subareas_perimeter = this->current_perimeter / number_of_fragmented_areas;

        // Creation of new fragments
        int creation = 0;
        while (creation < number_of_fragmented_areas)
        {
            // New Fragment
            ENTITY::Fragment new_fragment;

            new_fragment.identification = fragment_identification;
            new_fragment.original_area = subareas_area;
            new_fragment.original_perimeter = subareas_perimeter;
            new_fragment.current_area = subareas_area;
            new_fragment.current_perimeter = subareas_perimeter;
            new_fragment.initial_density = 0;
            new_fragment.Fragmentation_variables.source_fragment_identification = this->identification;
            new_fragment.reset_values_after_variation = this->reset_values_after_variation;

            // Fragment tables
            new_fragment.fragments_tables.reserve_memory(Settings.number_of_time_steps, Settings.CLEANING_DURATION);

            // New Populations
            int population_amount = this->populations.size();
            int index_population = 0;
            while (index_population < population_amount)
            {
                // Empty Population
                ENTITY::Population new_population(0, this->populations[index_population].Specie_identification, subareas_area, false);

                // Empty Group
                if (Settings.last_group_identification == LLONG_MAX)
                {
                    Settings.last_group_identification = 0;
                }
                int group_identification = Settings.last_group_identification++;
                ENTITY::Group new_group(group_identification, group_identification);
                new_population.groups.reserve(sizeof(new_group));
                new_population.groups.emplace_back(new_group);

                new_fragment.populations.reserve(sizeof(new_population));
                new_fragment.populations.emplace_back(new_population);

                index_population++;
            }

            this->new_fragments.reserve(sizeof(new_fragment));
            this->new_fragments.emplace_back(new_fragment);

            fragment_identification++;
            creation++;
        } // End Subareas creation

        // POPULATION - Fill emigrants vector with the entire population and clean the groups
        int index_population = 0;
        while (index_population < this->populations.size())
        {
            for (int index_group = 0; index_group < this->populations[index_population].groups.size(); index_group++)
            {
                // Females
                this->populations[index_population].emigrants.elements.females.insert(
                    this->populations[index_population].emigrants.elements.females.end(),
                    this->populations[index_population].groups[index_group].individuals.females.begin(),
                    this->populations[index_population].groups[index_group].individuals.females.end());
                // Males
                this->populations[index_population].emigrants.elements.males.insert(
                    this->populations[index_population].emigrants.elements.males.end(),
                    this->populations[index_population].groups[index_group].individuals.males.begin(),
                    this->populations[index_population].groups[index_group].individuals.males.end());
            }
            // Increase population migatrion exits counter
            this->populations[index_population].migration_exits += this->populations[index_population].abundance;
            // Clean Groups
            this->populations[index_population].Clear_groups();
            index_population++;
        } // End population loop

    } // End fragmentation verification
    // Populate the new fragments
    this->Populate_fragmented_areas();
}

void ENTITY::Fragment::Store_data()
{
    this->fragments_tables.Area[Settings.current_time_step] += this->current_area;
    this->fragments_tables.Perimeter[Settings.current_time_step] += this->current_perimeter;
}

void ENTITY::Fragment::Update()
{
    this->Apply_variations();
    this->Store_data();
    this->Fragmentation();
}
//--- End Fragment

//--- Progress bar
void TOOLS::PROGRESS_BAR::Update(int sample, int dataset, int total_sample, int total_datasets)
{
    double percentage = (double)sample * 100 / (double)total_sample;
    std::cout << " Dataset " << dataset << " of " << total_datasets << " [";
    int draw_char = 0;
    if (percentage < 20)
    {
        draw_char = 1;
    }
    else if (percentage < 30)
    {
        draw_char = 2;
    }
    else if (percentage < 40)
    {
        draw_char = 3;
    }
    else if (percentage < 50)
    {
        draw_char = 4;
    }
    else if (percentage < 60)
    {
        draw_char = 5;
    }
    else if (percentage < 70)
    {
        draw_char = 6;
    }
    else if (percentage < 80)
    {
        draw_char = 7;
    }
    else if (percentage < 90)
    {
        draw_char = 8;
    }
    else if (percentage < 100)
    {
        draw_char = 9;
    }
    else
    {
        draw_char = 10;
    }

    int i = 0;
    while (i < draw_char)
    {
        std::cout << "#";
        i++;
    }
    i = 0;
    while (i < this->progress_bar_lenght - draw_char)
    {
        std::cout << " ";
        i++;
    }
    std::cout << "] " << percentage << "% "
              << " \r";
    fflush(stdout);
}
//--- End Progress bar

MODEL_SETTINGS::GLOBAL_VARIABLES Settings;