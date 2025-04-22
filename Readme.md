# Projet 2A CanSat 2024-2025 🚀

![Version](https://img.shields.io/badge/version-1.0-blue)
![Status](https://img.shields.io/badge/status-en%20développement-yellow)

## Table des Matières

- [Introduction](#introduction)
- [Équipe](#équipe)
- [Journal de bord](#journal-de-bord)
   - [Objectifs](#objectifs)
- [Livrable](#livrables)
   - [PCB](#pcb)
   - [Station sol](#station-sol)

---

## Introduction
- Il s'agit d'un projet de 2e année d'école d'ingénieur en électronique à l'[ENSEA](https://www.ensea.fr/fr) à Cergy.
- Le but étant de réaliser - pour la **compétiton CanSat** organisée par **Planète science et le CNES** - un satellite en forme de canette.
- Celui-ci devra **réaliser plusieurs missions** lorsqu'il sera **lâché d'un drone à envrion 120m de hauteur devant un jury**.
- Le projet consiste à faire la **conception** et la **fabrication** de ce satellite.
   - Partie électronique
   (PCB, capteurs, alimentation, station au sol,... )
   - Partie mécanique
   (Châssis, taille, matériau,... )
   - Partie résultats
   (Exploitation des mesures, les mettre en formes en développent ou non une interface, les présenter devant le jury,... )
- Les 4 missions principales imposées en plus du réglement sont :
   - **Mission 1 : Séparation & Transmission**
      - Le CanSat doit se séparer en deux parties à une altitude comprise entre 40 et 60 mètres.​
      - La chaîne de séparation doit résister à une traction de 30N.​
      - Transmission : ​une des parties du CanSat doit transmettre l’altitude et une donnée sur l’atmosphère en temps réel vers une station sol.
   - **Mission 2 : Rétrécissement​**
      - Réduire le CanSat au format 33 cl vous permettra de multiplier votre score par 2 sur la partie technique.
   - **Mission 3 : Instrumentation cassé​**
      - Trouvez un moyen de déterminer votre altitude à partir d’un capteur autre qu’un altimètre barométrique ou un GPS.
   - **Mission 4 : Distance​**
      - Intégrez de quoi mesurer la distance entre les deux parties du CanSat et la communiquer par télémétrie vers la station sol.
      - La Mission 3 n'intervient pas ici.


## Équipe

<div align="center">

   **🙋‍♂️ [Hugo CARVALHO FONTES](https://github.com/HugouShare)**

   **🙋‍♂️ [Louis VOZZOLA](https://github.com/louisvoz57700)**

   **🙋‍♂️ [Nelven THÉBAULT](https://github.com/NelvTheb)**

   **🙋‍♂️ [Mathieu POMMERY](https://github.com/mathieupommery)**

   **🙋‍♂️ [Younés BEN AMEUR](https://github.com/younsben)**

</div>

## Journal de bord

### Présentation orale
- Vous pouvez trouver ici le lien en consultation de l'ensemble de nos [présentations orales](https://enseafr-my.sharepoint.com/:p:/g/personal/nelven_thebault_ensea_fr/EQFq_InugDZNr_ZuubaXSGAB7BGOFKGy7X5Re6X6XMLlZA?e=Kv3kp6)

### Objectifs

| Date       | Objectif                                                                          | Check      |
|------------|-----------------------------------------------------------------------------------|------------|
| 17-09-2024 | Définir les objectifs du projet, planifier et organiser les tâches                | ✅ Terminé |
| 23-10-2024 | Schématique et routage fini → Commander le PCB V0                                 | ✅ Terminé |
| 27-04-2025 | Livrable : Prototype CanSat Fonctionnel + Git final (manque partie communication) | ✅ Terminé |


### Séance 1, 17/09/24
- Organisation du projet et son déroulement
- Plannification des livrables et définition des tâches
- Capteurs
   - 6 axes --> branchements OK, code OK, fonctionnement OK
   - baromètre --> prise en main : fonctionnement & branchements
   - thermomète--> compréhension de la datsaheet
### Séance 2, 24/09/24
- Modélisation CanSat 3D V0
- Prise en main du GPS
- Baromètre : écriture du code permettant de récupérer les données et les afficher
- Prise en main du thermomètre et tentative d'afficher la température
### Séance 3, 01/10/24
- Modifications modèle 3D
- Prise en main du module radio
- Avancement PCB
- Baromètre : finalisation du code permettant de récupérer les données et les afficher
- Affichage de la température
### Séance 4, 08/10/24
- 1er Passage oral
   - point sur la progression du projet
- Avancement PCB
### Séance 5, 15/10/24
- Visio (10/10/24) avec l'organisation du CanSat → nouvelles règles
- Redéfinition du projet et des tâches
- Intégration de la RPi 5 dans le projet 
   - Objectif : récupérer l'ensemble de données du CanSat et pouvoir les afficher et les extraire.

### Séance 6, 22/10/24
- Test du module Tarvos-III:

Nous avons essayés de faire communiquer l'EV-Board du Tarvos-III avec son antenne USB. Cependant nous n'avons pas réussi, en identifiant les 2 parties, on se rend vite compte d'un problème sur l'EV-Board car lorsqu'on lui envoi des informations (par exemple sa réinitialisation) la carte ne répond pas. Il y a surement un composant HS sur l'EV-Board
- Vérification et commande du PCB V0
- Avancement sur le module radio
- Test sur servo via contrôle PWM
- RPi5 : Génération d'une trame UART générée par STM32L476RG
- Schéma fonctionnel de la V0 :
<img src="./Docs/IMG/Schema Fct PCB v0.png" alt="TOP" style="float: left; width: 70%">

### Séance 7, 05/11/24
- Modélisation 3D Cannette et PCB
- RPi5 : Récupération d'une trame UART (générée par STM32L476RG) et affichage dans le terminal
- Avancement du module radio
   - Problème détecté sur l'EV-kit du Tharvos III (plus de réponse)
- Commencer la mise en commun des codes dans le Git
- Réception des PCBs
   - Main board
   - GPS et Radio
- Commande d'un nouveau Tharvos III et des composants du PCB
- Finalisation de la partie code du servomoteur
### Séance 8, 12/11/24
- 2e passage oral
   - MAJ du CdC
   - Présentation du PCB V0 (MAJ) reçu
   - Mise en forme de la stratégie de code
   - Présentation de la modélisation de la partie châssis et du mécanisme de vérrouillage
   - Présentation de l'avancement sur la rPi 5
   - Présentation du travail sur le servomoteur
- En attente des composants pour commencer soudure et pour continuer sur le Tharvos III
- RPi5 : Récupération d'une trame UART (générée par STM32L476RG) et affichage dans le terminal
- Avancement sur le code de la mainboard et de certains capteurs

### Séance 9, 19/11/24
- RPi5 : Écriture du code permettant la mise en forme des données UART reçues
- Amélioration de la mise en forme code pour le 6 axes
- Simulation Python de la chute de la canette
- Mécanique : Réalisation de la fermeture permettant aux 2 parties du CANSAT de se séparer.
   <img src="./Docs/IMG/meca.png" alt="MECA" style="float: left; width: 45%">

### Séance 10, 26/11/24
- Soudure des composants sur les sensors PCBs
   - Tarvos III
   - GPS

### Séance 11, 03/12/24
- 3e passage oral
- Test du nouveau servomoteur
- Début modélisation 3D de la station sol
- Changement de la simulation python selon les conseils du jury.

### Séance 12, 10/12/24
- Avancement programmation station sol Qt
- Test du système de vérrouillage sur la canette entière
- Soudage du PCB avec la partie accéléromètre
- Modélisation Station sol
<img src="./Docs/IMG/stationSol.png" alt="TOP" style="float: left; width: 45%">

### Séance 13, 14/01/25
- Réunion mise au point de l'avancée du projet et nouvelles tâches assignées 
- Avancement programmation station sol Qt + traitement des problèmes softwares
- Début de l'implémentation du code sur la mainboard
- Capteur laser pour mesurer la distance
   - Démontage d'un capteur de 50m pour l'adapter au CanSat

### Séance 14, 21/01/25
<div style="display: flex; justify-content: space-between;">

<img src="./Docs/IMG/Case_v20.png" alt="Case" width="45%"/>
<img src="./Docs/IMG/Case_v20_int.png" alt="Interior" width="45%"/>

</div>

- 3D station sol -> intégration dans le châssis des emplacements pour l'électronique
- Recherches sur un télémètre laser viable pour le projet (Mission 3)
- Réadaptation de l'interface graphique Qt + développement software station sol
- Réalisation d'un code pour envoyer une trame et en recevoir une en DMA sur le module de communication (Tarvos III)
- Début de la création de la machine à état avec pour l'instant un échantillonnage des capteurs tous les 0.25s et de l'envoi des données toutes les secondes
- Impression de la cannette avec les modifications faites

### Séance 15, 28/01/25

- Présentation orale
   - Retours :
      - Faire plus concis (20min max)
      - Attention à la visibilité du code et en réduire la quantité montrée
      - Respecter la table des matières
      - Changer la police
   - Objectifs pour le prochain oral :
      - Commencer traitement des données du 6 axes en plus du Radio/GPS 
      - Imprimer des parties de la station sol
      - Tester la vitesse de chute et la résistance du CanSat
      - Finir la soudure de la mainboard
      - Refaire carte GPS plus carte ecran led.
      - Tests avec la Tarvos III & Raspberry Pi 5
      - Avoir fait un choix pour le capteur de distance et l’avoir commandé (si besoin)
      - Faire fonctionner le baromètre
- Station sol
   - L'épaisseur des murs a été baissé
   - Prêt à être imprimé
- Teste de la mesure de distance via RSSI intégré au moduleradio Tharvos III
   - On abandone donc l'idée du capteur laser pour mesurer la distance
<div style="display: flex; justify-content: space-between;">

<img src="./Docs/IMG/case v26 exterieur.jpg" alt="exterior" width="45%"/>
<img src="./Docs/IMG/case v26 interieur.jpg" alt="interior" width="45%"/>

### Séance 16, 04/02/25
- Impression de la station sol + CanSat détaillé
- Nous continuons à implémenter le code sur notre mainboard
- Travail sur le software station sol
- Soudure de la main board
- Préparation pour première démo (partiellement fonctionnelle) pour
   - Portes ouvertes de l'ENSEA le 08/02/25
   - Réunion d'avancement sur le CanSat sur Paris avec l'organisation du concours le 09/02/25
### Séance 17, 28/01/25

- Nous cherchons des parachutes coherent pour nos deux parties, donc des parachutes qui permettent une vitesse de descenbte comprise entre 2m/s et 15m/s, il nous en faut donc deux, un pour le tout, un autre pour le bas, d'un poids de respectivement 300gr et 150gr, on choisit donc les parachutes en 28' 14' et 18' afin de faire des test:
- [Sierrafoxhobbies](https://www.sierrafoxhobbies.com/fr/parachute/2301-parachute-cansat-xchute-cs.html)


### Séance 18, 11/02/25
- Finalisation du programme de la station sol + validation 
- Commande parachute et connecteurs pour la station sols
- MAJ du modèle 3D de la station sol
- Soudures pour le bouton et l'afficheur de la batterie pour la station sol
- Modification du mécanisme pour réduire les frottements + impression et validation du test
- Montage du mécanisme complet et test du mécanisme avec canette fermée , Test concluant ✅

### Séance 19, 04/03/25
- 5e passage oral
   - Mieux sur la gestion du temps mais toujours au dessus de 20min (à cause des questions et échanges)
   - Sinon le reste des remarques a été corrigé.
   - Objectifs pour la prochaine séance
      - Avoir un POC (proof of concept) fonctionnel
      - Pouvoir utiliser la station sol
      - Avoir commencé le Poster du CanSat (requis pour la compétition)
- Nouvelles soudures/électronique pour la station sol
   - Réglage d'un problème de connection lié à une couche du PCB arrachée au niveau du bouton
   - Changement pour des câbles plus souples
   - Arrivée et bon fonctionnement des nappes en molex avec connectiques interchangeables pour $`\mu`$HDMI/HDMI et $`\mu`$USB/USB pour la Raspberry Pi 5


### Séance 20, 11/03/25
- Station sol
   - Modification du bouton général par un plus jolie avec LED
   - Découpe au laser d'une planche en MDF pour faire la face du dessus de la station sol
   - Réarrangement des pièces et impression finale - si tout marche bien lors de la prochaine séance - des pièces
- 1er test du CanSat
   - Test concluant du parachute en larguant le CanSat d'un drone à environ 30-50 m, reste à valider la vitesse de chute
- Poster de projet
   - Nous avons commencé l'affiche du projet (nécessaire pour la compétition), choix du support : feuille A3 en $`\LaTeX`$

## Séance 21, 18/03/25
- Station sol
   - Modilisation et impression des finitions 
   <img src="./Docs/IMG/finition v1 station sol.jpeg" alt="TOP" style="float: left; width: 45%">
- Canette
   - Ajout de fonctions (servomoteur) sur le bouton : appuie simple -> verrouille ; appuie long libère
   - Problème sur la liaison ST-Link avec le PCB problèmes pour téléverser le code
- Software
   - Optimisation du code et mise en place d'un bouton pour que lorsqu'on appuie, les 2 parties soient libérées, et un appuie long le mécanisme se referme
   - Test du baromètre et mise en place du code pour l'ouverture du Cansat à une certaine distance (testé et validé)  
     ![Demo](Docs/IMG/Mecanisme.gif)
- Canette
   - En fin de séance, nous avons tester l'ouverture du cansat avec déploiement du second parachute
  ![Demo](Docs/IMG/Largage.gif)

### Séance 22, 25/03/25
- Stockage
   - modélisation et impression boite pour les tharvos III
- Canette
   - Identification des problèmes liés au pcb (mauvaises résistances, ...)
   
### Séance 23, 01/04/25
- Présentation du POC

# Livrables
## PCB
<div style="display: flex; justify-content: space-between;">

<img src="./Docs/IMG/pcb v1 top.png" alt="TOP PCB" width="45%"/>
<img src="./Docs/IMG/pcb v1 bottom.png" alt="BOTTOM PCB" width="45%"/>

</div>

  Mainboard, pcb 6 couches avec:
  * accelerometre
  * stm32g431cbu6
  * regulateur buck 6-28v vers 5v
  * 2 ldo 5v vers 3.3v dont 1 activable sur commande du MCU.
  * port de carte micro sd
  * 2 port uart dédiée
  * port servo dédié
  * pads de soudures pour accéssoire et debug(swd)

    ### V1:
    ![image](https://github.com/user-attachments/assets/cd9e87d8-c2c3-4230-bf21-cc41be00fb7c)

    ![image](https://github.com/user-attachments/assets/d206f71e-0aa6-4205-9962-9d1119d65502)


    ### V2:
    ![image](https://github.com/user-attachments/assets/0bf2e5e1-f7a6-4f15-8080-7eab5975f991)

    ![image](https://github.com/user-attachments/assets/66e0acc7-4901-464f-aa7d-59c530588c99)

* Capteur de courant enlevé car peu utile.
* Barometre bmp581 precision relative:+/-2cm ! 
* connecteur carte a carte 10 et 12 contacts pour une reduction de la place !
* carte principale avec carte auxiliaires flexibles.

### Carte auxiliaire flexible:
   ![image](https://github.com/user-attachments/assets/cfff1de9-bf17-4cf1-af5d-3f29cedd3072)

   * Module radio tarvos.
   * module gps pa1010d.
   * connection swd rapide avec bouton de gestion des modes.
   * Double témoin lumineux adressable 16millions de couleurs.
   * connecteur 2 broche pour connecter l'indicateur de largage.

### Suite à de nombreux probleme long a debugger:
   ### V3:
![image](https://github.com/user-attachments/assets/e7469fed-02ef-4e96-ba3d-03e8abccf841)
![image](https://github.com/user-attachments/assets/8da8ab3d-ad4c-427f-91be-2d5945198fd7)
* Connecteurs plus robustes
* Connecteur de la batterie deporte
* Servomoteur uniquement soude


   
* But: revenir a une design plus simple et robuste avec des connecteurs plus gros, tout en gardant le design estetique et le plus fonctionnel et robuste.
## PCB sensor Vfinale:
   ![image](https://github.com/user-attachments/assets/f4664456-b87f-4ebb-b6fe-57d5ffe10136)

   * Nous avons donc sortie le barometre
   * Design plus simple mais robuste
   * Uniquement des soudures ou connecteurs robuste
   * Debug beaucoup plus rapide
   * Modulaire
   * Beaucoup de fonctionnalitee.

## Station Sol
### Boîtier et électronique
   ![Station_sol](./Docs/IMG/Station_sol%20finale.jpeg)
   Spécifications :
   - Raspberry Pi 5 intégré
   - Module radio Tarvos III (Würth Elektronik)
   - Écran tactile 7" connecté au Raspberry Pi 5
   - Batterie Li-Ion de 20 000 mAh avec BMS et prises intégrées
   - Indicateur de charge et de niveau de la batterie
   - Prise USB-C (femelle) de 9V⎓2A (18W) pour la recharge de la batterie
   - Bouton ON/OFF (LED allumée lorsque le Pi 5 est sous tension)
   - Emplacement dédié pour ranger le CanSat
   - Emplacement pour le parachute
   - Châssis interne imprimé en 3D en PLA
   - Façade en MDF découpée et gravée au laser
   - Accès simplifié à l’intérieur de la boîte grâce aux vis à main (capuchons noirs)

   Fichier 3D aller dans `./Hardware/Fichier%203D%20Station%20sol/` :
   - Fichier global avec tout l'historique : `Case.f3z`
   - Pour impression, découpage en 3 prints pour le châssis : `Case G.stl`,`Case D.stl`, `Range cable.stl`
   - Pour la découpe laser de la planche en bois : `Plaque bois Cansat_laser.dxf`
   - Le reste, caches et contours sont pour la finition
   
### Software
