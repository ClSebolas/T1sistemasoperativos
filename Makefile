all: carrera

carrera: main.o
	g++ -std=c++11 -o carrera main.o -lpthread

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm *.o carrera

##Aqui All compila el programa creando el ejecutable Carrera
##Luego Carrera indica ejecutar con c++11  y habilita la biblioteca de hilos
##Ademas main.o compila el archivo fuente main.cpp para generar el archivo objeto main.o
##Finalmente el clean limpia los archivos objeto y el ejecutable. Para usarlo se usa el comando make clean.
