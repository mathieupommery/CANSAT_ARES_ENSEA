from PyQt5.QtWidgets import QDialog, QMainWindow, QApplication, QPushButton, QLabel, QWidget
from PyQt5.QtCore import QTimer, Qt
from PyQt5.uic import loadUi
from PyQt5 import QtWidgets
import serial
import time
import sys

#----- Sent buffer format -----
#sent_buffer will have the following informations:
#"$TEMP:23.5C;GPS:48.8566N,2.3522E;ACC:14.5;ALT:120m;PRES:1013hPa;DATE:2025-01-18;TIME:14:30:45#"
sent_buffer = "$23.5,48.8566,N,2.3522,E,14.5,120,1013,2025,01,18,14,30,45#"

#----- Login window -----
#Window through which you can login yourself
class Login(QDialog):

    def __init__(self, stack, mainPage):
        super(Login,self).__init__()
        self.stack = stack
        self.mainPage = mainPage
        loadUi("LoginScreen.ui",self)
        self.password.setEchoMode(QtWidgets.QLineEdit.Password)
        self.loginbutton.clicked.connect(self.loginfunction)

    def loginfunction(self):
        username = self.username.text()
        password = self.password.text()
        if (username=="a" and password=="a"):
            self.stack.setCurrentIndex(1)
            self.stack.repaint()
            self.mainPage.read_serial_data()

#----- Main window -----
#All the data will be displayed in this window
class Main(QMainWindow):
        
    def __init__(self, stack, readingState):
        super(Main, self).__init__()
        self.stack = stack
        self.readingState = readingState
        loadUi("ControlScreen.ui",self)
        
        self.ser_sup = serial.Serial('/dev/ttyAMA0',baudrate=115200,timeout=1)
        self.ser_inf = serial.Serial('/dev/ttyAMA2',baudrate=115200,timeout=1)
        
        self.stopbutton.clicked.connect(self.stop_reading)
        self.resumebutton.clicked.connect(self.resume_reading)
        self.okbutton.clicked.connect(self.port_setup)
        
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.read_serial_data)
        self.timer.start(100)
        
    def read_serial_data(self):
        if self.readingState:
            data_sup = self.read_buffer_sup()
            data_inf = self.read_buffer_inf()
            if data_sup!= None:
                self.interface_update_sup(data_sup)
                self.stack.repaint()
            if data_inf!= None:
                self.interface_update_inf(data_inf)
                self.stack.repaint()
        time.sleep(0.1)
        
    def port_setup(self):
        port_inf = self.portline__inf.text()
        port_sup = self.portline__sup.text()
        baudrate = int(self.baudrateline.text())
        ser_inf = serial.Serial(port_inf,baudrate,timeout=1)
        ser_sup = serial.Serial(port_sup,baudrate,timeout=1)
        return (ser_inf,ser_sup)
    
    def stop_reading(self):
        self.readingState = 0
     
    def resume_reading(self):
        self.readingState = 1

    @staticmethod
    def data_process(data): 
        data = data.replace('\r','').replace('$','').replace('#','').replace('\n','')
        data = data.split(',')
        return data
    
    def read_buffer_sup(self):
        if self.ser_sup.in_waiting >= len(sent_buffer):
            data = self.ser_sup.readline().decode('utf')
            return data
        else:
            return None
        
    def read_buffer_inf(self):
        if self.ser_inf.in_waiting >= len(sent_buffer):
            data = self.ser_inf.readline().decode('utf')
            return data
        else:
            return None

    def interface_update_sup(self,data):
        data_process = self.data_process(data)
        if (len(data_process)>=13):  
            self.date_day.setText(data_process[10])
            self.date_month.setText(data_process[9])
            self.date_year.setText(data_process[8])
            self.time_hour.setText(data_process[11])
            self.time_minute.setText(data_process[12])
            self.time_second.setText(data_process[13])
            self.gps_sup_N.setText(data_process[1])
            self.gps_sup_E.setText(data_process[3])
            self.altitude_sup.setText(data_process[6])
            self.pressure_sup.setText(data_process[7])
            self.temperature_sup.setText(data_process[0])
            self.acceleration_sup.setText(data_process[5])

    def interface_update_inf(self,data):
        data_process = self.data_process(data)
        if (len(data_process)>=13):  
            self.date_day.setText(data_process[10])
            self.date_month.setText(data_process[9])
            self.date_year.setText(data_process[8])
            self.time_hour.setText(data_process[11])
            self.time_minute.setText(data_process[12])
            self.time_second.setText(data_process[13])
            self.gps_inf_N.setText(data_process[1])
            self.gps_inf_E.setText(data_process[3])
            self.altitude_inf.setText(data_process[6])
            self.pressure_inf.setText(data_process[7])
            self.temperature_inf.setText(data_process[0])
            self.acceleration_inf.setText(data_process[5])

#----- Main code -----
#Main code program
if __name__=="__main__":

    app = QApplication(sys.argv)
    widget = QtWidgets.QStackedWidget()
    
    mainwindow = Main(widget,0)
    loginwindow = Login(widget,mainwindow)
    
    widget.addWidget(loginwindow)
    widget.addWidget(mainwindow)
    
    widget.setFixedWidth(1100) 
    widget.setFixedHeight(1400) 
    widget.show()
    
    app.exec_()
