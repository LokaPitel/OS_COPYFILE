#include <stdio.h>

#include <pthread.h>

#include <semaphore.h>



sem_t waitForReader;

sem_t waitForWriter;



char* fileToWrite = NULL;

char* fileToRead = NULL;



char line[80] = "";



FILE *fileToCopy = NULL;

FILE *fileToWriteIn = NULL;



void* readFile(void *arg)

{

	fileToCopy = fopen(fileToRead, "r");

	

	while (! feof(fileToCopy))

	{

		sem_wait(&waitForReader);

		

		fgets(line, 80, fileToCopy);

		

		sem_post(&waitForWriter);

	}

	

	return NULL;

}



void* writeFile(void *arg)

{

	fileToWriteIn = fopen(fileToWrite, "w");

	

	do

	{

		sem_wait(&waitForWriter);

		

		fprintf(fileToWriteIn, "%s", line);

		

		sem_post(&waitForReader);

	} while (! feof(fileToCopy));

	

	return NULL;

}



int main(int argc, char** argv)

{

	if (argc != 3)

	{

		puts("Неправильное количество аргументов");

		return 1;

	}

	

	fileToRead = argv[1];

	fileToWrite = argv[2];

	

	sem_init(&waitForReader, 0, 0);

	sem_init(&waitForWriter, 0, 1);

	

	pthread_t first;

	pthread_t second;

	

	pthread_create(&first, NULL, readFile, NULL);

	pthread_create(&second, NULL, writeFile, NULL);

	

	pthread_join(first, NULL);

	pthread_join(second, NULL);



	return 0;

}
