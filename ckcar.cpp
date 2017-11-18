#include "ckcar.h"

#define ANG_INC	10
#define GRIP	4

static int places=0;

car::car(void)
{
	char path[32]="";
	char path2[32]="";

	snprintf(path, sizeof(path), "graphics/weapons/missile.png");
	missarr[0]=CL_PNGProvider::create(path, NULL);
	for(int i=1; i<10; i++) // Minimise rotations, as they take a long time
		missarr[i]=CAImageManipulation::rotate(missarr[0], -i*10, true, false);
	for(int i=1; i<10; i++)
		missarr[i+9]=CAImageManipulation::flip(missarr[9-i], false);
	for(int i=1; i<18; i++)
		missarr[i+18]=CAImageManipulation::flip(missarr[18-i], true);

	snprintf(path, sizeof(path), "graphics/weapons/homer.png");
	homearr[0]=CL_PNGProvider::create(path, NULL);
	for(int i=1; i<10; i++) // Minimise rotations, as they take a long time
		homearr[i]=CAImageManipulation::rotate(homearr[0], -i*10, true, false);
	for(int i=1; i<10; i++)
		homearr[i+9]=CAImageManipulation::flip(homearr[9-i], false);
	for(int i=1; i<18; i++)
		homearr[i+18]=CAImageManipulation::flip(homearr[18-i], true);

	snprintf(path, sizeof(path), "graphics/weapons/mine.png");
	mine=CL_PNGProvider::create(path, NULL);

	snprintf(path, sizeof(path), "graphics/cars/car00.png");
	cararr1[0]=CL_PNGProvider::create(path, NULL);
	for(int i=1; i<10; i++) // Minimise rotations
		cararr1[i]=CAImageManipulation::rotate(cararr1[0], -i*10, true, false);
	for(int i=1; i<10; i++)
		cararr1[i+9]=CAImageManipulation::flip(cararr1[9-i], false);
	for(int i=1; i<18; i++)
		cararr1[i+18]=CAImageManipulation::flip(cararr1[18-i], true);

	for(int i=0; i<19; i++)
	{
		snprintf(path2, sizeof(path2), "graphics/cars/car%d.png", i);
		cararr2[i]=CL_PNGProvider::create(path2, NULL);
	}
	for(int i=1; i<18; i++)
	{
		cararr2[i+18]=CAImageManipulation::flip(cararr2[18-i], true);
	}

	for(int i=0; i<19; i++)
		cararr3[i]=CAImageManipulation::flip(cararr2[18-i], false);
	for(int i=1; i<18; i++)
		cararr3[i+18]=CAImageManipulation::flip(cararr2[36-i], false);
}

void car::reset(int pos, int mv, enum weapon efw, enum weapon erw, cktrack *track,
						int left, int right, int acc, int dec, int fw, int rw, int finish)
{
	human=1;
	setlaa(0);
	vel=0;
	x=track->get_sposx(pos);
	y=track->get_sposy(pos);
	ang=track->get_sposang(pos);
	sethorvel(0);
	setvervel(0);
	setdead(0);
	setlap(0);
	setlapsymb('A');
	if(finish) set_finish(0);
	track->locate_next_checkpoint(&repox, &repoy, getlapsymb());
	guide='a';
	image=cararr1[(int)ang/10];
	damage=100;
	killed=0;
	fwep_released=1;
	rwep_released=1;
	repos=0;
	bridge=0;

	if(acc_button) { delete acc_button; acc_button=NULL; }
	if(dec_button) { delete dec_button; dec_button=NULL; }
	if(left_button) { delete left_button; left_button=NULL; }
	if(right_button) { delete right_button; right_button=NULL; }
	if(front_weapon_button) { delete front_weapon_button; front_weapon_button=NULL; }
	if(rear_weapon_button) { delete rear_weapon_button; rear_weapon_button=NULL; }

	acc_button=new CL_InputButton_Group;
	dec_button=new CL_InputButton_Group;
	left_button=new CL_InputButton_Group;
	right_button=new CL_InputButton_Group;
	front_weapon_button=new CL_InputButton_Group;
	rear_weapon_button=new CL_InputButton_Group;

	left_button->add(CL_Input::keyboards[0]->get_button(left));
	right_button->add(CL_Input::keyboards[0]->get_button(right));
	acc_button->add(CL_Input::keyboards[0]->get_button(acc));
	dec_button->add(CL_Input::keyboards[0]->get_button(dec));
	front_weapon_button->add(CL_Input::keyboards[0]->get_button(fw));
	rear_weapon_button->add(CL_Input::keyboards[0]->get_button(rw));
}

void car::reset(int pos, int mv, enum weapon efw, enum weapon erw, cktrack *track, int eag, int finish)
{
	human=0;
	vel=0;
	x=track->get_sposx(pos);
	y=track->get_sposy(pos);
	ang=track->get_sposang(pos);
	setlaa(0);
	sethorvel(0);
	setvervel(0);
	setdead(0);
	setlap(0);
	setlapsymb('A');
	if(finish) set_finish(0);
	track->locate_next_checkpoint(&repox, &repoy, getlapsymb());
	guide='a';
	image=cararr1[(int)ang/10];
	damage=100;
	killed=0;
	fwep_released=1;
	rwep_released=1;
	repos=0;
	bridge=0;
}

car::car(car *globcar, int hsv, float x, float y, double a, int mv, enum weapon efw, enum weapon erw, cktrack *track, int eag)
{
	mine=CAImageManipulation::changeHSV(globcar->mine, hsv, 0, 0);

	for(int i=0; i<36; i++)
	{
		missarr[i]=CAImageManipulation::changeHSV(globcar->missarr[i], hsv, 0, 0);
		homearr[i]=CAImageManipulation::changeHSV(globcar->homearr[i], hsv, 0, 0);
		cararr1[i]=CAImageManipulation::changeHSV(globcar->cararr1[i], hsv, 0, 0);
		cararr2[i]=CAImageManipulation::changeHSV(globcar->cararr2[i], hsv, 0, 0);
		cararr3[i]=CAImageManipulation::changeHSV(globcar->cararr3[i], hsv, 0, 0);
	}

	for(int i=0; i<3; i++)
	{
		fwep[i]=new ckweapon(efw);
		rwep[i]=new ckweapon(erw);
	}

	ang=a;
	setlaa(0);
	vel=0;
	max_vel=mv;
	setx(x);
	sety(y);
	sethorvel(0);
	setvervel(0);
	setdead(0);
	setlap(0);
	setlapsymb('A');
	set_finish(0);
	set_position(places);
	fweapon=efw;
	rweapon=erw;
	places++;
	track->locate_next_checkpoint(&repox, &repoy, getlapsymb());
	human=0;
	guide='a';
	image=cararr1[(int)ang/10];
	eager=eag;
	damage=100;
	killed=0;
	repos=0;
	bridge=0;

	acc_button=NULL;
	dec_button=NULL;
	left_button=NULL;
	right_button=NULL;
	front_weapon_button=NULL;
	rear_weapon_button=NULL;
}

car::~car(void)
{
	int i;
	printf("A\n");
	if(mine) { delete mine; mine=NULL; }
	printf("B\n");

	for(i=0; i<36; i++)
	{
		if(missarr[i]) { delete missarr[i]; missarr[i]=NULL; }
		if(homearr[i]) { delete homearr[i]; homearr[i]=NULL; }
		if(cararr1[i]) { delete cararr1[i]; cararr1[i]=NULL; }
		if(cararr2[i]) { delete cararr2[i]; cararr2[i]=NULL; }
		if(cararr3[i]) { delete cararr3[i]; cararr3[i]=NULL; }
	}

	printf("C\n");
	for(i=0; i<3; i++)
	{
		if(fwep[i]) { delete fwep[i]; fwep[i]=NULL; }
		if(rwep[i]) { delete rwep[i]; rwep[i]=NULL; }
	}
	printf("D\n");
	if(acc_button) { delete acc_button; acc_button=NULL; }
	if(dec_button) { delete dec_button; dec_button=NULL; }
	if(left_button) { delete left_button; left_button=NULL; }
	if(right_button) { delete right_button; right_button=NULL; }
	if(front_weapon_button) { delete front_weapon_button; front_weapon_button=NULL; }
	if(rear_weapon_button) { delete rear_weapon_button; rear_weapon_button=NULL; }

//	if(image) { delete image; image=NULL; }
	image=NULL;
	printf("E\n");
}

int get_randno(int eager)
{
	if(random()%1000<eager)
		return 1;
	else
		return 0;
}

double car::getlaa(void)
{
	return laa;
}

void car::addlaa(double val)
{
	laa+=val;
}

void car::setlaa(double val)
{
	laa=val;
}

int car::isdead(void)
{
	return dead+killed;
}

void car::setdead(int val)
{
	vel=0;
	sethorvel(0);
	setvervel(0);
	setchv(0);
	setcvv(0);
	frameno=0;
	dead=val;
	if(dead) damage-=2;
}

int car::get_killed(void)
{
	return killed;
}

void car::set_avoid(int val)
{
	avoid=val;
}

float car::gethorvel(void)
{
	return hor_vel;
}

void car::sethorvel(float val)
{
	hor_vel=val;
}

float car::getvervel(void)
{
	return ver_vel;
}

void car::setvervel(float val)
{
	ver_vel=val;
}

float car::getchv(void)
{
	return chv;
}

void car::addchv(float val)
{
	chv+=val;
}

void car::setchv(float val)
{
	chv=val;
}

float car::getcvv(void)
{
	return cvv;
}

void car::addcvv(float val)
{
	cvv+=val;
}

void car::setcvv(float val)
{
	cvv=val;
}

int car::getlap(void)
{
	return lap;
}

int car::addlap(int val, int lap_target)
{
	lap+=val;
	if(lap==lap_target) return 1;
	return 0;
}

void car::dellap(int val)
{
	lap-=val;
}

void car::setlap(int val)
{
	lap=val;
}

char car::getlapsymb(void)
{
	return lapsymb;
}

void car::setlapsymb(char val)
{
	lapsymb=val;
}

void car::set_position(int val)
{
	position=val;
}

int car::get_position(void)
{
	return position;
}

ckweapon *car::get_fckweapon(int i)
{
	return fwep[i];
}

ckweapon *car::get_rckweapon(int i)
{
	return rwep[i];
}

void car::del_damage(float val)
{
	damage-=val;
}

void car::set_damage(float val)
{
	damage=val;
}

int car::get_damage(void)
{
	return (int)damage;
}

void car::start_avoid(enum coll_area col)
{
	int ai=30;

	avoid++;
	if(guide=='q')
	{
		if(col==NORTH_EAST) ang+=ai;
		else if(col==SOUTH_WEST) ang-=ai;
		else ang+=ai;
	}
	else if(guide=='w')
	{
		if(col==NORTH_WEST || col==SOUTH_WEST) ang-=ai;
		else if(col==NORTH_EAST || col==SOUTH_EAST) ang+=ai;
		else ang+=ai;
	}
	else if(guide=='e')
	{
		if(col==NORTH_WEST) ang-=ai;
		else if(col==SOUTH_EAST) ang+=ai;
		else ang+=ai;
	}
	else if(guide=='a')
	{
		if(col==NORTH_WEST || col==NORTH_EAST) ang+=ai;
		else if(col==SOUTH_WEST || col==SOUTH_EAST) ang-=ai;
		else ang+=ai;
	}
	else if(guide=='d')
	{
		if(col==NORTH_WEST || col==NORTH_EAST) ang-=ai;
		else if(col==SOUTH_WEST || col==SOUTH_EAST) ang+=ai;
		else ang+=ai;
	}
	else if(guide=='z')
	{
		if(col==NORTH_WEST) ang+=ai;
		else if(col==SOUTH_EAST) ang-=ai;
		else ang+=ai;
	}
	else if(guide=='w')
	{
		if(col==NORTH_WEST || col==SOUTH_WEST) ang+=ai;
		else if(col==NORTH_EAST || col==SOUTH_EAST) ang-=ai;
		else ang+=ai;
	}
	else if(guide=='q')
	{
		if(col==NORTH_EAST) ang-=ai;
		else if(col==SOUTH_WEST) ang+=ai;
		else ang+=ai;
	}
}

int car::update_checkpoint(cktrack *track)
{
	char t=getlapsymb();
	char s=track->get_track_symb((int)floor(double(getx()/BLK)),
						(int)floor(double(gety()/BLK)));

	if(s==t+1 && t<'I')
	{
		track->locate_next_checkpoint(&repox, &repoy, s);
		setlapsymb(s);
		return 1;
	}
	if(s=='A' && t=='I')
	{
		track->locate_next_checkpoint(&repox, &repoy, s);
		setlapsymb(s);
		addlap(1, track->get_laps());
		return 1;
	}
	if(t>'A' && s==t-1)
	{
		setlapsymb(s);
	}
	if(s=='I' && t=='A')
	{
		setlapsymb(s);
		dellap(1);
	}
//	printf("LAP: %d, SECTION: %c\n", getlap(), getlapsymb());
	return 0;
}

void car::check_weapons(ckweapon *wep[], enum weapon type)
{
	int i=0;
	while(i<3)
	{
		if(!wep[i]->get_active())
		{
			int a=(int)ang/10;
			if(type==FRONTM)
				wep[i]->activate(missarr[a], vel,
						ang, getx(), gety(), bridge);
			else if(type==REARM)
			{
				if(a<=18) a+=18;
				else if(a>18) a-=18;
				if(a==36) a=0;
				wep[i]->activate(missarr[a], vel,
						ang, getx(), gety(), bridge);
			}
			else if(type==MINE)
				wep[i]->activate(mine, 0,
						0, getx(), gety(), bridge);
			else if(type==HOMER)
			{
				wep[i]->activate(homearr[a], vel,
						ang, getx(), gety(), bridge);
			}
			break;
		}
		i++;
	}
}

void car::calc_vels(cktrack *track)
{
	int origx=getx();
	int origy=gety();
	float ahv=0;
	float avv=0;
	float hhv=0;
	float hvv=0;
	int c=0;
	double ang_change=0;
	double atu=ang;
	enum coll_area ca=NONE;
	char t=track->get_track_symb((int)floor(double(getx()/BLK)), (int)floor(double(gety()/BLK)));

	if(t=='-') bridge=0; // bridge stuff
	else if(t=='+') bridge=1;

	if(image) ca=detect_walls(getx(), gety(), image, track->get_data(), bridge);
	if(ca)
	{
		vel*=-0.7;
		damage-=0.2;
	}

	if(human && !dead && !finish)
	{
		if(left_button->is_pressed()==true && !ca)
		{
			if(acc_button->is_pressed()==true)
				ang+=(ANG_INC-GRIP);
			else
				ang+=ANG_INC;
		}
		else if(right_button->is_pressed()==true && !ca)
		{
			if(acc_button->is_pressed()==true)
				ang-=(ANG_INC-GRIP);
			else
				ang-=ANG_INC;
		}

		// This needs to be done before firing weapons, or segfault
		// can happen.
		if(ang>360) ang=ANG_INC;
		else if(ang<0) ang=360-ANG_INC;
		else if(ang==360) ang=0;

		if(front_weapon_button->is_pressed()==true && fwep_released)
		{
			fwep_released=0;
			check_weapons(fwep, fweapon);
		}
		else if(front_weapon_button->is_pressed()) fwep_released++;
		if(rear_weapon_button->is_pressed()==true && rwep_released)
		{
			rwep_released=0;
			check_weapons(rwep, rweapon);
		}
		else if(rear_weapon_button->is_pressed()) rwep_released++;
	}
	else if(!dead && !finish)
	{
		// This needs to be done before firing weapons, or segfault
		// can happen.
		if(ang>360) ang=ANG_INC;
		else if(ang<0) ang=360-ANG_INC;
		else if(ang==360) ang=0;

		if(get_randno(eager))
			check_weapons(fwep, fweapon); // Randomly fire front...
		if(get_randno(eager))
			check_weapons(rwep, rweapon); // ..and rear
	}

	if(!human && !avoid) autoguide(track, 15);

	// This needs to be done after autoguide, or segfault
	// can happen.
	if(ang>360) ang=ANG_INC;
	else if(ang<0) ang=360-ANG_INC;
	else if(ang==360) ang=0;

//	printf("ANG:%d\n", (int)ang);
//	printf("VEL:%d\n", (int)vel);
//	printf("GUIDE:%c\n", guide);

	/*
	if(gety()>(0) && gety()<(0))
	{
		ver_slo_vel++;
		image=cararr2[(int)ang/10];
	}
	else if(gety()>(0) && gety()<(0))
	{
		ver_slo_vel--;
		image=cararr3[(int)ang/10];
	}
	else
	*/
	//      {
	//              if(ver_slo_vel<0) ver_slo_vel++;
	//              else if(ver_slo_vel>0) ver_slo_vel--;

	if(t=='Z') image=cararr2[(int)ang/10];
	else if(t=='X') image=cararr3[(int)ang/10];
	else image=cararr1[(int)ang/10];

	//      }

	if(human)
	{
		if(acc_button->is_pressed()==true && vel<max_vel && !ca)
		{
			setlaa(ang);
			vel+=1;
		}
		else if(dec_button->is_pressed()==true && vel>-10 && !ca)
		{
			setlaa(ang);
			vel-=1;
		}
		else if(vel>0 && !ca)
		{
			if(vel<1) vel=0;
			else vel-=1;
		}
		else if(vel<0 && !ca)
		{
			if(vel>-1) vel=0;
			else vel+=1;
		}

		if(acc_button->is_pressed()==false
		  && dec_button->is_pressed()==false)
		{
			atu=getlaa();
		}
		else
		{
			atu=ang;
		}
	}
	else
	{
		if(vel<max_vel) vel+=1;
	}

	calc_vel(atu, vel, &hhv, &hvv);
	sethorvel(hhv);
	setvervel(hvv);
	calc_vel(ang, vel, &ahv, &avv);

	ahv=gethorvel()+ahv+getchv();
	avv=getvervel()+avv+getcvv();

	if(getchv()>0) addchv(-1);
	else if(getchv()<0) addchv(1);
	if(getcvv()>0) addcvv(-1);
	else if(getcvv()<0) addcvv(1);
	if(getchv()>0 && getchv()<1) setchv(0);
	else if(getchv()<0 && getchv()>-1) setchv(0);
	if(getcvv()>0 && getcvv()<1) setcvv(0);
	else if(getcvv()<0 && getcvv()>-1) setcvv(0);

	addx(ahv);
	addy(avv);

	while((ca=detect_walls(getx(), gety(), image, track->get_data(), bridge))!=NONE)
	{
		damage-=0.2;
		if(c>5)
		{
			setdead(1);
			setx(origx);
			sety(origy);
			break;
		}
		vel*=-0.7;
		if(ca==NORTH_WEST || ca==NORTH || ca==NORTH_EAST)
		{
			addy(5);
		}
		if(ca==SOUTH_WEST || ca==SOUTH || ca==SOUTH_EAST)
		{
			addy(-5);
		}
		if(ca==NORTH_WEST || ca==WEST || ca==SOUTH_WEST)
		{
			addx(5);
		}
		if(ca==NORTH_EAST || ca==EAST || ca==SOUTH_EAST)
		{
			addx(-5);
		}
//              if(ca==CENTRE) { setdead(1); break; }
		if(ang>360) ang=15;
		else if(ang<0) ang=360-15;
		else if(ang==360) ang=0;
		c++;
	}
	if(damage<=0) setdead(1); // Set killed after the explosion routine is done.
}

int car::display(car *p1, car *p2, CL_Surface *exp[], int swidth, int sheight)
{
	if(!dead && !finish)
	{
		display_on_screen2(p2, swidth, sheight);
		display_on_screen1(p1, swidth, sheight);
	}
	else if(frameno<16 && frameno!=-1)
	{
		if(frameno<8)
		{
			display_on_screen2(p2, swidth, sheight);
			display_on_screen1(p1, swidth, sheight);
		}
		display_on_screen2xy(exp[frameno], p2->getx(), p2->gety(), swidth, sheight);
		display_on_screen1xy(exp[frameno], p1->getx(), p1->gety(), swidth, sheight);
		frameno++;
	}
	else if(frameno==16)
	{
		frameno=-1;
		bridge=0;
		if(damage<=0) killed++;
	}
	else return 1;

	return 0;
}

int car::display(car *p1, CL_Surface *exp[], int swidth, int sheight)
{
	if(!dead)
	{
		display_on_screen(p1, swidth, sheight);
	}
	else if(frameno<16 && frameno!=-1)
	{
		if(frameno<8) display_on_screen(p1, swidth, sheight);
		display_on_screenxy(exp[frameno], p1->getx(), p1->gety(), swidth, sheight);
		frameno++;
	}
	else if(frameno==16)
	{
		frameno=-1;
		bridge=0;
		if(damage<=0) killed++;
	}
	else return 1;

	return 0;
}

void car::reposition(int pd, cktrack *track)
{
	int x=getx();
	int y=gety();
	int xdiff=(repox-x)/16;
	int ydiff=(repoy-y)/16;

	if(xdiff==0 && repox-x<0) xdiff=-1;
	else if(xdiff==0 && repox-x>0) xdiff=1;
	if(ydiff==0 && repoy-y<0) ydiff=-1;
	else if(ydiff==0 && repoy-y>0) ydiff=1;
	
	if(x!=repox) addx(xdiff);
	if(y!=repoy) addy(ydiff);

	if(getx()==repox && gety()==repoy && !killed && !pd)
	{
		laa=ang=get_guide_ang(find_guide(track));
		repos=0;
		setdead(0);
	}
}

int car::updatecheckpoint_and_calcvels(cktrack *track)
{
	if(!isdead() && !finish)
	{
		if(update_checkpoint(track))
		{
			calc_vels(track);
			return 1;
		}
		else calc_vels(track);
	}
	return 0;
}

void car::set_finish(int val)
{
	finish=val;
}

int car::get_finish(void)
{
	return finish;
}

int car::get_repos(void)
{
	return repos;
}

void car::set_repos(int val)
{
	repos=val;
}

int car::get_human(void)
{
	return human;
}

void car::mult_vel(double val)
{
	vel*=val;
}

float car::get_vel(void)
{
	return vel;
}

CL_Surface *car::get_homer_image(void)
{
	return homearr[(int)ang/10];
}
