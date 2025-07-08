# orientation_3d.py
from PyQt5.QtWidgets import QWidget, QHBoxLayout
import pyqtgraph.opengl as gl
from pyqtgraph.opengl import GLViewWidget, GLMeshItem
from pyqtgraph import Vector
import numpy as np
import math, time
import trimesh
import random

class Orientation3DWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        layout = QHBoxLayout()
        self.setLayout(layout)

        # Deux vues 3D
        self.view1 = self._create_view()
        self.view2 = self._create_view()
        layout.addWidget(self.view1)
        layout.addWidget(self.view2)
        self.orientation1 = np.array([0.0, 0.0, 0.0])  # roll, pitch, yaw pour mesh1
        self.orientation2 = np.array([0.0, 0.0, 0.0])
        # Charge les modèles UNE FOIS
        mesh1 = trimesh.load_mesh("models/haut_can.stl")
        mesh2 = trimesh.load_mesh("models/bas_can.stl")

        # Création des objets 3D
        self.mesh1 = self._create_mesh_item(mesh1, color=(1, 0, 0, 1))  # Rouge
        self.mesh2 = self._create_mesh_item(mesh2, color=(0, 0, 1, 1))  # Bleu

        self.view1.addItem(self.mesh1)
        self.view2.addItem(self.mesh2)

        # Centre les vues
        self.center_view_on_model(self.view1, mesh1)
        self.center_view_on_model(self.view2, mesh2)



    def _create_view(self):
        view = GLViewWidget()
        view.opts['distance'] = 60 # Valeur par défaut
        grid = gl.GLGridItem()
        grid.scale(1, 1, 1)
        view.addItem(grid)
        return view
    def _create_mesh_item(self, mesh: trimesh.Trimesh, color=(0.5, 0.7, 1, 1)):
        verts = np.array(mesh.vertices)
        faces = np.array(mesh.faces)
        return gl.GLMeshItem(vertexes=verts, faces=faces, smooth=True, color=color, shader="shaded", drawEdges=True)

    def center_view_on_model(self, view: GLViewWidget, mesh: trimesh.Trimesh):
        center = mesh.bounding_box.centroid
        size = mesh.bounding_box.extents
        max_dim = max(size)

        # Centre la vue
        view.opts['center'] = Vector(*center)
    
        # Distance adaptée au modèle
        view.setCameraPosition(distance=max_dim * 2.5, elevation=20, azimuth=45)


    def _create_custom_model(self, model_path, color=(0.5, 0.7, 1, 1)):
        mesh = trimesh.load_mesh(model_path)
        verts = np.array(mesh.vertices)
        faces = np.array(mesh.faces)
        mesh_item = gl.GLMeshItem(vertexes=verts, faces=faces, smooth=True, color=color, shader="shaded", drawEdges=True)
        return mesh_item  

    def update_orientation(self, mesh_id, gx, gy, gz, dt):

        # Intégration simple des vitesses pour obtenir l’angle
        delta_angles = np.array([gx, gy, gz]) * dt

        if mesh_id == 1:
            self.orientation1 += delta_angles
            roll, pitch, yaw = self.orientation1
            self.mesh1.resetTransform()
            self.mesh1.rotate(roll, 1, 0, 0)
            self.mesh1.rotate(pitch, 0, 1, 0)
            self.mesh1.rotate(yaw, 0, 0, 1)

        elif mesh_id == 2:
            self.orientation2 += delta_angles
            roll, pitch, yaw = self.orientation2
            self.mesh2.resetTransform()
            self.mesh2.rotate(roll, 1, 0, 0)
            self.mesh2.rotate(pitch, 0, 1, 0)
            self.mesh2.rotate(yaw, 0, 0, 1)

        self.update()


        