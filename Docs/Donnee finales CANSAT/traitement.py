import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_theme(style="whitegrid")
plt.rcParams["figure.figsize"] = (12, 6)

# Colonnes des fichiers
top_columns = [
    "flagcalib", "flagdrop", "flagseparation", "latitude", "longitude",
    "altitudegps", "altitudebaro", "groundspeed", "verticalspeed", "pressionbaro",
    "AccelX", "accelY", "accelZ", "nbsat", "temp", "vbat", "timeindex"
]

bot_columns = [
    "flagcalib", "flagdrop", "flagseparation", "latitude", "longitude",
    "altitudegps", "altitudebaro", "groundspeed", "verticalspeed", "pressionbaro",
    "accelX", "accelY", "accelZ", "distanceentremodules", "nbsat", "temp", "vbat", "timeindex"
]

sat_columns = ["temp", "vbat", "RSSI", "distance_RSSI", "timeindex"]

# Chargement des fichiers
top = pd.read_csv("TOP_02.csv", header=None, names=top_columns)
bot = pd.read_csv("BOT_02.csv", header=None, names=bot_columns)
sat = pd.read_csv("SAT_01.csv", header=None, names=sat_columns)

# Ajout de la colonne "secondes"
for df in [top, bot, sat]:
    df["secondes"] = df["timeindex"] * 0.2

# Fonction de sélection du temps max
def limiter_duree(df):
    try:
        tmax = float(input("Entrez la durée maximale à tracer (en secondes, ou vide pour tout tracer) : ") or 999999)
        return df[df["secondes"] <= tmax].copy()
    except ValueError:
        return df.copy()

# Fonction simple de tracé
def trace_valeur(x, y, label, color=None):
    plt.plot(x.values, y.values, label=label, color=color)

# Ajoute un fond coloré après séparation si flagseparation existe
def ajouter_fond_separation(df):
    if "flagseparation" in df.columns:
        try:
            idx_sep = df[df["flagseparation"] == 1].index[0]
            x_sep = df.loc[idx_sep, "secondes"]
            plt.axvspan(x_sep, df["secondes"].max(), color='lightgray', alpha=0.3, label="Après séparation")
        except IndexError:
            pass  # Pas de séparation détectée

### === GRAPHIQUES === ###

# 1. RSSI et Distance RSSI
sat_limited = limiter_duree(sat)
plt.figure()
trace_valeur(sat_limited["secondes"], sat_limited["RSSI"], "RSSI(dbm)")
trace_valeur(sat_limited["secondes"], sat_limited["distance_RSSI"], "Distance RSSI(m)")
plt.xlabel("Temps (s)")
plt.ylabel("Valeurs")
plt.title("RSSI & Distance RSSI")
plt.legend()
plt.tight_layout()
plt.show()

# 2. Distance entre modules + flag séparation
bot_limited = limiter_duree(bot)
plt.figure()
trace_valeur(bot_limited["secondes"], bot_limited["distanceentremodules"], "Distance entre modules(m)")
trace_valeur(bot_limited["secondes"], bot_limited["flagseparation"] * bot_limited["distanceentremodules"].max(), "Flag séparation", "gray")
ajouter_fond_separation(bot_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Distance")
plt.title("Distance entre modules & Séparation")
plt.legend()
plt.tight_layout()
plt.show()

# 3. Altitude GPS & Baro - TOP
top_limited = limiter_duree(top)
plt.figure()
trace_valeur(top_limited["secondes"], top_limited["altitudegps"], "Altitude GPS (m)(TOP)")
trace_valeur(top_limited["secondes"], top_limited["altitudebaro"], "Altitude Baro (m)(TOP)")
ajouter_fond_separation(top_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Altitude GPS & Baro - TOP")
plt.legend()
plt.tight_layout()
plt.show()

# 4. Altitude GPS & Baro - BOT
bot_limited = limiter_duree(bot)
plt.figure()
trace_valeur(bot_limited["secondes"], bot_limited["altitudegps"], "Altitude GPS (m)(BOT)")
trace_valeur(bot_limited["secondes"], bot_limited["altitudebaro"], "Altitude Baro (m)(BOT)")
ajouter_fond_separation(bot_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Altitude GPS & Baro - BOT")
plt.legend()
plt.tight_layout()
plt.show()

# 5. Altitude Baro - TOP vs BOT
top_limited = limiter_duree(top)
bot_limited = limiter_duree(bot)
plt.figure()
trace_valeur(top_limited["secondes"], top_limited["altitudebaro"], "Altitude Baro (TOP)")
trace_valeur(bot_limited["secondes"], bot_limited["altitudebaro"], "Altitude Baro (BOT)")
ajouter_fond_separation(top_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Altitude (m)")
plt.title("Altitude Baro - TOP vs BOT")
plt.legend()
plt.tight_layout()
plt.show()

# 6. Vitesse - TOP
top_limited = limiter_duree(top)
plt.figure()
trace_valeur(top_limited["secondes"], top_limited["verticalspeed"], "Vertical Speed (TOP)")
trace_valeur(top_limited["secondes"], top_limited["groundspeed"], "Ground Speed (TOP)")
ajouter_fond_separation(top_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Vitesse (m/s)")
plt.title("Vitesses - TOP")
plt.legend()
plt.tight_layout()
plt.show()

# 7. Vitesse - BOT
bot_limited = limiter_duree(bot)
plt.figure()
trace_valeur(bot_limited["secondes"], bot_limited["verticalspeed"], "Vertical Speed (BOT)")
trace_valeur(bot_limited["secondes"], bot_limited["groundspeed"], "Ground Speed (BOT)")
ajouter_fond_separation(bot_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Vitesse (m/s)")
plt.title("Vitesses - BOT")
plt.legend()
plt.tight_layout()
plt.show()

# 8. Accélérations - TOP
top_limited = limiter_duree(top)
plt.figure()
trace_valeur(top_limited["secondes"], top_limited["AccelX"], "AccelX (TOP)")
trace_valeur(top_limited["secondes"], top_limited["accelY"], "AccelY (TOP)")
trace_valeur(top_limited["secondes"], top_limited["accelZ"], "AccelZ (TOP)")
ajouter_fond_separation(top_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Accélération (m/s²)")
plt.title("Accélérations - TOP")
plt.legend()
plt.tight_layout()
plt.show()

# 9. Accélérations - BOT
bot_limited = limiter_duree(bot)
plt.figure()
trace_valeur(bot_limited["secondes"], bot_limited["accelX"], "AccelX (BOT)")
trace_valeur(bot_limited["secondes"], bot_limited["accelY"], "AccelY (BOT)")
trace_valeur(bot_limited["secondes"], bot_limited["accelZ"], "AccelZ (BOT)")
ajouter_fond_separation(bot_limited)
plt.xlabel("Temps (s)")
plt.ylabel("Accélération (m/s²)")
plt.title("Accélérations - BOT")
plt.legend()
plt.tight_layout()
plt.show()



# import pandas as pd
# import matplotlib.pyplot as plt
# import seaborn as sns
# 
# sns.set_theme(style="whitegrid")
# plt.rcParams["figure.figsize"] = (12, 6)
# 
# # Colonnes des fichiers
# top_columns = [
#     "flagcalib", "flagdrop", "flagseparation", "latitude", "longitude",
#     "altitudegps", "altitudebaro", "groundspeed", "verticalspeed", "pressionbaro",
#     "AccelX", "accelY", "accelZ", "nbsat", "temp", "vbat", "timeindex"
# ]
# 
# bot_columns = [
#     "flagcalib", "flagdrop", "flagseparation", "latitude", "longitude",
#     "altitudegps", "altitudebaro", "groundspeed", "verticalspeed", "pressionbaro",
#     "accelX", "accelY", "accelZ", "distanceentremodules", "nbsat", "temp", "vbat", "timeindex"
# ]
# 
# sat_columns = ["temp", "vbat", "RSSI", "distance_RSSI", "timeindex"]
# 
# # Chargement des fichiers
# top = pd.read_csv("TOP_02.csv", header=None, names=top_columns)
# bot = pd.read_csv("BOT_02.csv", header=None, names=bot_columns)
# sat = pd.read_csv("SAT_02.csv", header=None, names=sat_columns)
# 
# # Ajout de la colonne "secondes"
# for df in [top, bot, sat]:
#     df["secondes"] = df["timeindex"] * 0.2
# 
# # Fonction de sélection du temps max
# def limiter_duree(df):
#     try:
#         tmax = float(input("Entrez la durée maximale à tracer (en secondes, ou vide pour tout tracer) : ") or 999999)
#         return df[df["secondes"] <= tmax].copy()
#     except ValueError:
#         return df.copy()
# 
# # Fonction simple de tracé
# def trace_valeur(x, y, label, color=None):
#     plt.plot(x.values, y.values, label=label, color=color)
# 
# ### === GRAPHIQUES === ###
# 
# # 1. RSSI et Distance RSSI
# sat_limited = limiter_duree(sat)
# plt.figure()
# trace_valeur(sat_limited["secondes"], sat_limited["RSSI"], "RSSI(dbm)")
# trace_valeur(sat_limited["secondes"], sat_limited["distance_RSSI"], "Distance RSSI(m)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Valeurs")
# plt.title("RSSI & Distance RSSI")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 2. Distance entre modules + flag séparation
# bot_limited = limiter_duree(bot)
# plt.figure()
# trace_valeur(bot_limited["secondes"], bot_limited["distanceentremodules"], "Distance entre modules(m)")
# trace_valeur(bot_limited["secondes"], bot_limited["flagseparation"] * bot_limited["distanceentremodules"].max(), "Flag séparation", "gray")
# plt.xlabel("Temps (s)")
# plt.ylabel("Distance")
# plt.title("Distance entre modules & Séparation")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 3. Altitude GPS & Baro - TOP
# top_limited = limiter_duree(top)
# plt.figure()
# trace_valeur(top_limited["secondes"], top_limited["altitudegps"], "Altitude GPS (m)(TOP)")
# trace_valeur(top_limited["secondes"], top_limited["altitudebaro"], "Altitude Baro (m)(TOP)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Altitude (m)")
# plt.title("Altitude GPS & Baro - TOP")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 4. Altitude GPS & Baro - BOT
# bot_limited = limiter_duree(bot)
# plt.figure()
# trace_valeur(bot_limited["secondes"], bot_limited["altitudegps"], "Altitude GPS (m)(BOT)")
# trace_valeur(bot_limited["secondes"], bot_limited["altitudebaro"], "Altitude Baro (m)(BOT)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Altitude (m)")
# plt.title("Altitude GPS & Baro - BOT")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 5. Altitude Baro - TOP vs BOT
# top_limited = limiter_duree(top)
# bot_limited = limiter_duree(bot)
# plt.figure()
# trace_valeur(top_limited["secondes"], top_limited["altitudebaro"], "Altitude Baro (TOP)")
# trace_valeur(bot_limited["secondes"], bot_limited["altitudebaro"], "Altitude Baro (BOT)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Altitude (m)")
# plt.title("Altitude Baro - TOP vs BOT")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 6. Vitesse - TOP
# top_limited = limiter_duree(top)
# plt.figure()
# trace_valeur(top_limited["secondes"], top_limited["verticalspeed"], "Vertical Speed (TOP)")
# trace_valeur(top_limited["secondes"], top_limited["groundspeed"], "Ground Speed (TOP)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Vitesse (m/s)")
# plt.title("Vitesses - TOP")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 7. Vitesse - BOT
# bot_limited = limiter_duree(bot)
# plt.figure()
# trace_valeur(bot_limited["secondes"], bot_limited["verticalspeed"], "Vertical Speed (BOT)")
# trace_valeur(bot_limited["secondes"], bot_limited["groundspeed"], "Ground Speed (BOT)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Vitesse (m/s)")
# plt.title("Vitesses - BOT")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 8. Accélérations - TOP
# top_limited = limiter_duree(top)
# plt.figure()
# trace_valeur(top_limited["secondes"], top_limited["AccelX"], "AccelX (TOP)")
# trace_valeur(top_limited["secondes"], top_limited["accelY"], "AccelY (TOP)")
# trace_valeur(top_limited["secondes"], top_limited["accelZ"], "AccelZ (TOP)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Accélération (m/s²)")
# plt.title("Accélérations - TOP")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 
# # 9. Accélérations - BOT
# bot_limited = limiter_duree(bot)
# plt.figure()
# trace_valeur(bot_limited["secondes"], bot_limited["accelX"], "AccelX (BOT)")
# trace_valeur(bot_limited["secondes"], bot_limited["accelY"], "AccelY (BOT)")
# trace_valeur(bot_limited["secondes"], bot_limited["accelZ"], "AccelZ (BOT)")
# plt.xlabel("Temps (s)")
# plt.ylabel("Accélération (m/s²)")
# plt.title("Accélérations - BOT")
# plt.legend()
# plt.tight_layout()
# plt.show()
# 