# Wiki mushrooms controller

## Description du projet et des membres

Le projet consiste à réaliser un système de contrôle d'environnement pour aider au développement in vitro de champignons.

Le projet comporte 4 membres :
- Léo Marché
- Gwennan Eliezer
- Jean Le Menn
- Ariane Dauvergne

Nous avons tous participés à la rédaction de la spécification, à la recherche de solutions techniques et à l'élaboration des modèles. Même si le code a été écrit sur l'ordinateur de Léo Marché, nous avons tous participés à sa conception.

## Problématique liée au projet

La myciculture in vitro est une pratique qui consiste à faire grandir du mycélium, la partie principale d'un champignon, dans un environnement artificiel. Le développement est divisé en trois phases qui ont des besoins différents pour une croissance optimale :

1. L'incubation sur boite de Petri
2. L'inoculation et la colonisation
3. La fructification

Ces phases ont toutes des besoins en température et en humidité différents, qui varient en fonction des espèce en croissance. Par exemple, pour faire pousser du pleurote (_pleurotus ostreatus_), les conditions sont les suivantes :

1. Température de croissance (entre 20°C et 25°C) ; Humidité non contrôlée (entre 40% et 60% en France)
2. Température de croissance (entre 20°C et 25°C) ; Forte humidité (plus de 95%)
3. Basse température (entre 15°C et 20°C) ; Forte humidité (plus de 95%)

### Cahier des charges

Pour répondre à la problémtique, nous nous sommes posés un cahier des charges, sous forme d'une liste

- Stabilité des variables suivantes (et tolérances respectives) 
    - humidité de l’air (± 5%)
    - température de l’air (± 1°C)
- Résilience aux pannes réseau
- Adaptabilité à différentes tailles de chambres isolées
- Paramétrage pour la croissance d’espèce en particulier
- Différents modes de fonctionnement
    - "Croissance sèche" : 24°C ; XX%
    - "Colonisation du grain" : 24°C ; 95%
    - "Fructification" : XX°C ; 95%
- Possibilité de fonctionner sans le Pi (avec le contrôle manuel)


## Matériel utilisé

Notre problématique principale étant le contrôle de la température et de l'humidité dans l'environnement, nous avons besoin, pour chacune de ces deux métriques, d'un ou plusieurs capteur(s) et actionneur(s).

### Electronique

Pour gérer la logique d'asservissement, nous avons besoin d'une boucle de contrôle (numérique ou analogique). Nous avons décidés de la programmer sur un microcontroleur Arduino.

En terme d'alimentation, nous utilisons deux tensions différentes :
- 5V pour la logique de contrôle et l'alimentation des cartes et capteurs
- 12V pour l'alimentation

### Contrôle des paramètres physiques

Pour augmenter la température dans l'incubateur, nous avons opté pour une résistance chauffante, placée sur un radiateur.

<img src="photos/rad.png" alt="drawing" width="300"/>

Pour augmenter  l'humidité, nous avons choisi un système actif, un petit atomiseur ultrasonique, responsable de projeter des millions de gouttelettes d'eau, dont une grande partie viendra saturer l'air dans le volume. Ce système a l'avantage d'humidifier l'air extrêmement vite, avec le défaut que le contrôle précis de l'humidité est impossible. On peut donc considérer que l'humidité varie en tout ou rien, de 40% à 100%. Nous avons fait ce choix en conscience, puisque que pour toutes les espèces, l'humidité optimale est la saturation de l'air, à 100% d'humidité.

<img src="photos/atom.png" alt="drawing" width="300"/>

## Scénario d’utilisation

_c’est ici que vous décrivez en détail ce que votre projet fait, comment il fonctionne, avec à l’appui captures d’écran et vidéo(s) de démonstration; à vous de choisir la structure des sous-sections_

L'avantage de notre projet vis-à-vis de l'utilisation, c'est que notre création sera utilisée en pratique tous les jours pendant encore quelques années. L'incubateur viendra remplacer la première version primive de Jean, qui profitera de nos améliorations. La croissance en phase 2 et 3 étant relativement lente, l'autonomie du nouveau système sera bienvenue pour garantir un meilleur rendement de récolte de champignons.

### Pour une récolte type

1. Le phénotype (individu) est cloné d'une boîte de Petri à une autre pour garantir sa fraicheur pour la croissance. L'intervention est faite dans un milieu stérile.
2. La boîte est placée dans l'incubateur avec le réglage __"Croissance sèche"__, et la température s'adapte automatiquement pour 24°C (sans changement pour l'humidité).
3. Environ 3 jours s'écoulent.
4. Quand le mycélium a colonisé toute la surface de la boîte, un morceau de gélose est placé dans du grain stérilisé (du riz pour le pleurote), dans un pot en verre avec un couvercle filtrant.
5. Les pots sont à leur tout placés dans l'incubateur avec le réglage __"Colonisation du grain"__ de nouveau, la température se règle à 24°C et l'humidité à 95%.
6. Environ 7 jours s'écoulent.
7. Le grain est utilisé pour répendu le substrat stérilisé (de la paille céréalière pour le pleurote), le tout est placé dans un sac scellé, un sac de congélation dans notre cas. La masse de substrat choisie déterminera la masse de champignon récoltée.
8. Les sacs sont placés dans l'incubateur avec le réglage __"Croissance sèche"__.
9. 7 à 14 jours s'écoulent en fonction du volume du sac.
10. Les sacs sont fendus en forme de croix pour créer un contact entre le mycélium et l'air humide. Le champignon a assez d'énergie pour pousser à ce stade.
11. Les sacs ouverts sont placés dans l'incubateur avec le réglage __"Fructification"__, l'humidité est conservée à 95%, tandis que la température baisse jusqu'à l'ambiante.
12. Phase finale, les champignons poussent en quelques jours et sont prêts à être récoltés.
13. (Optionnellement, les sacs peuvent donner une deuxième voire une troisième récolte si on leur laisse le temps.)

## Budget

| Composants  | Prix |
| --- | --- |
| Commande | 70€ |
| Puissance | 44€ |
| Actionneurs | 25€ |
| Capteurs | 25€ |
| IHM (écran LCD) | 7€ |
| __Total__ | __171€__ |


## Bilan
_vous avez répondu à la problématique, même en partie_

Nous arrivons bien à contrôler les paramètres choisis dans l'enceinte de l'incubateur, ce qui répond au moins à l'essentiel de notre problématique.

_Cahier des charges point par point_

## Futur du projet

Manque du paramètre lumière.