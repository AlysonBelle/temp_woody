

#include "woodpacker.h"

unsigned char		*convert_address(Elf64_Addr original_entry)
{
	unsigned char	*address;
	unsigned char	bytes[4];
	unsigned char	patch[] = "\xb8\x00\x00\x00\x00\xff\xe0";
	unsigned long long swapped;

	swapped = ((original_entry >> 24) & 0xff) |
		((original_entry << 8) & 0xff0000) |
		((original_entry >> 8) & 0xff00) |
		((original_entry << 24) & 0xff000000);
	address = (char *)malloc(sizeof(char) * 7);
	bytes[0] = (swapped >> 24) & 0xff;
	bytes[1] = (swapped >> 16) & 0xff;
	bytes[2] = (swapped >> 8) & 0xff;
	bytes[3] = swapped & 0xff;
	memcpy(&patch[1], bytes, 4);
	memmove(address, patch, 7);
	return (address);
}
