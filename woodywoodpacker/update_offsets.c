

#include "woodpacker.h"

/*
**  the offsets to make needed space for the shellcode to be
*/

void            update_program_header_table_offsets(t_woody *wood,
                size_t data_offset)
{
        int     i;

        i = 0;
        while (i < (*wood).ehdr->e_phnum)
        {
                if ((*wood).phdr[i].p_offset > 0 &&
                (*wood).phdr[i].p_type == PT_LOAD)
                {
                        (*wood).phdr[i].p_flags |= PF_X;
                        (*wood).phdr[i].p_filesz += VSIZE;
                        (*wood).phdr[i].p_memsz += VSIZE;
                }
                if ((*wood).phdr[i].p_offset > data_offset)
                {
                        (*wood).phdr[i].p_offset += VSIZE;
                        (*wood).phdr[i].p_vaddr += VSIZE;
                }
                i++;
        }
}

void            update_section_table_offsets(t_woody *wood, size_t data_offset)
{
        int     i;

        i = 0;
        while (i < (*wood).ehdr->e_shnum)
        {
                if (strcmp(&(*wood).string_table[(*wood).shdr[i].sh_name],
                ".data") == 0)
                        (*wood).shdr[i].sh_size += VSIZE;
                if ((*wood).shdr[i].sh_offset > data_offset)
                {
                        (*wood).shdr[i].sh_offset += VSIZE;
                        (*wood).shdr[i].sh_addr += VSIZE;
                }
                i++;
        }
}
