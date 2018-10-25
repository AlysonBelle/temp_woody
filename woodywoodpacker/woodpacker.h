

#ifndef WOODPACKER_H
# define WOODPACKER_H

# include <stdio.h>
# include "alylibc/inc/lib.h"
# include <elf.h>

# define VSIZE 45

/*
** Struct for all the main sections in an ELF binary
** as well as the string table and the bytes needed to patch
** shellcode to point back to original entry point (patch_return)
*/

typedef struct 		s_woody
{
	Elf64_Ehdr	*ehdr;
	Elf64_Phdr	*phdr;
	Elf64_Shdr	*shdr;
	unsigned char	*patch_return;
	char		*string_table;
}			t_woody;

/*
** Section contents relevant to the data section (target for the patch)
*/

typedef struct		s_sect_info
{
	size_t          text_offset;
	size_t          entry_rewrite;
	size_t          text_size;
}			t_data_sect;

/*
** Code ot patch the original entry point to the start of the file
** Location : convert_address.c
*/

unsigned char 		*convert_address(Elf64_Addr original_entry);

/*
** Main woodywoodpacker program
** Location : woodywoodpacker.c
*/

void			woody_woodpacker(unsigned char *content, size_t size);


/*
** Update section and program headers after inserting shellcode.
** Location : update_offsets.c
*/

void            	update_section_table_offsets(t_woody *wood,
		size_t data_offset);
void            	update_program_header_table_offsets(t_woody *wood,
                size_t data_offset);

/*
** Write the final modified file to the "woody" executable
** Location: write_file.c
*/

void            	write_file(t_woody wood, t_data_sect data,
                unsigned char *content, size_t size);

/*
** Mapping the contents of the file into the unsigned char 
*/

void            	mapping(unsigned char **content, size_t *size, char *filename);










#endif 
