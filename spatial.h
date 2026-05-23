/**
 * @file spatial.h
 * @brief Interface de gestion des trajectoires et du positionnement 3D
 * @author Jonathan Ntoula
 * @date Mai 2026
 *
 * @details Ce fichier contient les prototypes des fonctions permettant d'envoyer 
 * au processeur DME7 des commandes de spatialisation d'objets.
 * Il définit la structure de données représentant les coordonnées des objets
 * pour le cours Interprétation & Compilation de l'Université Paris 8.
 */

#ifndef SPATIAL_H
#define SPATIAL_H

/**
 * @struct Point3D
 * @brief Représente des coordonnées cartésiennes dans l'espace tridimensionnel.
 * @details Cette structure est utilisée pour définir la position des objets audio
 * dans le repère du moteur immersif.
 */
typedef struct {
    float x; /**< Coordonnée sur l'axe horizontal x (gauche-droite, x positif à droite). */
    float y; /**< Coordonnée sur l'axe de profondeur y (avant-arrière, y positif devant). */
    float z; /**< Coordonnée sur l'axe vertical z (altitude, avec z positif en haut). */
} Point3D; 

/**
 * @struct LabelPosition
 * @brief Structure associant des coordonnées 3D à une étiquette textuelle (label).
 * @details Permet de stocker des positions mémorisées.
 */
typedef struct {
    char nom[32]; /**< Nom unique identifiant la position. */
    float x, y, z; /**< Coordonnées cartésiennes associées au label. */
} LabelPosition;

/* --- Déclarations des variables globales (partagées) --- */

/** @brief Table de spatialisation des 64 objets, maintenue par spatial.c. */
extern Point3D objets[65];

/** @brief Bibliothèque de positions mémorisées (labels). */
extern LabelPosition table_symboles[50];

/** @brief Nombre actuel d'étiquettes enregistrées. */
extern int nb_favoris;

/* --- Prototypes des fonctions --- */

/**
 * @brief Initialise les tables de données du moteur spatial.
 * @details Remplit la table des objets et la bibliothèque de labels avec des valeurs par défaut.
 */
void init_tables();

/**
 * @brief Positionne instantanément un objet audio (téléportation).
 * @details Envoie un message OSC unique pour placer l'objet aux coordonnées cibles.
 *
 * @param fd   Descripteur de la socket UDP connectée.
 * @param id   Identifiant de l'objet audio (index de 1 à 64).
 * @param B    Coordonnées cibles du point d'arrivée.
 */
void jump_to_position(int fd, int id, Point3D B);

/**
 * @brief Déplace un objet de manière continue entre deux points.
 * @details Calcule une trajectoire rectiligne entre A et B par interpolation linéaire (LERP)
 * et envoie une série de messages OSC via la fonction jump()
 * 
 * @param fd    Descripteur de fichier de la socket UDP connectée.
 * @param id    Identifiant de l'objet audio.
 * @param A     Point de départ du mouvement.
 * @param B     Point d'arrivée du mouvement.
 * @param speed Vitesse ou durée du déplacement (selon implémentation).
 */
void move_to_position(int fd, int id, Point3D A, Point3D B, float speed);

/**
 * @brief Enregistre une position 3D associée à un nom dans la bibliothèque.
 * @param nom Nom de l'étiquette.
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @param z Coordonnée Z.
 */
void set_position_label(char *nom, Point3D p);

/**
 * @brief Recherche les coordonnées associées à un label dans la bibliothèque.
 * @param nom Nom du label recherché.
 * @param x Pointeur pour récupérer la coordonnée X.
 * @param y Pointeur pour récupérer la coordonnée Y.
 * @param z Pointeur pour récupérer la coordonnée Z.
 * @return int 1 si le label est trouvé et les coordonnées extraites, 0 sinon.
 */
int get_position_by_label(char *nom, float *x, float *y, float *z);

#endif