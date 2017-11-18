#include "ckobj.h"

ckobj::ckobj(void)
{
	image=NULL;
}

ckobj::~ckobj(void)
{
//	printf("CKOBJ\n");
	if(image) { delete image; image=NULL; }
}

void ckobj::load_image(const char *path)
{
	image=CL_PNGProvider::create(path, NULL);
}

int ckobj::getx(void)
{
	return (int)x;
}

int ckobj::gety(void)
{
	return (int)y;
}

void ckobj::addx(float val)
{
	x+=val;
}

void ckobj::addy(float val)
{
	y+=val;
}

void ckobj::setx(float val)
{
	x=val;
}

void ckobj::sety(float val)
{
	y=val;
}

CL_Surface *ckobj::get_image(void)
{
	return image;
}

enum coll_area ckobj::detect_walls(float x, float y, CL_Surface *image,
						unsigned char *track_data, int b)
{
	int i, j;
	int bpp;
	int TBPP=2;
	unsigned char *data1=NULL;
	CL_Canvas *tmpCan=new CL_Canvas(image->get_width(), image->get_height());
	image->put_target(0, 0, 0, tmpCan);
	bpp=tmpCan->get_bytes_per_pixel();
	data1=(unsigned char *)tmpCan->get_data();

	if(bridge==1)
	{
		int r=32*BLK-5;
		if((int)x-CAR_L/2<r)
		{
			data1+=bpp*((int)x-r+CAR_L/2);
			for(i=0; i<CAR_L; i++)
			{
				if(*data1>0)
				{
					delete tmpCan;
					return WEST;
				}
				data1+=bpp*CAR_L;
			}
		}
		r=34*BLK+5;
		if((int)x+CAR_L/2>r)
		{
			data1+=bpp*((int)x-r+CAR_L/2);
			for(i=0; i<CAR_L; i++)
			{
				if(*data1>0)
				{ 
					delete tmpCan;
					return EAST;
				}
				data1+=bpp*CAR_L;
			}
		}
	}
	else
	{
		unsigned char *data2=track_data+TBPP*((70*64*((int)y-ADJ))+(int)x-ADJ);
		for(i=0; i<CAR_L; i++)
		{
			for(j=0; j<CAR_W; j++)
			{
				if(*data1>0 && *data2==137)
				{
					if(27<=i && i<=54 && 27<=j && j<=54) continue;
					delete tmpCan;
					if(i<27)
					{
						if(j<27) return NORTH_WEST;
						else if(j<54) return NORTH;
						else return NORTH_EAST;
					}
					else if(i<54)
					{
						if(j<27) return WEST;
						else if(j<54) return CENTRE;
						else return EAST;
					}
					else
					{
						if(j<27) return SOUTH_WEST;
						else if(j<54) return SOUTH;
						else return SOUTH_EAST;
					}
				}
				data1+=bpp;
				data2+=TBPP;
			}
			data2+=TBPP*((70*64)-CAR_L);
		}
	}
	delete tmpCan;

	return NONE;
}

void ckobj::display_on_screen1(ckobj *p, int swidth, int sheight)
{
	int px=p->getx();
	int py=p->gety();
	int w=image->get_width();
	int h=image->get_height();

	if((x < px+swidth/4+ADJ+w)
	  && (x > px-swidth/4+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
	{
		CL_Display::push_clip_rect(CL_ClipRect(0, 0, swidth/2, sheight));
		image->put_screen((int)((swidth/4)-ADJ-(px-x)),
						(int)((sheight/2)-ADJ-(py-y)));
		CL_Display::pop_clip_rect();
	}
}

void ckobj::display_on_screen1xy(CL_Surface *im, int px, int py, int swidth, int sheight)
{
	int w=im->get_width();
	int h=im->get_height();

	if((x < px+swidth/4+ADJ+w)
	  && (x > px-swidth/4+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
	{
		CL_Display::push_clip_rect(CL_ClipRect(0, 0, swidth/2, sheight));
		im->put_screen((int)((swidth/4)-ADJ-(px-x)),
						(int)((sheight/2)-ADJ-(py-y)));
		CL_Display::pop_clip_rect();
	}
}

void ckobj::display_on_screen2(ckobj *p, int swidth, int sheight)
{
	int px=p->getx();
	int py=p->gety();
	int w=image->get_width();
	int h=image->get_height();

	if((x < px+swidth/4+ADJ+w)
	  && (x > px-swidth/4+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
	{
		CL_Display::push_clip_rect(CL_ClipRect(swidth/2, 0, swidth, sheight));
		image->put_screen((int)((3*swidth/4)-ADJ-(px-x)),
						(int)((sheight/2)-ADJ-(py-y)));
		CL_Display::pop_clip_rect();
	}
}

void ckobj::display_on_screen2xy(CL_Surface *im, int px, int py, int swidth, int sheight)
{
	int w=im->get_width();
	int h=im->get_height();

	if((x < px+swidth/4+ADJ+w)
	  && (x > px-swidth/4+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
	{
		CL_Display::push_clip_rect(CL_ClipRect(swidth/2, 0, swidth, sheight));
		im->put_screen((int)((3*swidth/4)-ADJ-(px-x)),
						(int)((sheight/2)-ADJ-(py-y)));
		CL_Display::pop_clip_rect();
	}
}

void ckobj::display_on_screen(ckobj *p, int swidth, int sheight)
{
	int px=p->getx();
	int py=p->gety();
	int w=image->get_width();
	int h=image->get_height();

	if((x < px+swidth/2+ADJ+w)
	  && (x > px-swidth/2+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
	image->put_screen((int)((swidth/2)-ADJ-(px-x)),
					(int)((sheight/2)-ADJ-(py-y)));
}

void ckobj::display_on_screenxy(CL_Surface *im, int px, int py, int swidth, int sheight)
{
	int w=im->get_width();
	int h=im->get_height();

	if((x < px+swidth/2+ADJ+w)
	  && (x > px-swidth/2+ADJ-w)
	  && (y < py+sheight/2+h)
	  && (y > py-sheight/2-h))
		im->put_screen((int)((swidth/2)-ADJ-(px-x)),
						(int)((sheight/2)-ADJ-(py-y)));
}

void calc_vel(double ang, float vel, float *hor_vel, float *ver_vel)
{
	if(ang>=0 && ang<90)
	{
		*hor_vel=vel*sin(ang*(PI/180));
		*ver_vel=vel*cos(ang*(PI/180));
	}
	else if(ang>=90 && ang<180)
	{
		*hor_vel=vel*cos((ang-90)*(PI/180));
		*ver_vel=-vel*sin((ang-90)*(PI/180));
	}
	else if(ang>=180 && ang<270)
	{
		*hor_vel=-vel*sin((ang-180)*(PI/180));
		*ver_vel=-vel*cos((ang-180)*(PI/180));
	}
	else if(ang>=270 && ang<=360)
	{
		*hor_vel=-vel*cos((ang-270)*(PI/180));
		*ver_vel=vel*sin((ang-270)*(PI/180));
	}
}

int ckobj::get_bridge(void)
{
	return bridge;
}

int isguide(char c)
{
	if(c=='q' || c=='w' || c=='e' ||
	   c=='a' ||           c=='d' ||
	   c=='z' || c=='x' || c=='c') return 1;
	return 0;
}

double ckobj::get_guide_ang(char c)
{
	if(c=='z') return 315;
	if(c=='a') return 270;
	if(c=='q') return 225;
	if(c=='w') return 180;
	if(c=='e') return 135;
	if(c=='d') return 90;
	if(c=='c') return 45;
	if(c=='x') return 0;

	return 0;
}

char ckobj::find_guide(cktrack *track)
{
	int x=(int)floor(double(getx()/BLK));
	int y=(int)floor(double(gety()/BLK));
	int i;
	int j;

	char c=track->get_track_symb(x, y); // check exact location first
	if(isguide(c)) return c;

	for(i=-1; i<2; i++)
	{
		for(j=-1; j<2; j++)
		{
			if(i || j) // don't repeat first check
			{
				c=track->get_track_symb(x+i, y+j);
				if(isguide(c)) return c;
			}
		}
	}
	return 0;
}

void ckobj::autoguide(cktrack *track, double ang_inc)
{
	int a;
	int b=(int)ang;
	char c=find_guide(track);
	if(c) guide=c;

	a=(int)get_guide_ang(guide);

	if(a==0 && b!=a && b<=180) ang-=ang_inc;
	else if(a==0 && b!=a && b>180) ang+=ang_inc;

	else if(a==45 && b<=225 && b>45) ang-=ang_inc;
	else if(a==45 && (b>225 || b<45)) ang+=ang_inc;

	else if(a==90 && b<=270 && b>90) ang-=ang_inc;
	else if(a==90 && (b>270 || b<90)) ang+=ang_inc;

	else if(a==135 && b<=315 && b>135) ang-=ang_inc;
	else if(a==135 && (b>315 || b<135)) ang+=ang_inc;

	else if(a==180 && b!=a && b>180) ang-=ang_inc;
	else if(a==180 && b!=a && b<180) ang+=ang_inc;

	else if(a==225 && b<225 && b>=45) ang+=ang_inc;
	else if(a==225 && (b>225 || b<45)) ang-=ang_inc;

	else if(a==270 && b<270 && b>=90) ang+=ang_inc;
	else if(a==270 && (b>270 || b<90)) ang-=ang_inc;

	else if(a==315 && b<315 && b>=135) ang+=ang_inc;
	else if(a==315 && (b>315 || b<135)) ang-=ang_inc;

	b=(int)ang;
	if(b>=350 && b<=10) ang=0;
	else if(b>=35 && b<=55) ang=45;
	else if(b>=80 && b<=100) ang=90;
	else if(b>=125 && b<=145) ang=135;
	else if(b>=170 && b<=190) ang=180;
	else if(b>=215 && b<=235) ang=225;
	else if(b>=260 && b<=280) ang=270;
	else if(b>=305 && b<=325) ang=315;
}

int ckobj::get_guide(void)
{
	return guide;
}
