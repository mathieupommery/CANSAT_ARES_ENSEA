import sys
import math, time
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QGridLayout, QVBoxLayout, QHBoxLayout, QLabel, QFrame, QDialog

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
        if len(parts) != 24:
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
            "top_accx": float(parts[10]),
            "bottom_accx": float(parts[11]),
            "top_accy": float(parts[12]),
            "bottom_accy": float(parts[13]),
            "top_accz": float(parts[14]),
            "bottom_accz": float(parts[15]),
            "sat_hMSL": float(parts[16]),
            "top_pression": float(parts[17]),
            "top_temperature": float(parts[18]),
            "bottom_temperature": float(parts[19]),  # ⚠️ Distance dans cette version
            "bottom_pression": float(parts[20]),
            "timeindex": int(parts[23])  # parts[21] = unused?, parts[22] = maybe autre pression?
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
            "top_hMSL", "bottom_hMSL", "top_accx", "bottom_accx",
            "top_accy", "bottom_accy", "top_accz", "bottom_accz",
            "sat_hMSL", "top_pression", "top_temperature",
            "bottom_temperature",  # ⚠️ correspond maintenant à la distance
            "bottom_pression",     # 🔹 ajouté dans la nouvelle trame
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

        self.extra_frame = self._build_frame("Zone complémentaire")
        self.extra_frame.layout().addWidget(QLabel("À compléter plus tard..."))
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
            line = line.strip()
            if not line:
                continue
            data_dict = parse_trame(line)
            if data_dict is None:
                continue
            self.csv_writer.writerow([data_dict[h] for h in self.csv_fields])
            self.update_all(data_dict)

    def update_all(self, data):
        self.graph_widget.update_data(
            data['timeindex'],
            data['top_pression'] / 10000,
            data['top_temperature'],
            data['top_hMSL']
        )
        self.orientation_widget.set_orientation(
            1,
            compute_roll(data['top_accx'], data['top_accy'], data['top_accz']),
            compute_pitch(data['top_accx'], data['top_accy'], data['top_accz']) + math.radians(90),
            0
        )
        self.orientation_widget.set_orientation(
            2,
            compute_roll(data['bottom_accx'], data['bottom_accy'], data['bottom_accz']) + math.radians(80),
            compute_pitch(data['bottom_accx'], data['bottom_accy'], data['bottom_accz']),
            0
        )
         #Optionnel : mise à jour carte GPS
#         self.map_widget.add_point(data['top_lat'], data['top_lon'])
#         self.map_widget.add_point(data['bottom_lat'], data['bottom_lon'])
#         self.map_widget._update_map()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    config_dialog = SerialConfigDialog()
    if config_dialog.exec_() == QDialog.Accepted:
        port, baud = config_dialog.get_config()
        window = MainWindow(port=port, baudrate=baud)
        window.show()
        sys.exit(app.exec_())







