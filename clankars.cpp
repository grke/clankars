#include "ckmisc.h"

class MyApp : public CL_ClanApplication
{
public:
	virtual char *get_title()
	{
		return "ClanKars";
	}

	virtual int main(int argc, char **argv)
	{
		try
		{
			car *test;
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			cktrack *track=new cktrack;
			CL_Surface *exp[16];
			CL_Surface *smallexp[16];
			ckmenu *menu=new ckmenu("Fonts/fnt_xsensitive", "Fonts/fnt_xsensitive_invert", "Fonts/fnt_avantgarde");
			car *globcar=new car();
			car *plyr[MAX_PNO];
			ckobj *bridge=new ckobj();

			CL_Display::set_videomode(800, 500, 16, false, true);
			CL_Display::clear_display();

			bridge->load_image("graphics/track/bridge.png");
			bridge->setx(33*BLK-29);
			bridge->sety(16*BLK-24);

		        for(int i=0; i<16; i++)
			{
				char tmp[64]="";
				snprintf(tmp, sizeof(tmp), "graphics/explosions/explosion%d.png", i);
				exp[i]=CL_PNGProvider::create(tmp, NULL);
				snprintf(tmp, sizeof(tmp), "graphics/explosions/small%d.png", i);
				smallexp[i]=CL_PNGProvider::create(tmp, NULL);
			}

			menu->print_loading(WHITE, menu->get_swidth()/2, menu->get_sheight()/2, "LOADING TRACK CONFIG");
			if(track->load_config()) return 1;
			menu->print_loading(WHITE, menu->get_swidth()/2, menu->get_sheight()/2, "LOADING TRACK TWO");
			if((track->load_track("config/two.trk"))) return 1;

			for(int i=0; i<menu->get_pno(); i++)
			{
				char tmp[32]="";
				snprintf(tmp, sizeof(tmp), "LOADING CAR %d", menu->get_pno()-i);
				menu->print_loading(WHITE, menu->get_swidth()/2, menu->get_sheight()/2, tmp);
				plyr[i]=new car(globcar, 100*i, track->get_sposx(i), track->get_sposy(i), track->get_sposang(i), 20-i>8?20-i:8, FRONTM, i%10==0?REARM:MINE, track, (int)((i/6)+3));
			}

			malloc_stats();
			test=new car(globcar, 100, track->get_sposx(menu->get_pno()), track->get_sposy(menu->get_pno()), track->get_sposang(menu->get_pno()), 20>8?20:8, FRONTM, MINE, track, 9);
			malloc_stats();
			delete test;
			malloc_stats();

			menu->print_loading(WHITE, menu->get_swidth()/2, menu->get_sheight()/2, "HERE WE GO!");

			CL_InputButton_Group *escape_button=new CL_InputButton_Group;
			CL_InputButton_Group *pause_button=new CL_InputButton_Group;
			escape_button->
			 add(CL_Input::keyboards[0]->get_button(CL_KEY_ESCAPE));
			pause_button->
			 add(CL_Input::keyboards[0]->get_button(CL_KEY_P));

			while(1)
			{
				int i;
				int j;
				int pno=menu->get_pno();
				int players=menu->get_players();
				int swidth=menu->get_swidth();
				int sheight=menu->get_sheight();

				if(pause_button->is_pressed()==true)
				{
					CL_System::sleep(200);
					CL_System::keep_alive();
					menu->print(XSENS, swidth/2, sheight/2, "PAUSED");
					CL_Display::flip_display();
					while(pause_button->is_pressed()==false)
					{
						CL_System::keep_alive();
					}
					CL_System::sleep(200);
				}

				if(!menu->get_countdown())
				{				
					for(i=0; i<pno-1; i++)
						if(!plyr[i]->isdead() && !plyr[i]->get_finish())
							for(j=i+1; j<pno; j++)
								if(!plyr[j]->isdead() && !plyr[j]->get_finish()
									&& plyr[i]->get_bridge()==plyr[j]->get_bridge())
										detect_proximity(plyr[i], plyr[j]);
					for(i=0; i<pno; i++)
						if(!plyr[i]->get_human() && !plyr[i]->isdead() && !plyr[i]->get_finish())
							for(j=0; j<pno; j++)
								if(!plyr[j]->isdead() && i!=j && !plyr[j]->get_finish()
									&& plyr[i]->get_bridge()==plyr[j]->get_bridge())
										check_road_ahead(plyr[i], plyr[j]);

					for(i=0; i<pno; i++)
					{
						if(plyr[i]->get_repos())
						{
							int det2=0;
							// Check that the track is clear before the player reappears
							for(int j=0; j<pno; j++)
							{
								if(j!=i && !plyr[j]->isdead() && !plyr[j]->get_finish()
									&& plyr[i]->get_bridge()==plyr[j]->get_bridge())
								{
									int k=0;
									int pix=plyr[i]->getx();
									int piy=plyr[i]->gety();
									int pjx=plyr[j]->getx();
									int pjy=plyr[j]->gety();
									if(pix<pjx+(CAR_L*2) && pix>pjx-(CAR_L*2)
										 && piy<pjy+(CAR_L*2) && piy>pjy-(CAR_L*2))
									{
										det2++;
										continue;
									}
								}
							}
							plyr[i]->reposition(det2, track);
						}
						if(plyr[i]->updatecheckpoint_and_calcvels(track))
						{
							if(!menu->get_menu() && update_positions(plyr, track, players, pno))
							{
								CL_System::sleep(2000);
								menu->set_menu(4); // Finish track
								for(i=0; i<pno; i++) // Reset players
                                                			plyr[i]->reset(i, 10-i>8?10-i:8, FRONTM, i%10==0?REARM:MINE, track, (int)((i/6)+3), 0); // 0 means don't reset finish positions
							}
						}
					}

					CL_Display::clear_display(0.0f, 0.0f, 0.0f, 1.0f);
				}

				if(players==2)
				{
					CL_Display::fill_rect(0, 0, swidth/2, sheight, track->get_surface(), swidth/4-plyr[0]->getx(), (sheight/2)-plyr[0]->gety());
					CL_Display::fill_rect(swidth/2, 0, swidth, sheight, track->get_surface(), swidth/4+(swidth/2)-plyr[1]->getx(), (sheight/2)-plyr[1]->gety());
				}
				else CL_Display::fill_rect(0, 0, swidth, sheight, track->get_surface(), swidth/2-plyr[0]->getx(), (sheight/2)-plyr[0]->gety());

				if(!menu->get_countdown()) do_weapons_stuff(plyr, track, players, pno, smallexp, 0, swidth, sheight); // 0 means not on bridge

				for(i=0; i<pno; i++) // display cars not on bridges
				{
					if(plyr[i]->get_killed() || plyr[i]->get_bridge()==1) continue;
					if(players==2)
					{
						if(plyr[i]->display(plyr[0], plyr[1], exp, swidth, sheight)) plyr[i]->set_repos(1);
					}
					else
					{
						if(plyr[i]->display(plyr[0], exp, swidth, sheight)) plyr[i]->set_repos(1);
					}
					if(menu->get_menu()) plyr[i]->set_damage(100); // So cars on title screen don't disappear
				}

				if(players==2)
				{
					bridge->display_on_screen1(plyr[0], swidth, sheight);
					bridge->display_on_screen2(plyr[1], swidth, sheight);
				}
				else
				{
					bridge->display_on_screen(plyr[0], swidth, sheight);
				}

				if(!menu->get_countdown()) do_weapons_stuff(plyr, track, players, pno, smallexp, 1, swidth, sheight); // 1 means on bridge

				for(i=0; i<pno; i++) // display cars on bridges
				{
					if(plyr[i]->get_killed() || plyr[i]->get_bridge()==0) continue;
					if(players==2)
					{
						if(plyr[i]->display(plyr[0], plyr[1], exp, swidth, sheight)) plyr[i]->set_repos(1);
					}
					else
					{
						if(plyr[i]->display(plyr[0], exp, swidth, sheight)) plyr[i]->set_repos(1);
					}
					if(menu->get_menu()) plyr[i]->set_damage(100); // So cars on title screen don't disappear
				}

				if(escape_button->is_pressed()==true) menu->set_menu(1);
				if(!menu->get_menu())
				{
					int p0d=(int)plyr[0]->get_damage();
					if(players==2)
					{
						int p1d=(int)plyr[1]->get_damage();
						menu->print(WHITE, swidth/8, sheight-80, "POSITION");
						menu->print(WHITE, 2*swidth/8, sheight-80, "DAMAGE");
						menu->print(WHITE, 3*swidth/8, sheight-80, "LAPS");

						menu->print(WHITE, swidth/8+swidth/2, sheight-80, "POSITION");
						menu->print(WHITE, 2*swidth/8+swidth/2, sheight-80, "DAMAGE");
						menu->print(WHITE, 3*swidth/8+swidth/2, sheight-80, "LAPS");

						menu->print_int(WHITE, swidth/8, sheight-40, plyr[0]->get_position()+1);
						CL_Display::fill_rect(2*swidth/8-55, sheight-40, 2*swidth/8+55, sheight-20,
							0.0f, 0.0f, 0.0f, 0.5f);
						for(int i=0; i<p0d; i++)
								CL_Display::fill_rect(2*swidth/8-50+i, sheight-35, 2*swidth/8-50+i+1, sheight-25,
									i>55?0.2f:1.0f, i>55?1.0f:i>20?0.85f:0.0f, 0.0f, 1.0f);
						if(plyr[0]->get_finish())
							menu->print(WHITE, 3*swidth/8, sheight-40, "COMPLETE");
						else
							menu->print_int(WHITE, 3*swidth/8, sheight-40, track->get_laps()-plyr[0]->getlap()-1);

						menu->print_int(WHITE, swidth/8+swidth/2, sheight-40, plyr[1]->get_position()+1);
						CL_Display::fill_rect(2*swidth/8-55+swidth/2, sheight-40, 2*swidth/8+55+swidth/2, sheight-20,
							0.0f, 0.0f, 0.0f, 0.5f);
						for(int i=0; i<p1d; i++)
								CL_Display::fill_rect(2*swidth/8+swidth/2-50+i, sheight-35, 2*swidth/8+swidth/2-50+i+1,
									sheight-25, i>55?0.2f:1.0f, i>55?1.0f:i>20?0.85f:0.0f, 0.0f, 1.0f);
						if(plyr[1]->get_finish())
							menu->print(WHITE, 3*swidth/8+swidth/2, sheight-40, "COMPLETE");
						else
							menu->print_int(WHITE, 3*swidth/8+swidth/2, sheight-40, track->get_laps()-plyr[1]->getlap()-1);
					}
					else
					{
						menu->print(WHITE, swidth/4, sheight-80, "POSITION");
						menu->print(WHITE, 2*swidth/4, sheight-80, "DAMAGE");
						menu->print(WHITE, 3*swidth/4, sheight-80, "LAPS");

						menu->print_int(WHITE, swidth/4, sheight-40, plyr[0]->get_position()+1);
						CL_Display::fill_rect(2*swidth/4-55, sheight-40, 2*swidth/4+55, sheight-20,
							0.0f, 0.0f, 0.0f, 0.5f);
						for(int i=0; i<p0d; i++)
								CL_Display::fill_rect(2*swidth/4-50+i, sheight-35, 2*swidth/4-50+i+1, sheight-25,
									i>55?0.2f:1.0f, i>55?1.0f:i>20?0.85f:0.0f, 0.0f, 1.0f);
						if(plyr[0]->get_finish())
							menu->print(WHITE, 3*swidth/8, sheight-40, "COMPLETE");
						else
							menu->print_int(WHITE, 3*swidth/4, sheight-40, track->get_laps()-plyr[0]->getlap()-1);
					}
				}

				// Black vertical bar in two player mode.
				if(players==2) CL_Display::fill_rect(swidth/2-1, 0, swidth/2+1, sheight,
						0.0f, 0.0f, 0.0f, 1.0f);

				if(menu->get_countdown()>0) { menu->do_countdown(); CL_System::sleep(1000); }

				if(menu->update_and_display())
				{
					int m=0;
					printf("AH!\n");
					plyr[m]->reset(pno-1, 20, FRONTM, REARM, track,
						menu->get_p0left(), menu->get_p0right(), menu->get_p0acc(),
						menu->get_p0dec(), menu->get_p0fw(), menu->get_p0rw(), 1); // 1 means reset finish positions
					printf("lA!\n");
					m++;
					if(players==2)
					{
						plyr[m]->reset(pno-2, 20, FRONTM, REARM, track,
							menu->get_p1left(), menu->get_p1right(), menu->get_p1acc(),
							menu->get_p1dec(), menu->get_p1fw(), menu->get_p1rw(), 1);
						m++;
					}

					for(m; m<pno; m++)
						plyr[m]->reset(m-players, 10-m>8?10-m:8, FRONTM, m%10==0?REARM:MINE,
							track, (int)((m/6)+3), 1);
					track->set_next_position(1);
				}
				if(menu->get_pno()!=pno)
				{
					if(menu->get_pno()-pno<0)
					{
						pno--;
						printf("before delete...\n");
					//	malloc_stats();
						delete plyr[pno]; plyr[pno]=NULL;
						printf("after delete...\n");
					//	malloc_stats();
					}
					else
					{
						printf("New plyr!\n");
					//	malloc_stats();
						plyr[pno]=new car(globcar, 100*pno, track->get_sposx(0), track->get_sposy(0), track->get_sposang(0), 20-pno>8?20-pno:8, FRONTM, pno%10==0?REARM:MINE, track, (int)((pno/6)+3));
						printf("After new plyr!\n");
					//	malloc_stats();
						pno++;
					}
				}
				if(menu->get_menu()==4)
				{
					for(int i=0; i<pno; i++)
					{
						for(int j=0; j<pno; j++)
						{
							if(plyr[j]->get_finish()==i+1)
							{
								char tmp[64];
								snprintf(tmp, sizeof(tmp), "%d: PLAYER %d", i+1, j+1);
								menu->print(WHITE, swidth/2, sheight/5+20+20*(i+1), tmp);
							}
						}
					}
				}
				CL_Display::flip_display();
				CL_System::sleep(menu->get_speed());
				CL_System::keep_alive();
			}

			CL_System::sleep(2000);

			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}

		catch(CL_Error err)
		{
			std::cout << err.message.c_str() << std::endl;
			return 1;
		}

		return 0;
	}

} app;
