/* Copyright 2020 ZSA Technology Labs, Inc <@zsa>
 * Copyright 2020 Jack Humbert <jack.humb@gmail.com>
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include QMK_KEYBOARD_H
#include "version.h"
#include "print.h"
#define LED_SCL_PIN  C10
#define LED_CAPS_PIN C11
#define LED_NUM_PIN  C12
extern void hc595_init(void);
extern void en_ctrl_init(void);
extern void hc595_out(uint32_t data) ;

 static uint32_t hc595_shadow;
enum layers {
    BASE,  // default layer
    SYMB,  // symbols
   // MDIA,  // media keys
};
uint32_t mask=0x03FFFF;
enum custom_keycodes {
    VRSN = SAFE_RANGE,
};
static deferred_token flash_token = INVALID_DEFERRED_TOKEN;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_ESC,  KC_CAPS,       KC_NO,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,   KC_F7, KC_F8,    KC_F9,    KC_F10,    KC_F11,    KC_F12,   KC_NO,   KC_SCRL,   KC_END,
        KC_BSPC, KC_NO,         KC_GRV,   KC_1,      KC_2,     KC_3,     KC_4,    KC_5,     KC_6,   KC_7,   KC_8,     KC_9 ,     KC_0,     KC_MINS,    KC_EQL,  KC_BSPC ,KC_BSPC ,  KC_BSPC ,
        KC_TAB,KC_NO,KC_NO, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    TG(SYMB),         TG(SYMB), KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,KC_NO,KC_RBRC,
        KC_CAPS	, KC_CAPS	,KC_CAPS	,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_HYPR,           KC_MEH,  KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN,KC_COMM ,KC_COMM ,KC_BSLS, //LT(MDIA, KC_SCLN), LGUI_T(KC_QUOT),
        KC_LSFT, KC_LSFT, KC_LSFT, KC_Z,      KC_X,    KC_C,    KC_V,    KC_B,    KC_HYPR,           KC_MEH, KC_B,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_NO,KC_RSFT,
        KC_LCTL,KC_LCTL,KC_LCTL,KC_LEFT,KC_UP,KC_DOWN,KC_RIGHT,KC_SPACE,QK_BOOT, KC_ENTER,KC_SPACE,KC_LEFT,KC_UP,KC_DOWN,KC_RIGHT,KC_RALT,KC_NO,KC_RCTL
                                           
    ),
/*
    [SYMB] = LAYOUT(
        VRSN,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,           _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, _______,           _______, KC_UP,   KC_7,    KC_8,    KC_9,    KC_ASTR, KC_F12,
        _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,  _______,           _______, KC_DOWN, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,
        _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,                             KC_AMPR, KC_1,    KC_2,    KC_3,    KC_BSLS, _______,
        EE_CLR,  _______, _______, _______, _______,          RM_VALU,           RM_TOGG,          _______, KC_DOT,  KC_0,    KC_EQL,  _______,
                                            RM_HUED, RM_VALD, RM_HUEU, TOGGLE_LAYER_COLOR,_______, _______
    ),

    [MDIA] = LAYOUT(
        LED_LEVEL,_______,_______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, KC_MS_U, _______, _______, _______,           _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______,           _______, _______, _______, _______, _______, _______, KC_MPLY,
        _______, _______, _______, _______, _______, _______,                             _______, _______, KC_MPRV, KC_MNXT, _______, _______,
        _______, _______, _______, KC_BTN1, KC_BTN2,         _______,            _______,          KC_VOLU, KC_VOLD, KC_MUTE, _______, _______,
                                            _______, _______, _______,           _______, _______, _______
    ),
    */
};
static uint32_t key_flash_off(uint32_t trigger_time, void *col_arg) {
    uint16_t col = (uint16_t)(uintptr_t)col_arg;
    hc595_out(hc595_shadow | (1UL << col));
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
      
       uint8_t col = record->event.key.col;
       uint8_t row = record->event.key.row;
       uint32_t col_temp=0;//=(~(1UL<<(col-1)))&0x03FFFF;
     //  print("Hello from QMK /n");
      // uprintf("Keycode: %d\n" ,keycode);
     //  dprint("only in debug mode \n");
    //   dprintf("row=%u col=%u keycode=%u\n",
        record->event.key.row,
        record->event.key.col,
        keycode);
       if (row <6)
       {
    mask = ((1UL<<(row+18))&0xFFFFFF);
    if (col==0 )
      {
        col_temp=(~(1UL<<(col)))&0x03FFFF;   
      }
     else if (col==1 )
      {
        col_temp=(~(1UL<<16))&0x03FFFF;   
      }
      else if (col==16)
       {
        col_temp=(~(1UL<<17))&0x03FFFF;   
      }
    else if (col==17)
       {
        col_temp=(~(1UL<<(col-2)))&0x03FFFF;   
      }
    //   else if (col==17) 
     //  {
     //   col_temp=(~(1UL<<(col-2)))&0x03FFFF;   
    //  }
    else
     {col_temp=(~(1UL<<(col-1)))&0x03FFFF;}
} else {
    mask = 0x03FFFF;  /* 兜底值 */
} 
       hc595_out(col_temp|mask) ;             // 立即亮
         
        cancel_deferred_exec(flash_token);
       flash_token = defer_exec(300, key_flash_off, (void *)(uintptr_t)col);
    }
    return true;
}
void keyboard_post_init_user(void) {
    // 在这里写你的初始化代码
    debug_enable=true;
    debug_matrix=true;
    hc595_init();
    en_ctrl_init();
   
    gpio_set_pin_output(LED_NUM_PIN);
    gpio_set_pin_output(LED_CAPS_PIN);
    gpio_set_pin_output(LED_SCL_PIN);
    writePinHigh(LED_NUM_PIN);
    writePinHigh(LED_CAPS_PIN);
    writePinHigh(LED_SCL_PIN);
    hc595_out(0xFC0000);    
    wait_ms(2000);
    hc595_out(0x03FFFF); 
}


/* 每次 HID 状态变化都会触发此回调 */
bool led_update_user(led_t led_state) {
    gpio_write_pin(LED_NUM_PIN,  !led_state.num_lock);   // 低亮/高亮按硬件决定
    gpio_write_pin(LED_CAPS_PIN, !led_state.caps_lock);
    gpio_write_pin(LED_SCL_PIN,  !led_state.scroll_lock);
    return false;
}
