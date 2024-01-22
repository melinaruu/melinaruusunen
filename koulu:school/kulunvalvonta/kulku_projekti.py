"""
COMP.CS.100 kulunvalvonta projekti
Tekijä: Melina Ruusunen
Opiskelijanumero: 050259885
"""

DOORCODES = {'TC114': ['TIE'], 'TC203': ['TIE'], 'TC210': ['TIE', 'TST'],
             'TD201': ['TST'], 'TE111': [], 'TE113': [], 'TE115': [],
             'TE117': [], 'TE102': ['TIE'], 'TD203': ['TST'], 'TA666': ['X'],
             'TC103': ['TIE', 'OPET', 'SGN'], 'TC205': ['TIE', 'OPET', 'ELT'],
             'TB109': ['OPET', 'TST'], 'TB111': ['OPET', 'TST'],
             'TB103': ['OPET'], 'TB104': ['OPET'], 'TB205': ['G'],
             'SM111': [], 'SM112': [], 'SM113': [], 'SM114': [],
             'S1': ['OPET'], 'S2': ['OPET'], 'S3': ['OPET'], 'S4': ['OPET'],
             'K1705': ['OPET'], 'SB100': ['G'], 'SB202': ['G'],
             'SM220': ['ELT'], 'SM221': ['ELT'], 'SM222': ['ELT'],
             'secret_corridor_from_building_T_to_building_F': ['X', 'Y', 'Z'],
             'TA': ['G'], 'TB': ['G'], 'SA': ['G'], 'KA': ['G']}


class Accesscard:
    """
    This class models an access card which can be used to check
    whether a card should open a particular door or not.
    """

    def __init__(self, id, name):
        """
        Constructor, creates a new object that has no access rights.

        :param id: str, card holders personal id
        :param name: str, card holders name

        THIS METHOD IS AUTOMATICALLY TESTED, DON'T CHANGE THE NAME OR THE
        PARAMETERS!
        """

        self.__card_id = id
        self.__card_holder = name
        # a list is created to add access codes into
        self.__access = []

    def info(self):
        """
        The method has no return value. It prints the information related to
        the access card in the format:
        id, name, access: a1,a2,...,aN
        for example:
        777, Thelma Teacher, access: OPET, TE113, TIE
        Note that the space characters after the commas and semicolon need to
        be as specified in the task description or the test fails.

        THIS METHOD IS AUTOMATICALLY TESTED, DON'T CHANGE THE NAME, THE
        PARAMETERS, OR THE PRINTOUT FORMAT!
        """
        # prints the information stored in one card
        print(f"{self.__card_id}, {self.__card_holder}, access: ", end="")
        print(*sorted(self.__access), sep=", ")

    def get_name(self):
        """
        :return: Returns the name of the accesscard holder.
        """
        name = self.__card_holder
        return name

    def add_access(self, new_access_code):
        """
        The method adds a new accesscode into the accesscard according to the
        rules defined in the task description.

        :param new_access_code: str, the accesscode to be added in the card.

        THIS METHOD IS AUTOMATICALLY TESTED, DON'T CHANGE THE NAME, THE
        PARAMETERS, OR THE RETURN VALUE! DON'T PRINT ANYTHING IN THE METHOD!
        """
        # if the code isn't already stored in the card, it's added
        if new_access_code not in self.__access:
            self.__access.append(new_access_code)

            # if codes become useless, they are deleted
            for key, value in DOORCODES.items():
                if new_access_code in value or key == new_access_code:
                    for door in self.__access:
                        if door in DOORCODES:
                            for area in DOORCODES[door]:
                                if area in self.__access:
                                    self.__access.remove(door)

    def check_access(self, door):
        """
        Checks if the accesscard allows access to a certain door.

        :param door: str, the doorcode of the door that is being accessed.
        :return: True: The door opens for this accesscard.
                 False: The door does not open for this accesscard.

        THIS METHOD IS AUTOMATICALLY TESTED, DON'T CHANGE THE NAME, THE
        PARAMETERS, OR THE RETURN VALUE! DON'T PRINT ANYTHING IN THE METHOD!
        """
        # check if there is access to the door
        if door in self.__access:
            return True

        # checks if there is access to the door through some other code
        for door in DOORCODES[door]:
            if door in self.__access:
                return True
        # if there is no access
        return False

    def merge(self, card):
        """
        Merges the accesscodes from another accesscard to this accesscard.

        :param card: Accesscard, the accesscard whose access rights are
        added to this card.

        THIS METHOD IS AUTOMATICALLY TESTED, DON'T CHANGE THE NAME, THE
        PARAMETERS, OR THE RETURN VALUE! DON'T PRINT ANYTHING IN THE METHOD!
        """
        # goes through all the codes in "card"'s access code list and adds them
        for code in card.__access:
            self.add_access(code)


def read_file():
    """
    Opens and reads the file named "accessinfo.txt" and saves it's information
    in to the created dict "cards".
    :return: the dict "cards"
    """
    # a new dict is created
    cards = {}

    # the file that needs to be opened
    filename = "accessinfo.txt"

    try:

        file = open(filename, mode="r")

        # goes through all the rows in the file
        for row in file:
            row_strip = row.rstrip()
            # the row is split into different components
            components = row_strip.split(";")
            card_id = components[0]
            name = components[1]
            accesscodes = components[2].split(",")

            one_card = Accesscard(card_id, name)

            # the objects are added to the dict
            cards[card_id] = one_card

            # adds the access codes into the right cards
            for code in accesscodes:
                one_card.add_access(code)

        return cards

    except IOError:
        print("Error: file cannot be read.")
    except ValueError:
        print("Error: the file cannot be read.")


def main():
    # the dict "cards" is called so it can be used
    cards = read_file()

    # the different areas from the dict "DOORCODES" are added to a list
    areas = []

    for key, value in DOORCODES.items():
        for area in value:
            areas.append(area)

    while True:
        # asks which command should be done
        line = input("command> ")

        if line == "":
            break

        strings = line.split()
        command = strings[0]

        if command == "list" and len(strings) == 1:
            # all the different cards' info that is saved in the dict is
            # printed in a sorted order

            for card in sorted(cards):
                cards[card].info()

        elif command == "info" and len(strings) == 2:
            # one card's info is printed. the card_id affects which card is
            # printed
            card_id = strings[1]
            # checks if the card_id is found from the cards dict
            if card_id in cards:
                # if found, prints the info
                cards[card_id].info()
            else:
                # otherwise an error message is printed
                print("Error: unknown id.")

        elif command == "access" and len(strings) == 3:
            # checks if the specific card has access to a door
            card_id = strings[1]
            door_id = strings[2]

            # checks if the card is found from the dict, if not: error message
            if card_id in cards:
                # checks if the door is in the doorcodes, if not: error message
                if door_id in DOORCODES:
                    # checks if the card has access to a door
                    permission = cards[card_id].check_access(door_id)
                    # gets the name of the card holder
                    name = cards[card_id].get_name()
                    if permission is True:
                        # printed if the card has access
                        print(f"Card {card_id} ( {name} ) has access to door "
                              f"{door_id}")
                    else:
                        # printed if the card doesn't have access
                        print(f"Card {card_id} ( {name} ) has no access to "
                              f"door {door_id}")
                else:
                    print("Error: unknown doorcode.")
            else:
                print("Error: unknown id.")

        elif command == "add" and len(strings) == 3:
            # adds the given access code to the specific card
            card_id = strings[1]
            access_code = strings[2]

            # checks if the card is found from the dict, if not: error message
            if card_id in cards:
                # checks if the access code is one of the areas or doors
                if access_code not in areas and access_code not in DOORCODES:
                    # printed if access code is unknown
                    print("Error: unknown accesscode.")
                else:
                    # if the code is known, it's added into the cards access
                    # code list
                    cards[card_id].add_access(access_code)
            else:
                print("Error: unknown id.")

        elif command == "merge" and len(strings) == 3:
            # adds the access codes of one id to another
            card_id_to = strings[1]
            card_id_from = strings[2]

            # checks if the first card is found from the dict,
            # if not: error message
            if card_id_to in cards:
                # checks if the other card is found, if not: error message
                if card_id_from in cards:
                    # if both found, the access codes are merged
                    cards[card_id_to].merge(cards[card_id_from])
                else:
                    print("Error: unknown id.")
            else:
                print("Error: unknown id.")

        elif command == "quit":
            print("Bye!")
            return

        else:
            print("Error: unknown command.")


if __name__ == "__main__":
    main()
