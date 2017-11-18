#include "cktrack.h"

cktrack::cktrack(void) {}
cktrack::~cktrack(void) {}

static int TBPP=2;

int cktrack::load_config(void)
{
	FILE *fp=NULL;
	char data[32];
	int i=-1;
	int x, y, z;

	if((fp=fopen("config/config", "r"))==NULL)
	{
		fprintf(stderr, "could not open config file config/config\n");
		return -1;
	}
	while(fgets(data, sizeof(data), fp))
	{
		char *cp=NULL;
		char *dp=NULL;
		if(!(cp=strchr(data, ':'))) continue;
		if((dp=strrchr(data, '\n'))) *dp=0;
		if(data[0]=='n')
		{
			char tmp[64]="";

			i++;
			snprintf(tmp, sizeof(tmp), "graphics/track/%s", cp+1);
			tiles[i].image=CL_PNGProvider::create(tmp, NULL);
		}
		else if(data[0]=='s')
		{
			tiles[i].symbol=*(cp+1);
		}
	}
	fclose(fp);
	return 0;
}

int cktrack::load_track(const char *path)
{
	FILE *fp=NULL;
	char data[128]="";
	int x=0;
	int y=0;
	int i=0;
	CL_Canvas *tmpCan=new CL_Canvas(TRACK_SIZE_X*64, TRACK_SIZE_Y*64, 1,
	CL_Color::get_red_mask(RGB565),
	CL_Color::get_green_mask(RGB565),
	CL_Color::get_blue_mask(RGB565),
	CL_Color::get_alpha_mask(RGB565), true, 0);

	if((fp=fopen(path, "r"))==NULL)
	{
		fprintf(stderr, "could not open config file %s\n", path);
		delete tmpCan;
		return -1;
	}

	set_laps(6);

	while(fgets(data, sizeof(data), fp))
	{
		char *cp=data;
		if(strncmp(data, "laps=", strlen("laps="))==0)
		{
			cp+=strlen("laps=");
			set_laps(atoi(cp));
			continue;
		}
		else if(strncmp(data, "pos", strlen("pos"))==0)
		{
			if(i>29) continue;
			cp=strchr(data, '=');
			spos[i].x=atoi(cp+1);
			cp=strchr(data, ' ');
			spos[i].y=atoi(cp+1);
			spos[i].ang=atoi(strchr(cp+1, ' '));
			i++;
			continue;
		}

		while(cp && *cp!='\n')
		{
			int n=0;
			while(n<TILENO)
			{
				if(*cp==tiles[n].symbol)
				{
					tiles[n].image->put_target(x, y, 0, tmpCan);
					track_grid[x/BLK][y/BLK]=*cp;
					break;
				}
				n++;
				if(n==TILENO)
				{
					fprintf(stderr, "tile symbol '%c' not found\n", *cp);
					delete tmpCan;
					return -1;
				}
			}
			x+=BLK;
			cp++;
		}
		x=0;
		y+=BLK;
	}
	fclose(fp);

	track_data=(unsigned char *)tmpCan->get_data();
	TBPP=tmpCan->get_bytes_per_pixel();

	image=CL_Surface::create(tmpCan);

	return 0;
}

CL_Surface *cktrack::get_surface(void)
{
	return image;
}

unsigned char *cktrack::get_data(void)
{
	return track_data;
}

char cktrack::get_track_symb(int x, int y)
{
	return track_grid[x][y];
}

void cktrack::locate_next_checkpoint(int *x, int *y, char cursymbol)
{
	int i;
	int j;
	int first=0;

	for(j=0; j<TRACK_SIZE_Y; j++)
	{
		for(i=0; i<TRACK_SIZE_X; i++)
		{
			if(track_grid[i][j]==cursymbol)
			{
				if(!first) first++;
				else
				{
					*x=i*64+ADJ;
					*y=j*64+ADJ;
					return;
				}
			}
		}
	}
}

void cktrack::set_laps(int val)
{
	laps=val;
}

int cktrack::get_laps(void)
{
	return laps;
}

int cktrack::get_next_position(void)
{
	return next_position;
}

void cktrack::set_next_position(int val)
{
	next_position=val;
}

int cktrack::get_sposx(int val)
{
	return BLK*spos[val].x+ADJ;
}

int cktrack::get_sposy(int val)
{
	return BLK*spos[val].y+ADJ;
}

int cktrack::get_sposang(int val)
{
	return spos[val].ang;
}
