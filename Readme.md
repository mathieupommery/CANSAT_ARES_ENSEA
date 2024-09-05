# <Notre Projet>
Après de nombreuses randonnées effectuées en zone relativement reculée, il nous est apparu qu'un équipement de sécurité portatif d'extrême urgence et minimaliste, à utiliser que si nous n'avons plus de batterie sur l'ensemble de nos autres équipements, permettant de se localiser par rapport à ce qui nous entoure est nécéssaire et peut sauver des vies dans bien des cas. Après réflexion, cet équipement se doit d'être le plus petit possible afin qu'il se fasse oublier sauf lorsque l'on en a besoin, de plus, afin de ne pas l'oublier, il devra être raccroché à quelque chose que l'on n'oublie pas. C'est pour cela que nous avons décider d'en faire un porte-clé, nous aurons donc une balise GPS, avec un écran, ainsi qu'un compas le tout sur un circuit le plus petit possible et comprenant une batterie.
Le cahier des charges est assez simple:
* Taille suffisament petit afin d'en faire un porte clé moyen, donc aucune dimension ne doit excéder la longueur d'une clé de maison.
* Poids en conséquence, pas plus d'une vingtaine de grammes.
* Lisibilité et simplicité des menus : il s'agit d'un équipement d'urgence.
* Autonomie, elle doit largement couvrir la durée d'une randonnée en mode balise (mode ou l'on réveille le GPS à intervale de temps régulier pour récupérer un tracé du parcours).

# Equipe
Nous sommes une équipe d'étudiants ingénieurs à l'École Nationale Supérieure de l'Électronique et de ses Applications, notre équipe est constituée de trois personnes provenant du même groupe de TD.

# Journal de bord

## Séance 1, 29/02/24
* Choix du sujet : GPS
* Mise en place du cahier des charges
* Mise en place d'un diagramme fonctionnel du circuit désiré
* Début de la recherche des composants (cf. section PCB tout y est expliqué)


## Séance 2, 07/03/24
* Présentation cahier des charges et diagramme d'architecture
* Présentation rapide du schéma kicad fait hors séance 
* Commande des composants

## Séance 3, 14/03/24
* Revue schéma KiCad
* Finalisation du PCB
*Commande de la version 1 du PCB 

## Séance 4, 4/04/24
* Debut du codage, réflexion sur l'achitecture idéale de la machine à état nécéssaire.
* Réception des puces GPS en free sample, ainsi que des compas, préparation d'un GPS afin de les tester et d'implémenter leurs bibliotèques sur carte NUCLEO L476RG, très proche du processeur utilisé dans notre projet.

## Séance 5, 25/04/24

* Debut de la reception des autres composants, et reception du pcb
* préparation du pcb et assemblage
* soudure des composants par passage au four. 

## Séance 6, 02/05/24
* Correction des erreurs de design de la version 1 du PCB(cf pcb partie hadware v2)
* Continuation du code autour du gps.
*Debut de la modélisation 3d du boitier
## Séance 7, 16/05/24
* Design et amélioration du pcb v1 vers sa version 2
* Correction des erreurs d'empreintes.
* Commande du pcb v2
## Séance 8, 23/05/24
* Implementation de la machine à état et du code dans notre circuit v2*
* Assemblage et soudage de la v2 (fait avant la séance)

## Séance finales(9,10), 28/05/24 et 29/05/2024

# Livrables
## PCB
Pour ce qui est du pcb, nous avons voulu faire au plus petit, ce pourquoi nous avons choisit des composants les plus petits possible.
* Commencons par le schématic, première chose que nous avons determiné.



![Capture d'écran 2024-05-19 230101](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/18679fc9-716d-4a33-9805-a061cb18e988)



* Commençons par la partie procésseur, pour cela nous avons choisit le STM32L432KCT6 pour plusieurs raison,  


![Capture d'écran 2024-05-19 173412](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/6dd391b2-4bc2-46cd-a4f7-8b0b6d6aa219)
 
  
  son faible encombrement, car disponible en boitier UFGPN32, ne mesurant que 5mm par 5mm.


![Capture d'écran 2024-05-19 173635](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/2db51bd5-02e4-4efc-bee2-857c87780e93)
 
  
  Nous avons pris un processeur avec la dénomination 'L', il s'agit de la famille des processeurs low power comme on peut voir sur le tableau, à température ambiante, sa consommation reste très mesurée, ce qui est parfait pour notre projet.


![Capture d'écran 2024-05-19 173938](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/4e5c36a0-b6bc-4682-94b4-5e78d5010ae0)
 
  
  Enfin, nous nous somme tourné vers ce L432, afin d'avoir un processeur possédant suffisament de connectivité, il nous fallait un uart low power, deux i2c(un pour écran l'autre pour compas) et enfin un usb à la fois pour transmettre les donnée gps du parcours stocké dans la mémoire flash de 256ko mais aussi pour permettre à l'utilisateur de faire les mise à jour du firmware via dfu et par usb,ainsi que quelques gpio pour les boutons et le control de l'alimentation du gps ), enfin nous avons trouvé ce l432 répondant exactement aux attentes ce qui permet d'avoir le plus petit procésseur et donc le plus optimisé pour notre systeme en terme de consommation.

* Passons maintenant au GPS, nous nous somme immédiatement tournée vers une solution embarquant un processeur mt3333, pour son accessibilité, ainsi que sa gestion des trames nmea, encore une fois le mot d'ordre était la taille, ainsi que le poids.C'est pour cela que nous nous somme tournée vers le PA1010D, qui est la solution la plus compact, et celle qui consommait le moins


![Capture d'écran 2024-05-19 174319](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/507f1df2-a27f-4cdf-9335-0f71cd8b9918)
![Capture d'écran 2024-05-19 174313](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/71bbee37-7395-4f1e-9352-78485d848a23)
![Capture d'écran 2024-05-19 174400](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/ddcaa85b-3f62-48f1-8c7b-595efa08c16c)

  
  Ils se base sur un puce MT3333


![Capture d'écran 2024-05-19 174348](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/89ef22a6-b376-48aa-93f8-b2dbf325a5c3)
 
  
  Dans un package le plus réduit possible.


![Capture d'écran 2024-05-19 174442](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/afe29568-55bd-456e-ac94-18d0cb25f25c)
![Capture d'écran 2024-05-19 174429](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/1b3827e4-6d8b-42ef-91e3-ba4d41831c2a)
 
  
  Il possède de plus une précision plus que correcte et surtout un consommation de 28mA en marche ! Ce qui sera utile dans notre cas.

![Capture d'écran 2024-05-19 174524](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/68e1676d-39e1-487f-bedd-fc5a4c742189)

Par ailleur, il est intérressant comme sur la plus part des gps d'y rajouter une pile lithium de 1mah ou plus, afin de garder en mémoire les informations des satélites acquis lors de la dernière utlisation cela permet d'effectuer des démarrages à chaud, qui prennent que quelques secondes contre 30 pour un démarrage à froid, et cela sera particulièrement utile dans notre utilisation pour le mode balise.

![Capture d'écran 2024-05-19 174209](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/b2097750-45cc-41e8-b1d4-6d027a6831bf)

Nous avons choisit la MS621R-IVO3E, pour sa taille avant tout et bien entendu pour la tension quelle délivre, compatible avec la tension vbackup attendu par le gps (entre 2.0V et 4.3V)


* Bien entendu nous ne pouvons pas laisser le gps allumé constamment, sinon avec une batterie de 100mA, l'autonomie ne serait que de plus ou moins 3H en comptant le processeur en idle, (le compas et l'ecran étant négligeable) ce pourquoi nous allons commander l'alimentation du GPS par un mosfet, pour cela nous avons choisit le plus petit possible, cabable de driver un courant d'au moins 50mA nous nous somme tournée vers le 2N7002KQB avec un format minuscule.   


![Capture d'écran 2024-05-19 175233](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3019bfb7-942b-4055-8fbe-95e4150f37e6)
![Capture d'écran 2024-05-19 175246](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/99bd5aa9-4063-4888-9443-3230ace4cda3)
![Capture d'écran 2024-05-19 175256](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/50f14d78-c275-4af4-877f-ba3493329ed5)

Il est en boitié dfn1110D, peut répandu mais ne mesurant que environ 1mm par 1mm, ce qui est parfait dans notre cas, il existre en version n-mos et en versio p-mos

![Capture d'écran 2024-05-19 175240](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/93b50fee-cf67-4e34-bbba-3723ee535bb1)

Mais nous avons choisit la version n-mos, pour sa résistance Rds beaucoup plus faible, 

![Capture d'écran 2024-05-19 174452](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/8f59d8f5-3816-45e0-9d64-1782e860debf)

En effet comme on peut le voir ici notre GPS demande un tension minimale de 3.0V, or notre régulateur de tension sort comme nous allons le voir 3.3V donc lorsque le mosfet est en fonctionnement bloqué, et que le GPS fonctions à consommation maximale (36ma) , il est nécéssaire que la chute de tension n'induit pas une tension aux bornes du GPS inférieure à 3V, ce qui n'était pas possible avec la version pmos qui possède un résistance Rds de près de 10ohms dans le pire des cas.Nous aurons donc un plan de masse gps, qui sera connecté à la masse du circuit à l'aide de ce nmos, drivé par un gpio avec un résistance de pull down de forte valeurs entre G et S.

* Nous pouvons passer au compas, nous avons choisit le QMC5883L

![Capture d'écran 2024-05-19 174025](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/f3eddd03-8d39-4e97-941b-36f1c621339c)
![Capture d'écran 2024-05-19 174012](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/d6447837-16de-4610-9f8f-2d5e727699f1)

De par sa taille minuscule à l'instar des autres composants, seulement 3mm par 3mm, mais aussi par sa simplicité d'utilisation étant donné qu'il communique via I2C, son atout le plus fort est sa consommation, comme on peut le voir 75uA en fonctionnement, ce qui est exactement ce qu'on rechercher.

![Capture d'écran 2024-05-19 174041](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/9a81b181-0ce4-4c0c-8717-e4365eddb079)

Pour ce qui est de son alimentation nous avons choisit de le connecter en mode single supply, nous n'avions pas besoins de redondance sur son alimentation.
Nous veillerons à faire attention à ses condensateurs de reserves, en particulier celui de 4.7uf que l'on préfèrera en boitier plus gros par exemple 0603 afin de gagner les caractéristiques low ESR nécéssaires.

* Nous pouvons passer à la partie puissance de notre systeme, avec le régulateur de tension principale, nous avons choisit un ldo le LDLC025PU33R, qui soit du 3.3v

![Capture d'écran 2024-05-19 174227](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/ce923918-eecc-4db6-ab54-26cd3c38c9da)

Son choix a avant tout été fait pour son boitier miniature, le DFN1x1, par ailleurs il permettra de sortir un 3.3V propre au moyen de condensateurs de découplages et, il sort bien assez de puissance pour tous les périphériques ici.

* Pour ce qui est du chargeur de la batterie lipo, nous avons opté pour un composant dédié à cette utilisation, il s'agit du LTC4065, un chargeur de batterie lipo 1s à partir de 5v donc d'un usb classique. 


![Capture d'écran 2024-05-19 174752](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/0109fe28-6080-455c-9f84-4afac66d50be)

De plus il vient dans un format très compact, origianelemtn designé pour les smartphone et objet connecté compact, ce qui est parfait pour nous.


![Capture d'écran 2024-05-19 174910](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/da44e388-9206-4d3c-a967-70ac70abf5c8)
![Capture d'écran 2024-05-19 174914](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/40350902-c912-4ed9-b76a-1c14ca28cc61)
![Capture d'écran 2024-05-19 174905](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/98ca1fec-7d1c-4c77-85b1-f9003ca7c126)

Par ailleurs, on peut voir dans son datasheet, la précision avec laquelle il régule la tension de sortie, dès lors que la tension d'entré est supérieur à 4.5v ce qui sera notre domaine d'utilisation.

![Capture d'écran 2024-05-19 175146](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3c9a0c4e-9735-4970-8b75-ad7f1b9109bd)

Cependant il n'est pas exent de défauts, en effet lorsque notre systeme ne sera pas en état de charge, le chargeur consommera sur la batterie un courant non voulu, heureusement faible, puisque il pour aller jusqu'à 1uA, ce qui déchargerai notre batterie lipo, en environ 11ans, de plus les batteries lipo utilisée sont équippé de circuit de protection contre la décharge forcée.

![Capture d'écran 2024-05-19 175209](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/fda6a565-a9af-42fe-8ed0-4aca0fdcb2dd)

Enfin, nous pouvons régler la valeur de courant de charge à l'aide d'une résistance de programmation, qui suit la loi suivante et qui se place comme suivant:

![Capture d'écran 2024-05-19 174951](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/e9a532f3-ff44-44f4-b650-b790990ecdb2)
![Capture d'écran 2024-05-19 174801](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/4c38a584-baa2-47a1-b4f6-f22cbeceb7eb)

Nous réglerons le courant de charge à 50mA, pour un temps de charge de plus ou moins deux heures.

* Passons maintenant aux périphériques, pour commencer l'écran, sur base de driver SSD1306, pour sa connection en I2C et sa facilité d'utilisation, nous avons opté pour la plus petite taille disponible,donc le 0.49 pouces.

![Capture d'écran 2024-05-19 175551](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/0fe02ea0-ac3d-4b94-aed7-9dfb8fd107e6)

* Pour ce qui est des boutons et intérrupteur, nous en avons de trois types différents,ils viennent tous de chez c&k switch pour leurs qualité.Les boutons de reset, des kxt341 choisit pour leurs tailles.

![Capture d'écran 2024-05-19 174626](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/9c09a3eb-23d5-4916-905e-5b61f9be4de8)

Le bouton on off principal,un PCM, le plus petit disponible qui commutera la borne + du ldo entre le chargeur et la batterie afin d'alimenter ou non le système.

![Capture d'écran 2024-05-19 174648](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/b5952642-c0dc-4a6c-b315-f80cfe221df7)

Enfin les boutons poussoir afin de changer les differents état, ce sont les plus petit boutons en angle disponible, des PTS840.

![Capture d'écran 2024-05-19 174122](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/a931fddb-1ef8-4bab-a00e-ac215408da92)

* Pour terminer sur les choix, on termine par la batterie il s'agit d'une 100mA, choisit pour sa taille, qui est inférieure à celle du circuit.Et car nous l'avions déja.

![Capture d'écran 2024-05-19 175637](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/45c7ead8-0775-479a-be82-247451d1e349)


* schémas 
Pour ce qui est du schémas, nous l'avons fait en fonction des recommandation constructeur de chacun des composant ainsi que bien entendu en accord avec notre schématique initiale.

![Capture d'écran 2024-05-20 001343](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/a92a3112-db12-48a1-ab0c-6ff204c22f8c)
![Capture d'écran 2024-05-20 001403](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/ff2768f4-eac4-4b66-9fcb-b227dcca523e)

Comme nous pouvons le voir et qui n'est pas dit dans le schémas, nous avons implémenté deux résistant et un condensateur autour des boutons et ce afin d'éviter l'effet de rebond.
Par ailleurs notre usb c est cablé à l'aide des deux résistance sur CC1 et CC2 afin de sortir du 5v (jusqu'à 3A) et ce peut importe le chargeur, qu'il soit power delivery ou non.
Une protectio nesd a été rajouté sur cet usb grace au LC6-2SC6 en boitier sot23-6

* routage

![Capture d'écran 2024-05-19 175342](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/11cefba5-5d7b-4de3-b20f-19fa560e475c)
![Capture d'écran 2024-05-20 001449](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/7d99a945-d430-4100-8f3b-485b4c916cab)
![Capture d'écran 2024-05-20 001511](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/928a967f-8ea2-498d-abf3-de03c6810c6a)


Pour ce qui est du routage, nous avons commencé par dimensionner le circuit, pour cela nous avons décider de placer tous gros composant (gps, ecran, boutons, usb) sur une face comme on peut le voir sur le visu 3d, puis on les a agancé au plus proche, puis nous avons décidé de mettre tout le reste de l'autre coté.Avec un routage suffisament bien pensé, deux couches suffisent.

![Capture d'écran 2024-05-20 004538](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/a01fa680-d2ae-4509-bad4-284f8001467b)

Petite subtilité du circuit le compas est bien entendu fortement sensible au champ magnétique, c'est pourquoi une restristion des via, des remplissages de zone et des piste et faite sous lui, afin que cela ne gêne pas sa detection du champs magnétique terreste, par ailleurs nous avons fait un effort en ajoutant d'avantages de via afin d'eviter un maximum le phénomène d'antennes (rmq: certain via comme ceux sur les pads de l'usb c sont pour la stabilité mécanique plus que pour ce problême d'antenne).

* Hardware version 2
  Comme il était prévisible, notre version une du hardware n'était pas exempte de défauts, ce pourquoi nous avons décidé de la refaire et donc de faire un second pcb qui sera une v2.
  Pour commencer nous en avons profité pour rendre le design encore plus compact, en se rendant compte que l'on pouvait placer l'usb c en dessous de l'ecran étant donne que de toute façons l'écran doit arriver à fleur du haut du module GPS.Ainsi grace à cette conception revu notre circuit garde la même largeur mais est 8mm moins long.

Nous avons reglé 3 problemes dont deux majeurs:

![Capture d'écran 2024-05-19 175452](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/c409d923-8117-4f95-807a-4ccb04c451cf)
![Capture d'écran 2024-05-19 175551](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3f06edd7-4ea4-4440-b4d2-630af7dbcab0)

Premièrement nous avions oublié de flipper le connecteur de l'écran de la face de devant à celle de derrière ainsi même si nous pouvions souder l'écran en place car il s'agit d'un connecteur traversant, toute les connection était inversée et donc fausses, l'écran ne pouvais donc pas marcher en l'état, et on était obligé de le connecter en flottant par des cables.


![Capture d'écran 2024-05-19 175353](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/d9ae6ccd-5a13-4939-9728-d47da248ebb6)

Le second problème critique était du à une erreur d'inattention lors de la création de l'empreinte du régulateur de tension, en effet le schémas des pin de connection est donnée sur la plus-part des datasheets en vue du dessus du composant par transparence, or nous avions cru que c'était par vu du dessous, il en résulte des connections completements fausse sur ce régulateur de tension,alors pour effectuer les premiers test, nous l'avons soudé en 'flottant' à l'aide de fils de cuivre émaillé.


![Capture d'écran 2024-05-20 013927](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/dbe7bba1-71b7-49b9-959d-957181df6e72)

La troisiême erreure est sur l'usb, en effet afin de passer le gps en mode dfu, il est nécéssaire de mettre sa broche correspondant à son boot0 (pin PH3) sur l'état logique 1 durant au moins les cinq premiers cycles d'horloges, or nous n'avons pas prévu de boutons pour cela sur notre circuit donc la v2 en rajoute 1.


![Capture d'écran 2024-05-19 175404](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/288e9a35-8ebd-4e1b-a13e-bfdd79da0688)


* enfin, probleme qui lui n'a pas été corrigé par la v2 car decouvert trop tard, tout comme le régulateur de tensions nous avons fait la même erreure d'inatention en créant l'empreinte du mosfet ainsi nous avons inversé les broches G et S, il en résulte que lorsque le GPIO passe a l'état haut, à la place d'avoir un Vgs positif et un mosfet passant, on a un Vgs négatif et donc un mosfet toujours bloqué.Nous avons donc soudé ce mosfet correctement en flottant avec des fils de cuivre émaillé.

  ![Capture d'écran 2024-05-20 012826](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/8695c5ec-3de6-477b-880c-512e5044c76b)

On peut voir le mosfet avec G et S reconnecté correctement.

![IMG_6474](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/908e361f-2429-4f1f-8c8f-53ae0cbbb74a)

Nous avons alors effectuer un test basique pour voir son fonctionnement correcte, pour cela on injecte 3.3v dans la broche pour simuler l'état haut du gpio, et on verifie que la mosfet deviens bien passant entre D et S
![IMG_6473](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/06159f40-1bcf-4142-961c-8f2c39ccfd6e)

mosfet ici passant car 3.3v connecté.

![IMG_6472](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/92cbf9f5-c4f2-4895-aa36-e6fee57f7447)

et mosfet ici bloqué, donc tout marche nickel.

* Pour la version 2, nous somme donc passé sur 4 couches, ce uniquement car nous avons changer de fabricant, et afin d'avoir des via d'un diametre qui nous convenait, 4 couches étaient obligatoires.

![Capture d'écran 2024-05-20 014129](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/03594cd4-7ad0-418d-9da3-e7cf19b1b316)
![Capture d'écran 2024-05-20 014142](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/f225782c-0716-46d7-b874-5519b4c6c4e7)

Nous en avons profité pour rajouter tous les pin swd sur des pads sur la premiere couche interne ainsi que le NRST qui avait été oublié sur la v1 et qui est obligatoire pour la programmation par st link via le lien swd.

![Capture d'écran 2024-05-20 014150](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/e9f8a581-733f-4ab7-8965-b310cbb49021)

Sur la seconde couche interne, comme nous avions de la place, nous en avons profité pour ajouter deux pads pour deux pins du processeurs comportant gpio et adc libre au cas ou pour un future projet, ou une amélioration future.

![Capture d'écran 2024-05-20 014234](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/7ed027fd-886e-4c3d-806f-bde9357d5563)

On se retrouve finalement avec un circuit plus compact, ou les principal défauts ont été réglé, et quelques améliorations ont été faite, seul le probleme du mosfet persistera, i lrestera soudé en flottant pour la v2 (le problème est déja réglé dans les fichiers mais il faudrait lancer une v3 en fabrication).

![Capture d'écran 2024-05-20 014251](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/af6d7f69-faf8-496b-9e07-13d1ca606adb)

On se retrouve avec une face coté périphérique, réduite au minimum, permettant tout juste de passer les differents boutons, l'usb c ainsi que l'écran donc le design n'est pas plus compactable: nous avons atteint la taille idéale.

*Résultats de la v2:

![IMG_6539](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/d92b4f16-909b-4e93-98b3-0ffb4a51259c)

![IMG_6540](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/80929b68-d452-4fa2-9dec-16fc160fbe2c)

On obtient donc un circuit bien plus compact comme prevu, comme on peut le voir sur la photo 2, l'usb c pase bien sous l'écran, ce dernier devient alors à la même hauteur que le module gps, on obtient alors la balise gps la plus compacte possible.

Cependant, apres avoir soudé la v2, nous nous sommes vite rendu compte que des erreurs persistaient, en effet comme dis plus tôt, le mosfet est toujours dans le mauvais sens, car nous nous sommes rendu compte de notre erreur qu'après la commande de la v2, c'est pourquoi, nous l'avons soudé en 'flotant' à l'aide de fil de cuivre émaillé, comme pour la v1.

De plus, la seconde erreur est sur le port usb, en effet il nous était impossible de reconnaitre la balise comme un stm32 en mode DFU sur l'ordinateur.Dans un premier temps, nous avons tester le bouton permettant de passser le pin boot0 (PH3) du stm32 à l'état haut, qui pour rappel doit être à l'état haut lors du démarrage du processeur, afin que ce dernier s'initialise en mode dfu et soit programmable par usb.

Tout ceci fonctionnait parfaitement, et donc apres avoir vérifier que l'usb c était correctement soudé, et qu'il remplissait bien sont rôle, nous nous somme intérréssé au seul composant entre le port usb C et le controlleur: la protection contre les surtensions USB-LC6-2SC6, en boitier sot23-6 et ça s'est avéré payant, en effet comme on peut le voir dans le datasheet:


![Capture1](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/dd1c8c97-b842-499f-b65c-a5ce3896f6ce)
![Capture](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/bdb80f2a-6acd-4ac5-b213-d85339d42d1a)

Ici les connections sur le schémas sont celle que nous avons effectué dans la v2 et la v2, on remarque directement, que cette protection était mal cablé, en effet elle court circuitait les bus D+ et D- du connecteur mais aussi du stm32, ceci n'est pas grave étant donné qu ce sont des bus de donnée, n'altera pas le fonctionnement des composant de manière définitive, cependant, cela empeche le stm32 de communiquer ou d'être reconnu en usb.


![Capture2](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/beaff6d3-2eda-4cba-8a5e-0162166de2c2)


* Solution: étant donné qu'il s'agit d'une protection contre les surtension, elle est utile mais pas obligatoire, ce pourquoi nous l'avons retiré afin de connecter directement les D+ et D- du port usb avec ceux du controleur, ainsi apres avoir effectué cela tout fonctionnait parfaitement.

* Conclusion: en conclusion, nos choix de composant se sont avéré payant, permettant d'obtenir le circuit le plus fonctionnel et le plus compact possible,et après test, l'ensemble des composant de mesure fonctionnent ainsi que l'ensemble des composant de puissance tel que par exemple le module de charge de la batterie, qui avec le courant de 50mA de charge que nous lui avons imposé en lui conectant une résistance de 20kohms sur sa broche de programmation du courant, permet de charger notre batterie de 100mAh en un peut moins de 2H, bien entendu cette resistance peut être changer pour augmenter le courant de charge jusqu'à 300 mA avant d'endommager prématurément la batterie à chaque charge.De plus comme nous pouvons le voir sur les images, une led rouge est commandée par ce module de charge, elle s'allume en rouge lorsuqe il charge la batterie, puis s'éteint lorsque cette derniere est chargée: à une tension de 4.20V. Nous avons donc une bonne platforme, compact, fonctionnelle et pratique pour l'utilisateur afin d'y implementer le code necessaire à en faire la balise gps que nous souhaitons.






   ## boitier et design exterieur:

  * Pour ce qui est du boitier et du design exterieur, il doit couvrir le circuit ainsi que la batterie, mais aussi être leger et design, car il s'agit quand même d'un porte clé, donc de quelque chose de visible.
    C'est pour cela que nous avons opté pour un boitié rectangulaire,avec toute ses faces possédant un motif isogrid, motif utilisé en aviation, pour sa rigidité ainsi que sa légèreté, voici un rendu du motif ainsi que du boitié: 


![Capture d'écran 2024-05-20 021837](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/e37089fd-9dd9-4882-b04b-e442c5fd5f4a)
![Capture d'écran 2024-05-20 021707](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/732d2104-b3b7-466a-9eda-7db429b6123b)

*Cependant, pour la présentation et afin de proteger l'éléctronique de la poussière extérieur, nous avons un second design plus simple, moins élégant mais plus fonctionnel que voici:
![Capture3](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/85339092-18c3-4184-ac27-147abf84bb60)
![Capture4](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/a40ca031-c708-438c-9147-09fcf1dcc3b3)


## Code
*Pour ce qui est du code, le but est d'avoir différentes pages de données ainsi que un état dans lequel, nous passerons afin de mettre le systeme en mode balise, c'est à dire une mise en veille du processeur, une extinction du gps grace au mosfet puis toute les cinq minute, le processeur se reveille,il reveille le gps, puis attend que ce dernier capte les satelites enfin récupère la position ainsi que le temps, puis stocke tout cela dans plusieurs pages de la flash utilisée afin d'émuler une eeprom.
Bien sur nous passons d'une page à une autre en appuyant sur un des deux boutons, le second bouton sera utilisé afin de modifier des informations au sein d'une page d'affichage, ainsi que pour passer au mode balise, qui est un mode qui doit être légèrement plus difficile à activer que le simple changement d'une page de donnée à une autre.

Pour effectuer cela, nous allons utiliser une FSM, (machine à état finis) de plus des intérruption ont été programmé sur les boutons et ce afin que si une itération de la boucle while générale prend 1s par exemple, afin que l'on ne doive pas rester appuyé 1s sur le bouton pour que cet appuis soit pris en compte, ce qui ne serait pas utilisable.

![diagrame machine à etat 1-Page-1](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3e7a7e93-57df-4b55-8b26-3b361b741e51)

On peut voir dans ce diagram les 4 état principaux, comprenand celui pour la balise; l'état usb lui est un état à part, qui ne sera activé que lorsque la balise sera connectée en usb en tant que périphérique afin d'envoyer toute les donnée quelle aura recueillit à l'ordinateur.

Maintenant présentons les différents état ainsi que leurs rendu:

![Capture6](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/ffb1580c-eef0-4e0d-8ace-741a90f4a76a)

Ce mode est assez spéciale, dans la mesure où il communique avec l'exterieur, dans ce mode la balise attendra un message précis par usb, que le programme python lui enverra, alors cette derniere debutera l'envois de l'intégralitée des valeurs de latitude, longitude et du temps quelle a enregistrée depuis la dernière connection usb; puis à la fin de la transmission, le programme python, si il a bien reçu toute les données, envera un message de fin de reception, alors la balise effacera toute les donnée contenu dans sa eeprom émulée, on pourra donc en réenregistrer et de plus les données récupérées sur l'ordinateur pourrons servir afin de faire un suivis du parcour effectuée, de la vitesse moyenne,ou encore des allures.

![Capture7](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/ed96d6a5-f974-43f6-a7e6-f9afd83e4b60)
![Capture8](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/295efebe-98a3-4eac-b841-bbd19df9a385)
![Capture9](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/bd872e4e-2090-470a-879f-ee76fc35d437)
![Capture5](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3e99f131-4b92-4fe2-8669-671c4353a69d)

*Maintenant nous pouvons passer à l'architecture des drivers, pour ceux ci, nous avons besoin de driver qui solicite le processeurs le moins souvent possible, afin de pouvoir avoir l'impression que plusieurs chose s'affiche sur l'écran en même temps nous utilisons des interruption sur les boutons, mais aussi nous utilisons le dma afin de receptionner les trame nmea, la reception se fait par pack de 64 caractère, puis on remplie un buffer de 512 caractères avec cela.Ce buffer se met automatiquement à jour grace au dma de manière circulaire, par la suite on utilise une serie de fonction, qui vont venir partitionner la trame et récupérer les informations intérréssantes afin de les stocker dans une structure de donnée spécifique au gps:

![Capture10](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/a64aa7af-c7d7-43b4-9794-f75ba6ff809e)

![Capture d'écran 2024-05-31 215551](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/5ebb5986-3482-4724-bebb-fd927f2ec289)

 Un exemple de traitement d'une trame, ici on récupère la vitesse d'une trame GNRMC,                                                                                                                                                                                                                                              
![Capture d'écran 2024-05-31 220016](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/9a0ee058-be4c-4d0e-892a-4bb56924ca0b)

Pour rappel voici cette trame, pour cela, on utilise un buffer de transition, dans lequel on va copier la trame sans le signe '$' pour la suite, la fonction strtok permet de partitionner un string ,notre buffer se comportera comme une matrice, bien entendu on le partitione avec pour token de partionnement, les virgules séparant les données, ainsi notre chaine de caracté représentant la vitesse en knots est la septième de notre buffer, par la suite strtof permet de la convertir facilement en flottant, on la divise par 1.944 pour obtenir la vitesse en m/s.

![Capture d'écran 2024-05-31 215514](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/cd9078ab-ce72-46c7-803e-5bc6cebca31a)

Le reste se pace dans la machine à état afin d'obtenir les donnée intérréssante à afficher.

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
Pour ce qui est du compas, sa librairie est imbriquée avec celle de l'écran, dans la mesure ou en plus de récupérer les données du compas, et de les mettre dans une structure de donnée dédiée au compas (sans dma, car les donnée sont moins complexe et moins lourde que pour le gps); notre librairie permet aussi de tracée des flèches visant le pole nord, et utilise donc pour cela une fonction de dessin de flèche qui a été codé grâce à la librairie de l'écran.
Voici un petit aperçu de cette imbrication:
                                                                                                                                                                                                                                   [Capture11](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/922e01d7-76b1-4761-b876-b4eb3a89b1d7)
                                                                                                                                                                                                                                            

*Fonctions spéciales: la gestion des boutons: Afin de gérer les boutons, il est primordial d'avoir recours aux intérruptions, car cela permet de pouvoir prendre en compte un appuie bref sur l'un d'eux même si la condition que cela fait évoluer n'est que plusieurs seconde plus tard dans le déroulement chronologique du code.Pour cela, les boutons sont déclaré comme des GPIOo_exti avec un certain channel, cela permet de généré une intérruption du système lorsque l'état lu sur l'un d'eux est haut.Par la suite, il suffit de faire evoluer la variable global correspondante au bouton dans la fonction de callback. Attention, il est primordial de ne pas avoir recours au HAL_Delay dans une fonction de callback, en effet ce dernier a recours à une intérruption de niveau 0 sur le systick, avoir recours à ce dernier dans une fonction de callback reviens à mettre en pause cette intérruption or cette derniere est nécéssaire pour que le procésseur sache combien de temps il a à attendre encore, il en résulte en une attente infinie.

Une méthode pour reglert cela serait d'abaisser la classe de notre intérruption sur les boutons, la passer en classe 1, or dans ce cas l'intérruption sur les boutons n'a plus aucun sens.
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
![Capture d'écran 2024-05-31 213937](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/4c268d44-f5d6-4ca8-bfc9-a96e84014364)


![Capture d'écran 2024-05-31 214058](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/3fa8ca1c-bbff-4f2f-8914-b4fdb3e4186e)

Enfin, l'utilitée d'avoir mis notre machine a état dans un fichier spécifique, est de pouvoir la changer de manière lisible et claire, tout en gardant une fonction while(1) petite et claire, seul la fonction de rafraichissement de l'écran a été sortie de la machine, elle s'execute donc qu'une fois par boucle ce qui est nécéssaire, car elle prend un certain temps, l'écriture dans le buffer de l'écran etant assez longue.



*Conclusion: En conclusion de la partie code, nous avons pu obtenir un code fonctionnel et polyvalent permettant de bien tirer partie des capteurs et composant présent sur la carte, il reste cependant deux mise à jour majeur à faire, la prise en compte d'un timer afin de réveiller le procésseur lorsque nous le voulons, cela permettrait pouir le client de faire passer l'autonomie du mode balise de quelques heures à plusieurs centaines.Enfin pour le mode balise, l'émulation d'une eeprom afin d'y stocker les latitudes et longitude ainsi que quelques données importantes, tel que la dernière vitesse maximale est encore à faire.De plus quelle est cohérent avec la place réstante encore dans la flash de 256ko: 


![Capture d'écran 2024-05-31 220910](https://github.com/Leo-PHAV/2324_Projet1A_GPS/assets/161692882/26879593-2151-420b-8970-33af58600211)







                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  

## Rendu final
> Video ici de votre projet
