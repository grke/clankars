#include <ClanLib/core.h>
#include <ClanLib/application.h>
//#include <ClanLib/png.h>

#include "caimagemanipulation.h"
#include "ckweapon.h"

class car : public ckobj
{
	private:
		CL_Surface *cararr1[36];
		CL_Surface *cararr2[36];
		CL_Surface *cararr3[36];
		CL_Surface *missarr[36];
		CL_Surface *homearr[36];
		CL_Surface *mine;
		int frameno;
		CL_InputButton_Group *acc_button;
		CL_InputButton_Group *dec_button;
		CL_InputButton_Group *left_button;
		CL_InputButton_Group *right_button;
		CL_InputButton_Group *front_weapon_button;
		CL_InputButton_Group *rear_weapon_button;
		float hor_vel;
		float ver_vel;
		float chv;
		float cvv;
		double laa;
		int dead;
		int lap;
		char lapsymb;
		int human;
		int repox;
		int repoy;
		int position;
		int avoid;
		int eager;
		float damage;
		int killed;
		int finish;
		enum weapon fweapon;
		enum weapon rweapon;
		int fwep_released;
		int rwep_released;
		int repos;
		ckweapon *fwep[3];
		ckweapon *rwep[3];
		double getlaa(void);
		void addlaa(double val);
		void setlaa(double val);
		void sethorvel(float val);
		void setvervel(float val);
		float getchv(void);
		void addchv(float val);
		int addlap(int val, int lap_target);
		void dellap(int val);
		void setlapsymb(char val);
		void calc_vels(cktrack *track);
		int update_checkpoint(cktrack *track);
	public:
		car(void);
		car(car *globcar, int hsv, float x, float y, double a, int mv,
			enum weapon ewf, enum weapon erw,
			cktrack *track, int eag);
		~car(void);
		void reset(int pos, int mv, enum weapon efw, enum weapon erw, cktrack *track,
				int left, int right, int acc, int dec, int fw, int rw, int finish);
		void reset(int pos, int mv, enum weapon efw, enum weapon erw, cktrack *track, int eag, int finish);
		int get_killed(void);
		void reposition(int pd, cktrack *track);
		void setdead(int val);
		void set_avoid(int val);
		int getlap(void);
		char getlapsymb(void);
		int isdead(void);
		float getvervel(void);
		float gethorvel(void);
		void setchv(float val);
		float getcvv(void);
		void addcvv(float val);
		void setcvv(float val);
		void set_position(int val);
		int get_position(void);
		int display(car *p1, CL_Surface *exp[], int swidth, int sheight);
		int display(car *p1, car *p2, CL_Surface *exp[], int swidth, int sheight);
		int updatecheckpoint_and_calcvels(cktrack *track);
		void start_avoid(enum coll_area col);
		ckweapon *get_fckweapon(int i);
		ckweapon *get_rckweapon(int i);
		void check_weapons(ckweapon *wep[], enum weapon type);
		int get_damage(void);
		void del_damage(float val);
		void set_damage(float val);
		void set_finish(int val);
		int get_finish(void);
		void setlap(int val);
		void set_repos(int val);
		int get_repos(void);
		int get_human(void);
		void mult_vel(double val);
		float get_vel(void);
		CL_Surface *get_homer_image(void);
};
