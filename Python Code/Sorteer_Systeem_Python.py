from tkinter import *
from tkinter.ttk import Style

windowMain = Tk()


def openButtonWindow():
    # Toplevel object which will
    # be treated as a new window
    buttonWindow = Toplevel(windowMain)
    framePositionButton = Frame(buttonWindow)
    # sets the title of the
    # Toplevel widget
    buttonWindow.title("Positie")

    buttonPosition0 = Button(buttonWindow, text="0", width=20, height=3)
    buttonPosition1 = Button(buttonWindow, text="1", width=20, height=3)
    buttonPosition2 = Button(buttonWindow, text="2", width=20, height=3)
    buttonPosition3 = Button(buttonWindow, text="3", width=20, height=3)

    buttonFinished = Button(buttonWindow, text="Klaar", width=10, height=3, command=buttonWindow.destroy)

    # A Label widget to show in toplevel
    Label(buttonWindow, text="Selecteer Sorteer Volgorde:").pack()

    buttonFinished.pack(side=BOTTOM, padx=5, pady=5)
    buttonPosition0.pack(side=LEFT, padx=5, pady=5)
    buttonPosition1.pack(side=LEFT, padx=5, pady=5)
    buttonPosition2.pack(side=LEFT, padx=5, pady=5)
    buttonPosition3.pack(side=LEFT, padx=5, pady=5)


Grid.rowconfigure(windowMain, 1, weight=1)
Grid.rowconfigure(windowMain, 2, weight=1)
Grid.rowconfigure(windowMain, 4, weight=1)

Grid.columnconfigure(windowMain, 0, weight=1)
Grid.columnconfigure(windowMain, 1, weight=1)
Grid.columnconfigure(windowMain, 2, weight=1)
Grid.columnconfigure(windowMain, 3, weight=1)
Grid.columnconfigure(windowMain, 4, weight=1)

# frameRegister = Frame(windowMain)
# frameSortingButtonModes = Frame(windowMain)
# frameWaitlist = Frame(windowMain)
# frameMessages = Frame(windowMain)

# frameSortingButtonModes.grid(column=0, columnspan=1, row=0, rowspan=1)
# frameRegister.grid(column=1, columnspan=2, row=0, rowspan=2)
# frameWaitlist.grid(column=1, columnspan=1, row=1, rowspan=1)
# frameMessages.grid(column=0, columnspan=1, row=1, rowspan=1)

############################Register componenten############################
labelRegister = Label(text="Register:")
listBoxRegister = Listbox()

labelRegister.grid(row=0, column=2, columnspan=3)
listBoxRegister.grid(row=1, column=2, columnspan=3, rowspan=2, sticky="nsew")

############################Sorteerknoppen componenten############################
labelSortingButtons = Label(text="Sorteer modus:")
buttonAuto = Button(text="Automatisch")
buttonManual = Button(text="Handmatig", command=openButtonWindow)
buttonStart = Button(text="Start", bg='green',fg='white')
buttonStop = Button(text="Stop", bg="red", fg='white')

labelSortingButtons.grid(row=0, column=0, columnspan=2)
buttonAuto.grid(row=2, column=0, columnspan=1, sticky="nsew")
buttonManual.grid(row=2, column=1, columnspan=1, sticky="nsew")
buttonStart.grid(row=1, column=0, columnspan=1, sticky="nsew")
buttonStop.grid(row=1, column=1, columnspan=1, sticky="nsew")

############################Wachtrij componenten############################
labelWaitlist = Label(text="Wachtrij:")
buttonWaitlist = Button(text="Wachtrij aanpassen")
listBoxWaitlist = Listbox()

labelWaitlist.grid(column=2, columnspan=2, row=3, rowspan=1)
listBoxWaitlist.grid(column=2, columnspan=2, row=4, rowspan=1, sticky="nsew")
buttonWaitlist.grid(column=4, row=4, sticky="nsew")

############################Meldingen componenten############################
labelBoxMessages = Label(text="Meldingen:")
listBoxMessages = Listbox()

labelBoxMessages.grid(column=0, row=3, columnspan=2, rowspan=1)
listBoxMessages.grid(column=0, row=4, columnspan=2, rowspan=1, sticky="nsew")

# for i in range(100):
#     listBoxRegister.insert(i, "Nummer" + str(i))

windowMain.protocol("WM_DELETE_WINDOW", quit)
windowMain.mainloop()
