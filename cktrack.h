#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
//#include <ClanLib/png.h>

#define TILENO		64
#define TRACK_SIZE_X	70
#define TRACK_SIZE_Y	35
#define BLK		64
#define ADJ		40

struct tile
{
	CL_Surface *image;
	char symbol;
};

struct start_pos
{
	int x;
	int y;
	int ang;
};

class cktrack
{
	protected:
		char track_grid[TRACK_SIZE_X][TRACK_SIZE_Y];
		unsigned char *track_data;
		tile tiles[TILENO];
		int laps;
		CL_Surface *image;
		start_pos spos[30];
		int next_position;
	public:
		cktrack(void);
		~cktrack(void);
		int load_track(const char *path);
		int load_config(void);
		CL_Surface *get_surface(void);
		unsigned char *get_data(void);
		char get_track_symb(int x, int y);
		void locate_next_checkpoint(int *x, int *y, char cursymb);
		void set_laps(int val);
		int get_laps(void);
		int get_next_position(void);
		void set_next_position(int val);
		int get_sposx(int val);
		int get_sposy(int val);
		int get_sposang(int val);
};
