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
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
              _______, _______, _______,        _______, _______,                   _______, _______, _______, _______, _______
  ),

  [4] = LAYOUT_universal(
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
              _______, _______, _______,        _______, _______,                   _______, _______, _______, _______, _______
  ),

  [5] = LAYOUT_universal(
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

#    ifdef SPLIT_KEYBOARD
#        include "transactions.h"
#    endif

typedef struct {
    uint8_t cpi;
    uint8_t scr;
} oled_sync_t;

static oled_sync_t oled_sync = {
    .cpi = 5,
    .scr = 5,
};

#    ifdef SPLIT_KEYBOARD
static void oled_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(oled_sync_t)) {
        const oled_sync_t *recv = (const oled_sync_t *)in_data;
        oled_sync.cpi = recv->cpi;
        oled_sync.scr = recv->scr;
    }
}
#    endif

void keyboard_post_init_user(void) {
#    ifdef SPLIT_KEYBOARD
    if (!is_keyboard_master()) {
        transaction_register_rpc(OLED_SYNC, oled_sync_handler);
    }

    // SCRの実動作初期値はmaster側で管理する。
    if (is_keyboard_master()) {
        keyball_set_scroll_div(5);
    }
#    else
    keyball_set_scroll_div(5);
#    endif
}

void matrix_scan_user(void) {
#    ifdef SPLIT_KEYBOARD
    static uint32_t last_sync = 0;

    // OLED同期だけをmaster側に限定する。
    // slave側ではreturnしない。
    // トラックボール搭載側がslaveの場合、ここでreturnすると
    // Keyball側の処理を阻害する可能性がある。
    if (is_keyboard_master() && timer_elapsed32(last_sync) > 200) {
        oled_sync.cpi = keyball_get_cpi();
        oled_sync.scr = keyball_get_scroll_div();

        transaction_rpc_send(OLED_SYNC, sizeof(oled_sync), &oled_sync);

        last_sync = timer_read32();
    }
#    endif
}

static void oled_write_u8(uint8_t value) {
    if (value >= 100) {
        oled_write_char('0' + value / 100, false);
        value %= 100;
        oled_write_char('0' + value / 10, false);
        oled_write_char('0' + value % 10, false);
    } else if (value >= 10) {
        oled_write_char('0' + value / 10, false);
        oled_write_char('0' + value % 10, false);
    } else {
        oled_write_char('0' + value, false);
    }
}

static uint8_t get_display_cpi(void) {
#ifdef SPLIT_KEYBOARD
    if (!is_keyboard_master()) {
        return oled_sync.cpi;
    }
#endif
    return keyball_get_cpi();
}

static uint8_t get_display_scr(void) {
#ifdef SPLIT_KEYBOARD
    if (!is_keyboard_master()) {
        return oled_sync.scr;
    }
#endif
    return keyball_get_scroll_div();
}

static void render_slave_oled(void) {
    uint8_t layer = get_highest_layer(layer_state);
    led_t led     = host_keyboard_led_state();

    oled_clear();

    // 1行目：レイヤーとCPIを大きめに見せるため短縮表示
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("L"), false);
    oled_write_u8(layer);
    oled_write_P(PSTR(" C"), false);
    oled_write_u8(get_display_cpi());

    // 2行目：SCR
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("S"), false);
    oled_write_u8(get_display_scr());

    // 3行目：Caps Lock
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("CAP "), false);
    oled_write_P(led.caps_lock ? PSTR("ON") : PSTR("--"), false);

    // 4行目：Num Lock
    oled_set_cursor(0, 3);
    oled_write_P(PSTR("NUM "), false);
    oled_write_P(led.num_lock ? PSTR("ON") : PSTR("--"), false);
}

// マスター側OLED。
// ケーブルを接続している側は、Keyball標準表示を維持する。
void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}

// スレーブ側OLED。
// ケーブルを接続していない側は、軽量なカスタム表示にする。
void oledkit_render_logo_user(void) {
    render_slave_oled();
}

#endif