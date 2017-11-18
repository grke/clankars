#include <ClanLib/display.h>
//#include <ClanLib/png.h>

#include "cktrack.h"
#include "ckmenu.h"

#define ADJ	40
#define CAR_W	80
#define CAR_L	80
#define BLK	64

enum coll_area {NONE=0, CENTRE, NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH,
                SOUTH_WEST, WEST, NORTH_WEST};

class ckobj
{
	protected:
		CL_Surface *image;
		float x;
		float y;
		float vel;
		int max_vel;
		double ang;
		int bridge;
		int guide;
		char find_guide(cktrack *track);
		void autoguide(cktrack *track, double ang_inc);
	public:
		ckobj(void);
		~ckobj(void);
		int getx(void);
		int gety(void);
		void addx(float val);
		void addy(float val);
		void setx(float val);
		void sety(float val);
		CL_Surface *get_image(void);
		void load_image(const char *path);
		enum coll_area detect_walls(float x, float y, CL_Surface *image,
				unsigned char *track_data, int b);
		void display_on_screen(ckobj *p1, int swidth, int sheight);
		void display_on_screenxy(CL_Surface *im, int px, int py, int swidth, int sheight);
		void display_on_screen1(ckobj *p1, int swidth, int sheight);
		void display_on_screen1xy(CL_Surface *im, int px, int py, int swidth, int sheight);
		void display_on_screen2(ckobj *p1, int swidth, int sheight);
		void display_on_screen2xy(CL_Surface *im, int px, int py, int swidth, int sheight);
		int get_bridge(void);
		int get_guide(void);
		double get_guide_ang(char c);
};

extern void calc_vel(double ang, float vel, float *hor_vel, float *ver_vel);
