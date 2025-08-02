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



#pragma once

#include "quantum.h"

extern bool mcp23018_leds[];

#define MCP23018_DEFAULT_ADDRESS 0b0100000

#define ML_LED_1(status) gpio_write_pin(B5, (bool)status)
#define ML_LED_2(status) gpio_write_pin(B4, (bool)status)
#define ML_LED_3(status) gpio_write_pin(B3, (bool)status)

#define ML_LED_4(status) mcp23018_leds[0] = (bool)status
#define ML_LED_5(status) mcp23018_leds[1] = (bool)status
#define ML_LED_6(status) mcp23018_leds[2] = (bool)status

enum planck_ez_keycodes {
    TOGGLE_LAYER_COLOR = QK_KB_0,
    LED_LEVEL,
};

#ifndef WEBUSB_ENABLE
#    define WEBUSB_PAIR KC_NO
#endif
#define LAYOUT(k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H,  \
               k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H,     \
               k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H,       \
               k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H,    \
               k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H, \
               k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F, k5G, k5H   \
              ) {  \
                { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09,  k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H }, \
              { k10, k11, k12, k13, k14, k15, k16, k17, k18, k19,  k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H }, \
            { k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H },           \
    { k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H },            \
    { k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H },           \
    { k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F, k5G, k5H }            \
}

typedef union {
  uint32_t raw;
  struct {
    bool         disable_layer_led   :1;
    bool         rgb_matrix_enable   :1;
    bool         led_level           :1;
    uint8_t      led_level_res       :2; // DO NOT REMOVE
  };
} keyboard_config_t;

extern keyboard_config_t keyboard_config;

bool is_transport_connected(void);
