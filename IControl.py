#!/usr/bin/python
# -*- coding: utf-8 -*-
from tkinter import *
from tkinter import filedialog
import tkinter.messagebox as tkMessageBox
import webbrowser, os , re , sys, time
from os import path
global pfad
pfad=str(os.getcwd())
stand = "Icontrol   Version: 1.0 Software Stand: 11.09.2019"

LED_COLOR = {
    "off"           : "grey",
    "red"           : "#fe3521",
    "green"         : "#30ff30",
    "pure-green"    : "#0fff87",
    "yellow-green"  : "#a8ee19",
    "yellow"        : "#fdb71a",
    "blue"          : "SteelBlue1",
    "white"         : "#fefefe",
}

NX = 8
NY = 8

LED_ON_COLOR = LED_COLOR["blue"]
LED_OFF_COLOR = LED_COLOR["off"]
INO_FILE_SELECTED = False
class Application(Frame):
    
    def __init__(self,master=None):
        Frame.__init__(self,master)
        self.configure(background='black')
        self.master = master
        self.tool_bar()
        self.Werbung()
        self.hintergrund()
        self.grid() 
        
        self.INO_FILE_SELECTED = False
        self.vars =[[IntVar() for x in range(NX)] for y in range(NY)]
        self.checks = [[self.__makecheck__(self.vars[x][y])
                        for x in range(NX)]
                        for y in range(NY)]
        
        for x in range(NX):
            for y in range(NY):
                self.checks[x][y].grid(row=x, column=y, padx=2, pady=2)

        self.b1 = Button(self, text="CLEAR", command=self.clear_all,bg='black',fg='SteelBlue1',font='impact 20',relief='flat')
        self.b1.grid(row=NX+1,column=0,columnspan=4)
 
        self.b2 = Button(self, text="SAVE", command=self.save_it,bg='black',fg='SteelBlue1',font='impact 20',relief='flat')
        self.b2.grid(row=NX+1,column=4,columnspan=4)

        self.varTN1 = StringVar()
        self.varTN1.set("_bmp")
        self.labelTN1 = Label(self,textvariable=self.varTN1,height = 1,bg='black',fg='SteelBlue1',font='impact 10',relief='flat')
        self.labelTN1.grid(row=NX+3,column=0,columnspan=NX)
        self._bmp="_bmp"
        self.boxTN1=Entry(self,textvariable=self._bmp,width=18,bg='grey',fg='SteelBlue1',font='impact 10',relief='flat')
        self.boxTN1.grid(row=NX+4,column=0,columnspan=NX)
        
        self.varTN2 = StringVar()
        self.varTN2.set("pwm_value == ")
        self.labelTN2 = Label(self,textvariable=self.varTN2,height = 1,bg='black',fg='SteelBlue1',font='impact 10',relief='flat')
        self.labelTN2.grid(row=NX+5,column=0,columnspan=NX)
        self.pwm_value = "0"       
        self.boxTN2=Entry(self,textvariable=self.pwm_value,width=18,bg='grey',fg='SteelBlue1',font='impact 10',relief='flat')
        self.boxTN2.grid(row=NX+6,column=0,columnspan=NX)        
        

        self.varTN3 = StringVar()
        self.varTN3.set("Arduino *.Ino Datei")
        self.labelTN3 = Label(self,textvariable=self.varTN3,height = 1,bg='black',fg='SteelBlue1',font='impact 10',relief='flat')
        self.labelTN3.grid(row=NX+7,column=0,columnspan=NX)
        self.INO_FILE = "Bitte *.Ino Datei auswählen..."       
        self.boxTN3=Entry(self,textvariable=self.INO_FILE,width=80,bg='grey',fg='SteelBlue1',font='impact 10',relief='flat')
        self.boxTN3.grid(row=NX+8,column=0,columnspan=NX*12,rowspan=NX*12)
        
        self.commacount = 0
        self.clear_all()
        
    def tool_bar(self):

        menu = Menu(self.master)
        self.master.config(menu=menu,bg='black')
        
        file = Menu(menu)
        file.add_command(label="Exit", command=self.quit)
        menu.add_cascade(label="Datei", menu=file)
        
        edit = Menu(menu)
        edit.add_command(label="öffne neue *.Ino Datei", command=self.open_new_ino)
        menu.add_cascade(label="*.Ino Datei", menu=edit)
        
        about = Menu(menu)
        about.add_command(label="Navarra Software Solutions", command=self.about)
        menu.add_cascade(label="About", menu=about)
    
    def quit(self):
        sys.exit(0)
            
    def about(self):

        self.about = Toplevel(self)
        self.about.configure(background='black')
        self.about.geometry("550x250")
        #self.about.iconbitmap(pfad +'NAVARRAICON.ico')
        Label(self.about, text=str("[") + ("   ") + str("]"),bg=(self.BG),fg='grey',font='Helvetica 50 bold ').place(x=10,y=10);
        Label(self.about, text=str("*"),bg=(self.BG),fg=(self.FGBLUE),font=((self.FONT) + ("124"))).place(x=40,y=00);
        
        Label(self.about, text=(self.NAVARRA), bg=(self.BG), fg=(self.FGBLUE),font=((self.FONT) + ("22"))).place(x=130,y=20);
        Label(self.about, text=(self.SLOGAN) , bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=130,y=60);
        Label(self.about, text=(self.WEBSITE), bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=130,y=100);
        Label(self.about, text=(self.MAIL)   , bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=130,y=140);
        Label(self.about, text=(stand)  , bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) +  ("8"))).place(x=350,y=230);
    
    def hintergrund(self):
        Label(self, text=str("[") + ("   ") + str("]"),bg=(self.BG),fg='grey',font='Helvetica 50 bold ').place(x=250,y=20);
        Label(self, text=str("*"),bg=(self.BG),fg=(self.FGBLUE),font=((self.FONT) + ("124"))).place(x=280,y=10);
        
        Label(self, text=(self.NAVARRA), bg=(self.BG), fg=(self.FGBLUE),font=((self.FONT) + ("22"))).grid(row=3,column=10,rowspan=3,columnspan=3);
        Label(self, text=(self.SLOGAN) , bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=255,y=160);
        Label(self, text=(self.WEBSITE), bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=255,y=200);
        Label(self, text=(self.MAIL)   , bg=(self.BG), fg=(self.FG)    ,font=((self.FONT) + ("14"))).place(x=255,y=240);
    
    def __makecheck__(self, var=None):
        return Checkbutton(self,
                            text="",
                            indicatoron=False,
                            height=1,
                            width=2,
                            borderwidth=2,
                            background=LED_OFF_COLOR,
                            selectcolor=LED_ON_COLOR,
                            variable=var,
                            command=self.display
                            )
               

    def display(self):
        
        self.boxTN1.delete(0 ,END)
        self.boxTN1.insert(0,(self._bmp))
        self.boxTN2.delete(0 ,END)
        self.boxTN2.insert(0,(self.pwm_value))
        self.boxTN3.delete(0 ,END)
        self.boxTN3.insert(0,(self.INO_FILE))
        

    def clear_all(self):
        """Callback for clear button."""
        for x in range(NX):
            for y in range(NY):
                self.vars[x][y].set(0)
        self.display()

    def save_it(self):
        """Callback for save button."""
        self.save_txt()
    
    def save_txt(self):
        """Save current bitmap to text file."""
        self._bmp      = self.boxTN1.get()
        self.pwm_value = self.boxTN2.get()
        filename=self._bmp + '8x8Matrix.txt'
        with open(filename,"w") as FILE:
            FILE.write((self._bmp)+"_bmp[] =\n { \n")
            for y in range(NY):
                FILE.write("B")
                self.commacount += 1
                #print(self.commacount)
                
                for x in range(NX):
                    FILE.write("{0}".format(self.vars[x][y].get()))
                if self.commacount < (NX): 
                    #print(self.commacount)
                    FILE.write(", ")
                FILE.write("\n")
            FILE.write("},")
            FILE.write('\n\n else if (pwm_value == ' + self.pwm_value +') \n  { \n' + '    Serial.println("'+ self._bmp + '"); \n' + '    matrix[MATRIX_EYES].drawBitmap(0, 0, '+ self._bmp + '_bmp, 8, 8, LED_ON); \n' + '    matrix[MATRIX_EYES].writeDisplay(); \n' + '    delay(500); \n ' + '  }')
        self.commacount = 0
        self.display()  
        webbrowser.open(filename)
        self.open_ino()
    
    def open_new_ino(self):
        self.INO_FILE_SELECTED = False
        #print ("neue Ino Datei kann ausgewählt werden.")
        self.open_ino()
        
    def open_ino(self):
        if self.INO_FILE_SELECTED is True:
           #print ("Ino Datei bereits ausgewählt")
           self.display()
           pass
        else:
            result = tkMessageBox.askquestion('Bitte wähle eine *.Ino Datei aus. ', 'willst du jetzt eine *.Ino Datei wählen?', icon="warning")
            if result == 'yes':
                self.INO_FILE =  filedialog.askopenfilename(initialdir = "/",title = "suche eine Ino Datei",filetypes = (("INO","*.ino"),("alle Dateien","*.*")))
                #print (str("Die INO Datei: " ) + (self.INO_FILE) + (" wird eingelesen."))
                webbrowser.open(self.INO_FILE)
                self.INO_FILE_SELECTED = True
                self.display()
    
    def Werbung(self):
        
        self.LOGO    = "[*]"
        self.NAVARRA = "NAVARRA SOFTWARE SOLUTIONS"
        self.BG      = 'black'
        self.FG      = 'white'
        self.FGBLUE  = 'SteelBlue1'
        self.FONT    = 'impact '
        self.SLOGAN  = "let my Software do it for YOU..."
        self.WEBSITE = "https://NAVARRA.home.blog"
        self.MAIL    = "contactNAVARRA@gmail.com"
        self.CEO     = "Sebastian de la Vega"
        self.PHONE   = "Phone: +49 (0) 151 566 57246 "
        print (stand)
        print ("Grafisches Tool zum erstellen von 8x8 Matrix für BOB`s wundervolle blaue Augen.")
        print ("Für: Protogene Factory Pierre Braun")
        print ("von:")
        print (self.NAVARRA)
        print (self.SLOGAN)
        print (self.WEBSITE)
        print (self.MAIL)
        print (self.CEO)
        print (self.PHONE)
        print ("\n")
    
def main():
    root = Tk()
    root.title("Icontrol")
    root.resizable(False, False)
    root.configure(background='black')
    root.wm_overrideredirect(False)
    root.geometry("800x500")
    #root.iconbitmap(pfad +'NAVARRAICON.ico')
    app = Application(root)
    root.mainloop()   

#-------------------------------------------------------------------------------
#  M A I N
#-------------------------------------------------------------------------------
if __name__ == "__main__":
    main()