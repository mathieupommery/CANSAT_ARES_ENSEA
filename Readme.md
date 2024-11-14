# Projet 2A CanSat 2024-2025 🚀

![Version](https://img.shields.io/badge/version-1.0-blue)
![Status](https://img.shields.io/badge/status-en%20développement-yellow)

## Table des Matières

- [Introduction](#introduction)
- [Équipe](#équipe)
- [Journal de bord](#journal-de-bord)
- [Livrable](#livrables)

---

# Introduction
- C'est un projet de 2e année d'école d'ingénieur en électronique à l'ENSEA à Cergy.
- Le but étant de réaliser - pour la compétiton CanSat organisée par Planète science et le CNES - un satellite en forme de canette.
- Celui-ci devra réaliser plusieurs missions lorsqu'il sera lâché d'un drone à envrion 120m de hauteur devant un jury.
- Le projet consiste à faire la conception et la fabrication de ce satellite.
   - Partie électronique (PCB, capteurs, alimentation, station au sol,...)
   - Partie mécanique (Châssis, taille, matériau,...)

# Équipe

<div align="center">

   **🙋‍♂️ Hugo CARVALHO FONTES**

   **🙋‍♂️ ![Louis VOZZOLA](https://github.com/louisvoz57700)**

   **🙋‍♂️ ![Nelven THÉBAULT](https://github.com/NelvTheb)**

   **🙋‍♂️ ![Mathieu POMMERY](https://github.com/mathieupommery)**

   **🙋‍♂️ Younés BEN AMEUR**

</div>

# Journal de bord

## Séance 1, 17/09/24
- Organisation du projet et son déroulement
- Plannification des livrables et définition des tâches
- Capteurs
   - 6 axes --> branchements OK, code OK, fonctionnement OK
## Séance 2, 24/09/24
- Modélisation CanSat 3D V0
- Prise en main du GPS
## Séance 3, 01/10/24
- Modifications modèle 3D
- Prise en main du module radio
- Avancement PCB
## Séance 4, 08/10/24
- 1er Passage oral
   - point sur la progression du projet
- Avancement PCB
## Séance 5, 15/10/24
- Visio (10/10/24) avec l'organisation du CanSat --> nouvelles règles
- Redéfinition du projet et des tâches

## Séance 6, 22/10/24
- Test du module Tarvos-III:

Nous avons essayés de faire communiquer l'EV-Board du Tarvos-III avec son antenne USB. Cependant nous n'avons pas réussi, en identifiant les 2 parties, on se rend vite compte d'un problème sur l'EV-Board car lorsqu'on lui envoi des informations (par exemple sa réinitialisation) la carte ne répond pas. Il y a surement un composant HS sur l'EV-Board
- Vérification et commande du PCB V0
- Avancement sur le module radio
- Test sur servo via contrôle PWM
- Schéma fonctionnel de la V0 :
<img src="./IMG/Schema Fct PCB v0.png" alt="TOP" style="float: left; width: 45%">

## Séance 7, 05/11/24
- Modélisation 3D Cannette et PCB
- Prise en main de la rPi 5
   - But final récupérer l'ensemble de données du CanSat et pouvoir les afficher/extraire.
- Avancement du module radio
   - Problème détecté sur l'EV-kit du Tharvos III (plus de réponse)
- Commencer la mise en commun des codes dans le Git
- Réception des PCBs
   - Main board
   - GPS et Radio

## Séance 8, 12/11/24
- 2e passage oral
   - MAJ du CdC
   - Commande du PCV V0 (MAJ)
   - Mise en forme de la stratégie de code
- 

## Séance 9, 19/11/24


## Séance 10, 26/11/24


## Séance 11, 03/12/24
- 3e passage oral
   - 

# Livrables
## PCB
   <img src="./IMG/pcb v1 top.png" alt="TOP" style="float: left; width: 45%">
   <img src="./IMG/pcb v1 bottom.png" alt="BOT" style="float: right; width: 45%">

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


 
## Code




                                                                                   

## Rendu final
> Video ici de votre projet
