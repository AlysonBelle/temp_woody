

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "woodpacker.h"

char shell[] = "\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0c\x00\x00\x00\x0f\x05\xeb\x14\x57\x56\x52\xe8\xe4\xff\xff\xff\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x0a\x5a\x5e\x5f";

void            write_file(t_woody wood, t_data_sect data,
                unsigned char *content, size_t size)
{
        int             fd;
        unsigned char   *file_content;
        size_t          plc;

        file_content = (unsigned char *)malloc(sizeof(unsigned char) * size +
        VSIZE);
        plc = 0;
        fd = open("woody", O_RDWR | O_CREAT | O_TRUNC, 0777);
        wood.ehdr->e_entry = data.entry_rewrite;
        memcpy(file_content, content, data.text_offset);
        plc += data.text_offset;
        memcpy(&file_content[plc], shell, VSIZE);
        plc += VSIZE;
        memcpy(&file_content[plc], wood.patch_return, 7);
        plc += 7;
        content = content + data.text_offset + data.text_size;
        size -= (data.text_offset + data.text_size);
        plc += size;
        memcpy(&file_content[plc], content, size);
        write(fd, file_content, plc);
        close(fd);
}
