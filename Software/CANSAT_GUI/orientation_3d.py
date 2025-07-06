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
        
        mesh1 = trimesh.load_mesh("models/haut_can.stl")
        verts1 = np.array(mesh1.vertices)
        faces1 = np.array(mesh1.faces)

        mesh2 = trimesh.load_mesh("models/bas_can.stl")
        verts2 = np.array(mesh2.vertices)
        faces2 = np.array(mesh2.faces)

        # Deux objets 3D (cubes ici)
        self.mesh1 = self._create_custom_model("models/haut_can.stl")  # Rouge
        self.view1.addItem(self.mesh1)
        self.center_view_on_model(self.view1, trimesh.load_mesh("models/haut_can.stl"))

        self.mesh2 = self._create_custom_model("models/bas_can.stl")  # Bleu
        self.view2.addItem(self.mesh2)
        self.center_view_on_model(self.view2, trimesh.load_mesh("models/bas_can.stl"))


    def _create_view(self):
        view = GLViewWidget()
        view.opts['distance'] = 100 # Valeur par défaut
        grid = gl.GLGridItem()
        grid.scale(1, 1, 1)
        view.addItem(grid)
        return view
    def center_view_on_model(self, view: GLViewWidget, mesh: trimesh.Trimesh):
        center = mesh.bounding_box.centroid
        size = mesh.bounding_box.extents
        max_dim = max(size)

        # Centrer la vue et adapter la distance caméra
        view.opts['center'] = Vector(*center)
        view.setCameraPosition(distance=max_dim * 2.5)


    def _create_custom_model(self, model_path, color=(0.5, 0.7, 1, 1)):
        mesh = trimesh.load_mesh(model_path)
        verts = np.array(mesh.vertices)
        faces = np.array(mesh.faces)
        mesh_item = gl.GLMeshItem(vertexes=verts, faces=faces, smooth=True, color=color, shader="shaded", drawEdges=True)
        return mesh_item  

    def set_orientation(self, mesh_id, roll, pitch, yaw):
        R = self._euler_to_matrix(roll, pitch, yaw)

        # PyQtGraph veut une matrice 4x4 (homogène)
        M = np.eye(4)
        M[:3, :3] = R

        if mesh_id == 1:
            self.mesh1.resetTransform()
            self.mesh1.rotate(roll * 180/np.pi, 1, 0, 0)
            self.mesh1.rotate(pitch * 180/np.pi, 0, 1, 0)
            self.mesh1.rotate(yaw * 180/np.pi, 0, 0, 1)
        elif mesh_id == 2:
            self.mesh2.resetTransform()
            self.mesh2.rotate(roll * 180/np.pi, 1, 0, 0)
            self.mesh2.rotate(pitch * 180/np.pi, 0, 1, 0)
            self.mesh2.rotate(yaw * 180/np.pi, 0, 0, 1)

        self.update()


    def _euler_to_matrix(self, roll, pitch, yaw):
        # Pour plus tard, si on veut utiliser des matrices plutôt que rotate()
        Rx = np.array([[1, 0, 0],
                       [0, np.cos(roll), -np.sin(roll)],
                       [0, np.sin(roll), np.cos(roll)]])
        Ry = np.array([[np.cos(pitch), 0, np.sin(pitch)],
                       [0, 1, 0],
                       [-np.sin(pitch), 0, np.cos(pitch)]])
        Rz = np.array([[np.cos(yaw), -np.sin(yaw), 0],
                       [np.sin(yaw), np.cos(yaw), 0],
                       [0, 0, 1]])
        return Rz @ Ry @ Rx
    def _update_orientation(self):
    # Par exemple, ici avec des angles simulés ou issus d'une trame série
    
        t = time.time()
        roll = math.sin(t) * 0.5
        pitch = math.cos(t) * 0.3
        yaw = math.sin(t * 0.7) * 0.8
        self.set_orientation(1, roll, pitch, yaw)

        # Pour le 2e objet :
        self.set_orientation(2, -roll, -pitch, yaw)
    def _simulate_orientation(self):
        
        t = time.time()
        self.set_orientation(1, math.sin(t)*0.5, math.cos(t)*0.3, math.sin(t*0.7)*0.8)
        self.set_orientation(2, math.cos(t)*0.2, math.sin(t*0.6)*0.4, math.sin(t)*0.5)
        