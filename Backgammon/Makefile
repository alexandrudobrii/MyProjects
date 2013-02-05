 

all:
	g++ -o Backgammon BGBoard.cpp Backgammon.cpp -Wall 

client5:
	./Backgammon 127.0.0.1 7777 5 5

client7:
	./Backgammon 127.0.0.1 7777 7 5

client8:
	./Backgammon 127.0.0.1 7777 8 5

server:
	java -jar server.jar 7777 30  

clean:
	rm -f Backgammon
