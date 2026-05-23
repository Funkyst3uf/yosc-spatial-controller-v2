/**
 * @file main.c
 * @brief Point d'entrée du programme de contrôle du processeur immersif Yamaha DME7
 * @author Jonathan Ntoula
 * @date Mai 2026
 * @details Pilote le cycle de vie du programme : initialisation de la connexion réseau (dial)
 * Initialisation des structures de données du module spatial 
 * * Projet réalisé pour le cours Interprétation & Compilation dirigé par M. Kislin
 * Licence Informatique, Université Paris 8
 */

#include "sys.h"
#include "dial.h"
#include "spatial.h"
#include "audio.h"
#include "y.tab.h" /* Généré par Bison, contient les définitions de tokens */

/** * @brief Descripteur fichier (socket UDP)
 * @details Utilisé pour la communication UDP avec le processeur Yamaha DME7
 */
int fd; 

/**
 * @brief Fonction d'analyse syntaxique générée par Bison.
 * @return 0 si l'analyse réussit (ou si une instruction de sortie comme QUIT est rencontrée)
 */
int yyparse();

/**
 * @brief Point d'entrée du programme.
 * @details Gère les arguments de la ligne de commande, établit la connexion réseau
 * et déclenche l'initialisation des tables gérées par le module spatial.
 *
 * @param ac Nombre d'arguments (adresse IP et Port).
 * @param av Tableau des arguments de la ligne de commande.
 * @return 0 en cas de sortie normale, 1 en cas d'échec de connexion ou d'erreur.
 */
int main(int ac, char * av[]) {
    printf("Démarrage du pilote DME7 (Version Compilée)...\n");

    /* Vérification des arguments (adresse IP, Port) */
    if (ac < 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", av[0]);
        return 1;
    }

    /* appel de la fonction dial() avec les informations saisies par l'utilisateur */
    fd = dial(av[1], av[2]);

    if (fd < 0) { // en cas d'échec de connexion
        perror("Erreur de connexion (dial)");
        return 1;
    }

    /* Délégation de l'initialisation des tables (objets et labels) au module spatial.c */
    init_tables(); 

    printf("Prêt à envoyer des données (JUMP, MOVE, MUTE) ...\n"); // Affichage si tout s'est bien passé
    printf("Tapez QUIT ou utilisez Ctrl+D pour quitter.\n");

    /* Lancement de l'analyseur syntaxique qui pilotera l'exécution des commandes */
    int resultat = yyparse();

    if (resultat == 0) { // retour du parser de Bison, 0 = fin propre
        printf("\nFin du programme (QUIT)\n");
    } else { // si retour !=0, c'est une erreur de syntaxe
        fprintf(stderr, "\nArrêt du programme suite à une erreur de syntaxe.\n");
    }

    /* Fermeture propre de la ressource réseau */
    close(fd);

    /* Renvoie directement le statut du parser stocké dans la variable resultat
     (0 = succès, 1 = erreur) */
    return resultat;
}