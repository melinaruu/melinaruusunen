// Datastructures.cc
//
// Student name: Melina Ruusunen
// Student email: melina.ruusunen@tuni.fi
// Student number: 050259885

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <iostream>
#include <utility>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return all_towns_.size();
}

void Datastructures::clear_all()
{
    all_towns_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if (all_towns_.find(id) != all_towns_.end())
    {
        return false;
    }
    else
    {
        auto town = std::make_shared<Town>(id, name, coord, tax);
        all_towns_.insert({id, town});
        return true;
    }
}

Name Datastructures::get_town_name(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        return NO_NAME;
    }
    else
    {
        return iter->second->townName_;
    }
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        return NO_COORD;
    }
    else
    {
        return iter->second->townCoords_;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        return NO_VALUE;
    }
    else
    {
        return iter->second->townTax_;
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> towns;

    for (auto& town : all_towns_)
    {
        towns.push_back(town.first);
    }
    return towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> towns;

    for (auto& town : all_towns_)
    {
        if (name == town.second->townName_)
        {
            towns.push_back(town.first);
        }
    }
    return towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        return false;
    }
    else
    {
        iter->second->townName_ = newname;
        return true;
    }
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::multimap<Name, TownID> towns_map;
    std::vector<TownID> towns_vector;

    for (auto& town : all_towns_)
    {
        towns_map.insert({town.second->townName_, town.first});
    }
    for (auto& town : towns_map)
    {
        towns_vector.push_back(town.second);
    }

    return towns_vector;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::multimap<int, TownID> towns_map;
    std::vector<TownID> towns_vector;
    unsigned int distance = 0;

    for (auto& town : all_towns_)
    {
        distance=sqrt(pow(town.second->townCoords_.x, 2)
                      +pow(town.second->townCoords_.y, 2));
        towns_map.insert({distance, town.first});
    }
    for (auto& town : towns_map)
    {
        towns_vector.push_back(town.second);
    }
    return towns_vector;
}

TownID Datastructures::min_distance()
{
    unsigned int first_town = 0;
    unsigned int distance = 0;
    unsigned int towns_distance = 0;
    TownID min_dist_town;

    if (town_count() == 0)
    {
        return NO_TOWNID;
    }

    for (auto& town : all_towns_)
    {
        towns_distance=sqrt(pow(town.second->townCoords_.x, 2)
                            +pow(town.second->townCoords_.y, 2));

        if (first_town == 0)
        {
            first_town = 1;
            distance = towns_distance;
            min_dist_town=town.first;
        }
        else if (towns_distance < distance)
        {
            distance = towns_distance;
            min_dist_town=town.first;
        }
    }
    return min_dist_town;
}

TownID Datastructures::max_distance()
{
    unsigned int distance = 0;
    unsigned int towns_distance = 0;
    TownID max_dist_town;

    if (town_count() == 0)
    {
        return NO_TOWNID;
    }

    for (auto& town : all_towns_)
    {
        towns_distance=sqrt(pow(town.second->townCoords_.x, 2)
                            +pow(town.second->townCoords_.y, 2));

        if (towns_distance > distance)
        {
            distance = towns_distance;
            max_dist_town=town.first;
        }
    }
    return max_dist_town;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator vassaliter = all_towns_.find(vassalid);
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator masteriter = all_towns_.find(masterid);

    if (vassaliter == all_towns_.end() or masteriter == all_towns_.end() or
            vassaliter->second->mastertown_ != nullptr)
    {
        return false;
    }
    else
    {
        vassaliter->second->mastertown_ = masteriter->second;
        masteriter->second->vassaltowns_.push_back(vassaliter->second);
        return true;
    }
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> vassal_towns;
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        vassal_towns.push_back(NO_TOWNID);
        return vassal_towns;
    }
    else
    {
        for (auto& town : iter->second->vassaltowns_)
        {
            vassal_towns.push_back(town->townID_);
        }
        return vassal_towns;
    }
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxpath_towns;
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);

    if (iter == all_towns_.end())
    {
        taxpath_towns.push_back(NO_TOWNID);
        return taxpath_towns;
    }
    else
    {
        taxpath_towns.push_back(id);
        std::shared_ptr<Town> mastertown = iter->second->mastertown_;
        while (mastertown != nullptr)
        {
            taxpath_towns.push_back(mastertown->townID_);
            mastertown = mastertown->mastertown_;
        }
        return taxpath_towns;
    }
}

bool Datastructures::remove_town(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator delete_iter = all_towns_.find(id);

    if (delete_iter == all_towns_.end())
    {
        return false;
    }
    else
    {
        std::shared_ptr<Town> mastertown = delete_iter->second->mastertown_;

        if (mastertown != nullptr)
        {
            for (auto& vassaltown : delete_iter->second->vassaltowns_)
            {
                mastertown->vassaltowns_.push_back(vassaltown);
                vassaltown->mastertown_ = mastertown;
            }

            std::vector<std::shared_ptr<Town>> new_vassaltowns_;

            for (auto& town : mastertown->vassaltowns_)
            {
                if (town->townID_ != delete_iter->first)
                {
                    new_vassaltowns_.push_back(town);
                }
            }

            mastertown->vassaltowns_ = new_vassaltowns_;
        }
        else
        {
            for (auto& vassaltown : delete_iter->second->vassaltowns_)
            {
                vassaltown->mastertown_ = nullptr;
            }
        }
        all_towns_.erase(id);
        return true;
    }
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::multimap<int, TownID> towns_map;
    std::vector<TownID> towns_vector;
    unsigned int distance = 0;

    for (auto& town : all_towns_)
    {
        distance=sqrt(pow(town.second->townCoords_.x-coord.x, 2)
                      +pow(town.second->townCoords_.y-coord.y, 2));
        towns_map.insert({distance, town.first});
    }
    for (auto& town : towns_map)
    {
        towns_vector.push_back(town.second);
    }
    return towns_vector;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);
    int total_tax = iter->second->townTax_;

    if (iter == all_towns_.end())
    {
        return NO_VALUE;
    }
    else
    {
        std::shared_ptr<Town> mastertown = iter->second->mastertown_;

        for (auto& town : iter->second->vassaltowns_)
        {
            total_tax += count_tax(town->townID_);
        }

        if (mastertown != nullptr)
        {
            total_tax -= total_tax*0.1;
            return total_tax;
        }
        else
        {
            return total_tax;
        }
    }
}

int Datastructures::count_tax(TownID id)
{
    std::unordered_map<TownID, std::shared_ptr<Town>>::iterator iter = all_towns_.find(id);
    int tax = iter->second->townTax_;

    for (auto& town : iter->second->vassaltowns_)
    {
        tax += 0.1*(town->townTax_+count_tax(town->townID_));

    }
    return 0.1*tax;
}
