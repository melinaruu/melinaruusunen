/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a patient's care period in hospital.
 *
Ohjelman kirjoittaja
Nimi: Melina Ruusunen
Opiskelijanumero: 50259885
Käyttäjätunnus: bpmeru
E-Mail: melina.ruusunen@tuni.fi
*/
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>

class CarePeriod
{
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    // Palauttaa arvon true jos hoitojakso on käynnissä
    bool is_active() const;

    // Palauttaa hoitojakson potilaan nimen
    std::string get_patient_id() const;

    // Lisää annetun henkilökunnan jäsenen hoitojakson henkilökuntaan
    void assign_staff(Person* staff);

    // Asettaa annetun päivän hoitojakson päätöspäiväksi
    void set_end(Date& end_date);

    // Palauttaa arvon true jos annettu henkilö on hoitojakson henkilökunanjäsen
    bool has_staff_member(std::string name) const;

    // Tulostaa hoitojakson alku- ja mahdollisen loppupäivän ja potilaan nimen
    void print_care_periods() const;

    // Tulostaa hoitojakson kaikki tiedot
    void print_patient_info() const;

private:
    Person* patient_;
    Date start_;
    Date end_;

    // More attributes and methods
    std::map<std::string, Person*> assigned_staff_;

};

#endif // CAREPERIOD_HH
