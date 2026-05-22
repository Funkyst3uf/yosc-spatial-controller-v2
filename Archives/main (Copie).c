/* n-dial.c
Renvoie une connexion vers un serveur, ou -1 en utilisant IPv4
*/

#include "sys.h"
#include "dial.h"
#include "spatial.h"
#include "audio.h"

int main(int ac, char * av[]) {
    printf("Démarrage du pilote DME7...\n");

    if (ac < 3) {
        fprintf(stderr, "Usage: %s <ACTION> <IP> <PORT>\n", av[0]);
        return 1;
    }

    int t, fd = dial(av[1], av[2]);

    if (fd < 0) {
        perror("Erreur de connexion (dial)");
        return 1;
    }

    printf("Prêt à envoyer des données (Ctrl+D pour quitter)\n");

    char buffer[1024];
    char path[1024];
    char cmd[16];
    int id, speed;
    Point3D A, B;

    while((t = read(0, buffer, sizeof(buffer) - 1)) > 0) { 
        buffer[t] = '\0';

        // On cherche : une chaîne, un entier, et trois flottants
        int i = sscanf(buffer, "%15s %d %f %f %f %d", cmd, &id, &B.x, &B.y, &B.z, &speed);

        if (i >= 2) { // Au moins la commande et l'ID
            snprintf(path, sizeof(path), OSC_PHYSICAL_POSITION "%d", id);

            if (strcmp(cmd, "JUMP") == 0 && i == 5) {
                jump_to_position(fd, path, B);
                A = B;  // stockage des coordonnées
            } 
            else if (strcmp(cmd, "MOVE") == 0 && i == 6) {
                move_to_position(fd, path, A, B, speed);
            } 

            else if (strcmp(cmd, "MUTE") == 0 && i == 3) {
                set_mute(fd, path, A, B, speed);
            } 
            else {
                printf("Usage : [JUMP|MOVE] ID X Y Z <option : SPEED>\n");
            }
        }
    }

    close(fd);
    return 0;

}
