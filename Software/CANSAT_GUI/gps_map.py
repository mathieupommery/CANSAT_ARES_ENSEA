from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl
import os

class GPSMapWidget(QWebEngineView):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.points = []

        # Charge le fichier HTML statique une seule fois
        map_path = os.path.abspath("map_template.html")
        self.load(QUrl.fromLocalFile(map_path))

    def add_point(self, lat, lon):
        self.points.append((lat, lon))
        js = f"addPoint({lat}, {lon});"
        self.page().runJavaScript(js)













# # gps_map.py
# from PyQt5.QtWebEngineWidgets import QWebEngineView
# from PyQt5.QtCore import QUrl
# import folium
# import os
# import tempfile
# 
# class GPSMapWidget(QWebEngineView):
#     def __init__(self, parent=None):
#         super().__init__(parent)
#         self.points = []
#         self.temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".html")
#         self._init_map()
# 
#     def _init_map(self):
#         self.map = folium.Map(location=[44.8416106, -0.5810938], zoom_start=2)
#         self._update_map()
# 
#     def add_point(self, lat, lon, update=False):
#         self.points.append((lat, lon))
#         if update:
#             self._update_map()
#         if len(self.points) % 10 == 0:
#             self.map_widget._update_map()
# 
#     def _update_map(self):
#         if not self.points:
#             self.map = folium.Map(location=[44.8416106, -0.5810938], zoom_start=2)
#         else:
#             # Centrer la carte sur le dernier point
#             self.map = folium.Map(location=self.points[-1], zoom_start=17)
# 
#             # Trace une ligne entre tous les points
#             folium.PolyLine(self.points, color="blue", weight=2.5, opacity=1).add_to(self.map)
# 
#             # Marque tous les points
#             for lat, lon in self.points:
#                 folium.CircleMarker(
#                     location=[lat, lon],
#                     radius=3,
#                     color='red',
#                     fill=True,
#                     fill_opacity=0.7
#                 ).add_to(self.map)
# 
#             # Adapter automatiquement le zoom pour contenir tous les points
#             lats, lons = zip(*self.points)
#             bounds = [[min(lats), min(lons)], [max(lats), max(lons)]]
#             self.map.fit_bounds(bounds)
# 
#         # Enregistrer dans le fichier temporaire et afficher
#         self.map.save(self.temp_file.name)
#         self.load(QUrl.fromLocalFile(self.temp_file.name))
