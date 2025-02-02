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

---

## Introduction
- C'est un projet de 2e année d'école d'ingénieur en électronique à l'[ENSEA](https://www.ensea.fr/fr) à Cergy.
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

| Date       | Objectif                                                                 | Check      |
|------------|--------------------------------------------------------------------------|------------|
| 17-09-2024 | Définir les objectifs du projet, planifier et organiser les tâches       | ✅ Terminé |
| 23-10-2024 | Schématique et routage fini → Commander le PCB V0                        | ✅ Terminé |
| 01-04-2025 | Livrable : Prototype CanSat Fonctionnel + Git final                      | 🔵 En cours|


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
- Changement dde la simulation python selon les conseils du jury.

### Séance 12, 10/12/24
- Modélisation Station sol 
<img src="./Docs/IMG/stationSol.png" alt="TOP" style="float: left; width: 45%">

- Test du système de vérrouillage sur la canette entière
- Soudage du PCB avec la partie accéléromètre

### Séance 13, 14/01/25
- Réunion mise au point de l'avancée du projet et nouvelles tâches assignées 
- Qt Hugo
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
<div style="display: flex; justify-content: space-between;">

<img src="./Docs/IMG/case v26 exterieur.jpg" alt="exterior" width="45%"/>
<img src="./Docs/IMG/case v26 interieur.jpg" alt="interior" width="45%"/>

## Livrables
### PCB
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


   ## boitier et design exterieur:
   <div style="display: flex; justify-content: space-between; align-items: center;">
    <img src="./Docs/IMG/pcb_v1_top.png" alt="TOP PCB" style="width: 45%;">
    <video style="width: 45%;" controls>
        <source src="https://github.com/user-attachments/assets/e469f96b-9811-490a-bc3c-5a03ff35372c" type="video/mp4">
        Your browser does not support the video tag.
    </video>
</div>
   



 
### Code



                                                                                   

### Rendu final
> Video ici 
