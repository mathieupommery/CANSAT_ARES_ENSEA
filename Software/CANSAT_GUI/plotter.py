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
    def __init__(self, parent=None, buffer_size=300):
        super().__init__(parent)

        self.buffer_size = buffer_size

        self.times = deque(maxlen=buffer_size)
        self.pressure = deque(maxlen=buffer_size)
        self.temperature = deque(maxlen=buffer_size)
        self.altitude = deque(maxlen=buffer_size)

        layout = QVBoxLayout()
        layout.setSpacing(10)
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

        # --- Plot 1 : Pression ---
        self.plot_pressure = pg.PlotWidget(title="Pression")
        self.plot_pressure.showGrid(x=True, y=True)
        self.plot_pressure.setLabel('left', 'Pression (Pa)')
        self.plot_pressure.setLabel('bottom', 'Temps')
        self.curve_pressure = self.plot_pressure.plot(pen='r')
        layout.addWidget(self.plot_pressure)

        # --- Plot 2 : Température ---
        self.plot_temperature = pg.PlotWidget(title="Température")
        self.plot_temperature.showGrid(x=True, y=True)
        self.plot_temperature.setLabel('left', 'Température (°C)')
        self.plot_temperature.setLabel('bottom', 'Temps')
        self.curve_temperature = self.plot_temperature.plot(pen='g')
        layout.addWidget(self.plot_temperature)

        # --- Plot 3 : Altitude ---
        self.plot_altitude = pg.PlotWidget(title="Altitude")
        self.plot_altitude.showGrid(x=True, y=True)
        self.plot_altitude.setLabel('left', 'Altitude (m)')
        self.plot_altitude.setLabel('bottom', 'Temps')
        self.curve_altitude = self.plot_altitude.plot(pen='b')
        layout.addWidget(self.plot_altitude)

    def update_data(self, t, p, temp, alt):
        self.times.append(t)
        self.pressure.append(p)
        self.temperature.append(temp)
        self.altitude.append(alt)

        self.curve_pressure.setData(self.times, self.pressure)
        self.curve_temperature.setData(self.times, self.temperature)
        self.curve_altitude.setData(self.times, self.altitude)