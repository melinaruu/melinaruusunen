"""
COMP.CS.100 gui projekti
Tekijä: Melina Ruusunen
Opiskelijanumero: 050259885

Projekti on pyritty toteuttamaan niin että se täyttäisi kehittyneen projektin
vaatimukset.

Kyseessä on monivalintatesti, jossa käyttäjän täytyy valita yksi annetuista
vaihtoehdoista. Testi aloitetaan painamalla "Start"-nappulaa ja siinä edetään
"Next"-nappulan avulla. Lopuksi käyttäjä saa tietää monta pistettä hän sai
(yksi kysymys on viiden pisteen arvoinen) ja yleisen kommentin suorituksesta.
Tämän jälkeen käyttäjä voi yrittää uudelleen tai sulkea ikkunan.

Toteutus on sellainen, että halutessa kasvattaa tai vähentää kysymyksien tai
vaihtoehtojen määrää, se sujuisi helposti muuttamalla vain muutamaa kohtaa.
"""

from tkinter import *

# list of questions to be asked
questions = ["What does Harry have tattooed on his mid torso?",
             "In what famous band did he get his start in?",
             "What is his second name?",
             "Which brand’s clothing is he known to wear?",
             "What fruit is not associated with any of his songs?"]

# list where the values are lists of possible answers for each question
answer_choice = [["a quote", "a spider", "a butterfly", "a sun compass"],
                 ["'N Sync", "One Direction", "Backstreet Boys", "Spice Girls"],
                 ["William", "Edward", "James", "Robert"],
                 ["Versace", "Yves Saint Laurent", "Louis Vuitton", "Gucci"],
                 ["a mango", "a kiwi", "a watermelon", "a cherry"]]

# a list of the correct answers for each question
answers = [3, 2, 2, 4, 1]


# a class is created
class Quiz:

    def __init__(self):

        # the main window is created and some preferences are made
        self.__mainwindow = Tk()
        self.__mainwindow.title("Quiz")
        self.__mainwindow.geometry("500x500")
        self.__mainwindow.config(bg="#E438F0")

        # a list is created and picture files are added into it
        self.__harry_pics = []
        for pic in ["hs1.png", "hs2.png", "hs3.png", "hs4.png"]:
            self.__harry_pics.append(PhotoImage(file=pic))

        # a variable where the selected option's value is stored
        self.__selected = IntVar()

        # counters for question number and how many answers have been correct
        self.__q_nro = 0
        self.__correct = 0

        # different labels and buttons are set up for future use in the
        # functions
        self.__header = Label(self.__mainwindow, text="How well do you know "
                                                      "Harry Styles? :)",
                              font=("Arial", 24, "bold"),
                              bg="#E438F0")

        self.__headerpic = Label(self.__mainwindow, image=self.__harry_pics[0],
                                 bd=0, bg="#E438F0")

        self.__start_button = Button(self.__mainwindow, text="S T A R T",
                                     font=("Arial", 16), bg="#E438F0", command=
                                     self.start, bd=0, relief=RAISED)

        self.__question = Label(self.__mainwindow, font=("Arial", 20, "bold"))

        self.__options = []

        self.__next = Button(self.__mainwindow, text="Next",
                             font=("Arial", 16), command=self.next_btn)

        self.__error = Label(self.__mainwindow, font=("Arial", 14))

        self.__resulttext = Label(self.__mainwindow, font=("Arial", 16))

        self.__resultpic = Label(self.__mainwindow, font=("Arial", 16))

        self.__try_again = Button(self.__mainwindow, text="Try again",
                                  command=self.try_again, font=("Arial", 16),
                                  bd=0)

        self.__quit = Button(self.__mainwindow, text="Quit", command=self.quit,
                             font=("Arial", 16), bd=0)

        # the function is called to create a starting screen
        self.starting_screen()

        self.__mainwindow.mainloop()

    def starting_screen(self):
        """
        This function creates the starting screen for the quiz.
        :return: doesn't return anything
        """
        self.__header.pack()

        self.__headerpic.pack()

        self.__start_button.pack(pady=10)

    def start(self):
        """
        This function is called by the "start"-button. It destroys the
        components that were in the starting screen and changes the background
        color to white. In addition, it also calls the function "start quiz".
        :return: doesn't return anything
        """
        self.__header.destroy()
        self.__headerpic.destroy()
        self.__start_button.destroy()

        self.__mainwindow.config(bg="#FFFFFF")

        self.startquiz()

    def startquiz(self):
        """
        This function is the main part of the quiz. It shows the question in
        turn, the corresponding radiobutton options and the "Next"-button.
        :return: doesn't return anything
        """
        # shows the question
        self.__question.pack(pady=20)

        # 4 radiobuttons are created by calling the function "create options"
        self.__options = self.create_options(4)

        # the function "display question" is called
        self.display_question(self.__q_nro)

        # the "Next"-button is also added
        self.__next.pack(pady=40)

    def create_options(self, n):
        """
        This function creates an n amount of radiobuttons.
        :param n: the number of radiobutton options to be created
        :return: returns the options as a list called "buttons"
        """
        # the amount of buttons at the beginning is set
        # it's also used to set the radiobutton's value
        button_val = 0

        # a list is set up to add the buttons into
        buttons = []

        # when the amount of buttons is smaller than preferred
        while button_val < n:

            # radiobuttons are created
            button = Radiobutton(self.__mainwindow, value=button_val+1,
                                 variable=self.__selected, font=("Arial", 16))

            # created buttons are added into the list
            buttons.append(button)

            button.pack(pady=5)

            # the amount increases by one
            button_val += 1

        # the list is returned with the correct amount of radiobuttons
        return buttons

    def display_question(self, question):
        """
        This function is called in the "start quiz" function. It displays the
        specific question and its options.
        :param question: the question's number
        :return: doesn't return anything
        """
        # the option's number in the beginning is set to start from zero
        button_val = 0

        # the option value is also set to zero
        self.__selected.set(0)

        # the correct question is added into the label's text
        self.__question.config(text=questions[question])

        # all the different answer choices for the specific question are gone
        # through
        for option in answer_choice[question]:

            # the answer choice is added to the radiobutton's text
            self.__options[button_val].config(text=option)

            # option's number increases
            button_val += 1

    def next_btn(self):
        """
        This function is called by the "Next"-button in the function "start
        quiz". Depending on the situation it can result in different things,
        so it decides what happens next.
        :return: doesn't return anything
        """
        # the selected option's value is stored in the variable
        x = self.__selected.get()

        # if value is the same as the correct value
        if x == answers[self.__q_nro]:

            # amount of correct answers increases by one
            self.__correct += 1

        # question number in turn is increased
        self.__q_nro = self.__q_nro + 1

        # if x is smaller than 0, so if no value is chosen
        if x < 1:

            # an error message is shown
            self.__error.pack()
            self.__error.config(text="You need to select an option!")

            # question number is decreased, the same question is answered again
            self.__q_nro = self.__q_nro - 1

        # question number is same as amount of questions, all have been asked
        elif self.__q_nro >= len(questions):

            # the function is called to show the results
            self.showresult()

        # question is answered in a correct way
        else:

            # the possible error message is erased
            self.__error.config(text="")

            # the next question is shown
            self.display_question(self.__q_nro)

    def showresult(self):
        """
        This function shows the result of the quiz. It's called in the function
        "next button" when all the questions have been asked. It shows the
        result in points with a general comment. Also a picture is added based
        on the result.
        :return: doesn't return anything
        """
        # all the components are destroyed
        self.__question.destroy()
        self.__next.destroy()
        self.__error.destroy()
        for radiobutton in self.__options:
            radiobutton.destroy()

        # the amount of correct answers is multiplied by 5
        score = self.__correct * 5

        # when score is considered good
        if score >= 20:
            # a happy picture is shown on an orange screen
            self.__mainwindow.config(bg="#FF5E00")
            self.__resultpic.config(image=self.__harry_pics[1], bd=0)
            self.__resultpic.pack(pady=10)

            # the amount of points is shown with a comment
            self.__resulttext.config(text=f"You got {score}/25 points.\n"
                                          f"Well done!",
                                     font=("Arial", 16), bg="#FF5E00")
            self.__resulttext.pack(pady=20)

        # when score is considered ok
        elif 10 <= score < 20:

            # a picture is shown on a yellow screen
            self.__mainwindow.config(bg="#F7FF00")
            self.__resultpic.config(image=self.__harry_pics[2], bd=0)
            self.__resultpic.pack(pady=10)

            # the amount of points is shown with a comment
            self.__resulttext.config(text=f"You got {score}/25 points.\n"
                                          f"You did ok.",
                                     font=("Arial", 16), bg="#F7FF00")
            self.__resulttext.pack(pady=20)

        # when score is considered bad
        else:

            # a sad picture is shown on a blue screen
            self.__mainwindow.config(bg="#0066FF")
            self.__resultpic.config(image=self.__harry_pics[3], bd=0)
            self.__resultpic.pack(pady=10)

            # the amount of points is shown with a comment
            self.__resulttext.config(text=f"You got {score}/25 points.\n"
                                          f"You need to look him up!",
                                     bg="#0066FF")
            self.__resulttext.pack(pady="20")

        # two different buttons are added
        self.__try_again.pack()

        self.__quit.pack(pady=5)

    def try_again(self):
        """
        This function is called by the "Try again"-button. It destroys the
        current window and opens a new one.
        :return: nothing
        """
        self.__mainwindow.destroy()

        Quiz()

    def quit(self):
        """
        This function is called by the "Quit"-button. It destroys the window.
        :return: nothing
        """
        self.__mainwindow.destroy()


def main():
    Quiz()


if __name__ == "__main__":
    main()
