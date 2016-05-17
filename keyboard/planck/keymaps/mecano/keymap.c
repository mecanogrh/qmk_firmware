/*
 * Hello World, this is the mecano keymap.
 *

 * It has the following functionnalities:
 • Every key can send any number of modifiers and/or key code on tap and/or hold.
 • Layers are order agnostic and can be activated at will not depending on their position in the layer queue.

 * It was made with this in mind :
 • allow to write all French special characters easily with a dedicated layer (and so is adaptable to other languages).
 • be OSX friendly.
 • be logical and consistent from OS to softwares.
 • be the most selfsufficient it can be and avoid relying on input methods and softwares.
 • be coherent with the use of VIM and tmux.
 • be ergonomic.

 * It follows these best practice/rules:
 KEYS
 • Control keys shall be positioned on the front row.
 • Parenthesis shall be on shift keys.
 • Common editing triggers (undo, cut, copy, paste) shall be layout agnostic.
 • All navigation and movement triggers shall be consistent with vim and tmux commands.

 LAYERS
 • There shall be no upper and lower layer depencies, i.e. you can trigger whichever layer whenever the layer you are in.
 • The layer key + Enter key shall toggle on the corresponding layer.
 • When a layer is toggled on, the Escape key shall toggle off the layer and go back to the default layer.
 • When a layer is toggled on, leds shall be on.
 • Due to the Planck led limitation (one channel), Capslock and Shiftlock shall be treated as any other layer and are affecting only the default layer.
 • Layers shall be organized logicaly i.e. Navigation, Hardware control, Special Characters and accents, Numerical, operationnal and currencies, etc.

 SPECIALS
 • The keyboard input shall be lockable when you are away to prevent pets and kids to ruin your work.
 • To exit the keyboard input lock shall not be easy as tapping one or a combination of two keys but shall also not involve twisting your fingers on a three or more keys combination.
 • GUI shall be controlable through keyboard instead of mouse.
 • Hardware shall be controlable through keyboard instead of diving into preferences and options.

 NOTES
 • Movement keys assume using jkl; (; is right, jkl stay the same as in VIM).
 • tmux prefix is C-a.

 * To witness it fullfilness use it with:
 ENVIRONNEMENT
 • OSX 10.11.
 • Display rotation through fb-rotate (https://github.com/CdLbB/fb-rotate) and Automator service shortcuts.
 • Window positions and sizes through Spectacle (https://github.com/eczarny/spectacle).

 * It will someday maybe include:
 TODO
 • Hook MAX7219 chip to allow led layer reporting.
 • Finish System wide VIM mode.

 * It takes inspiration from the following sources:
 REFERENCES
 • http://stevelosh.com/blog/2012/10/a-modern-space-cadet
 • http://www.catonmat.net/blog/why-vim-uses-hjkl-as-arrow-keys/

 THANKS
 To the people behind tmk (especialy Hasu for starting it) and qmk
 and of course Jack, Jack and Jack for the Planck.

*/

#include "planck.h"
// Time for tap to become an hold.
#define TAPPING_TERM 100
// Number of taps to toggle layer or mod
#define TAPPING_TOGGLE 2
enum layers
{
	L_QW = 0, L_BE, L_CO, L_WO, L_DV, L_NUM, L_NAV, L_CHA, L_KLK, L_HRD, L_VNM, L_VVM,
	L_TOTAL, // used for iteration to turn off layers
};
enum function_id
{
	_LH = 0, _RH, _LC, _RC, _LS, _RS, _LA, _RA, _LG, _RG, // modifiers
	_NUM, _NUMT, _NAV, _NAVT, _CHA, _CHAT, _KLK, _KUL, _SLK, _CLK, _HRD, _HRDT, _ESC, // layers
	_VNM, _VNMT, _VNAT, _VVMT, // vim modes
	_TOTAL,
};
enum keys_actions
{
	_OFF = 0, _LAYER_MOMENTARY, _MOD_MOMENTARY, _MOD_TOGGLE, _LAYER_TOGGLE, _CAPSLOCK_TOGGLE, _ESCAPE_LAYER,
};
enum _macros
{
	I_PAR = 0, I_BRA, I_ACC, V_GWB, V_GWE, V_GBW, V_RPL, V_SWD, V_SLN,
};
#define LC(kc) ACTION_MODS_KEY(MOD_LCTL, KC_##kc)
#define LA(kc) ACTION_MODS_KEY(MOD_LALT, KC_##kc)
#define LS(kc) ACTION_MODS_KEY(MOD_LSFT, KC_##kc)
#define LG(kc) ACTION_MODS_KEY(MOD_LGUI, KC_##kc)
#define LAL(kc) ACTION_MODS_KEY(MOD_LCTL|MOD_LALT|MOD_LSFT|MOD_LGUI, KC_##kc)
#define LSA(kc) ACTION_MODS_KEY(MOD_LSFT|MOD_LALT, KC_##kc)
#define LSG(kc) ACTION_MODS_KEY(MOD_LSFT|MOD_LGUI, KC_##kc)
#define TRNS KC_TRNS
#define MS_AC0 KC_MS_ACCEL0
#define KC_PGD KC_PGDOWN
#define KC_PGU KC_PGUP
#define KC_EUR LSA(2)
#define KP_AST KC_KP_ASTERISK
#define KC_ KC_NO
// common commands definitions
#define ZOOM_IN LSG(EQL)
#define ZOOM_OUT LG(MINS)

static uint8_t a_type, a_action, tap_mods, tap_key;
static uint8_t backlight_on = 0;
static bool caps_on = false;
static bool caps_set = false;
static bool caps_mom = false;
static uint8_t mods_on = 0;
static uint8_t active_layer = 0;
static bool vim_mode = false;
static void all_layers_off(void);
static void tap_caps(void);
static void de_caps(void);
static void de_mods(void);
static void backlight_check(void);

backlight_config_t backlight_config;

const uint32_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM =
{
	[L_QW] = {
		/* Qwerty
		 * .---------------------------------------------------------------------------------------------------.
		 * |Tab Hyper|   q   |   w   |   e   |   r   |   t   |   y   |   u   |   i   |   o   |   p   |BSP Hyper|
		 * |---------------------------------------------------------------------------------------------------|
		 * |Esc  Ctrl|   a   |   s   |   d   |   f   |   g   |   h   |   j   |   k   |   l   |   ;   |Ent  Ctrl|
		 * |---------------------------------------------------------------------------------------------------|
		 * |(  Shift |   z   |   x   |   c   |   v   |   b   |   n   |   m   |   ,   |   .   |   /   |)  Shift |
		 * |---------------------------------------------------------------------------------------------------|
		 * | Option  | « CMD |   »   |   "   | ' NUM |SPC LS | = CHA | - HRD |   \   |   [   | ] CMD | Option  |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{F(_LH),  KC_Q,  KC_W,    KC_E,      KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    F(_RH)},
		{F(_ESC), KC_A,  KC_S,    KC_D,      KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, F(_RC)},
		{F(_LS),  KC_Z,  KC_X,    KC_C,      KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, F(_RS)},
		{F(_LA),  F(_LG),LSA(BSLS),LS(QUOTE),F(_NUM),F(_NAV),F(_CHA),F(_HRD),KC_BSLS, KC_LBRC, F(_RG),  F(_RA)}
	},
	[L_NUM] = {
		/* numbers currencies and calculation
		 * .---------------------------------------------------------------------------------------------------.
		 * |         |   •   |   •   |   €   |   •   |   •   |   \   |   7   |   8   |   9   |   £   |capslock |
		 * |---------------------------------------------------------------------------------------------------|
		 * |ExitLayer|   •   |   •   |   •   |   •   |   •   |   ±   |   4   |   5   |   6   |   $   |LayerLock|
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |   •   |   •   |   ¢   |   •   |   •   |   ≠   |   1   |   2   |   3   |   /   |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |   ‹   |   ›   |       |       |       |       |       |   0   |       |       |         |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_TRNS, KC_,       KC_,     KC_EUR,  KC_,     KC_,     LA(Y),   KC_7,  KC_8, KC_9,  LA(3),   F(_CLK)},
		{F(_ESC), UC(0x0061),UC(0x0FD8),UC(0x28E3),UC(0x60a0),KC_, KP_AST,  KC_4,  KC_5, KC_6,  LS(4),F(_NUMT)},
		{KC_TRNS, KC_, KC_, KC_,     LA(4),   KC_,     LSA(4),  KC_1,    KC_2, KC_3,    KC_SLSH, KC_TRNS},
		{KC_TRNS, LSA(3), LSA(4), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_0, KC_TRNS, KC_TRNS, KC_TRNS}
	},
	[L_CHA] = {
		/* additional characters, accents
		 * .---------------------------------------------------------------------------------------------------.
		 * |shiftlk  |       |       |       |   ®   |   ™   |   ¡   |   &   |   *   |       |   §   |capslock |
		 * |---------------------------------------------------------------------------------------------------|
		 * |ExitLayer|   ¨   |   ˆ   |   `   |   ´   |   æ   |   •   |   ~   |   %   |   ^   |   $   |LayerLock|
		 * |---------------------------------------------------------------------------------------------------|
		 * |  ( | )  |   ∞   |   •   |   ç   |   •   |   œ   |   ª   |   !   |   @   |   #   |   ¶   |  ( | )  |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       |       |       |       |       |       |       |       | [ | ] | { | } |         |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{F(_SLK), KC_,   KC_,    LA(W),   LA(R),  LA(2),     LA(1),  LS(7),   LS(8),  KC_,     LA(6),  F(_CLK)},
		{F(_ESC), LA(U), LA(I),  LA(GRV), LA(E),  LA(QUOTE), LA(8),  KC_TILD, LS(5),  LS(6),   LS(4), F(_CHAT)},
		{M(I_PAR),LA(5), KC_,    LA(C),   KC_,    LA(Q),     LA(9),  LS(1),   LS(2),  LS(3),   LA(7), M(I_PAR)},
		{KC_TRNS, KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS,M(I_BRA),M(I_ACC), KC_TRNS}
	},
	[L_NAV] = {
		/* navigation and GUI
		 * .---------------------------------------------------------------------------------------------------.
		 * |         | Dock  | Menu  |TabPrev|TabNext| Tools | Float | Undo  | Copy  |  Cut  | Paste |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |ExitLayer|AppPrev|AppNext|WndPrev|WndNext|StsMenu|       | Down  |  Up   | Left  | Right |LayerLock|
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       |       |       |       |       |       | PageD | PageU | Home  |  End  |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         | Zoom- | Zoom+ |       |       |       |       |       |       |       |       |         |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_TRNS, LC(F3), LC(F2),LSG(LBRC),LSG(RBRC),LC(F5),  LC(F6), LG(Z),  LG(X), LG(C),  LG(V),   KC_TRNS},
		{F(_ESC), LSG(TAB),LG(TAB),LSA(TAB),LA(TAB), LC(F8),  KC_,    KC_DOWN,KC_UP, KC_LEFT,KC_RIGHT,F(_NAVT)},
		{KC_TRNS, KC_,    KC_,   KC_,      KC_,      KC_,     KC_,    KC_PGD, KC_PGU,KC_HOME,KC_END,  KC_TRNS},
		{KC_TRNS,ZOOM_OUT,ZOOM_IN,KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS}
	},
	[L_HRD] = {
		/* hardware control and mouse emulation
		 * .---------------------------------------------------------------------------------------------------.
		 * |         | DR180 | DR360 | DR270 | DR90  |       | VolMu |       |       |       |       |KbdReset |
		 * |---------------------------------------------------------------------------------------------------|
		 * |ExitLayer| <F15> | BLinc | MBtn3 | MBtn1 | MBtn2 | VolUp | MDown | MUp   | MLeft | MRight|LayerLock|
		 * |---------------------------------------------------------------------------------------------------|
		 * |         | <F14> | BLdec |       |       |       | VolDn | PageD | PageU | Home  | End   |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       | BLtog |       |       |       |       |       |       |       |       |  Power  |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{F(_KLK), LAL(1), LAL(3), LAL(2), LAL(9), KC_TRNS, KC_MUTE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,RESET},
		{F(_ESC), KC_F15, BL_INC, KC_BTN3,KC_BTN1,KC_BTN2, KC_VOLU, KC_MS_D, KC_MS_U, KC_MS_L,KC_MS_R,F(_HRDT)},
		{KC_TRNS, KC_F14, BL_DEC, BL_STEP,KC_TRNS,KC_TRNS, KC_VOLD, KC_PGD,  KC_PGU,  KC_HOME, KC_END, KC_TRNS},
		{KC_TRNS, KC_TRNS,BL_TOGG,KC_TRNS,KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PWR}
	},
	[L_KLK] = {
		/* lock keyboard
		 * .---------------------------------------------------------------------------------------------------.
		 * |Unlock   | note : triple tap to unlock                                                             |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_,     KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_},
		{F(_KUL), KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_},
		{KC_,     KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_},
		{KC_,     KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_, KC_}
	},
	[L_VNM] = {
		/* VIM normal mode
		 * .---------------------------------------------------------------------------------------------------.
		 * |         |       | WordB | WordE | Repla |       | Yank  | Undo  | Exit  | ExNLn | Paste |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         | ExAft | ExCut | Delet |       | Go    |       | Down  |  Up   | Left  | Right |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       | Cut   |       | Visu  | BWord | SrNxt |       |       |       | Ex+Sr |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       | ParUp |       | Prev  | Right | NewLn | Up    | Down  | ParDn |       |         |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_, KC_,    M(V_GWB), M(V_GWE), KC_,    KC_,    LG(C), LG(Z),   F(_VNMT), KC_,   LG(V),    KC_},
		{KC_, F(_VNAT), KC_,    KC_,    KC_,    KC_,    KC_, KC_DOWN, KC_UP,    KC_LEFT, KC_RIGHT, KC_},
		{KC_, KC_,    LG(X),    KC_,    F(_VVMT), M(V_GBW), KC_, KC_,   KC_PGU,   KC_HOME, KC_END,   KC_},
		{KC_, KC_,    KC_,    KC_,    KC_,    KC_,    KC_, KC_,   KC_,    KC_,   KC_,    KC_}
	},
	[L_VVM] = {
		/* VIM visual mode
		 * .---------------------------------------------------------------------------------------------------.
		 * |         | <F1>  | WordB | WordE |Replace|       | Yank  |LowerCs|       | ExNLn | Paste |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |Exit     | ExAft | ExCut | Delet |       | Go    |       | Down  | Up    | Left  | Right |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       | Cut   | <F11> | LyrTG | BWord | SrNxt |       |       |       | Ex+Sr |         |
		 * |---------------------------------------------------------------------------------------------------|
		 * |         |       | ParUp |       | Prev  | Right | NewLn | Up    | Down  | ParDn |       |         |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_,      KC_, M(V_GWB), M(V_GWE),KC_, KC_,    LG(C), LG(Z),   KC_,  KC_,   LG(V),    KC_},
		{F(_VVMT), KC_, KC_,    KC_,   KC_, KC_,    KC_, KC_DOWN, KC_UP,  KC_LEFT, KC_RIGHT, KC_},
		{KC_,      KC_, LG(X),    KC_,   KC_, M(V_GBW), KC_, KC_,   KC_PGU, KC_HOME, KC_END,   KC_},
		{KC_,      KC_, KC_,    KC_,   KC_, KC_,    KC_, KC_,   KC_,  KC_,   KC_,    KC_}
	},
	[L_BE] = {
		/* Bépo
		 * .---------------------------------------------------------------------------------------------------.
		 * |Tab Hyper|   b   |   é   |   p   |   o   |   è   |   ˆ   |   v   |   d   |   l   |   z   |BSP Hyper|
		 * |---------------------------------------------------------------------------------------------------|
		 * |Esc Ctrl |   a   |   u   |   i   |   e   |   ,   |   c   |   t   |   s   |   r   |   n   |Ent Ctrl |
		 * |---------------------------------------------------------------------------------------------------|
		 * |(  Shift |   ê   |   à   |   y   |   x   |   .   |   k   |   '   |   q   |   g   |   h   |) Shift |
		 * |---------------------------------------------------------------------------------------------------|
		 * | Option  |   @   |   w   | ç HRD | f NUM | SP LS | = GUI | m CHA |   [   |   ]   |   \   | Option  |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{F(_LH), KC_B,  KC_,   KC_P,    KC_O,    KC_,    KC_,     KC_V,   KC_D,    KC_L,    KC_Z, F(_RH)},
		{F(_LC), KC_A,  KC_U,  KC_I,    KC_E,    KC_COMM, KC_C,   KC_T,   KC_S,    KC_R,    KC_N, F(_RC)},
		{F(_LS), KC_,   KC_,   KC_Y,    KC_X,    KC_DOT,  KC_K,   KC_,    KC_Q,    KC_G,    KC_H, F(_RS)},
		{F(_LA), KC_W,  KC_, F(_HRD), F(_NUM), F(_NAV), F(_RG), F(_CHA), KC_LBRC, KC_RBRC, KC_BSLS, F(_RA)}
	},
	[L_CO] = {
		/* Colemak
		 * .---------------------------------------------------------------------------------------------------.
		 * |Tab Hypr  |  q  |  w  |  f  |  p  |  g  |  j  |  l  |  u  |  y  |  ;  |BSpace Hypr|
		 * |---------------------------------------------------------------------------------------------------|
		 * |Esc Ctrl  |  a  |  r  |  s  |  t  |  d  |  h  |  n  |  e  |  i  |  o  |Enter  Ctl |
		 * |---------------------------------------------------------------------------------------------------|
		 * |(  Shift  |  z  |  x  |  c  |  v  |  b  |  k  |  m  |  ,  |  .  |  /  |)    Shift |
		 * |---------------------------------------------------------------------------------------------------|
		 * |Option    |     |     |« HRD|' NUM|SP LS|= GUI|- CHA|  [  |  ]  |  \  |Option     |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_TAB,  KC_Q,  KC_W, KC_F,    KC_P,    KC_G,    KC_J,   KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC},
		{KC_ESC,  KC_A,  KC_R, KC_S,    KC_T,    KC_D,    KC_H,   KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT},
		{KC_LSFT, KC_Z,  KC_X, KC_C,    KC_V,    KC_B,    KC_K,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },
		{F(_LA),  KC_,   KC_,  F(_HRD), F(_NUM), F(_NAV), F(_RG), F(_CHA), KC_LBRC, KC_RBRC, KC_BSLS, F(_RA)}
	},
	[L_WO] = {
		/* Workman
		 * .---------------------------------------------------------------------------------------------------.
		 * |Tab Hypr  |  q  |  d  |  r  |  w  |  b  |  j  |  f  |  u  |  p  |  ;  |BSpace Hypr|
		 * |---------------------------------------------------------------------------------------------------|
		 * |Esc Ctrl  |  a  |  s  |  h  |  t  |  g  |  y  |  n  |  e  |  o  |  i  |Enter  Ctl |
		 * |---------------------------------------------------------------------------------------------------|
		 * |(  Shift  |  z  |  x  |  m  |  c  |  v  |  k  |  l  |  ,  |  .  |  /  |)    Shift |
		 * |---------------------------------------------------------------------------------------------------|
		 * |Option    |     |     |« HRD|' NUM|SP LS|= GUI|- CHA|  [  |  ]  |  \  |Option     |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{F(_LH), KC_Q,   KC_D,  KC_R,    KC_W,    KC_B,    KC_J,   KC_F,   KC_U,    KC_P,    KC_SCLN, F(_RH)},
		{F(_LC), KC_A,   KC_S,  KC_H,    KC_T,    KC_G,    KC_Y,   KC_N,   KC_E,    KC_O,    KC_I,    F(_RC)},
		{F(_LS), KC_Z,   KC_X,  KC_M,    KC_C,    KC_V,    KC_K,   KC_L,   KC_COMM, KC_DOT,  KC_SLSH, F(_RS)},
		{F(_LA), KC_,  KC_, F(_HRD), F(_NUM), F(_NAV), F(_RG), F(_CHA),KC_LBRC, KC_RBRC, KC_BSLS, F(_RA)}
	},
	[L_DV] = {
		/* Dvorak
		 * .---------------------------------------------------------------------------------------------------.
		 * |Tab Hypr  |  "  |  ,  |  .  |  p  |  y  |  f  |  g  |  c  |  r  |  l  |BSpace Hypr|
		 * |---------------------------------------------------------------------------------------------------|
		 * |Esc Ctrl  |  a  |  o  |  e  |  u  |  i  |  d  |  h  |  t  |  n  |  s  |Enter  Ctl |
		 * |---------------------------------------------------------------------------------------------------|
		 * |(  Shift  |  ;  |  q  |  j  |  k  |  x  |  b  |  m  |  w  |  v  |  z  |)    Shift |
		 * |---------------------------------------------------------------------------------------------------|
		 * |Option    |  •  |  •  |« HRD|' NUM|SP LS|= GUI|- CHA|  [  |  ]  |  \  |Option     |
		 * '---------------------------------------------------------------------------------------------------'
		 */
		{KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,   KC_Y,   KC_F,  KC_G,   KC_C,    KC_R,    KC_L,    KC_BSPC},
		{KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,   KC_I,   KC_D,  KC_H,   KC_T,    KC_N,    KC_S,    KC_SLSH},
		{KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,   KC_X,   KC_B,  KC_M,   KC_W,    KC_V,    KC_Z,    KC_ENT },
		{F(_LA),  KC_,     KC_,     F(_HRD), F(_NUM),F(_NAV),F(_RG),F(_CHA),KC_LBRC, KC_RBRC, KC_BSLS, F(_RA)}
	}
};
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
	// MACRODOWN only works in this function
	switch(id) {
		case I_PAR:
			if (record->event.pressed) {
				register_mods(MOD_BIT(KC_LSFT));
				register_code(KC_9);
				unregister_code(KC_9);
				register_code(KC_0);
				unregister_code(KC_0);
				unregister_mods(MOD_BIT(KC_LSFT));
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
			}
			break;
		case I_BRA:
			if (record->event.pressed) {
				register_code(KC_LBRC);
				unregister_code(KC_LBRC);
				register_code(KC_RBRC);
				unregister_code(KC_RBRC);
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
			}
			break;
		case I_ACC:
			if (record->event.pressed) {
				register_mods(MOD_BIT(KC_LSFT));
				register_code(KC_LBRC);
				unregister_code(KC_LBRC);
				register_code(KC_RBRC);
				unregister_code(KC_RBRC);
				unregister_mods(MOD_BIT(KC_LSFT));
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
			}
			break;
		case V_GWB: // OSX goto word begining
			if (record->event.pressed) {
				register_mods(MOD_BIT(KC_LALT));
				register_code(KC_RIGHT);
				unregister_code(KC_RIGHT);
				register_code(KC_RIGHT);
				unregister_code(KC_RIGHT);
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
				unregister_mods(MOD_BIT(KC_LALT));
			}
			break;
		case V_GWE: // OSX goto word end
			if (record->event.pressed) {
				set_oneshot_mods(MOD_BIT(KC_LALT));
				register_code(KC_RIGHT);
				unregister_code(KC_RIGHT);
			}
			break;
		case V_GBW: // OSX backward goto to word begining
			if (record->event.pressed) {
				set_oneshot_mods(MOD_BIT(KC_LALT));
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
			}
			break;
		case V_SWD: // OSX select word
			if (record->event.pressed) {
				register_mods(MOD_BIT(KC_LALT));
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
				set_oneshot_mods(MOD_BIT(KC_LSFT));
				register_code(KC_RIGHT);
				unregister_code(KC_RIGHT);
				unregister_mods(MOD_BIT(KC_LALT));
			}
			break;
		case V_SLN: // OSX select line
			if (record->event.pressed) {
				register_mods(MOD_BIT(KC_LGUI));
				register_code(KC_LEFT);
				unregister_code(KC_LEFT);
				set_oneshot_mods(MOD_BIT(KC_LSFT));
				register_code(KC_RIGHT);
				unregister_code(KC_RIGHT);
				unregister_mods(MOD_BIT(KC_LGUI));
			}
			break;
	}
	return MACRO_NONE;
};
void action_process(keyrecord_t *record, uint8_t ht, uint8_t hm, uint8_t tm, uint8_t tk)
{
	keyevent_t event = record->event;
	if (event.pressed) {
		dprintf(">>>>>>>> pressed\n");
		if (record->tap.count>0) {
			if (ht == _CAPSLOCK_TOGGLE) {
				de_mods();
				caps_on = caps_on?false:true;
				caps_set = true;
				if (caps_on) {
					backlight_on = 1;
					if (active_layer != 0) {
						all_layers_off();
						active_layer = 0;
					}
				} else {
					backlight_on = 0;
				}
			}
			if (ht == _MOD_TOGGLE) {
				de_caps();
				if (mods_on == hm) {
					de_mods();
				} else {
					if (active_layer != 0) {
						all_layers_off();
						active_layer = 0;
					}
					register_mods(hm);
					mods_on = hm;
					backlight_on = 1;
				}
			} else if ((ht != _ESCAPE_LAYER || ((!caps_on && !mods_on) && active_layer == 0)) && ht != _LAYER_TOGGLE) {
				add_weak_mods(tm); send_keyboard_report();
				register_code(tk);
			}
		} else {
			switch (ht) {
				case _LAYER_MOMENTARY:
					all_layers_off();
					if (caps_on) {
						tap_caps();
						caps_mom = true;
					} else if (mods_on > 0) {
						unregister_mods(mods_on);
					}
					layer_on(hm); backlight_on = 1;
					break;
				case _MOD_MOMENTARY:
				case _ESCAPE_LAYER:
					register_mods(hm);
					break;
			}
		}
	} else {
		dprintf("released >>>>>>>>\n");
		if (ht == _LAYER_TOGGLE && hm != 0 && record->tap.count > 0) {
			if (hm == L_VVM) {
				if (active_layer == L_VVM) {
					unregister_mods(MOD_BIT(KC_LSFT));
					layer_off(L_VVM);
					layer_on(L_VNM); active_layer = L_VNM;
				} else {
					register_mods(MOD_BIT(KC_LSFT));
					layer_on(hm); active_layer = L_VVM;
				}
			} else {
				if (hm == L_VNM) vim_mode = true;
				if (hm != active_layer) {
					all_layers_off();
					de_caps();
					de_mods();
					active_layer = hm; backlight_on = 1;
					//if (active_layer == L_KLK) breathing_pulse();
				} else {
					add_weak_mods(tm); send_keyboard_report();
					register_code(tk);
					clear_weak_mods(); send_keyboard_report();
					unregister_code(tk);
				}
			}
			record->tap.count = 0;
		} else if (ht == _ESCAPE_LAYER && record->tap.count > 0) {
			all_layers_off();
			de_caps();
			de_mods();
			active_layer = 0;
			backlight_on = 0;
			record->tap.count = 0;
		} else if (ht != _LAYER_TOGGLE && record->tap.count > 0) {
			clear_weak_mods(); send_keyboard_report();
			unregister_code(tk);
			record->tap.count = 0;
		} else if (ht == _LAYER_MOMENTARY) {
			all_layers_off();
			if (caps_mom || (caps_on && !caps_set)) {
				tap_caps();
				caps_mom = false;
			}
			caps_set = false;
			if (active_layer == 0) {
				backlight_on = 0;
			} else {
				layer_on(active_layer);
				backlight_on = 1;
			}
			if (mods_on != 0) {
				register_mods(mods_on);
				backlight_on = 1;
			}
			if (caps_on) backlight_on = 1;
		} else if (ht == _MOD_MOMENTARY || ht == _ESCAPE_LAYER) {
			unregister_mods(hm);
		} else if (ht == _LAYER_TOGGLE && (hm == 0 && record->tap.count > 2) && hm != L_VVM) {
			if (hm == L_VNM) vim_mode = false;
			all_layers_off();
			active_layer = 0;
			de_caps();
			de_mods();
			//breathing_disable();
			backlight_on = 0;
			record->tap.count = 0;
		}
	}
	backlight_check();
}
void all_layers_off()
{
	for (int i=0;i<L_TOTAL;i++) {
		layer_off(i);
	}
}
void tap_caps()
{
	register_code(KC_CAPS);
	unregister_code(KC_CAPS);
}
void de_caps()
{
	if (caps_on) {
		tap_caps();
		caps_on = false;
	}
}
void de_mods()
{
	if (mods_on != 0) {
		unregister_mods(mods_on);
		mods_on = 0;
	}
}
void backlight_check() {
	if (backlight_on != backlight_config.enable) {
		backlight_toggle();
	}
}
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
	a_action = NULL; tap_mods = NULL; tap_key = NULL;
	switch (id) {
		case _LH:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_LSFT)|MOD_BIT(KC_LALT)|MOD_BIT(KC_LGUI)|MOD_BIT(KC_LCTL); // layer id or mods
			tap_key=KC_TAB; // single tap
			break;
		case _RH:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_RSFT)|MOD_BIT(KC_RALT)|MOD_BIT(KC_RGUI)|MOD_BIT(KC_RCTL); // layer id or mods
			tap_key=KC_BSPC; // single tap
			break;
		case _LC:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_LCTL); // layer id or mods
			tap_key=KC_ESC; // single tap
			break;
		case _RC:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_RCTL); // layer id or mods
			tap_key=KC_ENTER; // single tap
			break;
		case _LS:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_LSFT); // layer id or mods
			tap_mods=MOD_BIT(KC_RSFT);// single tap mods
			tap_key=KC_9; // single tap
			break;
		case _RS:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_RSFT); // layer id or mods
			tap_mods=MOD_BIT(KC_LSFT); // single tap mods
			tap_key=KC_0; // single tap
			break;
		case _LA:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_LALT); // layer id or mods
			break;
		case _RA:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_RALT); // layer id or mods
			break;
		case _LG:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_LGUI); // layer id or mods
			tap_mods=MOD_BIT(KC_RALT); // single tap mods
			tap_key=KC_BSLS; // single tap
			break;
		case _RG:
			a_type=_MOD_MOMENTARY;
			a_action=MOD_BIT(KC_RGUI); // layer id or mods
			tap_key=KC_RBRC; // single tap
			break;
		case _NUM:
			a_type=_LAYER_MOMENTARY;
			a_action=L_NUM; // layer id or mods
			tap_key=KC_QUOTE; // single tap
			break;
		case _NUMT:
			a_type=_LAYER_TOGGLE;
			a_action=L_NUM;
			tap_key=KC_ENTER; // single tap
			break;
		case _NAV:
			a_type=_LAYER_MOMENTARY;
			a_action=L_NAV; // layer id or mods
			tap_key=KC_SPC; // single tap
			break;
		case _NAVT:
			a_type=_LAYER_TOGGLE;
			a_action=L_NAV;
			tap_key=KC_ENTER; // single tap
			break;
		case _CHA:
			a_type=_LAYER_MOMENTARY;
			a_action=L_CHA; // layer id or mods
			tap_key=KC_EQL; // single tap
			break;
		case _CHAT:
			a_type=_LAYER_TOGGLE;
			a_action=L_CHA;
			tap_key=KC_ENTER; // single tap
			break;
		case _KLK:
			a_type=_LAYER_TOGGLE;
			a_action=L_KLK; // layer id or mods
			break;
		case _KUL:
			a_type=_LAYER_TOGGLE;
			a_action=L_QW; // layer id or mods
			break;
		case _SLK:
			a_type=_MOD_TOGGLE;
			a_action=MOD_BIT(KC_LSFT); // layer id or mods
			break;
		case _CLK:
			a_type=_CAPSLOCK_TOGGLE;
			tap_key=KC_CAPS; // single tap
			break;
		case _HRD:
			a_type=_LAYER_MOMENTARY;
			a_action=L_HRD; // layer id or mods
			tap_key=KC_MINS; // single tap
			break;
		case _HRDT:
			a_type=_LAYER_TOGGLE;
			a_action=L_HRD; // layer id or mods
			tap_key=KC_ENTER; // single tap
			break;
		case _ESC:
			a_type=_ESCAPE_LAYER;
			a_action=MOD_BIT(KC_LCTL); // mod
			tap_key=KC_ESC; // single tap
			break;
		case _VNMT:
			a_type=_LAYER_TOGGLE;
			a_action=L_VNM; // layer id or mods
			break;
		case _VNAT:
			a_type=_LAYER_TOGGLE;
			a_action=L_VNM; // layer id or mods
			break;
		case _VVMT:
			a_type=_LAYER_TOGGLE;
			a_action=L_VVM; // layer id or mods
			break;
		default:
			break;
	}
	action_process(record, a_type, a_action, tap_mods, tap_key);
}
const uint16_t fn_actions[] PROGMEM =
{
	[_LH] = ACTION_FUNCTION_TAP(_LH),
	[_RH] = ACTION_FUNCTION_TAP(_RH),
	[_LC] = ACTION_FUNCTION_TAP(_LC),
	[_RC] = ACTION_FUNCTION_TAP(_RC),
	[_LS] = ACTION_FUNCTION_TAP(_LS),
	[_RS] = ACTION_FUNCTION_TAP(_RS),
	[_LA] = ACTION_FUNCTION_TAP(_LA),
	[_RA] = ACTION_FUNCTION_TAP(_RA),
	[_LG] = ACTION_FUNCTION_TAP(_LG),
	[_RG] = ACTION_FUNCTION_TAP(_RG),
	[_NUM] = ACTION_FUNCTION_TAP(_NUM),
	[_NUMT] = ACTION_FUNCTION_TAP(_NUMT),
	[_NAV] = ACTION_FUNCTION_TAP(_NAV),
	[_NAVT] = ACTION_FUNCTION_TAP(_NAVT),
	[_CHA] = ACTION_FUNCTION_TAP(_CHA),
	[_CHAT] = ACTION_FUNCTION_TAP(_CHAT),
	[_KLK] = ACTION_FUNCTION_TAP(_KLK),
	[_SLK] = ACTION_FUNCTION_TAP(_SLK),
	[_CLK] = ACTION_FUNCTION_TAP(_CLK),
	[_KUL] = ACTION_FUNCTION_TAP(_KUL),
	[_HRD] = ACTION_FUNCTION_TAP(_HRD),
	[_HRDT] = ACTION_FUNCTION_TAP(_HRDT),
	[_ESC] = ACTION_FUNCTION_TAP(_ESC),
	[_VNM] = ACTION_FUNCTION_TAP(_VNM),
	[_VNMT] = ACTION_FUNCTION_TAP(_VNMT),
	[_VVMT] = ACTION_FUNCTION_TAP(_VVMT),
};
void matrix_init_user() {
	backlight_toggle(); backlight_toggle();
	backlight_check();
	set_unicode_mode(UC_OSX);
}
