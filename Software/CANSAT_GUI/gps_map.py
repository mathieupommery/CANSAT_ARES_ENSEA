# gps_map.py
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl
import folium
import os
import tempfile

class GPSMapWidget(QWebEngineView):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.points = []
        self.temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".html")
        self._init_map()

    def _init_map(self):
        self.map = folium.Map(location=[0, 0], zoom_start=2)
        self._update_map()

    def add_point(self, lat, lon):
        self.points.append((lat, lon))
        self._update_map()

    def _update_map(self):
        self.map = folium.Map(zoom_start=17)

        if self.points:
            folium.PolyLine(self.points, color="blue", weight=2.5, opacity=1).add_to(self.map)
            for lat, lon in self.points:
                folium.CircleMarker([lat, lon], radius=3, color='red', fill=True).add_to(self.map)

            # Adapter la vue à tous les points
            self.map.fit_bounds([min(self.points), max(self.points)])
        else:
            self.map.location = [0, 0]
            self.map.zoom_start = 2

        self.map.save(self.temp_file.name)
        self.load(QUrl.fromLocalFile(self.temp_file.name))
