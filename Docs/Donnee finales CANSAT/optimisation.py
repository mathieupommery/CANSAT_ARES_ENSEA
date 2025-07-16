import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Charger les données depuis le fichier unique
df = pd.read_csv("opt.csv", header=None, names=["altitude_baro", "RSSI"])

# Générer l'axe temporel basé sur un pas de 200 ms
df["secondes"] = np.arange(len(df)) * 0.2

# Définir la fonction modèle non linéaire : altitude en fonction du RSSI
def rssi_to_altitude(RSSI, RSSI0, n):
    return 10 ** ((RSSI0 - RSSI) / (10 * n))

# Altitude estimée avec les paramètres initiaux
altitude_init = rssi_to_altitude(df["RSSI"], -54, 2.5)

# Optimisation des paramètres RSSI0 et n
params_opt, _ = curve_fit(rssi_to_altitude, df["RSSI"], df["altitude_baro"], p0=[-54, 2.5])
RSSI0_opt, n_opt = params_opt

# Altitude estimée après optimisation
altitude_opt = rssi_to_altitude(df["RSSI"], RSSI0_opt, n_opt)

# Tracer les courbes
plt.figure(figsize=(12, 6))
plt.plot(df["secondes"], df["altitude_baro"], label="Altitude Baro mesurée", color="blue")
plt.plot(df["secondes"], altitude_init, label="Altitude RSSI initiale (RSSI0=-54, n=2.5)", linestyle="--", color="orange")
plt.plot(df["secondes"], altitude_opt, label=f"Altitude RSSI optimisée (RSSI0={RSSI0_opt:.2f}, n={n_opt:.2f})", linestyle="--", color="green")
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Comparaison Altitude Baro vs RSSI (initiale et optimisée)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
