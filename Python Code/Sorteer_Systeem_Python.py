from tkinter import *
from tkinter.ttk import Style
from time import sleep
import serial

windowMain = Tk()
fontSize = 15
fontText = ("Calibri", fontSize)
waitlist = []

try:
    ser = serial.Serial('COM6', 9600)
except:
    ser = serial.Serial()
    print("Geen seriële verbinding beschikbaar op COM6")

def startMain():
    print()

def manualWaitlist():
    print("waitlist")

def dcToArduino(message):
    ser.write(b"dc")

def stepperToArduino(message):
    ser.write(b"" + message)

def encoder():
    ser.read

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

    buttonFinished = Button(buttonWindow, text="Klaar", width=10, height=3, command= manualWaitlist&buttonWindow.destroy)

    # A Label widget to show in toplevel
    Label(buttonWindow, text="Selecteer Sorteer Volgorde:").pack()

    buttonFinished.pack(side=BOTTOM, padx=5, pady=5)
    buttonPosition0.pack(side=LEFT, padx=5, pady=5)
    buttonPosition1.pack(side=LEFT, padx=5, pady=5)
    buttonPosition2.pack(side=LEFT, padx=5, pady=5)
    buttonPosition3.pack(side=LEFT, padx=5, pady=5)


Grid.rowconfigure(windowMain, 1, weight=1)
Grid.rowconfigure(windowMain, 2, weight=1)
Grid.rowconfigure(windowMain, 3, weight=1)
Grid.rowconfigure(windowMain, 5, weight=1)

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

labelRegister.grid(row=0, column=2, columnspan=3, padx=5, pady=5)
listBoxRegister.grid(row=1, column=2, columnspan=3, rowspan=3, sticky="nsew", padx=5, pady=5)



############################Sorteerknoppen componenten############################
labelSortingButtons = Label(text="Sorteer modus:")
buttonAuto = Button(text="Automatisch")
buttonManual = Button(text="Handmatig", command=openButtonWindow)
buttonStart = Button(text="Start", bg='green',fg='white', command=startMain)
buttonStop = Button(text="Stop", bg="#5A5A5A", fg='white')
buttonEmergencyStop = Button(text="NOODSTOP", bg="red", fg='white')

labelSortingButtons.grid(row=0, column=0, columnspan=2, padx=5, pady=5)
buttonAuto.grid(row=2, column=0, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonManual.grid(row=2, column=1, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonStart.grid(row=1, column=0, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonStop.grid(row=1, column=1, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonEmergencyStop.grid(row=3, column=0, columnspan=2, sticky="nsew", padx=5, pady=5)

buttonAuto.configure(font=fontText)
buttonStop.configure(font=fontText)
buttonStart.configure(font=fontText)
buttonManual.configure(font=fontText)
buttonEmergencyStop.configure(font=fontText)

############################Wachtrij componenten############################
labelWaitlist = Label(text="Wachtrij:")
buttonWaitlist = Button(text="Wachtrij aanpassen")
listBoxWaitlist = Listbox()

labelWaitlist.grid(column=2, columnspan=2, row=4, rowspan=1, padx=5, pady=5)
listBoxWaitlist.grid(column=2, columnspan=2, row=5, rowspan=1, sticky="nsew", padx=5, pady=5)
buttonWaitlist.grid(column=4, row=5, sticky="nsew", padx=5, pady=5)

buttonWaitlist.configure(font=fontText)

############################Meldingen componenten############################
labelBoxMessages = Label(text="Meldingen:")
listBoxMessages = Listbox()

labelBoxMessages.grid(column=0, row=4, columnspan=2, rowspan=1, padx=5, pady=5)
listBoxMessages.grid(column=0, row=5, columnspan=2, rowspan=1, sticky="nsew", padx=5, pady=5)

# for i in range(100):
#     listBoxRegister.insert(i, "Nummer" + str(i))

windowMain.protocol("WM_DELETE_WINDOW", quit)
windowMain.mainloop()

