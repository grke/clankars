#include "ckweapon.h"

ckweapon::ckweapon(enum weapon ewf)
{
	max_vel=0;
	active=0;
	frameno=-1;
	type=ewf;
}

ckweapon::~ckweapon(void)
{
	image=NULL;
} // don't free image here.

int ckweapon::get_active(void)
{
	return active;
}

void ckweapon::activate(CL_Surface *im, float velinit, double anginit, int xinit, int yinit, int b)
{
	active++;
	x=xinit;
	y=yinit;
	ang=anginit;
	image=im;
	bridge=b;
	frameno=-1;
	if(type==FRONTM) vel=velinit-2;
	else if(type==REARM) vel=velinit+2;
	else if(type==MINE)
	{
		mine_detonation_time=250;
		vel=0;
	}
	else if(type==HOMER)
	{
		max_vel=60;
		vel=velinit+2;
	}
	else vel=0;
}

void ckweapon::calc_vels(cktrack *track, CL_Surface *im)
{
	float hv=0;
	float vv=0;
	char t=track->get_track_symb((int)floor(double(getx()/BLK)), (int)floor(double(gety()/BLK)));

	if(t=='-') bridge=0;
	else if(t=='+') bridge=1;

	if(image==NULL)
	{
		printf("type: %d\n", type);
		printf("ang: %d\n", (int)ang);
		printf("vel: %d\n", (int)vel);
		printf("image was NULL in ckweapon::calc_vels!\n");
	}
	else if(detect_walls(getx(), gety(), image, track->get_data(), bridge))
	{
		frameno=0;
	}
	else
	{
		if(type==FRONTM) vel+=2;
		else if(type==REARM) vel-=2;
		else if(type==MINE)
		{
			mine_detonation_time--;
			vel=0;
			if(!mine_detonation_time) { frameno=0; }
		}
		else if(type==HOMER)
		{
			if(vel<max_vel) vel++;
			autoguide(track, 45);
			if(ang>360) ang=45;
			else if(ang<0) ang=360-45;
			else if(ang==360) ang=0;
			image=im;
		}
		calc_vel(ang, vel, &hv, &vv);
		addx(hv);
		addy(vv);
	}

}

void ckweapon::set_active(int val)
{
	active=val;
}

void ckweapon::set_frameno(int val)
{
	frameno=val;
}

int ckweapon::get_frameno(void)
{
	return frameno;
}

enum weapon ckweapon::get_type(void)
{
	return type;
}
