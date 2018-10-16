

#include "ELFlowlevel.h"

#define DUMMY_PADDING 	51 // 44 + 7 for the return patch 
//size_t size = 44;



char shell[] = "\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0c\x00\x00\x00\x0f\x05\xeb\x14\x57\x56\x52\xe8\xe4\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x0a\x5a\x5e\x5f";

void		expand_text_section(unsigned char *content, size_t size)
{
	Elf64_Ehdr 	*ehdr;
	Elf64_Phdr	*phdr;
	Elf64_Shdr	*shdr;
	int		i;
	char		*string_table;
	unsigned char	*patch_file;

	ehdr = (Elf64_Ehdr *)content;
	phdr = (Elf64_Phdr *)&content[ehdr->e_phoff];
	shdr = (Elf64_Shdr *)&content[ehdr->e_shoff];

	/* Start of the string table
	**/
	
	patch_file = convert_address(ehdr->e_entry);	

	string_table = &content[shdr[ehdr->e_shstrndx].sh_offset];
	
	/* Find the text section of the file 
	*/

	size_t 		text_offset;
	size_t		text_size;

	/* Turn everything below into a function for neatness 
	********
	*****  call it something generic like find_text_offset or somethi
	***    or somethign.
	*/	
	size_t entry_rewrite;
	i = 0;
	while (i < ehdr->e_shnum)
	{
		if (strcmp(&string_table[shdr[i].sh_name], ".data") == 0)
		{
			printf("Found text section. It's offset is at %zu\n", 
			shdr[i].sh_offset);
			text_offset = shdr[i].sh_offset;
			entry_rewrite = shdr[i].sh_addr;
			text_size = shdr[i].sh_size;
			shdr[i].sh_size += DUMMY_PADDING;
			break ;
		}
		i++;
	}

	/* Reposition everythin */
	/* Inside the first */ 

	/* update program header section offsets */

	i = 0;
	while (i < ehdr->e_phnum)
	{
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_offset > 0)
		{
			phdr[i].p_flags |= PF_X;
			phdr[i].p_filesz += DUMMY_PADDING;
			phdr[i].p_memsz += DUMMY_PADDING;
		}
		if (phdr[i].p_offset > text_offset)
		{
			phdr[i].p_offset += DUMMY_PADDING;			
			phdr[i].p_vaddr += DUMMY_PADDING;
		}
		i++;
	}
	
	/* expand the text section */
	i = 0;
	while (i < ehdr->e_shnum)
	{
		if (shdr[i].sh_offset > text_offset)
		{
			shdr[i].sh_offset += DUMMY_PADDING;
			shdr[i].sh_addr += DUMMY_PADDING;
		}
		i++;
	}


	/* Write the zeroed out padding to the file 
	*/
	
	FILE *fp = fopen("outfile", "ab+");
//	ehdr->e_shoff += DUMMY_PADDING;
//	ehdr->e_entry = size;
//	ehdr->e_shstrndx += DUMMY_PADDING;
	ehdr->e_entry = entry_rewrite;
	fwrite(content, text_offset  /*+ text_size */, 1, fp);
	/*write in the buffer of zeroes after all the executable code  */
	
	/* patching the binary */
//	fwrite(content, size, 1, fp);
	fwrite(shell, 44, 1, fp);
//	fclose(fp);
//	exit(1);		
//	fwrite(shell, 44, 1, fp);
	fwrite(patch_file, 7, 1, fp);
/*	if (text_size > 44)
	{
		char zer[text_size - 51];
		bzero(zer, text_size - 51);
		fwrite(zer, text_size - 51, 1, fp);
	} */
//	char	zero_buffer[DUMMY_PADDING];
//	bzero(zero_buffer, DUMMY_PADDING);
//	fwrite(zero_buffer, DUMMY_PADDING, 1, fp);	
	
	/* shift the content buffer to everything after the text section 
	*  write the remaininig contents into teh file
	*/

	content = content + text_offset + text_size;
	size -= (text_offset + text_size);
	fwrite(content, size, 1, fp);
	fclose(fp);		
	
	printf("done\n");

}


int		main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage : %s <file>\n", argv[0]);
		exit(1);
	}
	unsigned char 	*content;
	size_t		size;

	/* map file */

	map_file(&content, &size, argv[1]);
	
	/* Find hte text section */
	
	expand_text_section(content, size);

}
