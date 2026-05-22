/**
 * @file dial.h
 * @brief Interface de création et gestion de la connexion UDP.
 * @author Jonathan Ntoula
 * @date Mai 2026
 * 
 * Ce fichier contient le prototype de la fonction dial()
 * qui crée et gère la liaison permettant l'envoi de commandes OSC.
 */

#ifndef DIAL_H  // "Si cet identifiant n'est pas encore défini..."
#define DIAL_H  // "...alors définis-le maintenant"

/**
 * @brief Établit une connexion UDP sortante.
 * * Résout le nom d'hôte et le port, crée une socket et la connecte 
 * pour permettre des écritures simplifiées.
 * 
 * @param machine Nom d'hôte ou adresse IP cible.
 * @param port Numéro de port cible (sous forme de chaîne).
 * @return int Descripteur de fichier (positif) de socket ou -1 en cas d'erreur.
 */

int dial( char * machine, char * port);

#endif