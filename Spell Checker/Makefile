# Declarațiile de variabile
CC = g++
CCFLAGS = -Wall
SRC = SpellChecker.cpp
PROGRAM = SpellChecker

# Regulă de compilare
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)

run:
	./SpellChecker

# Regulile de "curațenie" (se folosesc pentru ștergerea fișierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~ 
