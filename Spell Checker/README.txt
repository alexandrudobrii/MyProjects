  Nume: Dobrii Ionut-Alexandru
  Grupa si seria: 324CA
  
  Tema este structurata in doua parti: o parte de generare a tuturor 
solutiilor valide si o parte care consta in compunerea si testarea
solutiilor generate anterior.

=======================================================================

  Generarea se face pe baza algoritmului Levenstein( sursa: ) care
calculeaza abatere minima dintre 2 cuvinte. Am modificat algoritmul
astel incat sa genereze distanta minima nu numai pentru primul cuvant
dat ca parametru, ci si pentru toate sufixele acestuia. Astfel, se 
parcurg sirul citit de la tastatura(din care am eliminat spatiile albe) 
si dictionarul citit din fisier si se apeleaza functia de creere a 
solutiilor (mentionata mai sus) pentru fiecare substing din sirul 
initial, respectiv pentru fiecare cuvant din dictionar. Solutiile 
create eficient sunt memorate intr-o matrice('solution').

=======================================================================

   Testarea se face prin parcurgerea matricei create prin generare.
La fiecare pas 'i = 1:lungimeSir' se combina se toate solutiile de pe 
fiecare linie punandu-se pe rand cate un spatiu intre oricare doua cuvinte, 
creandu-se astfel o noua solutie care contine sirul creat anterior, la
care se adauga suma dintre frecventele si abaterilor optime pana la pasul 
'j = 1:i' cu cele de la pasul curent. Solutia optima este actualizata doar 
daca este indeplinita una din urmatoarele 4 conditii:
	
	1. se va alege posibilitatea pentru care se obtine o abatere 
	   totala minima 
	
	2. daca exista mai multe posibilitati, dintre cele cu abaterea 
	   minima se va alege cea care contine un numar minim de cuvinte 
	
	3. daca si în acest caz exista mai multe posibilitati, dintre 
	   variantele ramase se alege cea cu frecventa totala maxima 

	4. se alege varianta in care sirul obtinut sa fie minim lexicografic

  Daca una din conditiile de mai sus este indeplinita se actualizeaza solutia
optima pentru pasul respectiv. La terminarea de parcurs a matricei, pe ultima 
pozitie a sirului optim se gaseste solutia cautata care va fi si afisata.

=======================================================================

Complexitatea algoritmului => O(n^4)
	-generare: O(n^4)
	-testare+combinare: O(n^2)
	
=======================================================================

Sursa algoritm Levenstein:
	http://en.wikipedia.org/wiki/Levenshtein_distance