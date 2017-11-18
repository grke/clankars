#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include "ckobj.h"

enum weapon {FRONTM=0, REARM, MINE, HOMER};

class ckweapon : public ckobj
{
	private:
		enum weapon type;
		int active;
		int mine_detonation_time;
		int exploding;
		int frameno;
	public:
		ckweapon(enum weapon efw);
		~ckweapon(void);
		int get_active(void);
		enum weapon get_type(void);
		void activate(CL_Surface *im, float velinit, double anginit, int xinit, int yinit, int b);
		void calc_vels(cktrack *track, CL_Surface *im);
		void set_active(int val);
		int get_frameno(void);
		void set_frameno(int val);
};
