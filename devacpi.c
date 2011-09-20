/*
 * ACPI driver
 * currently only poweroff command
 */
#include	"u.h"
#include	"../port/lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"
#include	"../port/error.h"

void 			init_acpi();
unsigned int 	*acpi_check_rsd_ptr(unsigned int *ptr);
unsigned int 	*acpi_get_rsd_ptr();
int			acpi_check_headr(unsigned int *ptr, char *sig);
int			acpi_enable();
void			acpi_poweroff();

ulong 		*SMI_CMD;
uchar		ACPI_ENABLE;
uchar		ACPI_DISABLE;
ulong		*PM1a_CNT;
ulong		*PM1b_CNT;
uint			SLP_TYPa;
uint			SLP_TYPb;
uint			SLP_EN;
uint			SCI_EN;
uchar		PM1_CNT_LEN;

struct rsdptr
{
	uchar	signature[8];
	uchar	checksum;
	uchar	oemid[6];
	uchar	revision;
	ulong	*rsdtaddr;
};

struct facp
{
	uchar		signature[4];
	ulong		length;
	uchar		unneded1[40-8];
	ulong		*dsdt;
	uchar		unneded2[48-44];
	ulong		*SMI_CMD;
	uchar		ACPI_ENABLE;
	uchar		ACPI_DISABLE;
	uchar		unneded3[64-54];
	ulong		*PM1a_CNT_BLK;
	ulong		*PM1b_CNT_BLK;
	uchar		unneded4[89-72];
	uchar		PM1_CNT_LEN;
};
	
void
do_poweroff(int n, char *va)
{
	acpi_enable();
	acpi_poweroff();
	return;
}

enum {
	Qdir,
	Qpoweroff,
};

static Dirtab acpidir[] = {
	".",			{Qdir, 0, QTDIR},	0,	DMDIR|055,
	"poweroff",	{Qpoweroff},		0,	0777,
};

static void
acpireset(void)
{
	init_acpi();
}

static void
acpiinit(void)
{
	init_acpi();
}

static Chan*
acpiattach(char *spec)
{
	return devattach('A', spec);
}

static Walkqid*
acpiwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, acpidir, nelem(acpidir), devgen);
}

static int
acpistat(Chan *c, uchar *dp, int n)
{
	return devstat(c, dp, n, acpidir, nelem(acpidir), devgen);
}

static Chan*
acpiopen(Chan *c, int omode)
{
	if( !iseve() ) {
		error( Eperm );
	}
	return devopen( c, omode, acpidir, nelem(acpidir), devgen);
}

static void
acpiclose(Chan *c)
{
	if( c->aux ) {
		free( c->aux );
		c->aux = nil;
	}
	return;
}

static long
acpiread(Chan *c, void *va, long n, vlong off)
{
	switch( (ulong)c->qid.path ) {
		case Qdir:
			return devdirread( c, va, n, acpidir, nelem(acpidir), devgen);
			break;
		case Qpoweroff:
			n = readstr(off, va, n, "power is on.. maybe");
			break;
		default:
			break;
	}

	return n;
}

static long
acpiwrite(Chan *c, void *va, long n, vlong)
{
	switch( (ulong)c->qid.path ) {
		case Qdir:
			error( Eisdir );
			break;
		case Qpoweroff:
			do_poweroff(n, va);
			break;
		default:
			break;
	}

	return n;
}

Dev acpidevtab = {
	'A', 
	"acpi",

	devreset,
	devinit,
	devshutdown,
	acpiattach,
	acpiwalk,
	acpistat,
	acpiopen,
	devcreate,
	acpiclose,
	acpiread,
	devbread,
	acpiwrite,
	devbwrite,
	devremove,
	devwstat,
};

void 
init_acpi()
{
}

unsigned int *
acpi_check_rsd_ptr(unsigned int *ptr)
{
}

unsigned int*
acpi_get_rsd_ptr()
{
}

int			
acpi_check_headr(unsigned int *ptr, char *sig)
{
}

int			
acpi_enable(){
}

void			
acpi_poweroff()
{
}