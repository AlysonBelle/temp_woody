

#include <sys/mman.h>
#include "woodpacker.h"

void	help(void)
{
	printf("Usage : ./woody <filename>\n");
	exit(1);
}

int	is_valid_elf(unsigned char *content)
{
	Elf64_Ehdr	*ehdr;

	ehdr = (Elf64_Ehdr *)content;
	if (content[0] == 0x7f && strncmp((char *)&content[1], "ELF", 3) == 0 &&
	ehdr->e_ident[EI_CLASS] == ELFCLASS64 && ehdr->e_type == ET_EXEC)
		return (1);
	return (-1);	
}

int	main(int argc, char **argv)
{
	unsigned char 	*content;
	size_t		size;

	if (argc != 2)
		help();
	mapping(&content, &size, argv[1]);
	if (is_valid_elf(content) == -1)
	{
		printf("File is not an ELF64 executable\n");
		munmap(content, size);
		help();
	}
	woody_woodpacker(content, size);		
}
