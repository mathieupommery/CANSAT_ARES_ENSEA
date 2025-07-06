# serial_config.py
from PyQt5.QtWidgets import QDialog, QVBoxLayout, QComboBox, QPushButton, QLabel
import serial.tools.list_ports

import serial.tools.list_ports

def list_serial_ports_with_description():
    ports = serial.tools.list_ports.comports()
    return [(port.device, port.description) for port in ports]

class SerialConfigDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Configuration Port Série")

        layout = QVBoxLayout()
        self.setLayout(layout)

        self.port_combo = QComboBox()
        ports = list_serial_ports_with_description()
        for device, desc in ports:
            self.port_combo.addItem(f"{device} - {desc}", device)
        layout.addWidget(self.port_combo)

        layout.addWidget(QLabel("Baudrate:"))
        self.baud_combo = QComboBox()
        self.baud_combo.addItems(["9600", "19200", "38400", "57600", "115200", "230400"])
        self.baud_combo.setCurrentText("230400")
        layout.addWidget(self.baud_combo)

        self.start_button = QPushButton("Démarrer")
        self.start_button.clicked.connect(self.accept)
        layout.addWidget(self.start_button)

    def _populate_ports(self):
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.port_combo.addItem(port.device)

    def get_config(self):
        return self.port_combo.currentData(), int(self.baud_combo.currentText())
