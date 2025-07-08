# plotter.py
from PyQt5.QtWidgets import QWidget, QVBoxLayout
import pyqtgraph as pg
from collections import deque
import numpy as np
import random
from PyQt5.QtWidgets import QWidget, QVBoxLayout
from collections import deque
import pyqtgraph as pg

class RealTimePlotWidget(QWidget):
    def __init__(self, parent=None, buffer_size=800):
        super().__init__(parent)

        self.buffer_size = buffer_size

        self.times = deque(maxlen=buffer_size)
        self.altbaro1 = deque(maxlen=buffer_size)
        self.altbaro2 = deque(maxlen=buffer_size)
        self.altbaro3 = deque(maxlen=buffer_size)
        self.temperature1 = deque(maxlen=buffer_size)
        self.temperature2 = deque(maxlen=buffer_size)
        self.speed1 = deque(maxlen=buffer_size)
        self.speed2 = deque(maxlen=buffer_size)

        layout = QVBoxLayout()
        layout.setSpacing(10)
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

        # --- Plot 1 : altbaro ---
        self.plot_pressure = pg.PlotWidget(title="altitude baro")
        self.plot_pressure.showGrid(x=True, y=True)
        self.plot_pressure.setLabel('left', 'alt baro (m)')
        self.plot_pressure.setLabel('bottom', 'Temps')
        self.curve_altbaro1 = self.plot_pressure.plot(pen='r')
        self.curve_altbaro2 = self.plot_pressure.plot(pen='g')
        self.curve_altbaro3 = self.plot_pressure.plot(pen='b')
        layout.addWidget(self.plot_pressure)

        # --- Plot 2 : Température ---
        self.plot_temperature = pg.PlotWidget(title="Température")
        self.plot_temperature.showGrid(x=True, y=True)
        self.plot_temperature.setLabel('left', 'Température (°C)')
        self.plot_temperature.setLabel('bottom', 'Temps')
        self.curve_temperature1 = self.plot_temperature.plot(pen='g')
        self.curve_temperature2 = self.plot_temperature.plot(pen='b')
        layout.addWidget(self.plot_temperature)

        # --- Plot 3 : vitesse ---
        self.plot_altitude = pg.PlotWidget(title="Vertical speed")
        self.plot_altitude.showGrid(x=True, y=True)
        self.plot_altitude.setLabel('left', 'speed (km/h)')
        self.plot_altitude.setLabel('bottom', 'Temps')
        self.curve_vspeed1 = self.plot_altitude.plot(pen='g')
        self.curve_vspeed2 = self.plot_altitude.plot(pen='b')
        layout.addWidget(self.plot_altitude)

    def update_data(self, t, altbaro1,temp1,vspeed1,altbaro2=None,altbaro3=None,temp2=None,vspeed2=None):
        self.times.append(t)
        
        self.altbaro1.append(altbaro1)
        self.altbaro2.append(altbaro2 if altbaro2 is not None else altbaro1)
        self.altbaro3.append(altbaro3 if altbaro3 is not None else altbaro1)
        
        self.temperature1.append(temp1)
        self.temperature2.append(temp2 if temp2 is not None else temp1)
        
        
        self.speed1.append(vspeed1)
        self.speed2.append(vspeed2 if vspeed2 is not None else vspeed1)

        # Mise à jour des courbes
        self.curve_altbaro1.setData(self.times, self.altbaro1)
        self.curve_altbaro2.setData(self.times, self.altbaro2)
        self.curve_altbaro3.setData(self.times, self.altbaro3)

        self.curve_temperature1.setData(self.times, self.temperature1)
        self.curve_temperature2.setData(self.times, self.temperature2)

        self.curve_vspeed1.setData(self.times, self.speed1)
        self.curve_vspeed2.setData(self.times, self.speed2)

        
