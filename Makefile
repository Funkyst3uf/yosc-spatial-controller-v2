# Makefile pour le contrôleur spatial DME7
# Auteur : Jonathan Ntoula

CC = gcc
YACC = yacc
LEX = lex
CFLAGS = -Wall
LIBS = -llo -lm

CLIENT_TARGET = spatial-controller
SERVER_TARGET = dme7-sim

# --- LISTE DES OBJETS ---
CLIENT_OBJ = main.o dial.o spatial.o audio.o y.tab.o lex.yy.o
SERVER_OBJ = dme7_simulator.o

# Tous les headers indispensables pour le client
HEADERS = sys.h dial.h spatial.h audio.h y.tab.h

# Par défaut, on compile tout
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# --- CIBLES SÉPARÉES (Raccourcis de compilation) ---

# Raccourci pour compiler UNIQUEMENT le client
client: $(CLIENT_TARGET)

# Raccourci pour compiler UNIQUEMENT le serveur de simulation
server: $(SERVER_TARGET)

# Raccourci pour générer le code du parser (.c et .h)
parser: y.tab.c y.tab.h

# Clin d'œil Jamiroquai : Génère le fichier texte de l'automate (y.output)
tomato: yosc.y
	$(YACC) -v yosc.y

# --- ÉDITION DE LIENS (Production des exécutables) ---

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT_TARGET) $(LIBS)

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER_TARGET) -llo

# --- GÉNÉRATION DES SOURCES DE LA GRAMMAIRE (Flex / Bison) ---

# Génération classique (uniquement le code et le header pour le parser)
y.tab.c y.tab.h: yosc.y
	$(YACC) -d yosc.y

# Génère lex.yy.c à partir de yosc.l
lex.yy.c: yosc.l y.tab.h
	$(LEX) yosc.l

# --- COMPILATION DES FICHIERS OBJETS (.c -> .o) ---

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

spatial.o: spatial.c $(HEADERS)
	$(CC) $(CFLAGS) -c spatial.c -o spatial.o

dial.o: dial.c $(HEADERS)
	$(CC) $(CFLAGS) -c dial.c -o dial.o

audio.o: audio.c $(HEADERS)
	$(CC) $(CFLAGS) -c audio.c -o audio.o

dme7_simulator.o: dme7_simulator.c
	$(CC) $(CFLAGS) -c dme7_simulator.c -o dme7_simulator.o

y.tab.o: y.tab.c y.tab.h
	$(CC) $(CFLAGS) -c y.tab.c -o y.tab.o

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.yy.o

# --- NETTOYAGE ---

clean:
	rm -f *.o $(CLIENT_TARGET) $(SERVER_TARGET) y.tab.c y.tab.h y.output lex.yy.c

re: clean all

.PHONY: all clean re client server parser tomato
