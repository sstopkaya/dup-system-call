
/* libraries */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 5000

/* this function prints the input format */
void printUsage ()
    {
    perror("Usage: ");
    perror("./appendMeMore filename num-bytes [x]\n");
    return ;
    }

int main(int argc, char *argv[]) 
	{
	if (argc!=3 && argc!=4)		/*check whether args are valid */
		{
		printUsage();
		exit(EXIT_FAILURE);
		}
	if (argc==4 && strcmp(argv[3],"x")!=0) /*check whether args are valid */
		{
		printUsage();
		exit(EXIT_FAILURE);
		}

	int i=0, j=0;			/*	identifiers */
	char **filePath;		/* to handle the relative path issue */
	char *fileName, *tempString;
	
	filePath=(char**)malloc(SIZE*sizeof(char*));	/* Dynamic memory allocation */
    fileName = (char*)malloc(SIZE*sizeof(char));
    tempString = (char*)malloc(SIZE*sizeof(char));
	for (int i = 0; i < SIZE; i++)
		{
		filePath[i] = (char*)malloc(SIZE*sizeof(char));
		}

    for (int i = 0; i < SIZE; ++i) /* initialization part */
    	{
    	for (int j = 0; j < SIZE; ++j)
    		{
    		filePath[i][j]='\0';
    		}
    	}
    for (int i = 0; i < SIZE; ++i)
    	{
    	fileName[i]='\0';
    	}
    for (int i = 0; i < SIZE; ++i)
    	{
    	tempString[i]='\0';
    	}
	strcpy(tempString, argv[1]);
	char *filePathTemp=strtok(tempString, "/"); /* parsing path */
	while(filePathTemp!=NULL)
		{
		strcpy(filePath[i], filePathTemp);
		filePathTemp=strtok(NULL, "/");
		i++;
		}
	for (j=0; j<i; j++)
		{
		/*fprintf(stdout, "--%s\n", filePath[j]); */
		}
	strcpy(fileName, filePath[(j-1)]);
	/* fprintf(stdout,"<%s\n", fileName); */

	/*____________________________________________________________________*/
	int fd;
	if (argc==3)
		{
		fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, 0666); 	/* creating a file */ 
		if (fd == -1)
			{
    		perror("open file");	/* error handling*/
			exit(EXIT_FAILURE);
			}
		for (int i = 0; i < atoi(argv[2]); ++i)
			{
			if (write(fd, "-", 1)==-1)	/* write a byte to the file */
				{
				perror("write error");	/* error handling*/
				exit(EXIT_FAILURE);
				}
			}
		}
	else if(argc==4 && strcmp(argv[3],"x")==0)
		{
		fd = open(argv[1], O_WRONLY | O_CREAT, 0666);  /* creating a file */ 
		if (fd == -1)
			{
    		perror("open file");	/* error handling*/
			exit(EXIT_FAILURE);
			}
		for (int i = 0; i < atoi(argv[2]); ++i)
			{
			lseek(fd, 0, SEEK_END); 	/* It moves file pointer position to the end of file */
			if (write(fd, "+", 1)==-1)	/* write a byte to the file */
				{
				perror("write error");	/* error handling*/
				exit(EXIT_FAILURE);
				}
			}
		}

	if (close(fd) < 0) 	/* closing file descriptor */
		{
	    perror("close file"); 	/* error handling */
		exit(1);
		}

	/* deallocation */
	for (int i = 0; i < SIZE; i++)
    	{
		free(filePath[i]);
    	}
    free(filePath);
    free(fileName);
    free(tempString);

	return 0;
}
