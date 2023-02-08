/*
Ohjelman kirjoittaja
Nimi: Melina Ruusunen
Opiskelijanumero: 50259885
Käyttäjätunnus: bpmeru
E-Mail: melina.ruusunen@tuni.fi

*/

#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

bool CarePeriod::is_active() const
{
    //Jos alku päivää ei ole asetettu niin hoitojakso on aktiivinen
    if (end_ == Date())
    {
         return true;
    }
    else
    {
        return false;
    }
}

std::string CarePeriod::get_patient_id() const
{
    std::string patient_id = patient_->get_id();
    return patient_id;
}

void CarePeriod::assign_staff(Person *staff)
{
    //annettu henkilö lisätään hoitojakson hoitohenkilökunta mappiin
    assigned_staff_.insert({staff->get_id(),staff});
}

void CarePeriod::set_end(Date &end_date)
{
     end_ = end_date;
}

bool CarePeriod::has_staff_member(std::string name) const
{
    for(auto person : assigned_staff_)
    {
        if(person.second->get_id() == name)
        {
            return true;
        }
    }
    return false;
}

void CarePeriod::print_care_periods() const
{
    Date start = start_;
    Date end = end_;

    start.print();
    std::cout << " - ";
    // lopetuspäivä tulostetaan vain jos hoitojakso ei ole käynnissä
    if (is_active()==false)
    {
         end_.print();
    }
    std::cout << std::endl;
    std::cout << "* Patient: "<< patient_->get_id() << std::endl;
}

void CarePeriod::print_patient_info() const
{
    Date start = start_;
    Date end = end_;
    std::cout << "* Care period: ";
    start.print();
    std::cout << " - ";
    // lopetuspäivä tulostetaan vain jos hoitojakso ei ole käynnissä
    if (is_active()==false)
    {
         end_.print();
    }
    std::cout << std::endl;
    std::cout << "  - Staff:";
    std::vector<std::string> staff;
    for(auto person : assigned_staff_)
    {
        if(has_staff_member(person.second->get_id()))
        {
            staff.push_back(person.second->get_id());
        }
    }
    //jos vektoriin ei ole lisätty yhtään henkilökuunnan jäsentä tulee "none"
    if(staff.size() == 0)
    {
        std::cout << " None";
    }
    //muuten tulostetaan henkilökunnan jäsenten nimet
    for(auto name : staff)
    {
        std::cout << " " << name;
    }
    std::cout << std::endl;
}
