/* Hospital
 * Kuvaus:
 * Ohjelmassa on luotu sairaala, jonka tietoja voi tarkastella eri komentojen
 * avulla. Sairaalaa pyöritetään siihen asti kunnes on annettu quit-käsky,
 * jolloinka sairaala poistetaan ja ohjelma päättyy.
 *  Sairaalaan voidaan lisätä ja poistaa potilaita ja lisätä henkilökuntaa.
 * Jokaiselle potilaalle luodaan käyntikohtainen hoitojakso jolle voidaan
 * nimittää henkilökuntaa. Potilaan lähtiessä, hoitojaksolle asetetaan päätös-
 * päivä. Komennolla print_care_periods_per_staff voidaan tulostaa yhdelle
 * henkilökunnan jäsenelle nimitetyt hoitojaksot
 *  Jokaiselle potilaalle voidaan myös lisätä ja poistaa lääkkeitä. Kaikki eri
 * eri potilaille määrätyt lääkkeet voidaan tulostaa print_all_medicines
 * komennon avulla. Potilaiden omat lääkkeet tulostuvat näkyviin myös silloin
 * kun tulostetaan yhden potilaan, kaikkien potilaiden tai tämänhetkisten
 * potilaiden hoitojaksot. Näille kaikille on omat komentonsa.
 *  Tietoja voidaan myös lukea tiedostosta, joka nopeuttaa suuren määrän tietoja
 * tallennusta.
 *  Jotkut komennoista vaativat eri määriä parametreja ja ohjelma tarkastaa että
 * määrä on oikea. Komennolla help saadaan näkyviin kaikki eri komennot ja
 * niiden tarkoitus.

Ohjelman kirjoittaja
Nimi: Melina Ruusunen
Opiskelijanumero: 50259885
Käyttäjätunnus: bpmeru
E-Mail: melina.ruusunen@tuni.fi

*/

#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
