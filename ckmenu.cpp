#include "ckmenu.h"
#include <ClanLib/Core/Resources/resource_manager.h>

ckmenu::ckmenu(char *f, char *f_i, char *f_w)
{
	resources=new CL_ResourceManager("font.scr", 0, false);
	font=CL_Font::load(f, resources);
	font_invert=CL_Font::load(f_i, resources);
	font_white=CL_Font::load(f_w, resources);
	menu_up=new CL_InputButton;
	menu_down=new CL_InputButton;
	menu_left=new CL_InputButton;
	menu_right=new CL_InputButton;
	menu_select=new CL_InputButton;

	menu_up->bind(CL_Input::keyboards[0]->get_button(CL_KEY_UP));
	menu_down->bind(CL_Input::keyboards[0]->get_button(CL_KEY_DOWN));
	menu_left->bind(CL_Input::keyboards[0]->get_button(CL_KEY_LEFT));
	menu_right->bind(CL_Input::keyboards[0]->get_button(CL_KEY_RIGHT));
	menu_select->bind(CL_Input::keyboards[0]->get_button(CL_KEY_ENTER));
	menu_select->bind(CL_Input::keyboards[0]->get_button(CL_KEY_SPACE));

	titleleft=-800/4;
	titleright=5*800/4;

	selected=0;
	max_sel=0;
	menu=1;

	players=2;
	pno=10;
	speed=20;

	p0left=CL_KEY_A;
	p0right=CL_KEY_D;
	p0acc=CL_KEY_W;
	p0dec=CL_KEY_S;
	p0fw=CL_KEY_LSHIFT;
	p0rw=CL_KEY_LCTRL;
	p1left=CL_KEY_LEFT;
	p1right=CL_KEY_RIGHT;
	p1acc=CL_KEY_UP;
	p1dec=CL_KEY_DOWN;
	p1fw=CL_KEY_RSHIFT;
	p1rw=CL_KEY_RCTRL;
}

ckmenu::~ckmenu(void)
{
}

int ckmenu::get_menu(void)
{
	return menu;
}

void ckmenu::set_menu(int val)
{
	menu=val;
}

CL_Font *ckmenu::get_font(enum enfont f)
{
	if(f==XSENS) return font;
	else if(f==XSENS_INV) return font_invert;
	else if(f==WHITE) return font_white;
	else return NULL;
}

int ckmenu::print_item(enum enfont fs, enum enfont f, int lx, int rx, int h,
			char *s1, char *s2, int selected)
{
	CL_Font *fp1=get_font(fs);
	CL_Font *fp2=get_font(f);

	if(!fp1 || !fp2) return -1;

	if(selected)
	{
		fp1->print_right(lx, h, s1);
		fp1->print_left(rx, h, s2);
	}
	else
	{
		fp2->print_right(lx, h, s1);
		fp2->print_left(rx, h, s2);
	}
	return 0;
}

int ckmenu::print_item_mix(enum enfont fs, enum enfont f, enum enfont w, int lx, int rx, int h,
				char *s1, char *s2, int selected)
{
	CL_Font *fp1=get_font(fs);
	CL_Font *fp2=get_font(f);
	CL_Font *fp3=get_font(w);

	if(!fp1 || !fp2 || !fp3) return -1;

	if(selected) fp1->print_right(lx, h, s1);
	else fp2->print_right(lx, h, s1);
	fp3->print_left(rx, h, s2);
	return 0;
}

int ckmenu::update_and_display(void)
{
	if(menu)
	{
		int reset=0;
		int swidth=get_swidth();
		int sheight=get_sheight();

		if(titleleft+10<=swidth/2) titleleft+=10;
		else titleleft=swidth/2;
		if(titleright-10>=swidth/2)
		{
			titleright-=10;
			// So that the 'real' one prints to screen faster.
			if(menu==1) print(WHITE, 0, sheight*2, (char *)"BY GRAHAM KEELING");
		}
		else
		{
			titleright=swidth/2;
			CL_Display::fill_rect(swidth/4, sheight/5-40, 3*swidth/4, 2*sheight/5+240,
				0.0f, 0.0f, 0.0f, 0.5f);
			if(menu==1) print(WHITE, swidth/2, sheight/5+40, (char *)"BY GRAHAM KEELING");
		}

		if(menu_up->pop_pressed()) selected--;
		if(menu_down->pop_pressed()) selected++;
		if(selected==-1) selected=max_sel;
		else if(selected==max_sel+1) selected=0;

		if(menu==1)
		{
			max_sel=3;
			print_item(XSENS, XSENS, titleleft, titleright, sheight/5, (char *)"CLAN", (char *)"KARS", 0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5, (char *)"PL", (char *)"AY", selected==0?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+40,(char *) "SETT", (char *)"INGS", selected==1?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+80, (char *)"CONFIGU", (char *)"RE CARS", selected==2?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+120, (char *)"QU", (char *)"IT", selected==3?1:0);
		}
		else if(menu==2)
		{
			max_sel=3;
			char tmp[10];
			snprintf(tmp, sizeof(tmp), (char *)"T %d", sheight);
			print_item(XSENS, XSENS, titleleft, titleright, sheight/5, (char *)"SETT", (char *)"INGS", 0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5, (char *)"HEIGH", tmp, selected==0?1:0);
			snprintf(tmp, sizeof(tmp), "H %d", swidth);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+40, (char *)"WIDT", tmp, selected==1?1:0);
			snprintf(tmp, sizeof(tmp), "D %d", speed);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+80, (char *)"SPEE", tmp, selected==2?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+160, (char *)"BA", (char *)"CK", selected==3?1:0);
		}
		else if(menu==3)
		{
			max_sel=3;
			char tmp[10];
			print_item(XSENS, XSENS, titleleft, titleright, sheight/5, (char *)"CONFIGU", (char *)"RE CARS", 0);
			if(players==1) print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5, (char *)"ONE P", (char *)"LAYER", selected==0?1:0);
			else print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5, (char *)"TWO P", (char *)"LAYER", selected==0?1:0);
			snprintf(tmp, sizeof(tmp), "ER %d", pno);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+40, (char *)"NUMB", tmp, selected==1?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+80, (char *)"COL", (char *)"OUR", selected==2?1:0);
			print_item(XSENS_INV, XSENS, titleleft, titleright, 2*sheight/5+120, (char *)"BA", (char *)"CK", selected==3?1:0);
		}
		else if(menu==4)
		{
			print_item(XSENS, XSENS, titleleft, titleright, sheight/5, (char *)"RES", (char *)"ULT", 0);
		}

		if(menu_select->pop_pressed())
		{
			printf("%d selected!\n", selected);
			if(menu==1)
			{
				if(selected==0) // start game
				{
					menu=0;
					selected=0;
					countdown=4;
					return 1;
				}
				else if(selected==1) { menu=2; selected=0; }
				else if(selected==2) { menu=3; selected=0; }
				else if(selected==3) exit(0);
			}
			else if(menu==2)
			{
				if(selected==3) { menu=1; selected=0; }
			}
			else if(menu==3)
			{
				if(selected==0)
				{
					if(players==2) players=1;
					else players=2;
					selected=0;
				}
				else if(selected==2) { selected=0; }
				else if(selected==3) { menu=1; selected=0; }
			}

		}

		if(menu_left->pop_pressed())
		{
			if(menu==2)
			{
				if(selected==0) { sheight-=10; reset++; }
				else if(selected==1) { swidth-=10; reset++; }
				else if(selected==2) speed-=1;
			}
			else if(menu==3)
			{
				if(selected==1 && pno>5) { pno--; }
			}
		}
		if(menu_right->pop_pressed())
		{
			if(menu==2)
			{
				if(selected==0) { sheight+=10; reset++; }
				else if(selected==1) { swidth+=10; reset++; }
				else if(selected==2) speed+=1;
			}
			else if(menu==3)
			{
				if(selected==1 && pno<MAX_PNO-1) { pno++; }
			}
		}
		if(reset)
		{
			CL_Display::set_videomode(swidth, sheight, 16, false, true);
			CL_Display::clear_display();
		}
	}
	return 0;
}

int ckmenu::print_loading(enum enfont f, int xpos, int ypos, char *string)
{
	CL_Font *fp=get_font(f);

	if(!fp) return -1;

	fp->print_center(xpos, ypos, string);
	CL_Display::flip_display();
	CL_Display::clear_display(0.0f, 0.0f, 0.0f, 1.0f);
	return 0;
}

int ckmenu::print(enum enfont f, int xpos, int ypos, char *string)
{
	CL_Font *fp=get_font(f);

	if(!fp) return -1;

	fp->print_center(xpos, ypos, string);
	return 0;
}

int ckmenu::print_int(enum enfont f, int xpos, int ypos, int i)
{
	char string[5]="";
	CL_Font *fp=get_font(f);

	if(!fp) return -1;

	snprintf(string, sizeof(string), "%d", i);
	fp->print_center(xpos, ypos, string);

	return 0;
}

int ckmenu::get_players(void)
{
	return players;
}

void ckmenu::set_players(int val)
{
	players=val;
}

int ckmenu::get_pno(void)
{
	return pno;
}

void ckmenu::set_pno(int val)
{
	pno=val;
}

int ckmenu::get_speed(void)
{
	return speed;
}

void ckmenu::set_speed(int val)
{
	speed=val;
}

int ckmenu::get_countdown(void)
{
	return countdown;
}

void ckmenu::do_countdown(void)
{
	char tmp[2]="";
	if(countdown-1>0)
	{
		snprintf(tmp, sizeof(tmp), "%d", countdown-1);
		print(XSENS, get_swidth()/2, 80, tmp);
	}
	countdown--;
}

int ckmenu::get_swidth(void)
{
	return CL_Display::get_width();
}

int ckmenu::get_sheight(void)
{
	return CL_Display::get_height();
}

int ckmenu::get_p0left(void)
{
	return p0left;
}

int ckmenu::get_p0right(void)
{
	return p0right;
}

int ckmenu::get_p0acc(void)
{
	return p0acc;
}

int ckmenu::get_p0dec(void)
{
	return p0dec;
}

int ckmenu::get_p0fw(void)
{
	return p0fw;
}

int ckmenu::get_p0rw(void)
{
	return p0rw;
}

int ckmenu::get_p1left(void)
{
	return p1left;
}

int ckmenu::get_p1right(void)
{
	return p1right;
}

int ckmenu::get_p1acc(void)
{
	return p1acc;
}

int ckmenu::get_p1dec(void)
{
	return p1dec;
}

int ckmenu::get_p1fw(void)
{
	return p1fw;
}

int ckmenu::get_p1rw(void)
{
	return p1rw;
}
