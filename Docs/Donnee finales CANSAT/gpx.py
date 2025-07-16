import pandas as pd
from xml.etree.ElementTree import Element, SubElement, tostring
from xml.dom.minidom import parseString

def creer_gpx(df, nom_fichier_gpx):
    gpx = Element("gpx", version="1.1", creator="Python Script", xmlns="http://www.topografix.com/GPX/1/1")
    trk = SubElement(gpx, "trk")
    name = SubElement(trk, "name")
    name.text = nom_fichier_gpx.replace(".gpx", "")
    trkseg = SubElement(trk, "trkseg")

    for _, row in df.iterrows():
        lat, lon = row["latitude"], row["longitude"]
        if pd.notnull(lat) and pd.notnull(lon):
            trkpt = SubElement(trkseg, "trkpt", lat=str(lat), lon=str(lon))

    xml_str = parseString(tostring(gpx)).toprettyxml(indent="  ")
    with open(nom_fichier_gpx, "w", encoding="utf-8") as f:
        f.write(xml_str)

# Noms des colonnes (car pas d'en-tête dans les fichiers CSV)
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

# Lecture des fichiers CSV
top = pd.read_csv("TOP_02.csv", header=None, names=top_columns)
bot = pd.read_csv("BOT_02.csv", header=None, names=bot_columns)

# Création des fichiers GPX
creer_gpx(top, "TOP_02.gpx")
creer_gpx(bot, "BOT_02.gpx")

print("Fichiers GPX créés : TOP_02.gpx et BOT_02.gpx")
