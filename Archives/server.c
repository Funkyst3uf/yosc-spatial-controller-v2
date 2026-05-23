#include "sys.h"

char * answer(char * port, int (* fun)(int)) {
    struct addrinfo * info, * p;
    struct addrinfo indices;
    int fd, t;
    struct sockaddr_storage fromaddr; // sockaddr_storage est plus sûr pour la taille
    socklen_t len = sizeof fromaddr;
    char buffer[1]; // Pour le premier octet déclencheur

    memset(&indices, 0, sizeof indices);
    indices.ai_flags = AI_PASSIVE; 
    indices.ai_family = AF_INET; 
    indices.ai_socktype = SOCK_DGRAM; // UDP

    t = getaddrinfo(0, port, &indices, &info);
    if (t != 0)
        return "answer: cannot get info on port";

    // Ouvrir la socket
    for(p = info; p != 0; p = p->ai_next){
        fd = socket(p->ai_family, p->ai_socktype, 0);
        if (fd >= 0) break;
    }

    if(p == 0) return "answer: pas moyen d'avoir une socket";

    t = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof t);

    if (bind(fd, p->ai_addr, p->ai_addrlen) < 0){
        close(fd);
        return "answer: pas moyen de binder la socket";
    }

    freeaddrinfo(info);

    // ATTENTION : listen() et accept() sont supprimés ici car inexistants en UDP.

    for(;;) {
        /* En UDP, on utilise recvfrom pour "attendre" un paquet.
           MSG_PEEK permet de voir le paquet sans le consommer, 
           pour que la fonction 'fun' puisse le lire entièrement.
        */
        t = recvfrom(fd, buffer, 1, MSG_PEEK, (struct sockaddr *)&fromaddr, &len);
        if (t < 0) return "answer: erreur sur recvfrom\n";

        t = fork();
        if (t < 0){
            perror("fork");
            continue;
        }

        if (t != 0){ 
            /* Parent : En UDP, on ne ferme pas 'fd' car c'est la seule socket qu'on a.
               On attend un peu ou on utilise un mécanisme pour ne pas boucler 
               sur le même paquet instantanément (ici, une petite pause ou la 
               gestion du flux dans l'enfant est nécessaire). */
            usleep(1000); // Petite pause pour laisser l'enfant consommer le paquet
        } 
        else { 
            /* Enfant */
            // Note: En UDP, 'connect' sur une socket DGRAM permet de fixer 
            // le destinataire pour utiliser read/write comme en TCP.
            connect(fd, (struct sockaddr *)&fromaddr, len);
            fun(fd);
            exit(0);
        }
    }
}

#ifdef DEBUG
int foo(int n){
    char buf[1024];
    int t;
    
    // En UDP, on lit le paquet entier
    t = read(n, buf, sizeof(buf));
    if (t > 0) {
        printf("J'ai reçu un paquet UDP de %d octets\n", t);
        // Echo
        write(n, buf, t);
    }
    return 0;
}

int main(int ac, char * av[]) {
    if (ac < 2) return 1;
    char * p = answer(av[1], foo);
    perror(p);
    return 1;
}
#endif