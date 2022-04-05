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

    buttonPosition0 = Button(buttonWindow, text="0", width= 20, height=6)
    buttonPosition1 = Button(buttonWindow, text="1", width= 20, height=6)
    buttonPosition2 = Button(buttonWindow, text="2", width= 20, height=6)
    buttonPosition3 = Button(buttonWindow, text="3", width= 20, height=6)

    buttonFinished = Button(buttonWindow, text="Klaar", width= 10, height=3, command=buttonWindow.destroy)

    # A Label widget to show in toplevel
    Label(buttonWindow, text="Selecteer Sorteer Volgorde:").pack()

    buttonFinished.pack(side=BOTTOM, padx=5, pady=5)
    buttonPosition0.pack(side=LEFT, padx=5, pady=5)
    buttonPosition1.pack(side=LEFT,padx=5, pady=5)
    buttonPosition2.pack(side=LEFT,padx=5, pady=5)
    buttonPosition3.pack(side=LEFT,padx=5, pady=5)

Grid.rowconfigure(windowMain,0, weight=1)
Grid.rowconfigure(windowMain,1, weight=1)
Grid.columnconfigure(windowMain,0, weight=1)
Grid.columnconfigure(windowMain,1, weight=1)

frameRegister = Frame(windowMain)
frameSortingButtonModes = Frame(windowMain)
frameWaitlist = Frame(windowMain)
frameMessages = Frame(windowMain)

frameSortingButtonModes.grid(column=0,columnspan=1,row=0,rowspan=1)
frameRegister.grid(column=1,columnspan=2,row=0,rowspan=2)
frameWaitlist.grid(column=1,columnspan=1,row=1,rowspan=1)
frameMessages.grid(column=0,columnspan=1,row=1,rowspan=1)

############################Register componenten############################
labelRegister = Label(frameRegister, text="Register:")
listBoxRegister = Listbox(frameRegister, width= 100, height=12)
scrollBarRegister = Scrollbar(frameRegister)

############################Sorteerknoppen componenten############################
labelSortingButtons = Label(frameSortingButtonModes, text="Sorteer modus:")
buttonAuto = Button(frameSortingButtonModes, text="Automatisch", width= 50, height=4)
buttonManual = Button(frameSortingButtonModes, text="Handmatig", width=50, height=4, command=openButtonWindow)

############################Wachtrij componenten############################
labelWaitlist = Label(frameWaitlist, text="Wachtrij:")
buttonWaitlist = Button(frameWaitlist, text="Wachtrij aanpassen", width= 30, height=7)
listBoxWaitlist = Listbox(frameWaitlist, width= 65, height=4)

############################Meldingen componenten############################
labelBoxMessages = Label(frameMessages, text="Meldingen:", width= 60, height=4)
listBoxMessages = Listbox(frameMessages, width= 60, height=4)

labelSortingButtons.grid(row=0,column=0)
buttonAuto.grid(row=1,column=0)
buttonManual.grid(row=2,column=0)

labelRegister.pack(side=TOP, padx=5, pady=5)
listBoxRegister.pack(side=LEFT)
scrollBarRegister.pack(side=RIGHT)

labelWaitlist.grid(column=0,columnspan=1,row=0,rowspan=1)
buttonWaitlist.grid(column=3,columnspan=2,row=0,rowspan=2)
listBoxWaitlist.grid(column=0,columnspan=2,row=1,rowspan=1)

listBoxMessages.grid(column=0,row=1)
labelBoxMessages.grid(column=0,row=0)

windowMain.protocol("WM_DELETE_WINDOW", quit)
windowMain.mainloop()