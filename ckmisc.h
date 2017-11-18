#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
//#include <ClanLib/png.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "caimagemanipulation.h"
#include "ckcar.h"

extern int detect_proximity(car *plr1, car *plr2);
extern int check_road_ahead(car *plr1, car *plr2);
extern int surface_collision(CL_Surface *p1image, int p1x, int p1y,
				CL_Surface *p2image, int p2x, int p2y,
				enum coll_area *col1, enum coll_area *col2);
extern int update_positions(car *p[], cktrack *track, int players, int pno);
extern int do_weapons(ckweapon *weap, cktrack *track, car *plyr[], int i, int players, int pno, CL_Surface *exp[], int b,
							int swidth, int sheight);
extern int do_weapons_stuff(car *plyr[], cktrack *track, int players, int pno, CL_Surface *exp[], int b,
							int swidth, int sheight);
