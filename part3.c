
/* libraries */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* main func */
int main(int argc, char *argv[])
	{
	int fd=0, fd2=0, fd3=0;	/* file descriptor identifier */
	char str[]="dup file descriptor --> fd\n"; 				/* input string */
	char str2[]="dup copy of the file descriptor --> fdCpy\n";/* input string */
	char str3[]="dup2 file descriptor --> fd2\n"; 				/* input string */
	char str4[]="dup2 file descriptor --> fd3\n"; 				/* input string */
	char str5[]="dup2 file descriptor --> fd4\n"; 				/* input string */

	/*______________________________________________________________dup part_*/

	fd=open("test.txt", O_WRONLY | O_APPEND | O_CREAT, 0666); /* opening the input file with write flag */
	if(fd < 0) 		/* file does not open */
		{
		perror("dup - file descriptor error\n");		/* error handling */
		}

	int fdCpy=dup(fd);						/* file descriptor duplicated */
	if (fdCpy < 0) 							/* duplicate error */
		{
		perror("dup - copied file descriptor error\n");	/* error handling */
		}

	if (write(fd, str, strlen(str))==-1)	/* write a str to the inp file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);
		}
		
	if (write(fdCpy,str2, strlen(str2))==-1)/* write a str to the same file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);
		}

	/* test if the file descriptors share the same offset */
	fprintf(stdout, "fd offset is : %ld\n", lseek(fd,0,SEEK_CUR));
	fprintf(stdout, "fdCpy offset is : %ld\n", lseek(fdCpy,0,SEEK_CUR));

	close(fd);		/* closing file descs */
	close(fdCpy);

	/*______________________________________________________________dup2 part_*/

	fd2 = open("test2.txt", O_WRONLY | O_APPEND | O_CREAT, 0666); /* opening file with write and append flag */
	if (fd2 < 0) 
		{
		perror("dup2 - file descriptor error\n"); /* error handling*/
		}
	if (dup2(fd2, fd3) == -1) 	/* DUP2 - duplicate the file descriptor */
		{
		perror("dup2 error");				/* error handling*/
		}
	if (write(fd2, str3, strlen(str3))==-1) 	/* write a str to the inp file */
		{
		perror("write error");				/* error handling*/	
		}
	if (write(fd3, str4, strlen(str4))==-1) 	/*	write a str to the same file */
		{
		perror("write error");				/* error handling*/		
		}

	/* test they share the same offset */
	fprintf(stdout, "fd2 offset is : %ld\n", lseek(fd2,0,SEEK_CUR));
	fprintf(stdout, "fd3 offset is : %ld\n", lseek(fd3,0,SEEK_CUR));

	close(fd);		/* closing file descriptors */
	close(fdCpy);
	close(fd2);
	close(fd3);

	fprintf(stderr, "______Test part for error handling______\n");
	int fd4 = open("test3.txt",O_WRONLY  | O_APPEND | O_CREAT, 0666); /* opening file with write and append flag */
	if (dup2(-1,fd4)==-1) /* invalid file descriptor */
		{
		perror("dup2 error (invalid oldfd) ");	/* error handling*/	
		}
	if (dup2(fd4,-1)==-1) /* invalid file descriptor */
		{
		perror("dup2 error (invalid newfd) ");	/* error handling*/	
		}
	int fd5=fd4;
	if (dup2(fd4,fd5)==-1)
		{
		perror("Error: same fd");	/* error handling*/	
		}
	if (write(fd4, str5, strlen(str5))==-1) 	/*	write a str to the same file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);
		}
	fprintf(stdout, "fd4 offset:%ld\nfd5 offset%ld\n", lseek(fd4,0,SEEK_CUR), lseek(fd5,0,SEEK_CUR));

	close(fd4);
	close(fd5);

	return 0;
	}
