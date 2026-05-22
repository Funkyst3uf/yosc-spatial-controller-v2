# DME7 Spatial Controller Interpreter

Ce projet est un interpréteur de commandes en ligne de commande (CLI) écrit en C, conçu pour piloter le moteur de rendu audio spatial du processeur immersif **Yamaha DME7**. 

## Le Projet
À l'aide d'un analyseur lexical et syntaxique robuste généré par **Flex et Bison**, ce contrôleur traduit un langage de script dédié (DSL) en paquets réseau binaires. Ces paquets sont ensuite sérialisés et transmis en temps réel au DME7 via le protocole **OSC (Open Sound Control) sur UDP**.

L'objectif : offrir une interface de contrôle fluide, fiable et scriptable pour la gestion spatiale des objets audio.

## Stack Technique
* **Langage :** C
* **Analyse syntaxique/lexicale :** GNU Bison & Flex
* **Communication réseau :** Sockets UDP (POSIX)
* **Protocole :** OSC (via la librairie `liblo`)
* **Build System :** Makefile

## Fonctionnalités du Langage (DSL)
Le contrôleur embarque sa propre grammaire permettant d'orchestrer 64 objets audio dans un espace 3D :
* `SET` : Mémorisation de coordonnées dans l'espace (x, y, z).
* `JUMP` : Téléportation instantanée d'un objet.
* `MOVE` : Déplacement continu et fluide avec gestion du temps de trajet.
* `SWAP` : Permutation croisée des positions entre deux sources.
* `PONG` : Séquences d'allers-retours automatisées.
* `MUTE` : (Dés)activation audio binaire.

## Compilation & Utilisation
Le projet s'appuie sur un système de build classique sous environnement Linux/Unix. 

```bash
# Compilation du projet (client & serveur de test)
make all

# Lancement du client
./spatial-controller <adresse IP> <port>

# Lancement du serveur de test (localhost, port 50528)
./dme7-sim
