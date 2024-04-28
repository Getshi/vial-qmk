#include QMK_KEYBOARD_H


enum my_keycodes {
   KC_UALT = QK_KB_0, // add new key code starting from qmk's safe value
 };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // static uint8_t ualt_registeredkc = KC_NO;
  static bool ualt_active = false;

  switch (keycode) {
    case KC_UALT:
      // go into umlaut mode while held
      if (record->event.pressed) { // pressed
        ualt_active = true; 
      } else { // released
        ualt_active = false;
      }
      return false; // Skip all further processing of this key
      // break;
    default: // other keys
      if(ualt_active) { // umlaut mode
        // allow shift press/release to be sent in ualt mode, so uppercase letters dont depend on shift being pressed before ualt
        switch (keycode) {
          case KC_LSFT: 
            return true;
          case KC_RSFT:
            return true;
        }
        if (record->event.pressed) { // pressed any other key in umlaut mode
          bool shifted = get_mods() & MOD_MASK_SHIFT;
          switch (keycode) {
            case KC_A:
              if (shifted)
                send_unicode_string("Ä");
              else
                send_unicode_string("ä");
              break;
            case KC_O:
              if (shifted)
                send_unicode_string("Ö");
              else
                send_unicode_string("ö");
              break;
            case KC_U:
              if (shifted)
                send_unicode_string("Ü");
              else
                send_unicode_string("ü");
              break;
            case KC_S:
              send_unicode_string("ß");
              break;
            case KC_E:
              send_unicode_string("€");
              break;
            case KC_N:
              if (shifted)
                send_unicode_string("Ñ");
              else
                send_unicode_string("ñ");
              break;
            case KC_1:
              send_unicode_string("¡");
              break;
            case KC_SLASH:
              send_unicode_string("¿");
              break;
            default:
              // ualt_registeredkc = keycode;
              // register_code16(RALT(ualt_registeredkc));
              // TODO better fallbacks, and just make another dedicated physical compose key
              break;
            }
          return false;
        } else { // released any other key
          // if(ualt_registeredkc != KC_NO)
          // 	unregister_code16(RALT(ualt_registeredkc));
          // ualt_registeredkc = KC_NO;
          // TODO ALLOW SHIFT
          return false;
        }
      } else {
        // not in umlaut mode, ie any normal keypress
        return true; // do default key processing.
      }
      // break;
  }
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * ,------------------------------------------------------------------------------------------------------------------------.
 * |  ESC  |   1   |   2   |   3   |   4   |   5   ||                      ||   6   |   7   |   8   |   9   |   0   |  DEL  |
 * |-----------------------------------------------| PSCR  ||      || BSPC  |-----------------------------------------------|
 * |  GRV  |   Q   |   W   |   E   |   R   |   T   ||                      ||   Y   |   U   |   I   |   O   |   P   | BSLS  |
 * |-----------------------------------------------| MINS  ||      ||  EQL  |-----------------------------------------------|
 * |  TAB  |   A   |   S   |   D   |   F   |   G   ||                      ||   H   |   J   |   K   |   L   | SCLN  | QUOT  |
 * |-----------------------------------------------| LBRC  ||      || RBRC  |-----------------------------------------------|
 * | LSFT  |   Z   |   X   |   C   |   V   |   B   ||                      ||   N   |   M   | COMM  |  DOT  | SLSH  | RSFT  |
 * |------------------------------------------------------------------------------------------------------------------------|
 * | LCTL  | LGUI  | LALT  | LALT  ||                                                      || LEFT  | DOWN  |  UP   | RGHT  |
 * ,------------------------------------------------------------------------------------------------------------------------.
 *         ,--------------------------,       ,--------------------------,
 *         || LT(2 _ KC_SPC) |  LT   ||       || UALT  | LT(3 _ KC_ENT) ||
 * |----------------------------------|       |---------------------------------|
 * || MO(1) | LT(2 _ KC_SPC) |  SPC  ||       || MO(1) | LT(3 _ KC_ENT) | MO(5) |
 * ,----------------------------------,       ,---------------------------------,
 */
  [0] = LAYOUT_4key_2u_inner(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_PSCR, KC_BSPC, KC_6, KC_7, KC_8, KC_9, KC_0, KC_DEL, KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_MINS, KC_EQL, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_LBRC, KC_RBRC, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LT, KC_UALT, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_LALT, MO(1), LT(2,KC_SPC), KC_SPC, MO(1), LT(3,KC_ENT), MO(5), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),

/*
 * ,------------------------------------------------------------------------------------------------------------------------.
 * |   _   |  F1   |  F2   |  F3   |  F4   |  F5   ||                      ||  F6   |  F7   |  F8   |  F9   |  F10  |  F11  |
 * |-----------------------------------------------|       ||      ||   _   |-----------------------------------------------|
 * |       | EXLM  |  AT   | HASH  |  DLR  | PERC  ||                      || CIRC  | AMPR  | ASTR  | LPRN  | RPRN  |  F12  |
 * |-----------------------------------------------|       ||      ||       |-----------------------------------------------|
 * |   _   |   1   |   2   |   3   |   4   |   5   ||                      ||   6   |   7   |   8   |   9   |   0   |       |
 * |-----------------------------------------------|       ||      ||       |-----------------------------------------------|
 * |   _   |   Y   |       |       |       |       ||                      ||       |       |       |       |       |   _   |
 * |------------------------------------------------------------------------------------------------------------------------|
 * |   _   |   _   |   _   |   _   ||                                                      ||   _   |   _   |   _   |   _   |
 * ,------------------------------------------------------------------------------------------------------------------------.
 *         ,-----------------,       ,-----------------,
 *         ||  ENT  |       ||       || RALT  |   _   ||
 * |-------------------------|       |------------------------|
 * ||   _   |  ENT  | MO(4) ||       ||   _   |   _   |   _   |
 * ,-------------------------,       ,------------------------,
 */
  [1] = LAYOUT_4key_2u_inner(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_TRNS, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_NO, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_NO, KC_NO, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_F12, KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_NO, KC_NO, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_TRNS, KC_Y, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_ENT, MO(4), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  [2] = LAYOUT_4key_2u_inner(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_DEL, KC_TILD, KC_EXLM, KC_CIRC, KC_AMPR, KC_LBRC, KC_UNDS, KC_NO, KC_NO, KC_NO, KC_RBRC, KC_BSLS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_MINS, KC_PLUS, KC_EQL, KC_LPRN, KC_LT, KC_SPC, KC_NO, KC_GT, KC_RPRN, KC_COLN, KC_QUES, KC_DQUO, KC_QUOT, KC_TRNS, KC_SLSH, KC_ASTR, KC_PIPE, KC_LCBR, KC_PERC, KC_NO, KC_NO, KC_HASH, KC_RCBR, KC_AT, KC_DLR, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
  [3] = LAYOUT_4key_2u_inner(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_DEL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PGUP, KC_HOME, KC_UP, KC_END, KC_INS, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_WH_U, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R, KC_NO),
/*
 * ,------------------------------------------------------------------------------------------------------------------------.
 * |       |       |       |       |       |       ||                      ||       |       |       |       |       |       |
 * |-----------------------------------------------|       ||      ||       |-----------------------------------------------|
 * |       | QK_RBT |       |       |       |       ||                      ||       | QK_UNICODE_MODE_NEXT |       |       |       |       |
 * |-----------------------------------------------|       ||      ||       |-----------------------------------------------|
 * |       |       |       |       |       |       ||                      ||       |       |       |       |       |       |
 * |-----------------------------------------------|       ||      ||       |-----------------------------------------------|
 * |       |       |       |       |       |       ||                      ||       |       |       |       |       |       |
 * |------------------------------------------------------------------------------------------------------------------------|
 * |       |       |       |       ||                                                      ||       |       |       |       |
 * ,------------------------------------------------------------------------------------------------------------------------.
 *         ,-----------------,       ,-----------------,
 *         ||   _   |       ||       ||       |   _   ||
 * |-------------------------|       |------------------------|
 * ||   _   |   _   |   _   ||       ||   _   |   _   |   _   |
 * ,-------------------------,       ,------------------------,
 */
  [4] = LAYOUT_4key_2u_inner(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_RBT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_UNICODE_MODE_NEXT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO),
  [5] = LAYOUT_4key_2u_inner(KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_BSPC, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_DEL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LNUM, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_F12, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_P1, KC_P2, KC_P3, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(4), MO(4), KC_TRNS, KC_TRNS, KC_P0, KC_PDOT, KC_COMM, KC_NO)
};
