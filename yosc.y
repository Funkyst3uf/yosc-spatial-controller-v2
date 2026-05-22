/**
 * @file yosc.y
 * @brief Analyseur syntaxique (Parser) pour le contrôleur spatial DME7
 * @author Jonathan Ntoula
 * @date Mai 2026
 * @details 
 * ### Langage de Commande Spatiale (DSL)
 * Ce fichier implémente la grammaire permettant de piloter le moteur audio. 
 * Voici les commandes supportées par l'interpréteur :
 * - **SET label x y z** : Associe un nom (label) à une position (coordonnées).
 * - **JUMP id x y z** (ou *label*) : Mouvement instantané (téléportation).
 * - **MOVE id x y z temps** (ou *label*) : Déplacement continu.
 * - **SWAP id1 id2 temps** : Permutation des positions de deux objets.
 * - **PONG id cible repetitions temps** : Aller-retour d'un objet depuis sa position vers une position cible n fois.
 * - **MUTE id etat** : (Dés)activation audio. État binaire (0 = OFF, 1 = ON).
 * - **STATUS id** : Consultation de la table de spatialisation.
 * - **QUIT** : Fin de session propre.
 */

/** @cond BISON_HIDDEN_PROLOGUE */
%{    
#include <stdio.h>
#include <string.h>
#include <unistd.h>   /* Pour usleep */
#include "sys.h"   
#include "spatial.h" 
#include "audio.h"    

extern int fd;  // Défini dans main.c
extern Point3D objets[65]; // Défini dans spatial.c
extern void yyerror(const char *s); // Fonction d'erreur attendue par Bison
extern int yylex(); // Fonction d'analyse lexicale générée par Flex
%} 
/** @endcond */

/**
 * @union yylval
 * @brief Valeurs sémantiques (types) des terminaux et non-terminaux.
 */
%union {  // Type des valeurs attachées aux nœuds
    int i;          /**< Entier : ID des objets, états binaires, itérations */
    float f;        /**< Flottant : coordonnées, vitesses */
    char* s;        /**< Chaîne de caractères (labels) */
    Point3D p;      /**< Structure pour les coordonnées 3D */
}

/* --- Déclarations des Tokens --- */
%token YJUMP      /**< Commande de téléportation */
%token YMOVE      /**< Commande de mouvement continu */
%token YMUTE      /**< Commande de coupure/activation audio */
%token YNEWLINE   /**< Caractère de fin d'instruction (Entrée) */
%token YSEMI      /**< Séparateur de commandes séquentielles ou fin (;) */
%token YSET       /**< Enregistrement d'une position nommée */
%token YPONG      /**< Séquence de mouvement aller-retour */
%token YSWAP      /**< Échange de positions entre deux objets */
%token YSTATUS    /**< Requête d'affichage des positions d'un objet */
%token YQUIT      /**< Commande de fermeture du contrôleur */

%token <i> YINT   /**< Symbole terminal entier */
%token <f> YFLOAT /**< Symbole terminal flottant */
%token <s> YLABEL /**< Symbole terminal texte (étiquette) */

/* --- Déclarations des Types (Non-terminaux) --- */
%type <f> number        /* Non-terminal représentant un nombre entier ou décimal */
%type <i> object_id     /* Non-terminal représentant l'identifiant d'un objet (entier) */
%type <p> coordinates   /* Triplet de flottants (coordonnées cartésiennes) */
%type <f> travel_time   /* Durée des déplacements en secondes */
%type <i> repeats       /* Nombre d'allers-retours pour le PONG */
%type <i> on_off        /* État binaire pour le MUTE (0/1) */

%start input  // Point de départ des règles de la grammaire


%% /* --- Règles de la grammaire --- */

/** @cond BISON_HIDDEN_RULES */

/** @brief Point d'entrée récursif de l'analyseur */
input: 
    /* vide */
    | input phrase 
    ;

/** @brief Une phrase correspond à une ou plusieurs commandes suivies d'un retour à la ligne */
phrase:
    YNEWLINE           
    | command_list YNEWLINE 
    | error YNEWLINE   
    { 
        yyerrok; // Erreur de syntaxe, cas possibles
        printf("Erreur de syntaxe. Usage attendu :\n"
       "  - JUMP/MOVE/MUTE : CMD ID ARGS\n"
       "  - SET            : CMD LABEL ARGS\n"
       "  - SWAP           : CMD ID ID ARG\n");
    }
    ;

/** @brief Syntaxe des commandes */
command_list:    
      command                     // Une commande seule             
    | command_list YSEMI command  // Une liste de commandes, un ";", une commande 
    | command_list YSEMI          // Une liste de commandes (ou une seule), suivie d'un ";"    
    ;

/** @brief Ensemble des commandes autorisées */
command:
      set_cmd
    | jump_cmd 
    | move_cmd 
    | swap_cmd
    | pong_cmd
    | mute_cmd 
    | status_cmd
    | quit_cmd
    ;

/** @brief SET : Associe un nom (label) à une position (coordinates) */
set_cmd:
    YSET YLABEL coordinates
    {
        set_position_label($2, $3);
        printf("Label '%s' enregistré en [%.2f, %.2f, %.2f]\n", $2, $3.x, $3.y, $3.z);
        free($2); 
    }
    ;

/** @brief JUMP : Mouvement instantané (téléportation) */
jump_cmd:
    YJUMP object_id coordinates // Via coordonnées explicites (saisie)
    {   
        jump_to_position(fd, $2, $3);
        printf("Action : JUMP obj %d vers [%.2f, %.2f, %.2f]\n", $2, $3.x, $3.y, $3.z);
    }
    | YJUMP object_id YLABEL  // Via un label précédemment enregistré
    {    
        float x, y, z;
        if (get_position_by_label($3, &x, &y, &z)) {
            Point3D p = {x, y, z};
            jump_to_position(fd, $2, p);
            printf("Action : JUMP obj %d vers label '%s'\n", $2, $3);
        } else {
            printf("Erreur : Label '%s' inconnu\n", $3);
        }
        free($3);
    }
    ;

/** @brief MOVE : Déplacement continu */
move_cmd:
    YMOVE object_id coordinates travel_time
    { 
        move_to_position(fd, $2, objets[$2], $3, $4);
        printf("Action : MOVE obj %d vers [%.2f, %.2f, %.2f] en %.2fs\n", $2, $3.x, $3.y, $3.z, $4);
    }
    | YMOVE object_id YLABEL travel_time
    {    
        float x, y, z;
        if (get_position_by_label($3, &x, &y, &z)) {
            Point3D b = {x, y, z};
            move_to_position(fd, $2, objets[$2], b, $4);
            printf("Action : MOVE obj %d vers label '%s' (%.2fs)\n", $2, $3, $4);
        } else {
            printf("Erreur : Label '%s' inconnu\n", $3);
        }
        free($3);
    }
    ;

/** @brief SWAP : Permutation des positions de deux objets */
swap_cmd:
    YSWAP object_id object_id travel_time
    {
        Point3D posA = objets[$2]; 
        Point3D posB = objets[$3]; 
        move_to_position(fd, $2, posA, posB, $4); 
        move_to_position(fd, $3, posB, posA, $4);
        printf("Action : SWAP entre objets %d et %d (%.2fs)\n", $2, $3, $4);
    }
    ;

/** @brief PONG : Aller-retour d'un objet depuis sa position vers une position cible n fois */
pong_cmd: 
    YPONG object_id coordinates repeats travel_time // Via coordonnées explicites
    {
        int i;
        Point3D start = objets[$2];
        for(i = 0 ; i < $4; i++) {
            move_to_position(fd, $2, start, $3, $5);
            move_to_position(fd, $2, $3, start, $5);
        }
        printf("Action : PONG terminé (%d cycles)\n", $4);
    }
    | YPONG object_id YLABEL repeats travel_time // Via coordonnées enregistrées
    {
        float x, y, z; // Adresse à passer à la fonction pour traitement
        if (get_position_by_label($3, &x, &y, &z)) {
            Point3D cible = {x, y, z};
            Point3D start = objets[$2]; // Coordonnées de départ
            int i;
            for(i = 0 ; i < $4; i++) {
                move_to_position(fd, $2, start, cible, $5); // Déplacement vers coordonnées cibles
                // L'absence de thread rend inutile la temporisation initialement prévue
                move_to_position(fd, $2, cible, start, $5); // Retour aux coordonnées de départ
            }
            printf("Action : PONG (via label '%s') terminé (%d cycles)\n", $3, $4); // Message de confirmation
        } else {
            printf("Erreur : Label '%s' inconnu pour l'action PONG\n", $3); // Si le label n'existe pas
        }
        free($3);  // Libération de la mémoire allouée par le lexer pour le stockage du label
    }
    ;

/** @brief MUTE : (Dés)activation audio. État binaire (0 = OFF, 1 = ON) */
mute_cmd:
    YMUTE object_id on_off // La règle on_off sécurise les valeurs possibles du non-terminal
    { 
        set_mute(fd, $2, $3);
        
        if ($3 == 1) { 
            printf("Action : MUTE objet %d -> ON\n", $2);
        } else {  
            printf("Action : MUTE objet %d -> OFF\n", $2);
        }
    }
    ;

/** @brief STATUS : Consultation de la table de spatialisation */
status_cmd:
    YSTATUS object_id
    {
        Point3D p = objets[$2];
        printf("Status Objet %d : x=%.2f, y=%.2f, z=%.2f\n", $2, p.x, p.y, p.z);
    }
    ;
    
/** @brief QUIT :