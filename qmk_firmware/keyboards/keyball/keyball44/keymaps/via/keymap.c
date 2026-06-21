/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
*/

#include QMK_KEYBOARD_H
#include "quantum.h"
#include <string.h>

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"
#endif

#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif

// clang-format off

enum layers {
    _BASE = 0,
    _NAV,
    _SYM,
    _VSCROLL,
    _HSCROLL,
    _ADJUST,
};

// OLED表示用の簡易ステータス。
// 実際のCPI/SCR処理はKeyball標準処理に任せ、ここでは表示値だけ追跡する。
typedef struct {
    uint8_t cpi;
    uint8_t scr;
    bool caps;
    bool num;
} oled_status_t;

static oled_status_t oled_status = {
    .cpi = 5,
    .scr = 5,
    .caps = false,
    .num = false,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Base
  [_BASE] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT, KC_LGUI, LCTL_T(KC_LNG2), LT(_NAV, KC_SPC), LT(_VSCROLL, KC_LNG1),        LT(_HSCROLL, KC_BSPC), LT(_SYM, KC_ENT), RCTL_T(KC_LNG2), KC_RALT, KC_PSCR
  ),

  // Layer 1: Navigation / Mouse
  [_NAV] = LAYOUT_universal(
    SSNP_FRE , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                        KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT , _______  , _______  , KC_UP    , KC_ENT   , KC_DEL   ,                                        KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR , _______  , KC_LEFT  , KC_DOWN  , KC_RGHT  , KC_BSPC  ,                                        KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
              _______  , _______  , _______  , _______  , MO(_ADJUST),                           _______  , _______  , _______  , _______  , _______
  ),

  // Layer 2: Symbols
  [_SYM] = LAYOUT_universal(
    _______  , S(KC_QUOT), KC_7    , KC_8     , KC_9     , S(KC_8)  ,                                        S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  , S(KC_SCLN), KC_4    , KC_5     , KC_6     , KC_RBRC  ,                                        KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  , S(KC_MINS), KC_1    , KC_2     , KC_3     , S(KC_RBRC),                                       S(KC_NUHS), S(KC_INT1), KC_EQL , S(KC_LBRC), S(KC_SLSH), S(KC_INT3),
              KC_0      , KC_DOT  , _______  , _______  , MO(_ADJUST),                           KC_DEL   , _______  , _______  , _______  , _______
  ),

  // Layer 3: Vertical Scroll
  // このレイヤ中は layer_state_set_user() によりトラックボールが縦スクロールになる。
  [_VSCROLL] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        KC_BTN1  , KC_BTN2  , KC_BTN3  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
              _______  , _______  , _______  , _______  , _______  ,                             _______  , _______  , _______  , _______  , _______
  ),

  // Layer 4: Horizontal Scroll
  // このレイヤ中は layer_state_set_user() によりトラックボールが横スクロールになる。
  [_HSCROLL] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        KC_BTN1  , KC_BTN2  , KC_BTN3  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
              _______  , _______  , _______  , _______  , _______  ,                             _______  , _______  , _______  , _______  , _______
  ),

  // Layer 5: Adjust
  // 旧Layer 3の設定系キーをここへ移動。
  [_ADJUST] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                        RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , SCRL_DVI ,                                        RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
              QK_BOOT  , KBC_RST  , _______  , _______  , _______  ,                             _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),
};

// clang-format on

static uint8_t clamp_level(uint8_t value, int8_t diff) {
    int16_t next = (int16_t)value + diff;

    if (next < 1) {
        return 1;
    }
    if (next > 9) {
        return 9;
    }
    return (uint8_t)next;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case CPI_I100:
        case CPI_I1K:
            oled_status.cpi = clamp_level(oled_status.cpi, +1);
            break;

        case CPI_D100:
        case CPI_D1K:
            oled_status.cpi = clamp_level(oled_status.cpi, -1);
            break;

        case SCRL_DVI:
            oled_status.scr = clamp_level(oled_status.scr, +1);
            break;

        case SCRL_DVD:
            oled_status.scr = clamp_level(oled_status.scr, -1);
            break;
    }

    // 重要:
    // Keyball標準のCPI/SCR処理を壊さないため、必ず true を返す。
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
        case _VSCROLL:
            keyball_set_scroll_mode(true);
            keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_VERTICAL);
            break;

        case _HSCROLL:
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

static void update_lock_status(void) {
    led_t led_state = host_keyboard_led_state();

    oled_status.caps = led_state.caps_lock;
    oled_status.num  = led_state.num_lock;
}

#ifdef SPLIT_KEYBOARD

enum {
    RPC_ID_OLED_STATUS_SYNC = 1,
};

static void oled_status_sync_slave_handler(uint8_t in_size, const void *in_data, uint8_t out_size, void *out_data) {
    if (in_size == sizeof(oled_status_t)) {
        memcpy(&oled_status, in_data, sizeof(oled_status_t));
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_ID_OLED_STATUS_SYNC, oled_status_sync_slave_handler);
}

void housekeeping_task_user(void) {
    static uint32_t last_sync = 0;

    if (!is_keyboard_master()) {
        return;
    }

    update_lock_status();

    if (timer_elapsed32(last_sync) > 100) {
        transaction_rpc_send(RPC_ID_OLED_STATUS_SYNC, sizeof(oled_status_t), &oled_status);
        last_sync = timer_read32();
    }
}

#else

void housekeeping_task_user(void) {
    update_lock_status();
}

#endif

#ifdef OLED_ENABLE

void oledkit_render_info_user(void) {
    // USB接続側OLEDはKeyball標準表示を維持
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}

static void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    for (uint8_t i = 0; i < w; i++) {
        for (uint8_t j = 0; j < h; j++) {
            oled_write_pixel(x + i, y + j, true);
        }
    }
}

// KeyballのOLEDは物理的に縦向きに見るため、
// 論理OLEDのX方向を「縦方向」として使う7セグ描画。
static void draw_segment_at(uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t t, uint8_t segment) {
    switch (segment) {
        case 0:
            draw_rect(x, y + t, t, w - t * 2);
            break;

        case 1:
            draw_rect(x + t, y + w - t, h / 2 - t, t);
            break;

        case 2:
            draw_rect(x + h / 2, y + w - t, h / 2 - t, t);
            break;

        case 3:
            draw_rect(x + h - t, y + t, t, w - t * 2);
            break;

        case 4:
            draw_rect(x + h / 2, y, h / 2 - t, t);
            break;

        case 5:
            draw_rect(x + t, y, h / 2 - t, t);
            break;

        case 6:
            draw_rect(x + h / 2 - t / 2, y + t, t, w - t * 2);
            break;
    }
}

static void draw_digit_at(uint8_t digit, uint8_t x, uint8_t y, uint8_t h, uint8_t w, uint8_t t) {
    switch (digit) {
        case 0:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            draw_segment_at(x, y, h, w, t, 4);
            draw_segment_at(x, y, h, w, t, 5);
            break;

        case 1:
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            break;

        case 2:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 6);
            draw_segment_at(x, y, h, w, t, 4);
            draw_segment_at(x, y, h, w, t, 3);
            break;

        case 3:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 6);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            break;

        case 4:
            draw_segment_at(x, y, h, w, t, 5);
            draw_segment_at(x, y, h, w, t, 6);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            break;

        case 5:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 5);
            draw_segment_at(x, y, h, w, t, 6);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            break;

        case 6:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 5);
            draw_segment_at(x, y, h, w, t, 6);
            draw_segment_at(x, y, h, w, t, 4);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            break;

        case 7:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            break;

        case 8:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            draw_segment_at(x, y, h, w, t, 4);
            draw_segment_at(x, y, h, w, t, 5);
            draw_segment_at(x, y, h, w, t, 6);
            break;

        case 9:
            draw_segment_at(x, y, h, w, t, 0);
            draw_segment_at(x, y, h, w, t, 1);
            draw_segment_at(x, y, h, w, t, 2);
            draw_segment_at(x, y, h, w, t, 3);
            draw_segment_at(x, y, h, w, t, 5);
            draw_segment_at(x, y, h, w, t, 6);
            break;
    }
}

static void render_small_lock_status(void) {
    // 論理カーソル文字は回転しないため、最小限の表示にする。
    // C/c = Caps, N/n = Num
    oled_set_cursor(0, 0);
    oled_write_P(oled_status.caps ? PSTR("C") : PSTR("c"), false);

    oled_set_cursor(0, 1);
    oled_write_P(oled_status.num ? PSTR("N") : PSTR("n"), false);
}

static void render_custom_slave_oled(void) {
    uint8_t layer = get_highest_layer(layer_state);

    if (layer > 9) {
        layer = 9;
    }

    oled_clear();

    // OLED全体 128x32 を、物理的な縦方向として以下の比率で使用。
    // Lock : 約1割
    // SCR  : 約2割
    // CPI  : 約2割
    // LYR  : 約5割

    render_small_lock_status();

    // SCR: 上側2割
    draw_digit_at(oled_status.scr, 12, 6, 24, 18, 3);

    // CPI: 中央2割
    draw_digit_at(oled_status.cpi, 40, 6, 24, 18, 3);

    // LYR: 下側5割
    draw_digit_at(layer, 70, 4, 54, 24, 4);
}

// ケーブルを接続していない側、つまりスレーブ側OLEDの表示
void oledkit_render_logo_user(void) {
    render_custom_slave_oled();
}

#endif