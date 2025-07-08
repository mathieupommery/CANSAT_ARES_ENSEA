import sys
import math, time
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QGridLayout, QVBoxLayout, QHBoxLayout, QLabel, QFrame,QFormLayout, QLineEdit, QDialog
from PyQt5.QtGui import QColor, QPalette
from PyQt5.QtCore import Qt, QTimer
from orientation_3d import Orientation3DWidget
from plotter import RealTimePlotWidget  # Doit contenir une méthode .add_data(x, y1, y2, y3)
from gps_map import GPSMapWidget
from serial_config import SerialConfigDialog
import random
import csv
import serial




def parse_trame(line):
    """
    line: str, exemple:
    "1,0,0,0,48.8566,48.8565,2.3522,2.3523,100,90,120.01,119.98,0,0,0,0,0,0,1013.2,25.3,120.5,1012.7,101325,12345678"

    Retourne un dict avec les bons types.
    """
    try:
        parts = line.strip().split(',')
        if len(parts) != 26:
            print(f"Warning: trame invalide (nb champs {len(parts)}): {line}")
            return None

        data = {
            "flag_calib": int(parts[0]),
            "flag_drop": int(parts[1]),
            "flag_separation": int(parts[2]),
            "flag_fin": int(parts[3]),
            "top_lat": float(parts[4]),
            "bottom_lat": float(parts[5]),
            "top_lon": float(parts[6]),
            "bottom_lon": float(parts[7]),
            "top_hMSL": float(parts[8]),
            "bottom_hMSL": float(parts[9]),
            "top_altbaro": float(parts[10]),
            "bottom_altbaro": float(parts[11]),
            "top_gyrx": float(parts[12]),
            "bottom_gyrx": float(parts[13]),
            "top_gyry": float(parts[14]),
            "bottom_gyry": float(parts[15]),
            "top_gyrz": float(parts[16]),
            "bottom_gyrz": float(parts[17]),
            "sat_hRSSI": float(parts[18]),
            "top_pression": float(parts[19]),
            "top_temperature": float(parts[20]),
            "distance_modules": float(parts[21]),  # ⚠️ Distance dans cette version
            "bottom_pression": float(parts[22]),
            "top_vspeed": float(parts[23]),
            "bottom_vspeed": float(parts[24]),
            "timeindex": int(parts[25])
        }

        return data
    except Exception as e:
        print(f"Erreur parsing trame: {e} - line: {line}")
        return None

def compute_roll(ax, ay, az):
    return math.atan2(ay, az)

def compute_pitch(ax, ay, az):
    return math.atan2(-ax, math.sqrt(ay**2 + az**2))
# ... imports en haut du fichier (inchangés) ...
class MainWindow(QMainWindow):
    def __init__(self, port, baudrate):
        super().__init__()

        self.setWindowTitle("Station CANSAT - Realtime Viewer")
        self.setGeometry(100, 100, 1400, 900)
        self.port = port
        self.baudrate = baudrate
        self.showMaximized()

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        grid_layout = QGridLayout()
        central_widget.setLayout(grid_layout)

        self.pending_data = []
        self.serial_buffer = ""
        self.csv_file = open('data.csv', 'w', newline='')
        self.csv_writer = csv.writer(self.csv_file)

        # ⬇️ Mise à jour avec les 24 champs (nom "bottom_temperature" reste inchangé même si c’est la distance)
        self.csv_fields = [
            "flag_calib", "flag_drop", "flag_separation", "flag_fin",
            "top_lat", "bottom_lat", "top_lon", "bottom_lon",
            "top_hMSL", "bottom_hMSL","top_altbaro","bottom_altbaro", "top_gyrx", "bottom_gyrx",
            "top_gyry", "bottom_gyry", "top_gyrz", "bottom_gyrz",
            "sat_hRSSI", "top_pression", "top_temperature",
            "distance_modules",
            "bottom_pression",
            "top_vspeed",
            "bottom_vspeed",
            "timeindex"
        ]
        self.csv_writer.writerow(self.csv_fields)

        self.serial = serial.Serial(self.port, self.baudrate, timeout=0.01)

        self.orientation_widget = Orientation3DWidget()
        self.orientation_frame = self._build_frame("Orientation 3D")
        self.orientation_frame.layout().addWidget(self.orientation_widget)
        grid_layout.addWidget(self.orientation_frame, 0, 0)

        self.graph_widget = RealTimePlotWidget()
        self.graphs_frame = self._build_frame("Graphes")
        self.graphs_frame.layout().addWidget(self.graph_widget)
        grid_layout.addWidget(self.graphs_frame, 0, 1)

        from gps_map import GPSMapWidget
        self.map_widget = GPSMapWidget()
        self.map_frame = self._build_frame("Carte GPS")
        self.map_frame.layout().addWidget(self.map_widget)
        grid_layout.addWidget(self.map_frame, 1, 0)

        self.extra_layout = QVBoxLayout()

        # Champs pour valeurs
        self.value_fields = {
            "altitude_top": QLineEdit(),
            "Température": QLineEdit(),
            "hauteur_RSSI": QLineEdit(),
            "Distance": QLineEdit()
        }
        form = QFormLayout()
        for name, field in self.value_fields.items():
            field.setReadOnly(True)
            form.addRow(name, field)

        # Indicateurs de flag
        self.flag_boxes = {}
        flag_names = ["Calib", "Drop", "Séparation", "Fin"]
        flag_layout = QHBoxLayout()
        for name in flag_names:
            box = QFrame()
            box.setFixedSize(25, 25)
            box.setFrameShape(QFrame.Box)
            box.setStyleSheet("background-color: red;")
            self.flag_boxes[name] = box
            flag_layout.addWidget(QLabel(name))
            flag_layout.addWidget(box)

        self.extra_layout.addLayout(form)
        self.extra_layout.addLayout(flag_layout)
        
        self.extra_frame = self._build_frame("Zone complémentaire")
        self.extra_frame.layout().addLayout(self.extra_layout)
        
        grid_layout.addWidget(self.extra_frame, 1, 1)

        grid_layout.setRowStretch(0, 1)
        grid_layout.setRowStretch(1, 1)
        grid_layout.setColumnStretch(0, 1)
        grid_layout.setColumnStretch(1, 1)

        self.timer_serial = QTimer()
        self.timer_serial.setTimerType(Qt.PreciseTimer)
        self.timer_serial.timeout.connect(self.read_serial)
        self.timer_serial.start(100)

        self.flush_timer = QTimer()
        self.flush_timer.setTimerType(Qt.VeryCoarseTimer)
        self.flush_timer.timeout.connect(self.flush_csv)
        self.flush_timer.start(1000)

    def flush_csv(self):
        for row in self.pending_data:
            self.csv_writer.writerow(row)
        self.pending_data = []
        self.csv_file.flush()

    def _build_frame(self, title: str):
        frame = QFrame()
        frame.setFrameShape(QFrame.StyledPanel)
        layout = QVBoxLayout()
        label = QLabel(f"<b>{title}</b>")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
        frame.setLayout(layout)
        return frame

    def read_serial(self):
        data = self.serial.read(self.serial.inWaiting()).decode('ascii', errors='ignore')
        if not data:
            return
        self.serial_buffer += data

        while '\n' in self.serial_buffer:
            line, self.serial_buffer = self.serial_buffer.split('\n', 1)
            if line.strip().lower() == "fin":
                self.capture_and_quit()
                return
            line = line.strip()
            if not line:
                continue
            data_dict = parse_trame(line)
            print(data_dict)
            if data_dict is None:
                continue
            self.csv_writer.writerow([data_dict[h] for h in self.csv_fields])
            self.update_all(data_dict)
    def capture_and_quit(self):
        screen = QApplication.primaryScreen()
        screenshot = screen.grabWindow(self.winId())
        screenshot.save("dashboard_final.png", "PNG")
        self.csv_file.close()
        self.serial.close()
        QApplication.quit()
    def update_all(self, data):
        self.graph_widget.update_data(
            data['timeindex'],
            data['top_altbaro'],
            data['top_temperature'],
            data['top_vspeed'],
            data['bottom_altbaro'],
            data['sat_hRSSI'],
            None,  
            data['bottom_vspeed']
        )

        self.orientation_widget.update_orientation(
            1,
            data['top_gyrx'], data['top_gyry'], data['top_gyrz'],
            0.02
        )
        self.orientation_widget.update_orientation(
            2,
            data['bottom_gyrx'], data['bottom_gyry'], data['bottom_gyrz'],
            0.02
        )
        
        
        
         #Optionnel : mise à jour carte GPS
        
        if(data['timeindex']%10==0):
            if(data['top_lat']!=0.0 and data['top_lon']!=0.0):
                self.map_widget.add_point(data['top_lat'], data['top_lon'])              
            if(data['bottom_lat']!=0.0 and data['bottom_lon']!=0.0):
                self.map_widget.add_point(data['bottom_lat'], data['bottom_lon'])
        self.value_fields["altitude_top"].setText(f"{data['top_altbaro']} m")
        self.value_fields["Température"].setText(f"{data['top_temperature']} °C")
        self.value_fields["hauteur_RSSI"].setText(f"{data['sat_hRSSI']} dBm")
        self.value_fields["Distance"].setText(f"{data['distance_modules']} m")

        self.flag_boxes["Calib"].setStyleSheet("background-color: green;" if data["flag_calib"] else "background-color: red;")
        self.flag_boxes["Drop"].setStyleSheet("background-color: green;" if data["flag_drop"] else "background-color: red;")
        self.flag_boxes["Séparation"].setStyleSheet("background-color: green;" if data["flag_separation"] else "background-color: red;")
        self.flag_boxes["Fin"].setStyleSheet("background-color: green;" if data["flag_fin"] else "background-color: red;")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    config_dialog = SerialConfigDialog()
    config_dialog.showMaximized()
    if config_dialog.exec_() == QDialog.Accepted:
        port, baud = config_dialog.get_config()
        
        window = MainWindow(port=port, baudrate=baud)
        window.showMaximized() 
        sys.exit(app.exec_())







