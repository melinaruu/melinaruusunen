// Datastructures.hh
//
// Student name: Melina Ruusunen
// Student email: melina.ruusunen@tuni.fi
// Student number: 050259885

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <memory>
#include <cmath>


// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};



class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: time complexity of size()
    unsigned int town_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: time complexity of clear()
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: time complexity of inserting one element to
    // an unordered_map
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: average O(1), worst case O(n)
    // Short rationale for estimate: time complexity of find() for unordered_map
    Name get_town_name(TownID id);

    // Estimate of performance: average O(1), worst case O(n)
    // Short rationale for estimate: time complexity of find() for unordered_map
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: average O(1), worst case O(n)
    // Short rationale for estimate: time complexity of find() for unordered_map
    int get_town_tax(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(n) for for loop and O(1) for push_back()
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(n) for for loop and O(1) for push_back()
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: average O(1), worst case O(n)
    // Short rationale for estimate: time complexity of find() for unordered_map
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: 2*O(n) for for loops = O(n) and O(log(n))
    // for inserting one element to multimap. O(1) for push_back()
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: 2*O(n) for for loops = O(n) and O(log(n))
    // for inserting one element to multimap. O(1) for push_back()
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: time complexity of one for loop
    TownID min_distance();

    // Estimate of performance: O(n)
    // Short rationale for estimate: time complexity of one for loop
    TownID max_distance();

    // Estimate of performance: average O(1), worst case O(n)
    // Short rationale for estimate: time complexity of find() for unordered_map
    // O(1) for push_back()
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: time complexity of one for loop. Time
    // complexity for find() doesn't have an effect because in the worst case
    // 2*O(n)=O(n). O(1) for push_back()
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(n) for going through the mastertowns.
    // Time complexity for find() doesn't have an effect because in the worst
    // case 2*O(n)=O(n). O(1) for push_back()
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: none of the for loops are within each other
    // and time complexity for erase() is O(n) in the worst case, so total worst
    // case is 4*O(n)=O(n)
    bool remove_town(TownID id);

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: 2*O(n) for for loops = O(n) and O(log(n))
    // for inserting one element to multimap. O(1) for push_back()
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: looping through the town's vassaltowns,
    // time complexity of find() doesn't have an effect
    int total_net_tax(TownID id);

private:

    struct Town
    {
        TownID townID_;
        Name townName_;
        Coord townCoords_;
        int townTax_;

        std::vector<std::shared_ptr<Town>> vassaltowns_;

        std::shared_ptr<Town> mastertown_;

        Town(TownID id, Name name, Coord coords, int tax)
        {
            townID_ = id;
            townName_ = name;
            townCoords_ = coords;
            townTax_ = tax;
            mastertown_ = nullptr;
        }
    };

    std::unordered_map<TownID, std::shared_ptr<Town>> all_towns_;

    int count_tax(TownID id);

};

#endif // DATASTRUCTURES_HH
