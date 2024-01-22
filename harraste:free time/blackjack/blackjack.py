import random

maat = ["hertta", "ruutu", "risti", "pata"]

kortin_arvot = {'A': 11, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7,
                '8': 8, '9': 9, '10': 10, 'J': 10, 'Q': 10, 'K': 10}


def laske_pisteet(kortit, pisteet) -> int:

    for kortti in kortit:

        if pisteet >= 11 and kortti[0] == 'A':
            pisteet += 1
        else:
            pisteet += kortin_arvot[kortti[0]]

    return pisteet


def alku_kortit(pakka) -> int:

    pelaajan_kortit = []

    for i in range(2):

        kortin_indeksi = random.randint(0, len(pakka) - 1)

        pelaajan_kortit.append(pakka[kortin_indeksi])

        del pakka[kortin_indeksi]

    print(f"Sait kortit {pelaajan_kortit[0][1]} {pelaajan_kortit[0][0]} ja "
          f"{pelaajan_kortit[1][1]} {pelaajan_kortit[1][0]}")
    print(f"Pisteesi ovat: {laske_pisteet(pelaajan_kortit, 0)}")

    return laske_pisteet(pelaajan_kortit, 0)


def lisaa_kortteja(pakka, pisteet) -> int:

    kayttajan_syote = input("Haluatko uuden kortin? (Y) ")

    pelaajan_kokonaispisteet = pisteet

    pelaajan_kortit = []

    while kayttajan_syote == "y" or kayttajan_syote == "Y":

        kortin_indeksi = random.randint(0, len(pakka) - 1)

        pelaajan_kortit.append(pakka[kortin_indeksi][0])

        print(f"Sait kortin {pakka[kortin_indeksi][1]} {pakka[kortin_indeksi][0]}")

        pelaajan_kokonaispisteet = laske_pisteet(pelaajan_kortit,
                                                 pelaajan_kokonaispisteet)

        print(f"Pisteesi ovat: {pelaajan_kokonaispisteet}")

        del pakka[kortin_indeksi]

        if pelaajan_kokonaispisteet > 21:
            print("Pisteet ylittivät rajan. Hävisit.")
            break

        kayttajan_syote = input("Haluatko uuden kortin? (Y) ")

    return pelaajan_kokonaispisteet


def luo_pakka() -> list:
    pakka = []

    for maa in maat:
        for arvo in kortin_arvot:
            pakka.append((arvo, maa))

    return pakka


def lue_pelaajat() -> list:
    pelaajat = []

    with open("pelaajat.txt") as tiedosto:
        sisalto = tiedosto.read()
        nimet = sisalto.split(" ")

        for nimi in nimet:
            pelaajat.append(nimi)

    return pelaajat


def maarita_voittaja(pelaajien_pisteet):

    pelaajat = list(pelaajien_pisteet.keys())
    pisteet = list(pelaajien_pisteet.values())

    if len(pelaajat) == 0:
        voittaja = None
        return voittaja

    suurimmat_pisteet = max(pisteet)

    if pisteet.count(suurimmat_pisteet) != 1:
        voittaja = "tasapeli"
    elif len(pelaajat) == 0:
        voittaja = None
    else:
        pisteiden_indeksi = pisteet.index(suurimmat_pisteet)
        voittaja = pelaajat[pisteiden_indeksi]

    return voittaja


def main():

    pakka = luo_pakka()

    pelaajat = lue_pelaajat()

    pelaajien_pisteet = {}

    blackjack = 0

    blkjk_voittaja = ""

    for pelaaja in pelaajat:

        print(f"{pelaaja}n vuoro.")

        pelaajan_pisteet = alku_kortit(pakka)

        if pelaajan_pisteet == 21:
            print("Blackjack!")
            print(f"Lopulliset pisteesi ovat: {pelaajan_pisteet}\n")
            blackjack = 1
            blkjk_voittaja = pelaaja

        if pelaajan_pisteet < 21:
            lisaa_pisteita = lisaa_kortteja(pakka, pelaajan_pisteet)

            pelaajan_pisteet = lisaa_pisteita

            print(f"Lopulliset pisteesi ovat: {pelaajan_pisteet}\n")

            if pelaajan_pisteet <= 21:
                pelaajien_pisteet[pelaaja] = pelaajan_pisteet

    if blackjack != 0:
        voittaja = blkjk_voittaja
        print(f"Voittaja on {voittaja}!")
    else:
        voittaja = maarita_voittaja(pelaajien_pisteet)

        if voittaja is None:
            print("Kaikki hävisivät!")
        elif voittaja == "tasapeli":
            print("Tasapeli!")
        else:
            print(f"Voittaja on {voittaja}!")

    with open("tulokset.txt", "w") as tiedosto:
        tiedosto.write("Peli on päättynyt.")


if __name__ == "__main__":
    main()
