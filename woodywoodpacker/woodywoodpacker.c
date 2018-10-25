

#include <fcntl.h>
#include <unistd.h>
#include "woodpacker.h"

/*
** sets up the woody struct by pointing each variable to it's position
** in memory -> sets up the program header table, section table as well
** as creating the address to patch to the end of the shellcode
** and finally, pointing at the string table
*/

t_woody		init_woody(unsigned char *content)
{
        t_woody woodyboi;

        woodyboi.ehdr = (Elf64_Ehdr *)content;
        woodyboi.shdr = (Elf64_Shdr *)&content[woodyboi.ehdr->e_shoff];
        woodyboi.phdr = (Elf64_Phdr *)&content[woodyboi.ehdr->e_phoff];
        woodyboi.patch_return = convert_address(woodyboi.ehdr->e_entry);
	woodyboi.string_table = &content[woodyboi.shdr[
         	woodyboi.ehdr->e_shstrndx].sh_offset];
	return (woodyboi);
}

/*
** Finding the data section and getting all it's variables to prepare 
** for it's editing. Getting it's offset to know where to patch the file
** as well as where to write in the shellcode.
*/

t_data_sect	init_data_sect(t_woody wood)
{
	t_data_sect	data;
	int		i;

	i = 0;
        while (i < wood.ehdr->e_shnum)
        {
                if (strcmp(&wood.string_table[wood.shdr[i].sh_name],
                ".data") == 0)
                {
                        data.text_offset = wood.shdr[i].sh_offset;
                        data.entry_rewrite = wood.shdr[i].sh_addr;
                        data.text_size = wood.shdr[i].sh_size;
                	return (data);
		}
                i++;
        }
	printf("Error : Data section not found\nExiting..\n");
	exit(1);
}

/*
** Main woodywoodpacker body
*/

void		woody_woodpacker(unsigned char *content, size_t size)
{
	t_woody		wood;
	t_data_sect	data;

	wood = init_woody(content);
	data = init_data_sect(wood);
	update_section_table_offsets(&wood, data.text_offset);
	update_program_header_table_offsets(&wood, data.text_offset);
	write_file(wood, data, content, size);
}





