import numpy as np
from matplotlib import pyplot as plt
# Paramètre supplémentaire
temp_dep = 2.8 # Temps de déploiement du parachute (s)

# Réinitialisation des variables
h_initial=150
m=0.5
Diametre=0.40
g=9.81
A=(np.pi*(Diametre/2)**2)
dt=0.001
Cd=1.4
rho=1.22
t_max=50

#Liste pour valable l'entièreté de la canette avant la séparation



# Initialisation des variables pour les deux parties de la canette avant la séparation
time_avant_hsep = [0]  # Temps pour la partie avec parachute actif
altitude_avant_hsep = [h_initial]  # Altitude de cette partie
vitesse_avant_hsep = [0]  # Vitesse de cette partie

# Séparation de la canette

h_sep = 60  # Altitude de séparation (m)

time_apres_hsep_para = []  # Temps pour la partie avec parachute actif après la séparation des 2 modules
altitude_apres_hsep_para = []  # Altitude de cette partie


time_apres_hsep = []  # Temps pour la partie avec parachute qui se déploie après h_sep
altitude_apres_hsep = []  # Altitude de cette partie


# Phase avant séparation
t = 0
h = h_initial
v = 0
al=altitude_avant_hsep.copy()

while h > h_sep:
    if t < temp_dep:
        # Chute libre (avant déploiement du parachute)
        a = g  #chute libre
    else:
        # Chute avec parachute déployé
        Fd = 0.5 * Cd * rho * A * v**2
        a = g - (Fd / m)

    # Mise à jour de la vitesse et de l'altitude
    v = v + a * dt
    h = h - v * dt
    # Stockage des résultats pour la partie commune
    t += dt
    time_avant_hsep.append(t)
    altitude_avant_hsep.append(h)
    t_sep=t

# Après séparation (parachute déjà actif)



h1 = h_sep
v1 = v

while h1 > 0:
    # Chute avec parachute actif
    Fd1 = 0.5 * Cd * rho * A * v1**2
    a1 = g - (Fd1 / (m / 2))  # Masse divisée par 2 après séparation
    v1 = v1 + a1 * dt
    h1 = h1 - v1 * dt
    t += dt
    time_apres_hsep_para.append(t)
    altitude_apres_hsep_para.append(max(h1, 0))  # Évite une altitude négative

# Après séparation (partie 2 : parachute déployé après un délai)
t2 = t_sep
h2 = h_sep
v2 = v


while h2 > 0:
    if t2 - t_sep < temp_dep :
        # Chute libre
        a2 = g
        h_sep_para=h2
    else:
        # Chute avec parachute actif
        Fd2 = 0.5 * Cd * rho * A * v2**2
        a2 = g - (Fd2 / (m / 2))  # Masse divisée par 2 après séparation

    # Mise à jour de la vitesse et de l'altitude
    v2 = v2 + a2 * dt
    h2 = h2 - v2 * dt
    t2 += dt
    time_apres_hsep.append(t2)
    altitude_apres_hsep.append(max(h2, 0))  # Évite une altitude négative


# Il faut que la vitesse des deux 2 modules n'excèdent pas 15m/s au moment de se poser au sol pour des conditions de sécurité.
print(v1)
print(v2)

# Tracé des deux graphiques
plt.figure(figsize=(12, 6))

# Graphique pour la partie avec parachute actif immédiatement
plt.subplot(1, 2, 1)
plt.plot(time_avant_hsep, altitude_avant_hsep,color="green", label="Altitude avant séparation ")
plt.plot(time_apres_hsep_para, altitude_apres_hsep_para,color="blue", label="Altitude partie haute")
plt.plot(time_apres_hsep,altitude_apres_hsep, label="Altitude partie basse", color="yellow")
plt.axhline(h_sep, color="green", linestyle="--", label="Séparation de la cannette")
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Partie avec parachute actif immédiatement")
plt.legend()
plt.grid()

# Graphique pour la partie avec parachute déployé après délai
plt.subplot(1, 2, 2)
plt.plot(time_apres_hsep, altitude_apres_hsep, label="Altitude (m)", color="Yellow")
plt.axhline(h_sep_para, color="brown", linestyle="--", label="Altitude de déploiement du parachute")
plt.axvline(t_sep+temp_dep, color="red", linestyle="--", label="Temps de déploiement du parachute")
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Partie avec parachute déployé après délai")
plt.legend()
plt.grid()

plt.show()
