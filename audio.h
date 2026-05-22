/**
 * @file audio.h
 * @brief Interface de contrôle des paramètres audio (Mute, Gain).
 * @author Jonathan Ntoula
 * @date Mai 2026
 * 
 * @details Ce fichier contient les prototypes des fonctions permettant d'agir sur les 
 * paramètres audio (notamment le Muting) des objets du moteur immersif DME7.
 * Il assure le lien entre le parser et le module d'implémentation audio.c.
 */

#ifndef AUDIO_H
#define AUDIO_H

/**
 * @brief Modifie l'état de muting (ON/OFF) d'un objet audio.
 * 
 * @details Cette fonction construit et envoie le message OSC correspondant
 * au paramètre de "Mute" pour un objet spécifique identifié par son ID.
 * 
 * @param fd   Descripteur de la socket UDP ouverte.
 * @param id   Identifiant numérique de l'objet (index 1 à 64).
 * @param mute Entier représentant l'état (1 pour actif/silence, 0 pour inactif/diffusion).
 */
void set_mute(int fd, int id, int mute);

#endif