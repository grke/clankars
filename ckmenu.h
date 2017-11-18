#include <ClanLib/display.h>

#define MAX_PNO	50

enum enfont { XSENS=0, XSENS_INV, WHITE };

class ckmenu
{
	private:
		CL_ResourceManager *resources;
		CL_Font *font;
		CL_Font *font_invert;
		CL_Font *font_white;
		CL_InputButton *menu_up;
		CL_InputButton *menu_down;
		CL_InputButton *menu_left;
		CL_InputButton *menu_right;
		CL_InputButton *menu_select;
		int titleleft;
		int titleright;
		int menu;
		int max_sel;
		int selected;
		int players;
		int speed;
		int pno;
		int countdown;
		int p0left; 
		int p0right;
		int p0acc;
		int p0dec;
		int p0fw;
		int p0rw;
		int p1left; 
		int p1right;
		int p1acc;
		int p1dec;
		int p1fw;
		int p1rw;
		CL_Font *get_font(enum enfont f);
		int print_item(enum enfont fs, enum enfont f, int lx, int rx,
				int h, char *s1, char *s2, int selected);
		int print_item_mix(enum enfont fs, enum enfont f, enum enfont w, int lx, int rx,
				int h, char *s1, char *s2, int selected);
	public:
		ckmenu(char *f, char *f_i, char *f_w);
		~ckmenu(void);
		int get_menu(void);
		void set_menu(int val);
		int update_and_display(void);
		int print(enum enfont f, int xpos, int ypos, char *string);
		int print_loading(enum enfont f, int xpos, int ypos, char *string);
		int print_int(enum enfont f, int xpos, int ypos, int i);
		int get_players(void);
		void set_players(int val);
		int get_pno(void);
		void set_pno(int val);
		int get_speed(void);
		void set_speed(int val);
		int get_countdown(void);
		void do_countdown(void);
		int get_swidth(void);
		int get_sheight(void);
		int get_p0left(void); 
		int get_p0right(void);
		int get_p0acc(void);
		int get_p0dec(void);
		int get_p0fw(void);
		int get_p0rw(void);
		int get_p1left(void); 
		int get_p1right(void);
		int get_p1acc(void);
		int get_p1dec(void);
		int get_p1fw(void);
		int get_p1rw(void);
};
