OBJS	= main.o list.o p0.o p1.o p2.o p3.o
SOURCE	= main.c list.c p0.c p1.c p2.c p3.c
HEADER	= list.h p0.h p1.h p2.h p3.h
OUT	= shell
CC	 = gcc
FLAGS	 = -g3 -c -Wall
LFLAGS	 = 
# -g modo debug
# -c flag que genera codigo objeto en archivos separados


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# crear/compilar los archivos individuales (separados)
main.o: main.c
	$(CC) $(FLAGS) main.c 

list.o: list.c
	$(CC) $(FLAGS) list.c 

p0.o: p0.c
	$(CC) $(FLAGS) p0.c 

p1.o: p1.c
	$(CC) $(FLAGS) p1.c 

p2.o: p2.c
	$(CC) $(FLAGS) p2.c 

p3.o: p3.c
	$(CC) $(FLAGS) p3.c 


# limpiar archivos salida
clean:
	rm -f $(OBJS) $(OUT)

# ejecutar el programa
run: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	./$(OUT)

ejecutar: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	./$(OUT)

# compilar el programa con informacion del debugger
debug: $(OUT)
	valgrind $(OUT)

# ejecutar el programa con valgrind para detectar errores
valgrind: $(OUT)
	valgrind ./$(OUT)

# ejecutar el programa con valgrind para buscar leaks
valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full ./$(OUT)
# ejecutar el programa con valgrind para buscar problemas de memoria
valgrind_mem: $(OUT)
	valgrind --tool=memcheck ./$(OUT)

# ejecutar programa con varlgrind para buscar leaks (extremo)
valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT)
