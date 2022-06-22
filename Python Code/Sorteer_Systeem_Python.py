from tkinter import *
from tkinter.ttk import Style
from time import sleep
import serial
import csv

windowMain = Tk()
starting = True

#Alle font functies
fontSize = 15
fontText = ("Calibri", fontSize)
errorFont = ("Calibri", 25)

#De wachtrij array
waitlist = []

#Alle variabelen die te maken hebben met welke modus de sorteermachine moet opereren
modeMessage = "MANUAL"
previousModeMessage = "MANUAL"
#Als stopped false is dan is de stop of kalibreer knop niet ingedrukt dus hoeft er niet naar de previousmessage gekeken worden
stopped = False

#De tekst die in een errormessage komt te staan
errormessage = ""
arduinoMessage = ""
information = []

#De comport waarop de arduino is aangesloten
comPort = "COM9"


try:
    serialArduino = serial.Serial(comPort, 250000)

except:
    serialArduino = serial.Serial()
    print("Geen seriële verbinding beschikbaar op " + comPort)

#dit is de main klasse die activeert als de start knop wordt ingedrukt
def startMain():
    global modeMessage, stopped, starting
    #Als modemessage auto is dan moet de machine op automatische stand werken
    if modeMessage == "AUTO":
        print("Automatische wachtrij geselecteerd")
        messageToArduino("AUTO")
    # Als modemessage stop is dan moet de machine stoppen
    elif modeMessage == "STOP":
        #Hier wordt gecheckt of stop nog maar net is ingedrukt of dat dit al 1 keer is uitgevoerd
        messageToArduino("STOP")
        modeMessage = previousModeMessage
    # Als modemessage manual is dan moet de machine op handmatige stand werken
    elif modeMessage == "MANUAL":
        if len(waitlist) > 0:
            #hier onder wordt het cijfer uit de wachtrij +1 gedaan omdat in de arduino als een string wordt verstuurd dit naar 0 wordt vertaald wat betekent dat als we hier 0 gebruiken er mogelijk problemen komen
            print("Wachtlijst cijfer is: "+str(waitlist[0]))
            stringMessage = str(waitlist[0]+1)
            print("Verhoogd cijfer is: " + stringMessage)
            messageToArduino(stringMessage)
            waitlist.pop(0)
        else:
            errorWindow = Toplevel(windowMain)
            errorWindow.title("ERROR message")
            errorlabel = Label(errorWindow, text="Wachtrij is leeg!")
            errorlabel.pack()
            errorlabel.configure(font = errorFont)
    # Als modemessage calibrate is dan moet de machine kalibreren
    elif modeMessage == "CALIBRATE":
        messageToArduino("CALIBRATE")
        modeMessage = previousModeMessage
    else:
        print("Error modeMessage heeft geen legitime waarde")

#als je strings wilt sturen gebruik dit: serialArduino.write(message.encode())
def messageToArduino(message):
    print("MessageToArduino string is: " +message)
    listBoxWaitlist.delete(0)
    serialArduino.write(message.encode())

def updateListbox():
    if len(waitlist) > 0:
        place = len(waitlist) - 1
        print(place)
        listBoxWaitlist.insert(place, "Pot moet naar positie " + str(waitlist[place]) + " worden verplaatst")
    else:
        place = 0
        print(place)
        listBoxWaitlist.insert(place, waitlist[place])

def waitlistChange():
    if len(waitlist) > 0:
        y = 0
        for x in waitlist:
            listBoxWaitlist.delete(y, END)
            print("Wachtrij listbox heeft positie: " + str(y) + " verwijderd")
            y+=1
        waitlist.clear()
    else:
        print("wachtlijst is leeg")
        waitlist.clear()
    openManualButtonWindow()

def automaticWaitlistEnable():
    global modeMessage
    global previousModeMessage
    modeMessage = "AUTO"
    previousModeMessage = "AUTO"
    print("wachtrij modus naar automatisch aangepast")
    # serialArduino.write(modeMessage.encode())

#Alle manualWaitlist functies zijn gekoppeld aan de knoppen die openen bij de handmatige invoer van een wachtrij,
#ze voegen aan de array waitlist een cijfer toe die bij de knop hoort en voert updateListbox() uit
def manualWaitlist0():
    waitlist.append(0)
    print(waitlist)
    updateListbox()

def manualWaitlist1():
    waitlist.append(1)
    print(waitlist)
    updateListbox()

def manualWaitlist2():
    waitlist.append(2)
    print(waitlist)
    updateListbox()

def manualWaitlist3():
    waitlist.append(3)
    print(waitlist)
    updateListbox()

def stopSystemButton():
    global modeMessage
    modeMessage = "STOP"

def calibrateButton():
    global modeMessage
    modeMessage = "CALIBRATE"

#OpenButtonWindow is de functie om een popup te openen voor handmatige invoer van de wachtrij
def openManualButtonWindow():
    global previousModeMessage
    global modeMessage
    modeMessage = "MANUAL"
    previousModeMessage = "MANUAL"
    manual = "MANUAL"
    serialArduino.write(manual.encode())
    print("wachtrij modus naar handmatig aangepast")

    buttonWindow = Toplevel(windowMain)

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
buttonAuto = Button(text="Automatisch", command=automaticWaitlistEnable)
buttonManual = Button(text="Handmatig", command=openManualButtonWindow)
buttonStart = Button(text="Start", bg='green',fg='white', command=startMain)
buttonStop = Button(text="Stop", bg="red", fg='white', command=stopSystemButton)
buttonCalibrate = Button(text="Kalibreer apparaat", bg="blue", fg='white', command=calibrateButton)

labelSortingButtons.grid(row=0, column=0, columnspan=2, padx=5, pady=5)
buttonAuto.grid(row=2, column=0, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonManual.grid(row=2, column=1, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonStart.grid(row=1, column=0, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonStop.grid(row=1, column=1, columnspan=1, sticky="nsew", padx=5, pady=5)
buttonCalibrate.grid(row=3, column=0, columnspan=2, sticky="nsew", padx=5, pady=5)

buttonAuto.configure(font=fontText)
buttonStop.configure(font=fontText)
buttonStart.configure(font=fontText)
buttonManual.configure(font=fontText)
buttonCalibrate.configure(font=fontText)

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

def getArduinoMessage():
    global arduinoMessage, errormessage, information, serialArduino


    if serialArduino.in_waiting > 0:
        try:
            arduinoMessage = serialArduino.readline().decode()
        except UnicodeDecodeError:
            print("Kon arduino bericht niet decoden.")
            windowMain.after(20, getArduinoMessage)
            return
        print("arduinomessage: " + str(arduinoMessage))
        if arduinoMessage.strip() == "NEXT":
            print("Wachtrij aangevraagd")
            if len(waitlist) > 0:
                print("wachtrij verstuurd")
                messageToArduino(str(waitlist[0]+1))
                waitlist.pop(0)

        if arduinoMessage.strip() == "CYLINDER_OPEN_ERROR" or arduinoMessage.strip() =="TURN_RIGHT_BLOCKED_ERROR" or arduinoMessage.strip() =="TURN_LEFT_BLOCKED_ERROR" or arduinoMessage.strip() =="TURN_BLOCKED_ERROR" or arduinoMessage.strip() =="STEPPER_LIFT_ERROR" or arduinoMessage.strip() =="STEPPER_DOWN_ERROR" or arduinoMessage.strip() =="CYLINDER_CLOSED_ERROR" or arduinoMessage.strip() =="CYLINDER_ERROR" or arduinoMessage.strip() =="STEPPER_ERROR" or arduinoMessage.strip() =="DC_ERROR":
            errormessage = arduinoMessage
            listBoxMessages.insert(0, errormessage)
            information = ["Error occured, position not reached", errormessage]

            with open('logboek.csv', 'a', encoding='UTF8', newline='') as logboek:
                # create the csv writer
                writer = csv.writer(logboek)
                # write a row to the csv file
                writer.writerow(information)

            with open('logboek.csv', 'r', encoding='UTF8', newline='') as logboek:
                csv_reader = csv.reader(logboek, delimiter=',')
                listBoxRegister.option_clear()
                for row in csv_reader:
                    listBoxRegister.insert(0, row)

        if arduinoMessage.strip() == "1" or arduinoMessage.strip() == "2" or arduinoMessage.strip() == "3" or arduinoMessage.strip() == "4":
            message = str(int(arduinoMessage)-1)
            print("Verplaatsing voltooid, pot verplaatst naar plek: " + message)
            listBoxMessages.insert(0, "Verplaatsing voltooid, pot verplaatst naar plek: " + message)
            errormessage = "Geen fouten tijdens uitvoering"

            information = [message, errormessage]

            #Het programma om in een csv logboek de dingen op te slaan
            with open('logboek.csv', 'a', encoding='UTF8', newline='') as logboek:
                # create the csv writer
                writer = csv.writer(logboek)
                # write a row to the csv file
                writer.writerow(information)

            with open('logboek.csv', 'r', encoding='UTF8', newline='') as logboek:
                csv_reader = csv.reader(logboek, delimiter=',')
                listBoxRegister.option_clear()
                for row in csv_reader:
                    listBoxRegister.insert(0, row)


        else:
            listBoxMessages.insert(0, "Actie: " + arduinoMessage)
    windowMain.after(1, getArduinoMessage)

windowMain.protocol("WM_DELETE_WINDOW", quit)
windowMain.after(1, getArduinoMessage)
windowMain.mainloop()

