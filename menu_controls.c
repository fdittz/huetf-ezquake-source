/*
Copyright (C) 2011 ezQuake team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*

	Controls Menu module

	Uses Ctrl_Tab and Settings modules

	Naming convention:
	function mask | usage    | purpose
	--------------|----------|-----------------------------
	Menu_Opt_*    | external | interface for menu.c
	CT_Opt_*      | external | interface for Ctrl_Tab.c

	made by:
		johnnycz, Jan 2006

*/

#include "quakedef.h"
#include "settings.h"
#include "settings_page.h"
#include "Ctrl_EditBox.h"
#include "vx_stuff.h"
#include "vx_tracker.h"
#include "gl_model.h"
#include "gl_local.h"
#include "tr_types.h"
#include "teamplay.h"
#include "EX_FileList.h"
#include "Ctrl.h"
#include "Ctrl_Tab.h"
#include "input.h"
#include "qsound.h"
#include "menu.h"
#include "keys.h"
#include "hud.h"
#include "hud_common.h"
#include "config_manager.h"

typedef enum {
	OPTPG_BINDS,
	OPTPG_SCOUT,
	OPTPG_SNIPER,
	OPTPG_SOLDIER,
	OPTPG_DEMOMAN,
	OPTPG_MEDIC,
	OPTPG_HWGUY,
	OPTPG_PYRO,
	OPTPG_SPY,
	OPTPG_ENGINEER
}	controls_tab_t;

CTab_t controls_tab;
int controls_unichar;	// variable local to this module

extern cvar_t     scr_scaleMenu;
extern int        menuwidth;
extern int        menuheight;

extern qbool    m_entersound; // todo - put into menu.h
void M_Menu_Help_f (void);	// todo - put into menu.h
extern cvar_t scr_scaleMenu;
qbool OnMenuAdvancedChange(cvar_t*, char*);



static qbool InvertMouse(void) { return m_pitch.value < 0; }
const char* InvertMouseRead(void) { return InvertMouse() ? "on" : "off"; }
void InvertMouseToggle(qbool back) { Cvar_SetValue(&m_pitch, -m_pitch.value); }


//=============================================================================
// <BINDS>

extern cvar_t in_raw, in_m_smooth, m_rate, in_m_os_parameters;
const char* in_raw_enum[] = { "off", "on" };
const char* in_m_os_parameters_enum[] = { "off", "Keep accel settings", "Keep speed settings", "Keep all settings" };

void Menu_Input_Restart(void) { Cbuf_AddText("in_restart\n"); }

settings_page settbinds;

void CT_Opt_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw(x2, y2, w, h, &settbinds);
}

int CT_Opt_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key(&settbinds, k, unichar);
}

void OnShow_SettBinds(void) { Settings_OnShow(&settbinds); }

qbool CT_Opt_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&settbinds, ms);
}

CTabPage_Handlers_t controls_controls_handlers = {
	CT_Opt_Binds_Draw,
	CT_Opt_Binds_Key,
	OnShow_SettBinds,
	CT_Opt_Binds_Mouse_Event
};

// </BINDS>
//=============================================================================


//=============================================================================
// <SCOUT>
settings_page scoutbinds;

void CT_Opt_Scout_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &scoutbinds, SCOUT_NUM);
}

int CT_Opt_Scout_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&scoutbinds, k, unichar, SCOUT_NUM);
}

void OnShow_SettScoutBinds(void) { Settings_OnShow(&scoutbinds); LoadClassConfig(classconfigfiles[SCOUT_NUM],SCOUT_NUM);}

qbool CT_Opt_Scout_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&scoutbinds, ms);
}

CTabPage_Handlers_t controls_scout_handlers = {
	CT_Opt_Scout_Binds_Draw,
	CT_Opt_Scout_Binds_Key,
	OnShow_SettScoutBinds,
	CT_Opt_Scout_Binds_Mouse_Event
};
// </SCOUT>
//=============================================================================

//=============================================================================
// <SNIPER>
settings_page sniperbinds;

void CT_Opt_Sniper_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &sniperbinds, SNIPER_NUM);
}

int CT_Opt_Sniper_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&sniperbinds, k, unichar, SNIPER_NUM);
}

void OnShow_SettSniperBinds(void) { Settings_OnShow(&sniperbinds); LoadClassConfig(classconfigfiles[SNIPER_NUM],SNIPER_NUM); }

qbool CT_Opt_Sniper_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&sniperbinds, ms);
}

CTabPage_Handlers_t controls_sniper_handlers = {
	CT_Opt_Sniper_Binds_Draw,
	CT_Opt_Sniper_Binds_Key,
	OnShow_SettSniperBinds,
	CT_Opt_Sniper_Binds_Mouse_Event
};
// </SNIPER>
//=============================================================================

//=============================================================================
// <SOLDIER>
settings_page soldierbinds;

void CT_Opt_Soldier_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &soldierbinds, SOLDIER_NUM);
}

int CT_Opt_Soldier_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&soldierbinds, k, unichar, SOLDIER_NUM);
}

void OnShow_SettSoldierBinds(void) { Settings_OnShow(&soldierbinds); LoadClassConfig(classconfigfiles[SOLDIER_NUM],SOLDIER_NUM); }

qbool CT_Opt_Soldier_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&soldierbinds, ms);
}

CTabPage_Handlers_t controls_soldier_handlers = {
	CT_Opt_Soldier_Binds_Draw,
	CT_Opt_Soldier_Binds_Key,
	OnShow_SettSoldierBinds,
	CT_Opt_Soldier_Binds_Mouse_Event
};
// </SOLDIER>
//=============================================================================

//=============================================================================
// <DEMOMAN>
settings_page demomanbinds;

void CT_Opt_Demoman_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &demomanbinds, DEMOMAN_NUM);
}

int CT_Opt_Demoman_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&demomanbinds, k, unichar, DEMOMAN_NUM);
}

void OnShow_SettDemomanBinds(void) { Settings_OnShow(&demomanbinds); LoadClassConfig(classconfigfiles[DEMOMAN_NUM],DEMOMAN_NUM); }

qbool CT_Opt_Demoman_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&demomanbinds, ms);
}

CTabPage_Handlers_t controls_demoman_handlers = {
	CT_Opt_Demoman_Binds_Draw,
	CT_Opt_Demoman_Binds_Key,
	OnShow_SettDemomanBinds,
	CT_Opt_Demoman_Binds_Mouse_Event
};
// </DEMOMAN>
//=============================================================================

//=============================================================================
// <MEDIC>
settings_page medicbinds;

void CT_Opt_Medic_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &medicbinds, MEDIC_NUM);
}

int CT_Opt_Medic_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&medicbinds, k, unichar, MEDIC_NUM);
}

void OnShow_SettMedicBinds(void) { Settings_OnShow(&medicbinds); LoadClassConfig(classconfigfiles[MEDIC_NUM],MEDIC_NUM); }

qbool CT_Opt_Medic_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&medicbinds, ms);
}

CTabPage_Handlers_t controls_medic_handlers = {
	CT_Opt_Medic_Binds_Draw,
	CT_Opt_Medic_Binds_Key,
	OnShow_SettMedicBinds,
	CT_Opt_Medic_Binds_Mouse_Event
};
// </MEDIC>
//=============================================================================

//=============================================================================
// <HWGUY>
settings_page hwguybinds;

void CT_Opt_Hwguy_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &hwguybinds, HWGUY_NUM);
}

int CT_Opt_Hwguy_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&hwguybinds, k, unichar, HWGUY_NUM);
}

void OnShow_SettHwguyBinds(void) { Settings_OnShow(&hwguybinds); LoadClassConfig(classconfigfiles[HWGUY_NUM],HWGUY_NUM); }

qbool CT_Opt_Hwguy_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&hwguybinds, ms);
}

CTabPage_Handlers_t controls_hwguy_handlers = {
	CT_Opt_Hwguy_Binds_Draw,
	CT_Opt_Hwguy_Binds_Key,
	OnShow_SettHwguyBinds,
	CT_Opt_Hwguy_Binds_Mouse_Event
};
// </HWGUY>
//=============================================================================

//=============================================================================
// <PYRO>
settings_page pyrobinds;

void CT_Opt_Pyro_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &pyrobinds, PYRO_NUM);
}

int CT_Opt_Pyro_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&pyrobinds, k, unichar, PYRO_NUM);
}

void OnShow_SettPyroBinds(void) { Settings_OnShow(&pyrobinds); LoadClassConfig(classconfigfiles[PYRO_NUM],PYRO_NUM); }

qbool CT_Opt_Pyro_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&pyrobinds, ms);
}

CTabPage_Handlers_t controls_pyro_handlers = {
	CT_Opt_Pyro_Binds_Draw,
	CT_Opt_Pyro_Binds_Key,
	OnShow_SettPyroBinds,
	CT_Opt_Pyro_Binds_Mouse_Event
};
// </PYRO>
//=============================================================================

//=============================================================================
// <SPY>
settings_page spybinds;

void CT_Opt_Spy_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &spybinds, SPY_NUM);
}

int CT_Opt_Spy_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&spybinds, k, unichar, SPY_NUM);
}

void OnShow_SettSpyBinds(void) { Settings_OnShow(&spybinds); LoadClassConfig(classconfigfiles[SPY_NUM],SPY_NUM); }

qbool CT_Opt_Spy_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&spybinds, ms);
}

CTabPage_Handlers_t controls_spy_handlers = {
	CT_Opt_Spy_Binds_Draw,
	CT_Opt_Spy_Binds_Key,
	OnShow_SettSpyBinds,
	CT_Opt_Spy_Binds_Mouse_Event
};
// </SPY>
//=============================================================================

//=============================================================================
// <ENGINEER>
settings_page engineerbinds;

void CT_Opt_Engineer_Binds_Draw (int x2, int y2, int w, int h, CTab_t *tab, CTabPage_t *page) {
	Settings_Draw_2(x2, y2, w, h, &engineerbinds, ENGINEER_NUM);
}

int CT_Opt_Engineer_Binds_Key (int k, wchar unichar, CTab_t *tab, CTabPage_t *page) {
	return Settings_Key_2(&engineerbinds, k, unichar, ENGINEER_NUM);
}

void OnShow_SettEngineerBinds(void) { Settings_OnShow(&engineerbinds); LoadClassConfig(classconfigfiles[ENGINEER_NUM],ENGINEER_NUM); }

qbool CT_Opt_Engineer_Binds_Mouse_Event(const mouse_state_t *ms)
{
	return Settings_Mouse_Event(&engineerbinds, ms);
}

CTabPage_Handlers_t controls_engineer_handlers = {
	CT_Opt_Engineer_Binds_Draw,
	CT_Opt_Engineer_Binds_Key,
	OnShow_SettEngineerBinds,
	CT_Opt_Engineer_Binds_Mouse_Event
};
// </ENGINEER>
//=============================================================================


void Menu_Controls_Key(int key, wchar unichar) {
    int handled = CTab_Key(&controls_tab, key, unichar);
	controls_unichar = unichar;

	if (!handled && (key == K_ESCAPE || key == K_MOUSE2))
		M_Menu_Main_f();
}

void Menu_Controls_Draw(void) {
	int x, y, w, h;

	M_Unscale_Menu();

    // this will add top, left and bottom padding
    // right padding is not added because it causes annoying scrollbar behaviour
    // when mouse gets off the scrollbar to the right side of it
	w = vid.width - OPTPADDING; // here used to be a limit to 512x... size
	h = vid.height - OPTPADDING*2;
	x = OPTPADDING;
	y = OPTPADDING;

	CTab_Draw(&controls_tab, x, y, w, h);
}

// CONTROLS TAB
// please try to put mostly binds in here
setting settbinds_arr[] = {
	
	ADDSET_SEPARATOR("Mouse Settings"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BOOL		("Freelook", freelook),
	ADDSET_BASIC_SECTION(),
	ADDSET_NUMBER	("Sensitivity", sensitivity, 1, 20, 0.25), // My sens is 16, so maybe some people have it up to 20?
	ADDSET_ADVANCED_SECTION(),
	ADDSET_NUMBER	("Menu Mouse Sensitivity", cursor_sensitivity, 0.10 , 3, 0.10),
	ADDSET_NUMBER	("Acceleration", m_accel, 0, 1, 0.1),
	ADDSET_BASIC_SECTION(),
	ADDSET_CUSTOM	("Invert Mouse", InvertMouseRead, InvertMouseToggle, "Inverts the Y axis."),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_STRING   ("X-axis Sensitivity", m_yaw),
	ADDSET_STRING   ("Y-axis Sensitivity", m_pitch),
	ADDSET_NAMED    ("Raw Mouse Input", in_raw, in_raw_enum),
	ADDSET_BASIC_SECTION(),

	ADDSET_SEPARATOR("Movement"),
	ADDSET_BIND("Attack", "+attack"),
	ADDSET_BIND("Jump/Swim up", "+jump"),
	ADDSET_BIND("Move Forward", "+forward"),
	ADDSET_BIND("Move Backward", "+back"),
	ADDSET_BIND("Strafe Left", "+moveleft"),
	ADDSET_BIND("Strafe Right", "+moveright"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BIND("Swim Up", "+moveup"),
	ADDSET_BIND("Swim Down", "+movedown"),
	ADDSET_BIND("Zoom In/Out", "+huetfclient_zoom"),
	ADDSET_BASIC_SECTION(),

	ADDSET_SEPARATOR("TEAM FORTRESS SPECIFIC"),
	ADDSET_BIND("Flag Information", "flaginfo"),
	ADDSET_BIND("Inventory Information", "inv"),
	ADDSET_BIND("Drop Flag", "dropitems"),
	ADDSET_BIND("Discard Ammo", "discard"),
	ADDSET_BIND("Grenade 1 (Hand/Caltrops/Impact)", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2 (Class Grenade)", "huetfclient_gren2"),

	ADDSET_SEPARATOR("Weapons"),
	ADDSET_BIND("Previous Weapon", "weapon 12"),
	ADDSET_BIND("Next Weapon", "weapon 10"),
	ADDSET_BIND("Axe", "weapon 1"),
	ADDSET_BIND("Shotgun", "weapon 2"),
	ADDSET_BIND("Super Shotgun", "weapon 3"),
	ADDSET_BIND("Nailgun", "weapon 4"),
	ADDSET_BIND("Super Nailgun", "weapon 5"),
	ADDSET_BIND("Grenade Launcher", "weapon 6"),
	ADDSET_BIND("Rocket Launcher", "weapon 7"),
	ADDSET_BIND("Thunderbolt", "weapon 8"),

	ADDSET_SEPARATOR("Chat settings"),
	ADDSET_BIND	("Chat", "messagemode"),
	ADDSET_BIND	("Teamchat", "messagemode2"),

	ADDSET_SEPARATOR("Miscellaneous"),
	ADDSET_BIND("Show Scores", "+showscores"),
	ADDSET_BIND("Screenshot", "screenshot"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BIND("Pause", "pause"),
	ADDSET_BIND("Quit", "quit"),
	ADDSET_BIND("Proxy Menu", "toggleproxymenu"),
	ADDSET_BASIC_SECTION(),

	ADDSET_SEPARATOR("Demo & Spec"),
	ADDSET_BIND("Demo Controls", "demo_controls"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BIND("Autotrack", "autotrack"),
	ADDSET_BIND("Play", "cl_demospeed 1;echo Playing demo."),
	ADDSET_BIND("Stop", "disconnect"),
	ADDSET_BIND("Pause", "cl_demospeed 0;echo Demo paused."),
	ADDSET_BIND("Fast Forward", "cl_demospeed 5;echo Demo paused."),
	ADDSET_BASIC_SECTION(),

	ADDSET_SEPARATOR("Teamplay"),
	ADDSET_BIND("Report Status", "tp_msgreport"),
	ADDSET_BIND("Lost Location", "tp_msglost"),
	ADDSET_BIND("Location Safe", "tp_msgsafe"),
	ADDSET_BIND("Point At Item", "tp_msgpoint"),
	ADDSET_BIND("Took Item", "tp_msgtook"),
	ADDSET_BIND("Need Items", "tp_msgneed"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BIND("Yes/Ok", "tp_msgyesok"),
	ADDSET_BIND("Coming From Location", "tp_msgcoming"),
	ADDSET_BASIC_SECTION(),
	ADDSET_BIND("Help Location", "tp_msghelp"),
	ADDSET_ADVANCED_SECTION(),
	ADDSET_BIND("Get Quad", "tp_msggetquad"),
	ADDSET_BIND("Get Pent", "tp_msggetpent"),
	ADDSET_BIND("Enemy Quad Dead", "tp_msgquaddead"),
	ADDSET_BIND("Enemy Has Powerup", "tp_msgenemypwr"),
	ADDSET_BIND("Trick At Location", "tp_msgtrick"),
	ADDSET_BIND("Replace At Location", "tp_msgreplace"),
	ADDSET_BIND("You Take Item", "tp_msgutake"),
	ADDSET_BIND("Waiting", "tp_msgwaiting"),
	ADDSET_BIND("Enemy Slipped", "tp_msgslipped"),
	ADDSET_BASIC_SECTION(),
};


setting scoutbinds_arr[] = {
	ADDSET_BIND("Toggle Autoscanner", "autoscan"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Impact/Caltrop", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Concussion", "huetfclient_gren2")
};

setting sniperbinds_arr[] = {
	ADDSET_BIND("Zoom In/Out", "+huetfclient_zoom"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Flare", "huetfclient_gren2")
};

setting soldierbinds_arr[] = {	
	ADDSET_BIND("Rocket Jump (attack + jump)", "+huetfclient_rjump"),
	ADDSET_BIND("Reload", "reload"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Nail Grenade", "huetfclient_gren2")
};

setting demomanbinds_arr[] = {	
	ADDSET_BIND("Detonate Pipebombs", "detpipe"),
	ADDSET_BIND("Reload", "reload"),
	ADDSET_BIND("Set Detpack - 5 Seconds", "+det5"),
	ADDSET_BIND("Set Detpack - 20 Seconds", "+det20"),
	ADDSET_BIND("Set Detpack - 50 Seconds", "+det50"),
	ADDSET_BIND("Set Detpack - 255 Seconds", "+det225"),
	ADDSET_BIND("Pipebomb Jump (attack + detpipe)","huetfclient_pipejump"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Mirv Grenade", "huetfclient_gren2")
};

setting medicbinds_arr[] = {
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Concussion", "huetfclient_gren2")
};

setting hwguybinds_arr[] = {
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Mirv Grenade", "huetfclient_gren2")
};

setting pyrobinds_arr[] = {
	ADDSET_BIND("Rocket Jump (attack + jump)", "+huetfclient_rjump"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Napalm Grenade", "huetfclient_gren2")
};

setting spybinds_arr[] = {
	ADDSET_BIND("Disguise", "disguise"),
	ADDSET_BIND("Feign Death", "feign"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: Gas Grenade", "huetfclient_gren2")
};

setting engineerbinds_arr[] = {
	ADDSET_BIND("Build Menu", "build"),
	ADDSET_BIND("Detonate Dispenser", "detdispenser"),
	ADDSET_BIND("Detonate Sentry Gun", "detsentry"),
	ADDSET_BIND("Anti-EMP Discard", "huetfclient_antiemp"),
	ADDSET_BIND("Grenade 1: Hand Grenade", "huetfclient_gren1"),
	ADDSET_BIND("Grenade 2: EMP Grenade", "huetfclient_gren2")
};

qbool Menu_Controls_Mouse_Event(const mouse_state_t *ms)
{
	mouse_state_t nms = *ms;

    if (ms->button_up == 2) {
        Menu_Controls_Key(K_MOUSE2, 0);
        return true;
    }

	// we are sending relative coordinates
	nms.x -= OPTPADDING;
	nms.y -= OPTPADDING;
	nms.x_old -= OPTPADDING;
	nms.y_old -= OPTPADDING;

	if (nms.x < 0 || nms.y < 0) return false;

	return CTab_Mouse_Event(&controls_tab, &nms);
}
void Menu_Controls_Init(void) {
	Settings_MainInit();

	Settings_Page_Init(settbinds, settbinds_arr);
	Settings_Page_Init(scoutbinds, scoutbinds_arr);
	Settings_Page_Init(sniperbinds, sniperbinds_arr);
	Settings_Page_Init(soldierbinds, soldierbinds_arr);
	Settings_Page_Init(demomanbinds, demomanbinds_arr);
	Settings_Page_Init(medicbinds, medicbinds_arr);
	Settings_Page_Init(hwguybinds, hwguybinds_arr);
	Settings_Page_Init(pyrobinds, pyrobinds_arr);
	Settings_Page_Init(spybinds, spybinds_arr);
	Settings_Page_Init(engineerbinds, engineerbinds_arr);
	
	Cvar_SetCurrentGroup(CVAR_GROUP_MENU);
	Cvar_ResetCurrentGroup();

	CTab_Init(&controls_tab);
	CTab_AddPage(&controls_tab, "General", OPTPG_BINDS, &controls_controls_handlers);
		CTab_AddPage(&controls_tab, "Scout", OPTPG_SCOUT, &controls_scout_handlers);
		CTab_AddPage(&controls_tab, "Sniper", OPTPG_SNIPER, &controls_sniper_handlers);
		CTab_AddPage(&controls_tab, "Soldier", OPTPG_SOLDIER, &controls_soldier_handlers);
		CTab_AddPage(&controls_tab, "Demoman", OPTPG_DEMOMAN, &controls_demoman_handlers);
		CTab_AddPage(&controls_tab, "Medic", OPTPG_MEDIC, &controls_medic_handlers);
		CTab_AddPage(&controls_tab, "HWGuy", OPTPG_HWGUY, &controls_hwguy_handlers);
		CTab_AddPage(&controls_tab, "Pyro", OPTPG_PYRO, &controls_pyro_handlers);
		CTab_AddPage(&controls_tab, "Spy", OPTPG_SPY, &controls_spy_handlers);
		CTab_AddPage(&controls_tab, "Engineer", OPTPG_ENGINEER, &controls_engineer_handlers);
	}

qbool Menu_Controls_IsBindingKey (void)
{
	// Controls/Binds, and waiting for a keypress
	return m_state == m_controls && (
		(CTab_GetCurrentId (&controls_tab) == OPTPG_BINDS && settbinds.mode == SPM_BINDING) ||
		(CTab_GetCurrentId (&controls_tab) == OPTPG_SCOUT && scoutbinds.mode == SPM_BINDING) ||
		(CTab_GetCurrentId (&controls_tab) == OPTPG_SNIPER && sniperbinds.mode == SPM_BINDING)
	);
}
