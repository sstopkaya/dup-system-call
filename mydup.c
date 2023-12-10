
/* libraries */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int mydup(int fd)	/* my dup() implementation */
	{
	int newfd=0;
	newfd=fcntl(fd, F_DUPFD, 0); /* duplicate the file descriptor */
	return newfd;
	}

int mydup2(int oldfd, int newfd) 	/* my dup2() implementation */
	{
	if (newfd < 0) 			/* checking if file desc is valid */
		{
    	errno=EBADF;		/* invalid file desc or does not refer to an open file */
    	perror("Error: ");	/* error handling */
    	return -1;
    	}
	if (fcntl (oldfd, F_GETFL) < 0) /* checking if oldfd is valid */
		{
		errno=EBADF;		/* file desc is not a valid value or does not refer to an open file */
		perror("Error: ");	/* error handling */
		return -1;
		}
	if (oldfd == newfd)	/* if descs are the same and old desc is valid*/
		{
		return newfd; 	/*  new desc will be returned */
		}
	close (newfd);		/* new file desc is closed before duplicate */
	return fcntl(oldfd, F_DUPFD, newfd); /* file desc is duplicated */
	}


/* main func */
int main(int argc, char *argv[])
	{
	int fd=0, fd2=0, fd3=0;		/* file desc identifiers */
	char str[]="mydup - first file descriptor --> fd\n"; /* input strings */
	char str2[]="mydup - the copy of my first file descriptor --> fdCpy\n";
	char str3[]="dup2 - mydup2 file descriptor --> fd2\n";
	char str4[]="dup2 - mydup2 file descriptor --> fd3\n";
	char str5[]="dup2 - mydup2 same file descriptors\n";
	/* ______________________________________________________________dup part____*/	

	fd=open("inp.txt", O_WRONLY | O_APPEND | O_CREAT, 0666); /* opening file with write and append flag */
	if(fd < 0) 
		{
		perror("dup - file descriptor error\n");	/* error handling */
		}
	int fdCpy=mydup(fd); 	/* DUP - duplicate the file descriptor */
	if (fdCpy < 0)
		{
		perror("dup - copied file descriptor error\n");	/* error handling */
		}
	if (write(fd, str, strlen(str))==-1) 		/* write a str to the inp file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);
		}
	if (write(fdCpy,str2, strlen(str2))==-1) 	/*	write a str to the same file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);		
		}
	/* test they share the same offset */
	fprintf(stdout, "fd offset is : %ld\n", lseek(fd,0,SEEK_CUR));
	fprintf(stdout, "fdCpy offset is : %ld\n", lseek(fdCpy,0,SEEK_CUR));

	/* ______________________________________________________________dup2 part____*/	

	fd2 = open("inp2.txt",O_WRONLY | O_APPEND | O_CREAT, 0666); /* opening file with write and append flag */
	if (fd2 < 0) 
		{
		perror("dup2 - file descriptor error\n"); /* error handling*/
		}
	if (mydup2(fd2, fd3) == -1) 	/* DUP2 - duplicate the file descriptor */
		{
		perror("dup2 error");				/* error handling*/
		}
	if (write(fd2, str3, strlen(str3))==-1) 	/* write a str to the inp file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);		
		}
	if (write(fd3, str4, strlen(str4))==-1) 	/*	write a str to the same file */
		{
		perror("write error");				/* error handling*/
		exit(EXIT_FAILURE);		
		}

	/* test they share the same offset */
	fprintf(stdout, "fd2 offset is : %ld\n", lseek(fd2,0,SEEK_CUR));
	fprintf(stdout, "fd3 offset is : %ld\n", lseek(fd3,0,SEEK_CUR));

	close(fd);		/* closing file descriptors */
	close(fdCpy);
	close(fd2);
	close(fd3);

	fprintf(stderr, "______Test part for error handling______\n");
	int fd4 = open("inp3.txt",O_WRONLY | O_CREAT, 0666); /* opening file with write and append flag */
	if (mydup2(-1,fd4)==-1) /* invalid file descriptor */
		{
		perror("mydup2 error (invalid oldfd) ");	/* error handling*/	
		}
	if (mydup2(fd4,-1)==-1) /* invalid file descriptor */
		{
		perror("mydup2 error (invalid newfd) ");	/* error handling*/	
		}
	int fd5=fd4;
	if (mydup2(fd4,fd5)==-1)
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
