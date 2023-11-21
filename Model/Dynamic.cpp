// includes
#include "Include/Dynamic.hpp"

//--- Function POPULATE_FRAGMENTS ---
void ENVIRONMENTs::POPULATE_FRAGMENTS()
{
    int index_fragment = 0;
    // fragments
    while (index_fragment < Settings.fragments.size())
    {
        for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); index_population++)
        {
            Settings.fragments[index_fragment].populations[index_population].Start_Population(Settings.fragments[index_fragment].initial_density,
                                                                                              Settings.SPECIES_LIST[Settings.fragments[index_fragment].populations[index_population].Specie_identification].maximum_number_of_individuals_per_group);
        }
        index_fragment++;
    } // End fragments
}
//--- End Function POPULATE_FRAGMENTS ---

//--- ACT ---
void SPECIMENs::ACT(int index_fragment, int index_population, int index_group)
{
    // Loop males
    for (int specimen = 0; specimen < Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males.size(); specimen++)
    {
        Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males[specimen].Act(index_fragment, index_population, index_group, specimen);
    }
    // Sort males by variable able to mate
    Settings.fragments[index_fragment].populations[index_population].groups[index_group].Sort_specimens(true, false);
    // apagar
    // std::cout<<"\n****G: "<<index_group;
    // for(auto m : Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.males)
    //{
    //     std::cout<<"\nID: "<<m.identification<<", idade: "<<m.current_age<<", able_To_Mate: "<<m.able_to_mate<<", is_alpha: "<<m.is_alpha<<", TRUE("<<true<<")";
    // }
    // apagar
    // Specie ID
    int Specie_identification = Settings.fragments[index_fragment].populations[index_population].Specie_identification;
    // Update Alpha
    // if (Settings.SPECIES_LIST[Specie_identification].has_alpha == true)
    //{
    //   Settings.fragments[index_fragment].populations[index_population].groups[index_group].Update_alpha(Specie_identification);
    // }
    // Loop Females
    for (int specimen = 0; specimen < Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females.size(); specimen++)
    {
        Settings.fragments[index_fragment].populations[index_population].groups[index_group].individuals.females[specimen].Act(index_fragment, index_population, index_group, specimen);
    }
}
//--- END ACT ---

//--- Function CLEANING ---
void ENVIRONMENTs::CLEANING()
{
    if (Settings.CLEANING == true)
    {
        int cleaning_step = 0;
        bool original_disperse = Settings.DISPERSE;
        bool original_migrate = Settings.MIGRATE;

        Settings.DISPERSE = false;
        Settings.MIGRATE = false;

        while (cleaning_step < Settings.CLEANING_DURATION)
        {
            Settings.current_time_step = cleaning_step;
            // Loop through the entire environment
            for (int index_fragment = 0; index_fragment < Settings.fragments.size(); ++index_fragment)
            {
                // Update fragment
                Settings.fragments[index_fragment].Update();
                for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); ++index_population)
                {
                    // Update Environment
                    Settings.fragments[index_fragment].populations[index_population].Update(index_fragment);

                    for (int index_group = 0; index_group < Settings.fragments[index_fragment].populations[index_population].groups.size(); ++index_group)
                    {
                        // Act
                        SPECIMENs::ACT(index_fragment, index_population, index_group); //-> Senescence, Reproduce, Verhulst and Disperse
                    }                                                                  // Loop groups

                    // Exodus -> fragment no longer viable. All specimens migrate
                    Settings.fragments[index_fragment].populations[index_population].Exodus();
                    // Population data store
                    Settings.fragments[index_fragment].populations[index_population].Store_data();
                } // Loop population
            }     // Loop fragments
            cleaning_step++;
        } // Loop cleaning steps

        Settings.DISPERSE = original_disperse;
        Settings.MIGRATE = original_migrate;
    }
}
//--- End Function CLEANING ---

//--- Function MIGRATE ---
void ENVIRONMENTs::MIGRATE()
{
    // Migration is on
    if (Settings.MIGRATE == true)
    {
        if (Settings.fragments.size() >= 2)
        {
            for (int index_fragment = 0; index_fragment < Settings.fragments.size(); ++index_fragment)
            {
                for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); ++index_population)
                {
                    int Specie_identification = Settings.fragments[index_fragment].populations[index_population].Specie_identification;

                    // List of target fragments to migrate to
                    std::vector<int> target_fragments(Settings.fragments.size());
                    // Fill list with all the fragment indexs
                    std::iota(std::begin(target_fragments), std::end(target_fragments), 0);
                    // Remove the current index
                    target_fragments.erase(std::remove(target_fragments.begin(), target_fragments.end(), index_fragment), target_fragments.end());

                    // Loop untill has no specimens to migrate
                    while (true)
                    {
                        // Random choice of a fragment
                        auto rd = std::random_device{};
                        auto rng = std::default_random_engine{rd()};
                        std::shuffle(std::begin(target_fragments), std::end(target_fragments), rng);
                        int random_fragment = target_fragments[0];

                        // Random choice of a group
                        if (Settings.fragments[random_fragment].populations[Specie_identification].groups.size() == 0)
                        {
                            // Create an empty group for the random population
                            if (Settings.last_group_identification == LLONG_MAX)
                            {
                                Settings.last_group_identification = 0;
                            }
                            int group_identification = Settings.last_group_identification++;
                            ENTITY::Group new_group(group_identification, group_identification);
                            Settings.fragments[random_fragment].populations[Specie_identification].groups.reserve(sizeof(new_group));
                            Settings.fragments[random_fragment].populations[Specie_identification].groups.emplace_back(new_group);
                        }
                        Settings._aleatory.CREATE_RANDOM_NUMBERS(1, Settings.fragments[random_fragment].populations[Specie_identification].groups.size());
                        int random_group = Settings._aleatory.number;

                        // Select the identification of the last group of the specimens
                        int specimen_last_group;
                        if (Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.size() > 0)
                        {
                            specimen_last_group = Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males[0].last_group_identification;
                        }
                        else if (Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.size() > 0)
                        {
                            specimen_last_group = Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females[0].last_group_identification;
                        }
                        else
                        {
                            // Is empty you can not be in this loop anymore
                            break;
                        }
                        // Crossing cost variables
                        double managed_to_cross = 0;
                        double crossing_cost_value = std::stod(Settings.crossing_cost_matrix[index_fragment][random_fragment]);
                        // Store elements from the same group
                        ENTITY::Individuals specimens_from_same_group;
                        for (ENTITY::Specimen &specimen : Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males)
                        {
                            if (specimen.last_group_identification == specimen_last_group)
                            {
                                // Crossing cost verification
                                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                                managed_to_cross = Settings._aleatory.number;
                                if (managed_to_cross < crossing_cost_value)
                                {
                                    specimens_from_same_group.males.reserve(sizeof(specimen));
                                    specimens_from_same_group.males.emplace_back(specimen);
                                }
                                specimen.erase = true;
                            }
                        }
                        for (ENTITY::Specimen &specimen : Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females)
                        {
                            if (specimen.last_group_identification == specimen_last_group)
                            {
                                // Crossing cost verification
                                Settings._aleatory.CREATE_RANDOM_NUMBERS(1, 1);
                                managed_to_cross = Settings._aleatory.number;
                                if (managed_to_cross < crossing_cost_value)
                                {
                                    specimens_from_same_group.females.reserve(sizeof(specimen));
                                    specimens_from_same_group.females.emplace_back(specimen);
                                }
                                specimen.erase = true;
                            }
                        }

                        // Copy the elements to the new fragment and group
                        if (specimens_from_same_group.males.size() > 0)
                        {
                            Settings.fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.males.insert(
                                Settings.fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.males.end(),
                                specimens_from_same_group.males.begin(),
                                specimens_from_same_group.males.end());
                        }
                        if (specimens_from_same_group.females.size() > 0)
                        {
                            Settings.fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.females.insert(
                                Settings.fragments[random_fragment].populations[Specie_identification].groups[random_group].individuals.females.end(),
                                specimens_from_same_group.females.begin(),
                                specimens_from_same_group.females.end());
                        }
                        // Migration entries counter increase
                        Settings.fragments[random_fragment].populations[Specie_identification].migration_entries += specimens_from_same_group.males.size() + specimens_from_same_group.females.size();
                        // Delete from the emigrants vector
                        Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.erase(
                            std::remove_if(
                                Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.begin(),
                                Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.end(),
                                [](ENTITY::Specimen &specimen)
                                { return specimen.erase; }),
                            Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.end());
                        Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.erase(
                            std::remove_if(
                                Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.begin(),
                                Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.end(),
                                [](ENTITY::Specimen &specimen)
                                { return specimen.erase; }),
                            Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.end());

                        // Loop break condition
                        int specimens_to_migrate = Settings.fragments[index_fragment].populations[index_population].emigrants.elements.males.size() +
                                                   Settings.fragments[index_fragment].populations[index_population].emigrants.elements.females.size();
                        // Is empty you can not be in this loop anymore
                        if (specimens_to_migrate == 0)
                        {
                            break;
                        }
                    } // Eternal loop
                }     // End loop population
            }         // End loop fragments
        }             // More than 2 fragments condition
    }                 // End Migration is on check
}
//-- End Function MIGRATE ---

//-- RESET_DATA ---
void ENVIRONMENTs::RESET_DATA()
{
    // Reset Settings
    Settings.current_time_step = 0;
    Settings.last_group_identification = 0;
    for (auto specimen : Settings.SPECIES_LIST)
    {
        specimen.last_specimen_identification = 0;
        specimen.last_kinship_label = 0;
    }

    // Reset populations groups
    int index_fragment = 0;
    while (index_fragment < Settings.fragments.size())
    {
        for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); ++index_population)
        {
            Settings.fragments[index_fragment].populations[index_population].Clear_groups();
        }
        index_fragment++;
    }
}
//-- End RESET_DATA ---

//--- Function PASS_TIME ---
void ENVIRONMENTs::PASS_TIME()
{
    int sample_duration = Settings.number_of_time_steps + Settings.CLEANING_DURATION;
    int time_step = Settings.CLEANING_DURATION;
    while (time_step < sample_duration)
    {
        long long int specimens_amount = 0;
        // std::cout<<"\nTimeStep: "<<time_step;
        Settings.current_time_step = time_step;
        // Loop through the entire environment
        for (int index_fragment = 0; index_fragment < Settings.fragments.size(); index_fragment++)
        {
            // Update fragment
            Settings.fragments[index_fragment].Update();
            for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); index_population++)
            {
                // Update Environment
                Settings.fragments[index_fragment].populations[index_population].Update(index_fragment);
                // Loop groups
                for (int index_group = 0; index_group < Settings.fragments[index_fragment].populations[index_population].groups.size(); index_group++)
                {
                    // Act
                    SPECIMENs::ACT(index_fragment, index_population, index_group); //-> Senescence, Reproduce, Verhulst and Disperse
                }                                                                  // Loop groups

                // Exodus -> fragment no longer viable. All specimens migrate
                Settings.fragments[index_fragment].populations[index_population].Exodus();
                // Population data store
                Settings.fragments[index_fragment].populations[index_population].Store_data();

                specimens_amount += Settings.fragments[index_fragment].populations[index_population].abundance;
            } // Loop population
        }     // Loop fragments

        ENVIRONMENTs::MIGRATE(); // Relocate emigrants

        if (specimens_amount == 0)
        {
            time_step = sample_duration;
        }
        time_step++;

    } // Loop Time steps
}
//--- End Function PASS_TIME ---

//--- Function START_DYNAMICS ---
void ENVIRONMENTs::START_DYNAMICS()
{
    int dataset = 1;
    while (dataset <= Settings.datasets)
    {
        Settings.current_dataset = dataset;

        int sample = 1;
        while (sample <= Settings.sample_quantity)
        {
            // std::cout<<"\nSample: "<<sample;
            Settings.Progress_bar.Update(sample, Settings.current_dataset, Settings.sample_quantity, Settings.datasets);
            try
            {
                // Populate fragments
                ENVIRONMENTs::POPULATE_FRAGMENTS();
            }
            catch (const std::exception &e)
            {
                std::cout << "\n Error!: POPULATE_FRAGMENTS() | Dynamic.cpp " << '\n';
                std::cout << "Excepetion: " << e.what() << '\n';
            }

            try
            {
                // Cleaning
                ENVIRONMENTs::CLEANING();
            }
            catch (const std::exception &e)
            {
                std::cout << "\n Error!: CLEANING() | Dynamic.cpp " << '\n';
                std::cout << "Excepetion: " << e.what() << '\n';
            }

            try
            {
                // Pass time
                ENVIRONMENTs::PASS_TIME();
            }
            catch (const std::exception &e)
            {
                std::cout << "\n Error!: PASS_TIME() | Dynamic.cpp " << '\n';
                std::cout << "Excepetion: " << e.what() << '\n';
            }

            try
            {
                ENVIRONMENTs::RESET_DATA();
            }
            catch (const std::exception &e)
            {
                std::cout << "\n Error!: RESET_DATA() | Dynamic.cpp " << '\n';
                std::cout << "Excepetion: " << e.what() << '\n';
            }
            sample++;
        } // End sample loop

        try
        {
            OUTPUT_DATA::OUTPUT();
        }
        catch (const std::exception &e)
        {
            std::cout << "\n Error!: OUTPUT() | Dynamic.cpp " << '\n';
            std::cout << "Excepetion: " << e.what() << '\n';
        }

        // Reset populations Tables
        int index_fragment = 0;
        while (index_fragment < Settings.fragments.size())
        {
            Settings.fragments[index_fragment].fragments_tables.reset();
            for (int index_population = 0; index_population < Settings.fragments[index_fragment].populations.size(); ++index_population)
            {
                Settings.fragments[index_fragment].populations[index_population].data_table.reset(
                    Settings.SPECIES_LIST[Settings.fragments[index_fragment].populations[index_population].Specie_identification].maximum_age);
            }
            index_fragment++;
        }

        dataset++;
    }
}
//--- End START_DYNAMICS ---