#include "ckmisc.h"

struct pos
{
	int n;
	long val;
};

void swap_pos(pos *p1, pos *p2)
{
	pos tmp;
	tmp.n=p2->n;
	tmp.val=p2->val;

	p2->n=p1->n;
	p2->val=p1->val;

	p1->n=tmp.n;
	p1->val=tmp.val;
}

void adjust_car(car *c, enum coll_area col)
{
	int a=1;
	if(col==NORTH_WEST)
	{
		c->addx(a);
		c->addy(a);
	}
	else if(col==NORTH_EAST)
	{
		c->addx(-a);
		c->addy(a);
	}
	else if(col==SOUTH_WEST)
	{
		c->addx(a);
		c->addy(-a);
	}
	else if(col==SOUTH_EAST)
	{
		c->addx(-a);
		c->addy(-a);
	}
}

int do_weapons(ckweapon *weap, cktrack *track, car *plyr[], int i, int players, int pno, CL_Surface *exp[], int b, int swidth, int sheight)
{
	int frameno;

	if(weap->get_bridge()!=b) return -1; // Not on/off bridge

	if(weap->get_active() && weap->get_frameno()==-1) weap->calc_vels(track, weap->get_type()==HOMER?plyr[i]->get_homer_image():NULL);

	if(weap->get_active() && weap->get_frameno()==-1)
	{
		if(players==2)
		{
			weap->display_on_screen1(plyr[0], swidth, sheight);
			weap->display_on_screen2(plyr[1], swidth, sheight);
		}
		else weap->display_on_screen(plyr[0], swidth, sheight);

		for(int k=0; k<pno; k++)
		{
			if(k!=i && plyr[k]->get_bridge()==weap->get_bridge() && !plyr[k]->isdead() && !plyr[k]->get_finish())
			{
				enum coll_area col1, col2;
				if(surface_collision(weap->get_image(), weap->getx(), weap->gety(),
					plyr[k]->get_image(), plyr[k]->getx(),
					plyr[k]->gety(), &col1, &col2))
				{
					plyr[k]->setdead(1);
					weap->set_frameno(0);
				}
			}
		}
	}

	frameno=weap->get_frameno();

	if(frameno<16 && frameno!=-1)
	{
		if(players==2)
		{
			weap->display_on_screen2xy(exp[frameno], plyr[1]->getx(), plyr[1]->gety(), swidth, sheight);
			weap->display_on_screen1xy(exp[frameno], plyr[0]->getx(), plyr[0]->gety(), swidth, sheight);
		}
		else weap->display_on_screenxy(exp[frameno], plyr[0]->getx(), plyr[0]->gety(), swidth, sheight);
		weap->set_frameno(frameno+1);
	}
	else if(frameno==16)
	{
		weap->set_active(0);
		weap->set_frameno(-1);
	}

	return 0;
}

int do_weapons_stuff(car *plyr[], cktrack *track, int players, int pno, CL_Surface *exp[], int b, int swidth, int sheight)
{
	for(int i=0; i<pno; i++)
	{
		for(int j=0; j<3; j++)
		{
			do_weapons(plyr[i]->get_fckweapon(j), track, plyr, i, players, pno, exp, b, swidth, sheight);
			do_weapons(plyr[i]->get_rckweapon(j), track, plyr, i, players, pno, exp, b, swidth, sheight);
		}
	}
	return 0;
}

int detect_proximity(car *plr1, car *plr2)
{
	int det=0;
	enum coll_area col1=NONE;
	enum coll_area col2=NONE;

	while(surface_collision(plr1->get_image(), plr1->getx(), plr1->gety(),
		plr2->get_image(), plr2->getx(), plr2->gety(),
		&col1, &col2))
	{
		det++;
		adjust_car(plr1, col1);
		adjust_car(plr2, col2);
	}

	if(det)
	{
		// v1=(u1+u2-e*(u1-u2))/2;
		// v2=(u1+u2+e*(u1-u2))/2;
		float uh1=plr1->gethorvel();
		float uv1=plr1->getvervel();
		float uh2=plr2->gethorvel();
		float uv2=plr2->getvervel();

		plr1->setchv((uh1+uh2-0.7*(uh1-uh2))/2);
		plr1->setcvv((uv1+uv2-0.7*(uv1-uv2))/2);
		plr2->setchv((uh1+uh2+0.7*(uh1-uh2))/2);
		plr2->setcvv((uv1+uv2+0.7*(uv1-uv2))/2);

		plr1->mult_vel(-0.7);
		plr2->mult_vel(-0.7);

		plr1->del_damage(0.2);
		plr2->del_damage(0.2);

		return 1;
	}
	return 0;
}

int check_road_ahead(car *plr1, car *plr2)
{
	enum coll_area col1=NONE;
	enum coll_area col2=NONE;
	float xa;
	float ya;
	int px=plr1->getx();
	int py=plr1->gety();
	char c=plr1->get_guide();

	if(plr1->get_vel()<=plr2->get_vel()) { plr1->set_avoid(0); return 0; }

	if(c=='q') { xa=px-(CAR_L); ya=py-(CAR_L); }
	else if(c=='w') { xa=px; ya=py-(CAR_L); }
	else if(c=='e') { xa=px+(CAR_L); ya=py-(CAR_L); }
	else if(c=='a') { xa=px-(CAR_L); ya=py; }
	else if(c=='d') { xa=px+(CAR_L); ya=py; }
	else if(c=='z') { xa=px-(CAR_L); ya=py+(CAR_L); }
	else if(c=='x') { xa=px; ya=py+(CAR_L); }
	else if(c=='c') { xa=px+(CAR_L); ya=py+(CAR_L); }
	else return -1;

	if(surface_collision(plr1->get_image(), (int)xa, (int)ya,
		plr2->get_image(), plr2->getx(), plr2->gety(),
		&col1, &col2))
			plr1->start_avoid(col1);
	else plr1->set_avoid(0);

	return 0;
}

int surface_collision(CL_Surface *p1image, int p1x, int p1y,
                        CL_Surface *p2image, int p2x, int p2y,
                        enum coll_area *col1, enum coll_area *col2)
{
	int left1, left2, over_left;
	int right1, right2, over_right;
	int top1, top2, over_top;
	int bottom1, bottom2, over_bottom;
	int over_width, over_height;
	int i=0, j=0;
	unsigned char *pixel1, *pixel2;
	CL_Canvas *tmpCan1=NULL;
	CL_Canvas *tmpCan2=NULL;
	unsigned char *data1, *data2;
	int bpp1, bpp2;

	left1=p1x;
	left2=p2x;
	right1=p1x+CAR_L;
	right2=p2x+CAR_L;
	top1=p1y;
	top2=p2y;
	bottom1=p1y+CAR_L;
	bottom2=p2y+CAR_L;

	if(top1>bottom2) return 0;
	if(top2>bottom1) return 0;
	if(right1<left2) return 0;
	if(right2<left1) return 0;

	tmpCan1=new CL_Canvas(p1image->get_width(),
	p1image->get_height());
	tmpCan2=new CL_Canvas(p2image->get_width(),
	p2image->get_height());
	p1image->put_target(0, 0, 0, tmpCan1);
	p2image->put_target(0, 0, 0, tmpCan2);
	bpp1=tmpCan1->get_bytes_per_pixel();
	bpp2=tmpCan2->get_bytes_per_pixel();
	data1=(unsigned char *)tmpCan1->get_data();
	data2=(unsigned char *)tmpCan2->get_data();
	if(bottom1>bottom2)
	{
		*col1=NORTH;
		*col2=SOUTH;
		over_bottom=bottom2;
	}
	else
	{
		*col1=SOUTH;
		*col2=NORTH;
		over_bottom=bottom1;
	}

	if(top1>top2) over_top=top1;
	else over_top=top2;

	if(right1>right2)
	{
		if(*col1==NORTH) *col1=NORTH_WEST;
		else *col1=SOUTH_WEST;
		if(*col2==NORTH) *col2=NORTH_EAST;
		else *col2=SOUTH_EAST;
		over_right=right2;
	}
	else
	{
		if(*col1==NORTH) *col1=NORTH_EAST;
		else *col1=SOUTH_EAST;
		if(*col2==NORTH) *col2=NORTH_WEST;
		else *col2=SOUTH_WEST;
		over_right=right1;
	}

	if(left1>left2) over_left=left1;
	else over_left=left2;

	over_width=over_right-over_left;
	over_height=over_bottom-over_top;

	i=bpp1*(((over_top-top1)*CAR_L)+over_left-left1);
	pixel1=data1+i;

	j=bpp2*(((over_top-top2)*CAR_L)+over_left-left2);
	pixel2=data2+j;

	for(i=0; i<over_height; i++)
	{
		for(j=0; j<over_width; j++)
		{
			if((*pixel1>0) && (*pixel2>0))
			{
				delete tmpCan1;
				delete tmpCan2;
				return 1;
			}
			pixel1+=bpp1;
			pixel2+=bpp2;
		}
		pixel1+=bpp1*(CAR_L-over_width);
		pixel2+=bpp2*(CAR_L-over_width);
	}

	delete tmpCan1;
	delete tmpCan2;
	return 0;
}

int update_positions(car *p[], cktrack *track, int players, int pno)
{
	int i;
	int j;
	pos arr[pno];


	for(i=0; i<pno; i++)
	{
		arr[i].n=i;
		arr[i].val=1000*p[i]->getlap()+p[i]->getlapsymb();
	}

	for(i=0; i<pno; i++)
	{
		for(j=0; j<pno; j++)
		{
			if(i==j) continue;
			if(arr[i].val<arr[j].val) swap_pos(&arr[i], &arr[j]);
		}
	}

	for(i=0, j=pno-1; i<pno; i++, j--)
	{
		for(int k=0; k<pno; k++)
		{
			if(!p[k]->get_finish())
			{
				if(arr[i].n==k) p[k]->set_position(j);
				if(p[k]->getlap()==track->get_laps())
				{
					int t=track->get_next_position();
					p[k]->set_finish(t);
					track->set_next_position(t+1);
				}
			}
		}
	}

	if((p[0]->get_finish() || p[0]->get_killed())
		&& (players==1 || (p[1]->get_finish() || p[1]->get_killed())))
	{
		printf("FINAL TIMES:\n");
		for(i=0; i<pno; i++)
		{
			if(!p[i]->get_finish())
			{
				int t=track->get_next_position();
				p[i]->set_finish(t);
				track->set_next_position(t+1);
			}
			printf("p[%d]: %d\n", i, p[i]->get_finish());
		}
		return 1; // end of race
	}

	return 0;
}
