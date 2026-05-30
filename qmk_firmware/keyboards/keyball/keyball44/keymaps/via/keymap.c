/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                        RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , SCRL_DVI ,                                        RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
                  QK_BOOT  , KBC_RST  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 3:
            keyball_set_scroll_mode(true);
            keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_VERTICAL);
            break;
        case 4:
            keyball_set_scroll_mode(true);
            keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_HORIZONTAL);
            break;
        default:
            keyball_set_scroll_mode(false);
            keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_VERTICAL);
            break;
    }
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

static void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    for (uint8_t i = 0; i < w; i++) {
        for (uint8_t j = 0; j < h; j++) {
            oled_write_pixel(x + i, y + j, true);
        }
    }
}

static void draw_segment(uint8_t segment) {
    // 128x32 OLEDの中央に、7セグ風の大きい数字を表示する
    const uint8_t x = 44;
    const uint8_t y = 2;
    const uint8_t w = 40;
    const uint8_t h = 28;
    const uint8_t t = 4;

    switch (segment) {
        case 0:  // 上
            draw_rect(x + t, y, w - t * 2, t);
            break;
        case 1:  // 右上
            draw_rect(x + w - t, y + t, t, h / 2 - t);
            break;
        case 2:  // 右下
            draw_rect(x + w - t, y + h / 2, t, h / 2 - t);
            break;
        case 3:  // 下
            draw_rect(x + t, y + h - t, w - t * 2, t);
            break;
        case 4:  // 左下
            draw_rect(x, y + h / 2, t, h / 2 - t);
            break;
        case 5:  // 左上
            draw_rect(x, y + t, t, h / 2 - t);
            break;
        case 6:  // 中央
            draw_rect(x + t, y + h / 2 - t / 2, w - t * 2, t);
            break;
    }
}

static void render_big_layer_number(void) {
    uint8_t layer = get_highest_layer(layer_state);

    oled_clear();

    switch (layer) {
        case 0:
            draw_segment(0);
            draw_segment(1);
            draw_segment(2);
            draw_segment(3);
            draw_segment(4);
            draw_segment(5);
            break;

        case 1:
            draw_segment(1);
            draw_segment(2);
            break;

        case 2:
            draw_segment(0);
            draw_segment(1);
            draw_segment(6);
            draw_segment(4);
            draw_segment(3);
            break;

        case 3:
            draw_segment(0);
            draw_segment(1);
            draw_segment(6);
            draw_segment(2);
            draw_segment(3);
            break;

        case 4:
            draw_segment(5);
            draw_segment(6);
            draw_segment(1);
            draw_segment(2);
            break;

        case 5:
            draw_segment(0);
            draw_segment(5);
            draw_segment(6);
            draw_segment(2);
            draw_segment(3);
            break;

        case 6:
            draw_segment(0);
            draw_segment(5);
            draw_segment(6);
            draw_segment(4);
            draw_segment(2);
            draw_segment(3);
            break;

        case 7:
            draw_segment(0);
            draw_segment(1);
            draw_segment(2);
            break;

        case 8:
            draw_segment(0);
            draw_segment(1);
            draw_segment(2);
            draw_segment(3);
            draw_segment(4);
            draw_segment(5);
            draw_segment(6);
            break;

        case 9:
            draw_segment(0);
            draw_segment(1);
            draw_segment(2);
            draw_segment(3);
            draw_segment(5);
            draw_segment(6);
            break;

        default:
            oled_set_cursor(6, 1);
            oled_write_P(PSTR("?"), false);
            break;
    }
}

// これが「ケーブルを接続していない側」のOLED表示
void oledkit_render_logo_user(void) {
    render_big_layer_number();
}

#endif