from tkinter import *
from tkinter.ttk import Style
from time import sleep
import serial

windowMain = Tk()
fontSize = 15
fontText = ("Calibri", fontSize)
errorFont = ("Calibri", 25)
waitlist = []
comPort = "COM3"
comPortSchool = "COM10"

try:
    serialArduino = serial.Serial(comPort, 250000)

except:
    serialArduino = serial.Serial()
    print("Geen seriële verbinding beschikbaar op " + comPort)

# try:
#     serialArduinoSchool = serial.Serial(comPortSchool, 250000)
#
# except:
#     serialArduino = serial.Serial()
#     print("Geen seriële verbinding beschikbaar op " + comPortSchool)

#dit is de main klasse die activeert als de start knop wordt ingedrukt
def startMain():
    if len(waitlist) > 0:
        #hier onder wordt het cijfer uit de wachtrij +1 gedaan omdat in de arduino als een string wordt verstuurd dit naar 0 wordt vertaald wat betekent dat als we hier 0 gebruiken er mogelijk problemen komen
        messageToArduino((waitlist[0]+1))
        waitlist.pop(0)
    else:
        errorWindow = Toplevel(windowMain)
        errorWindow.title("ERROR message")
        errorlabel = Label(errorWindow, text="Wachtrij is leeg!")
        errorlabel.pack()
        errorlabel.configure(font = errorFont)


def updateListbox(number):
    if len(waitlist) >0:
        place = len(waitlist) - 1
        print(place)
        listBoxWaitlist.insert(place, waitlist[number])
    else:
        place = 0
        print(place)
        listBoxWaitlist.insert(place, waitlist[number])

def waitlistChange():
    if len(waitlist) > 0:
        for x in waitlist:
            listBoxWaitlist.delete(x, END)
        waitlist.clear()
    else:
        print("wachtlijst is leeg")
    openButtonWindow()

def manualWaitlist0():
    waitlist.append(0)
    print(waitlist)
    updateListbox(0)

def manualWaitlist1():
    waitlist.append(1)
    print(waitlist)
    updateListbox(1)

def manualWaitlist2():
    waitlist.append(2)
    print(waitlist)
    updateListbox(2)

def manualWaitlist3():
    waitlist.append(3)
    print(waitlist)
    updateListbox(3)
#als je strings wilt sturen gebruik dit: serialArduino.write(message.encode())
def messageToArduino(message):
    serialArduino.write(message.encode())

def encoder():
    serialArduino.read

#OpenButtonWindow is de functie om een popup te openen met 4 knoppen en 1 opslaan knop waarin een wachtrij aangemaakt kan worden
def openButtonWindow():
    # Toplevel object which will
    # be treated as a new window
    buttonWindow = Toplevel(windowMain)
    # sets the title of the
    # Toplevel widget
    buttonWindow.title("Positie")

    buttonPosition0 = Button(buttonWindow, text="0", width=20, height=3, command=manualWaitlist0)
    buttonPosition1 = Button(buttonWindow, text="1", width=20, height=3, command=manualWaitlist1)
    buttonPosition2 = Button(buttonWindow, text="2", width=20, height=3, command=manualWaitlist2)
    buttonPosition3 = Button(buttonWindow, text="3", width=20, height=3, command=manualWaitlist3)

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
buttonWaitlist = Button(text="Wachtrij aanpassen", command=waitlistChange)
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
# Als er string verstuurd worden moet je print(serialArduino.readline().decode())

def testprint():
    if serialArduino.in_waiting > 0:
        print(serialArduino.readline().decode())
    windowMain.after(50, testprint)

windowMain.protocol("WM_DELETE_WINDOW", quit)
windowMain.after(50, testprint)
windowMain.mainloop()

