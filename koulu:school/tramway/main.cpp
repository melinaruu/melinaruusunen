/*
Ohjelman kirjoittaja
Nimi: Melina Ruusunen
Opiskelijanumero: 050259889
Käyttäjätunnus: bpmeru
E-Mail: melina.ruusunen@tuni.fi

Ohjelman avulla voi hallita raitiovaunuun liittyviä tietoja. Käyttäjä antaa
tiedoston, josta löytyy erilaiset linja- ja pysäkkitiedot. Jokaisella pysäkillä
on nimi ja se on liitetty johonkin tai joihinkin linjoihin. Tiedostosta löytyy
myös etäisyydet pysäkkien välillä. Tiedot talletetaan map tietorakenteeseen
jonka avaimet ovat linjoja ja arvot mappeja joihin on tallennettu pysäkki
tiedot.
Eri komentojen avulla käyttäjä voi tarkastella tietoja ja esimerkiksi tulostaa
linjoja ja pysäkeitä. Niitä voi myös poistaa ja lisätä. Annettuja käskyjä
käydään läpi ja määritetään mitkä ovat oikein ja toimitaan sen mukaan.
Tarkastelun voidaan lopettaa komennolla "QUIT".
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <locale>

using Lines = std::map<std::string, std::map<double, std::string>>;
using Stops = std::map<double, std::string>;
const char DELIMITER = ';';
const std::string INPUT_ERROR = "Error: Invalid input.";
const std::string STOP_NOT_FOUND = "Error: Stop could not be found.";
const std::string LINE_NOT_FOUND = "Error: Line could not be found.";

// Tulostaa rassen

void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Jakaa tekstin osiin tietyn merkin mukaan
// Palauttaa vektorin jossa syötteen eri osat on eritelty oikein

std::vector<std::string> split(const std::string& text, char separator,
                               bool ignore_empty = false)
{
    std::vector<std::string> parts;
    std::string part;
    char delimiter = separator;

    std::string::size_type begin = 0;
    std::string::size_type end =0;

    end = text.find(delimiter, begin);

    while (end != std::string::npos)
    {
        end = text.find(delimiter, begin);
        part = text.substr(begin, end - begin);

        // Otetaan huomioon hipsuihin merkityt, moniosaiset syötteet

        if (part.front() == '"')
        {
            ignore_empty = true;
            delimiter = '"';
            end=text.find(delimiter, begin);
        }
        else if (part.front() == ' ')
        {
            ignore_empty = false;
            delimiter = ' ';
            end=text.find(delimiter, begin);
        }
        else if (part != "" || ignore_empty)
        {
            parts.push_back(part);
        }

        begin = end + 1;
        end = text.find(separator, begin);
    }

    std::string final_part = text.substr(begin, std::string::npos);
    parts.push_back(final_part);

    return parts;
}

// Avaa ja lukee annetun tiedoston ja lopuksi sulkee sen. Jos tiedosto
// saadaan onnistuneesti luettua, funktio palauttaa arvon true.
// Luetut tiedot talletetaan parametrinä annettuun vektoriin.

bool read_input_file(const std::string& filepath,
                     std::vector<std::string>& lines)
{
    std::ifstream input_file_stream;
    input_file_stream.open(filepath);

    if (!input_file_stream.is_open())
    {
        return false;
    }

    std::string line;

    while (std::getline(input_file_stream, line))
    {
        lines.push_back(line);
    }
    input_file_stream.close();

    return true;
}

// Lisää pysäkin tietorakenteeseen. Parametreinä on tietorakenne, linjan nimi,
// pysäkin nimi ja etäisyys.

bool add_stop(Lines& stops_by_line, const std::string& line_name,
              const std::string& stop_name, double distance)
{
    bool is_unique = true;

    // Tarkastetaan sisältääkö linja jo pysäkkiä annetulla etäisyydellä.

    if (stops_by_line[line_name].count(distance) != 0)
    {
        is_unique = false;
    }

    // Tarkastetaan löytyykö linjalta jo annetun nimistä pysäkkiä.

    for (auto& dist_stop_pair : stops_by_line[line_name])
    {
        if (dist_stop_pair.second == stop_name)
        {
            is_unique = false;
            break;
        }
    }

    // Jos ei vielä ole kyseisen nimistä pysäkkiä tai pysäkkiä kyseisellä
    // etäisyydellä, niin sellainen lisätään tietorakenteeseen. Muuten
    // tulostetaan virhe ilmoitus.

    if (is_unique)
    {
        stops_by_line[line_name][distance] = stop_name;
    }
    else
    {
        std::cout << "Error: Stop/line already exists." << std::endl;
    }

    return is_unique;
}

// Lisää linjan tietorakenteeseen. Parametreinä on tietorakenne ja linjan
// nimi.

bool add_line(Lines& stops_by_line, const std::string& line_name)
{
    // Tarkastetaan löytyykö mapista annetun linjan nimistä avainta.

    if (stops_by_line.count(line_name) != 0)
    {
        std::cout << "Error: Stop/line already exists." << std::endl;
        return false;
    }

    // Jos linjaa ei vielä ole, sellainen luodaan.

    stops_by_line[line_name] = {};

    return true;
}

// Tarkastaa onko annetun tiedosto tiedot oikean muotoisia. Parametreinä on
// tietorakenne ja vektori jossa on tiedoston eri kentät.

bool validate_input_line_format(Lines& stops_by_line,
                                const std::vector<std::string>& fields)
{
    // Tarkastetaan onko funktiossa oikea määrä kenttiä.

    if (fields.size() < 2 || fields.size() > 3)
    {
        return false;
    }

    // Tarkastetaan mitkä kentät ovat mahdollisesti tyhjiä

    for (size_t i = 0; i < fields.size(); ++i)
    {
        std::string field = fields.at(1);

        if (field == "" && i != 2 && stops_by_line.count(fields.at(0)))
        {
            return false;
        }
    }
    return true;
}

// Lisää tiedoston tiedot tietorakenteeseen. Parametreinä vektori jossa on
// tiedoston rivit ja tietorakenne.

bool parse_input_file(const std::vector<std::string>& lines,
                      Lines& stops_by_line)
{
    // Käydään läpi tiedoston rivit.

    for (const auto& line : lines)
    {
        // Rivin tiedot jaetaan osiin.

        std::vector<std::string> fields = split(line, DELIMITER);

        // Jos tiedot eivät ole oikean muotoisia, palauutetaan arvo false.

        if (!validate_input_line_format(stops_by_line, fields))
        {
            std::cout << "Error: Invalid format in file." << std::endl;
            return false;
        }

        std::string line_name = fields.at(0);
        std::string stop_name = fields.at(1);
        double distance = 0.0;

        if (fields.size() == 3 && fields.at(2) != "")
        {
            distance = stod(fields.at(2));
        }

        // Jos linjaa ei vielä löydy, sellainen lisätään

        if (stops_by_line.count(line_name) == 0)
        {
            add_line(stops_by_line, line_name);
        }

        // Jos tiedostossa on virheellisiä tietoja tai toistoa, siitä tulee
        // ilmoitus käyttäjälle.

        if (!add_stop(stops_by_line, line_name, stop_name, distance))
        {
            return false;
        }
    }

    // Jos tiedot saadaan onnistuneesti lisättyä, mitään ei tulostu ja
    // palautetaan arvo true.

    return true;
}

// Tulostaa kaikki olemassa olevat linjat aakkosjärjestyksessä. Parametrinä
// on tietorakenne.

void command_lines(const Lines& stops_by_line)
{
    std::cout << "All tramlines in alphabetical order:" << std::endl;
    for (const auto& line : stops_by_line)
    {
        std::cout << line.first << std::endl;
    }
}

// Tulostaa kaikki olemassa olevat pysäkit aakkosjärjestyksessä. Parametrinä
// on tietorakenne

void command_stops(const Lines& stops_by_line)
{
    // Luodaan vektori johon pysäkkien nimet talletetaan.

    std::vector<std::string> stops;

    // Käydään tietorakennetta läpi linja linjalta.

    for (const auto& line : stops_by_line)
    {
        // Luodaan muuttuja mapin sisällä olevasta pysäkkitietoja sisältävästä
        // mapista.

        Stops internal_map = line.second;

        // Käydään pysäkit läpi

        for (auto it: internal_map)
        {
            // Jos pysäkkiä ei vielä löydy vektorista, se lisätään sinne.

            if (std::find(stops.begin(), stops.end(), it.second) == stops.end())
            {
                stops.push_back(it.second);
            }
        }
    }

    // Järjestetään vektori aakkosjärjestykseen.

    std::sort(stops.begin(),stops.end());

    std::cout << "All stops in alphabetical order:" << std::endl;

    // Tulostetaan vektoriin talletetut pysäkit.

    for (auto stop : stops)
    {
        std::cout << stop << std::endl;
    }
}

// Tulostaa pysäkit joidenka läpi linja kulkee. Parametreinä on tietorakenne
// ja linjan nimi.

void command_line(const Lines& stops_by_line, const std::string& line_name)
{
    // Jos parametrinä annettua linjaa ei ole, tulostetaan virheilmoitus.

    if (stops_by_line.find(line_name) == stops_by_line.end())
    {
        std::cout << LINE_NOT_FOUND << std::endl;
    } else
    {
        // Jos linja löytyy, käydään linjoja läpi yksitellen että löytyy oikea.

        for (const auto& line : stops_by_line)
        {
            // Jos linjan nimi on sama kuin parametrinä annetun,
            // tulostetaan pysäkit.

            if (line.first==line_name)
            {
                std::cout << "Line " << line_name
                          << " goes through these stops in the order they are listed:"
                          << std::endl;

                Stops internal_map = line.second;

                for (auto it: internal_map)
                {
                    // Tulostetaan sisämapin tiedot

                    std::cout << " - " << it.second << " : " << it.first << std::endl;
                }
            }
        }
    }
}

// Tulostaa kahden pysäkin välisen etäisyyden. Parametreinä on tietorakenne,
// linjan nimi, ensimmäisen pysäkin nimi ja toisen pysäkin nimi.

void command_distance(const Lines& stops_by_line, std::string line_name,
                      std::string stop_name1, std::string stop_name2)
{
    double dist1;
    double dist2;

    // Luodaan vektori johon voidaan tallettaa pysäkit.

    std::vector<std::string> stops;

    // Jos parametrinä annettua linjaa ei ole, tulostetaan virheilmoitus.

    if (stops_by_line.find(line_name) == stops_by_line.end())
    {
        std::cout << LINE_NOT_FOUND << std::endl;
    } else
    {
        // Jos linja löytyy, käydään linjoja läpi yksitellen että löytyy oikea.

        for (const auto& line : stops_by_line)
        {
            // Edetään jos linjan nimi on sama kuin parametrinä annetun.

            if (line.first==line_name)
            {
                Stops internal_map = line.second;

                // Käydään läpi sisämappia

                for (auto it: internal_map)
                {
                    // Liäsätään pysäkit vektoriin. Tässä tapauksessa ei ole
                    // väliä vaikka pysäkki löytyisikin jo vektorista.

                    stops.push_back(it.second);

                    // Kun pysäkin nimet ovat samat kuin annetut, saadaan
                    // etäisyydet

                    if (it.second == stop_name1)
                    {
                        dist1 = it.first;
                    }
                    if (it.second == stop_name2)
                    {
                        dist2 = it.first;
                    }
                }
            }
        }

        // Luodaan muuttuja etäisyyksien erotukselle

        double distance;
        distance = dist1-dist2;

        // Jos ensimmäistä tai toista pysäkkiä ei löydy pysäkki-vektorista,
        // tulostetaan virheilmoitus.

        if (std::find(stops.begin(), stops.end(), stop_name1) == stops.end() ||
            std::find(stops.begin(), stops.end(), stop_name2) == stops.end())
        {
            std::cout << STOP_NOT_FOUND << std::endl;
        }

        // Muuten tulostetaan erotus hyödyntäen abs-funktiota jonka avulla
        // saadaan erotuskesta itseisarvo eli positiivinen luku.

        else
        {
        std::cout << "Distance between " << stop_name1 << " and " << stop_name2
                  << " is " << std::abs(distance) << std::endl;
        }
    }
}

// Poistaa halutun pysäkin kaikilta linjoilta. Parametreinä on tietorakenne
// ja pysäkin nimi.

void command_remove(Lines& stops_by_line, const std::string& stop_name)
{
    // Luodaan vektori johon pysäkit voidaan tallettaa.

    std::vector<std::string> stops;

    for (auto& line : stops_by_line)
    {
        Stops internal_map = line.second;

        for (auto& it: internal_map)
        {
            // Lisätään pysäkit vektoriin

            stops.push_back(it.second);

            // Kun pysäkin nimi on sama kuin parametrissä, se poistetaan mapista.

            if (it.second==stop_name)
            {
                line.second.erase(it.first);
            }
        }
    }

    // Jos pysäkkiä ei löydy pysäkki-vektorista, tulostetaan virheilmoitus.

    if (std::find(stops.begin(), stops.end(), stop_name) == stops.end())
    {
        std::cout << STOP_NOT_FOUND << std::endl;
    }

    // Muuten tulostetaan ilmoitus, että pysäkit on poistettu.

    else
    {
        std::cout << "Stop was removed from all lines." << std::endl;
    }
}

// Tulostaa miltä linjoilta annettu pysäkki löytyy. Parametreinä on
// tietorakenne ja pysäkin nimi.

void command_stop(const Lines& stops_by_line, const std::string& stop_name)
{
    // Luodaan vektorit linjoille ja pysäkeille.

    std::vector<std::string> lines;
    std::vector<std::string> stops;

    for (const auto& line : stops_by_line)
    {
        Stops internal_map = line.second;

        for (auto it: internal_map)
        {
            // Lisätään pysäkit vektoriin.

            stops.push_back(it.second);

            // Jos pysäkki on sama kuin annettu.

            if (it.second==stop_name)
            {
                // Lisätään vektoriin linjan nimi jolta pysäkki löytyy.

                if (std::find(lines.begin(), lines.end(), it.second)
                        == lines.end())
                {
                    lines.push_back(line.first);
                }
            }
        }
    }

    if (std::find(stops.begin(), stops.end(), stop_name) == stops.end())
    {
        std::cout << STOP_NOT_FOUND << std::endl;
    }
    else
    {
        // Järjestetään linjat aakkosjärjestykseen.

        std::sort(lines.begin(),lines.end());

        std::cout << "Stop " << stop_name <<
                     " can be found on the following lines: " << std::endl;

        // Tulostetaan linjat omille riveilleen.

        for (auto line : lines)
        {
            std::cout << " - " << line << std::endl;
        }
    }
}

// Sisältää virhetarkastelut pysäkin lisäykseen liittyen. Parametreinä on
// tietorakenne, linjan nimi, pysäkin nimi ja etäisyys

void command_addstop(Lines& stops_by_line, const std::string& line_name,
                     const std::string& stop_name, double distance)
{
    // Tarkastetaan onko pysäkki olemassa.

    if (stops_by_line.find(line_name) == stops_by_line.end())
    {
        std::cout << LINE_NOT_FOUND << std::endl;
    }

    // Jos pysäkki lisätään onnistuneesti, tulostetaan ilmoitus.

    else
    {
        if (add_stop(stops_by_line, line_name, stop_name, distance))
        {
            std::cout << "Stop was added." << std::endl;
        }
    }
}

// Muuttaa annetun tekstin isoiksi kirjaimiksi. Parametrinä jokin sana.

std::string to_upper(std::string command)
{
    for (size_t i=0; i < command.length(); i++)
    {
        command[i]=toupper(command[i]);
    }
    return command;
}

int main()
{
    print_rasse();

    std::cout << "Give a name for input file: ";
    std::string filepath;
    std::getline(std::cin, filepath);
    std::vector<std::string> lines;

    if (!read_input_file(filepath, lines))
    {
        std::cout << "Error: File could not be read." << std::endl;
        return EXIT_FAILURE;
    }

    Lines stops_by_line;
    if (!parse_input_file(lines, stops_by_line))
    {
        return EXIT_FAILURE;
    }

    while (true)
    {
        std::cout << "tramway> ";

        std::string user_command = "";
        std::getline(std::cin, user_command);

        // Jaetaan annettu syöte osiin.

        std::vector<std::string> command_fields = split(user_command, ' ');

        // Muutetaan komento isoiksi kirjaimiksi jotta sitä voidaan vertailla.

        std::string command = to_upper(command_fields.at(0));

        // Tarkastetaan onko syötteen osia vähintään yksi.

        if (command_fields.size() >= 1)
            {
            if (command == "LINES")
            {
                command_lines(stops_by_line);
            }

            else if (command == "LINE")
            {
                if (command_fields.size() >= 2)
                {
                    command_line(stops_by_line, command_fields.at(1));
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "STOPS")
            {
                command_stops(stops_by_line);
            }

            else if (command == "STOP")
            {
                if (command_fields.size() >= 2)
                {
                    command_stop(stops_by_line, command_fields.at(1));
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "DISTANCE")
            {
                if (command_fields.size() >= 4)
                {
                    command_distance(stops_by_line, command_fields.at(1),
                                     command_fields.at(2),
                                     command_fields.at(3));
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "ADDLINE")
            {
                if (command_fields.size() >= 2)
                {
                    if (add_line(stops_by_line, command_fields.at(1)))
                    {
                        std::cout << "Line was added." << std::endl;
                    }
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "ADDSTOP")
            {
                if (command_fields.size() >= 4)
                {
                    double dist = stod(command_fields.at(3));
                    command_addstop(stops_by_line, command_fields.at(1),
                                    command_fields.at(2), dist);
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "REMOVE")
            {
                if (command_fields.size() >= 2)
                {
                    command_remove(stops_by_line, command_fields.at(1));
                }
                else
                {
                    std::cout << INPUT_ERROR << std::endl;
                }
            }

            else if (command == "QUIT")
            {
                break;
            }

            else
            {
                std::cout << INPUT_ERROR << std::endl;
            }

        } else
        {
            std::cout << INPUT_ERROR << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
