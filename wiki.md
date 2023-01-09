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

Le développement in vitro de champignons est une pratique qui consiste à faire grandir des mycéliums dans un environnement artificiel. Le développement est divisé en trois phases qui ont des besoins différents :

1. L'incubation sur boite de Petri
2. L'inoculation et la colonisation du grain
3. La fructification

Ces phases ont comme besoin en température et en humidité

## Matériel utilisé

Notre problématique principale étant le contrôle de la température et de l'humidité dans l'environnement, nous avons besoin, pour chacune de ces deux métriques, d'un ou plusieurs capteur(s) et actionneur(s).

Pour gérer la logique d'asservissement, nous avons besoin d'une boucle de contrôle (numérique ou analogique). Nous avons décidés de la programmer sur un microcontroleur arduino.

En terme d'alimentation, nous utilisons deux tensions différentes :
- 5V pour la logique de contrôle et l'alimentation des cartes et capteurs
- 12V pour l'alimentation