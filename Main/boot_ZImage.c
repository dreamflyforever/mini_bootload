#include"setup.h"
#include"types.h"
#include"lib.h"
//from 0x00200000 to 0x30008000, size = 0x00300000
//uint params_to_linux=0x30000100;
#define params_to_linux	0x30000100
#define Boot_linux	0x30008000

static struct tag *params;

static void setup_start_tag(void);
static void setup_memory_tags(void);
static void setup_commandline_tag(char *commandline);
static void setup_end_tag(void);

int do_bootm_linux(void)
{

    int machid = 168;
    void (*kernel_entry)(int zero, int arch, uint params);
    char *commandline = "noinitrd root=/dev/mtdblock2 init=nuxrc console=ttySAC0";

    kernel_entry = (void (*)(int, int, uint))Boot_linux;

    setup_start_tag();
    setup_memory_tags();
    setup_commandline_tag(commandline);
    setup_end_tag();
    kernel_entry(0, machid, params_to_linux);

    return 1;
}

static void setup_start_tag(void)
{
    params = (struct tag *) params_to_linux;

    params->hdr.tag = 0x54410001;
    params->hdr.size = ((sizeof(struct tag_header) + sizeof(struct tag_core)) >> 2);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = ((struct tag *)((u32 *)(params) + (params)->hdr.size));
}


static void setup_memory_tags(void)
{
    int i;

    for (i = 0; i < 1; i++) {
	params->hdr.tag = 0x54410002;
	params->hdr.size = ((sizeof(struct tag_header) + sizeof(struct tag_mem32)) >> 2);

	params->u.mem.start = 0x30000000;
	params->u.mem.size = 0x4000000;

	params = ((struct tag *)((u32 *)(params) + (params)->hdr.size));
    }
}

static void setup_commandline_tag(char *commandline)
{
    char *p;

    if (!commandline)
	return;

    for (p = commandline; *p == ' '; p++);

    if (*p == '\0')
	return;

    params->hdr.tag = 0x54410009;
    params->hdr.size =
	(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;
    strcpy (params->u.cmdline.cmdline, p);
    params = ((struct tag *)((u32 *)(params) + (params)->hdr.size));
}

static void setup_end_tag(void)
{
    params->hdr.tag = 0x00000000;
    params->hdr.size = 0;
}
