
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void		mapping(unsigned char **content, size_t *size, char *filename)
{
	int		fd;
	off_t		offset;	

	fd = open(filename, O_RDONLY);
	offset = lseek(fd, 0, SEEK_END);
	if (offset < 0)
	{
		printf("Error : lseek\n");
		exit(1);
	}
	*size = offset;
	*content = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (*content == MAP_FAILED)
	{
		printf("Error : mmap\n");
		exit(1);
	}
	close(fd);
}
