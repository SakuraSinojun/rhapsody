


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <elf.h>

static void  _dump(const void * buf, int size)
{
    int         i, j, t;
    const char * buffer = (const char *)buf;
    for(i=0; i<size; i+=16)
    {
        printf("%08x: ", i);
        t = 16;
        if(i + 16 > size)
            t = size - i;
        for(j=0; j<t; j++)
        {
            printf("%02x ", buffer[i+j] & 0xff);
        }
        for(j=t; j<16; j++)
        {
            printf("   ");
        }
        printf("\t");
        for(j=0; j<t; j++)
        {
            if(buffer[i+j] >= 36 && buffer[i+j] <= 126)
                printf("%c", buffer[i+j] & 0xff);
            else
                printf(".");
        }
        printf("\n");
    }
    return;
}


int main(int argc, char * argv[])
{
    Elf32_Ehdr *    ehdr;
    Elf32_Shdr *    shdr;
    Elf32_Sym  *    sym;
    int             len;
    FILE    *       fp;
    char    *       buffer;
    int             bytes_read;
    int             i;
    char    *       p;
    int             SymCount;
    int             SymIndex = -1;
    int             StrIndex = -1;

    if(argc != 2)
    {
        printf("%s elffile\n", argv[0]);
    }

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        perror("fopen");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    printf("len = %d\n", len);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(len);
    bytes_read = fread(buffer, len, 1, fp);
    fclose(fp);
    printf("bytes_read = %d\n", bytes_read);

    ehdr = (Elf32_Ehdr *)buffer;
    _dump(ehdr, sizeof(Elf32_Ehdr));
    shdr = (Elf32_Shdr *)(buffer + ehdr->e_shoff);
    printf("ehdr->e_shentsize   = %d\n", ehdr->e_shentsize);
    printf("ehdr->e_shnum       = %d\n", ehdr->e_shnum);
    p = buffer + shdr[ehdr->e_shstrndx].sh_offset;
    _dump(p, shdr[ehdr->e_shstrndx].sh_size);

    for(i=0; i<ehdr->e_shnum; i++)
    {
        printf("%s\n", p + shdr->sh_name);
        if(shdr->sh_type == SHT_SYMTAB && strcmp(p + shdr->sh_name, ".symtab") == 0)
        {
            SymIndex = i;
        }
        if(shdr->sh_type == SHT_STRTAB && strcmp(p + shdr->sh_name, ".strtab") == 0)
        {
            StrIndex = i;
        }
        shdr ++;
    }
    if(SymIndex == -1)
    {
        printf("no symbol table.\n");
        return -1;
    }

    shdr = (Elf32_Shdr *)(buffer + ehdr->e_shoff);
    sym  = (Elf32_Sym *)(buffer + shdr[SymIndex].sh_offset);
    p    = (char *)(buffer + shdr[StrIndex].sh_offset);
    SymCount = shdr[SymIndex].sh_size / sizeof(Elf32_Sym);

    _dump(p, shdr[StrIndex].sh_size);

    for(i=0; i<SymCount; i++)
    {
        if(sym[i].st_name != 0)
        {
            printf("%20s\t%08x\n", p + sym[i].st_name, sym[i].st_value);
        }
    }

    // _dump(shdr, ehdr->e_shentsize * ehdr->e_shnum);
    return 0;
}


