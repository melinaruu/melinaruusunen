/*
Ohjelman kirjoittaja
Nimi: Melina Ruusunen
Opiskelijanumero: 50259885
Käyttäjätunnus: bpmeru
E-Mail: melina.ruusunen@tuni.fi

*/

#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>
#include <algorithm>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Deallocating patients
    for( std::map<std::string, Person*>::iterator
         iter = alltime_patients_.begin();
         iter != alltime_patients_.end();
         ++iter )
    {
        delete iter->second;
    }
}

void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::enter(Params params)
{
    std::string patient_id = params.at(0);
    // jos lisättävä potilas löytyy jo sen hetkisistä potilaista, tulee ilmoitus
    if( current_patients_.find(patient_id) != current_patients_.end() )
    {
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }
    //tarkastaa onko potilas ollut sairaalassa jo aiemmin
    else if ( alltime_patients_.find(patient_id) != alltime_patients_.end() )
    {
        std::cout << PATIENT_ENTERED << std::endl;

        // luo uuden hoitojakson olemassa olevalle potilaalle
        std::map<std::string, Person*>::const_iterator
                patient_iter = alltime_patients_.find(patient_id);
        CarePeriod* new_careperiod = new CarePeriod(utils::today,
                                                    patient_iter->second);
        all_careperiods_.push_back(new_careperiod);

        // lisää vanhan potilas-alkion nykyisiin potilaisiin
        current_patients_.insert({patient_id,patient_iter->second});
    }
    else
    {
        //luo uuden person alkion
        Person* new_patient = new Person(patient_id);

        //lisätään potilas molempiin mappeihin
        current_patients_.insert({patient_id, new_patient});
        alltime_patients_.insert({patient_id, new_patient});

        std::cout << PATIENT_ENTERED << std::endl;

        //luo uuden hoitojakson ja lisää sen hoitojakso vektoriin
        CarePeriod* new_careperiod = new CarePeriod(utils::today, new_patient);
        all_careperiods_.push_back(new_careperiod);
    }
}

void Hospital::leave(Params params)
{
    std::string leaving_patient_id = params.at(0);
    //tarkastaa onko lähtevää potilasta tämän hetkisissä potilaissa
    if( current_patients_.find(leaving_patient_id) == current_patients_.end() )
    {
        std::cout << CANT_FIND << leaving_patient_id << std::endl;
        return;
    }
    // poistetaan potilas tämän hetkisistä potilaista
    current_patients_.erase(leaving_patient_id);
    std::cout << PATIENT_LEFT << std::endl;

    //lopetetaan hoitojakso
    for( auto careperiod : all_careperiods_ )
    {
        if(careperiod->get_patient_id() == leaving_patient_id)
        {
            careperiod->set_end(utils::today);
        }
    }
}

void Hospital::assign_staff(Params params)
{
    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);

    //tarkastetaan löytyykö annetut nimet säiliöistä
    if( staff_.find(staff_id) == staff_.end() )
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }
    if( current_patients_.find(patient_id) == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // käydään läpi kaikki hoitojaksot
    for( auto careperiod : all_careperiods_)
    {
        //jos hoitojakson potilas on sama kuin annetun potilaan nimi ja
        //hoitojakso on aktiivinen niin lisätään henkilökunnanjäsen hoitojakson
        //hoitohenkilökuntaan
        if(careperiod->get_patient_id() == patient_id
                and careperiod->is_active())
        {
            careperiod->assign_staff(staff_.at(staff_id));
        }
    }

    std::cout << STAFF_ASSIGNED << patient_id << std::endl;
}

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;

    //jos lääke vektorissa ei vielä ole lisättävän lääkkeen nimistä alkiota
    //sellainen lisätään
    if( std::find(all_medicines_.begin(), all_medicines_.end(), medicine)
            == all_medicines_.end() )
    {
        all_medicines_.push_back(medicine);
    }
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);

    //tarkastetaan löytyykö annettua potilasta
    if( alltime_patients_.find(patient_id) == alltime_patients_.end() )
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    //käydään läpi hoitojaksoja
    for(auto careperiod : all_careperiods_)
    {
        //jos hoitojakson potilaan nimi on sama kuin annettu, tulostetaan
        //hoitojakson tiedot
        if(careperiod->get_patient_id() == patient_id)
        {
            careperiod->print_patient_info();
        }
    }

    //tulostetaan annetun henkilön lääkkeet
    std::map<std::string, Person*>::const_iterator
            patient_iter = alltime_patients_.find(patient_id);

    std::cout << "* Medicines:";
    patient_iter->second->print_medicines("  - ");

}

void Hospital::print_care_periods_per_staff(Params params)
{
    std::string staff_id = params.at(0);

    //tarkstetaan löytyykö annettu henkilökunnan jäsen
    if( staff_.find(staff_id) == staff_.end() )
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    // luodaan muuuttuja jonka avulla voidaan seurata onko mitään tulostettu
    bool careperiod_found = false;

    //käydään läpi hoitojaksoja
    for( auto careperiod : all_careperiods_)
    {
        //jos annettu henkilökunnanjäsen on hoitojakson henkilökuntaa niin
        //tulostetaan hoitojakso
        if(careperiod->has_staff_member(staff_id))
        {
            careperiod->print_care_periods();
            careperiod_found=true;
        }
    }

    //jos yhtään hoitojaksoa ei ole tulostettu, tulostetaan none
    if(careperiod_found == false)
    {
        std::cout << "None" << std::endl;
    }
}

void Hospital::print_all_medicines(Params)
{
    //luodaan muuttuja jonka avulla seurataan onko jotain tulostettu
    bool medicine_found = false;

    //käydään lääkkeitä läpi
    for(auto medicine : all_medicines_)
    {
        std::vector<std::string> patients_to_print;

        //käydään kaikki potilaat läpi
        for(auto patient : alltime_patients_)
        {
            //haetaan potilaan sen hetkiset lääkkeet
            std::vector<std::string> meds = patient.second->get_medicines();
            //järjestetään lääkkeet aakkosjärjestykseen
            sort(meds.begin(), meds.end());

            //jos potilaan lääkevektorista löytyy kyseisen lääkkeen nimi,
            //lisätään potilaan nimi vektoriin
            if( std::find(meds.begin(), meds.end(), medicine) != meds.end() )
            {
                patients_to_print.push_back(patient.first);
            }
            else
            {
                continue;
            }
        }

        //jos kyseinen lääke on löytynyt joltain potilaalta, vektorin koko on
        //suurempi kuin nolla ja potilaat tulostetaan
        if(patients_to_print.size()!=0)
        {
            std::cout << medicine << " prescribed for" << std::endl;
            for(auto patient : patients_to_print)
            {
                std::cout << "* " << patient << std::endl;
                medicine_found = true;
            }
        }
    }

    //jos lääkkeitä ei löytynyt keltään, tulostetaan none
    if(medicine_found == false)
    {
        std::cout << "None" << std::endl;
    }
}

void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::print_all_patients(Params)
{
    //jos ei ole ollut yhtään potilasta
    if( alltime_patients_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    //käydään läpi kokoajan potilaita
    for( std::map<std::string, Person*>::const_iterator iter
         = alltime_patients_.begin(); iter != alltime_patients_.end(); ++iter )
    {
        //tulostetaan potilaan nimi
        std::cout << iter->first << std::endl;

        //käydään läpi hoitojaksoja ja tulostetaan ne joissa potilaan nimet ovat
        //samat
        for( auto careperiod : all_careperiods_ )
        {
            if(careperiod->get_patient_id()==iter->first)
            {
                careperiod->print_patient_info();
            }
        }

        //tulostetaan hoitojaksojen perään potilaan lääkkeet
        std::cout << "* Medicines:";
        iter->second->print_medicines("  - ");
    }
}

void Hospital::print_current_patients(Params)
{
    //jos ei ole tämänhetkisiä potilaita
    if( current_patients_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    //käydään läpi potilaita
    for( std::map<std::string, Person*>::const_iterator iter
         = current_patients_.begin();iter != current_patients_.end();++iter )
    {
        //käydään läpi hoitojaksoja
        for(auto careperiod : all_careperiods_)
        {
            //jos potilaiden nimet ovat sanat ja kyseinen hoitojakso on
            //aktiivinen, nimi ja hoitojakso tulostetaan
            if(iter->first == careperiod->get_patient_id()
                    and careperiod->is_active())
            {
                std::cout << iter->first << std::endl;
                careperiod->print_patient_info();

                //hoitojakson perään tulostetaan lääkkeet
                std::map<std::string, Person*>::const_iterator
                        patient_iter = current_patients_.find(iter->first);

                std::cout << "* Medicines:";
                patient_iter->second->print_medicines("  - ");
            }
        }
    }
}

void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}
