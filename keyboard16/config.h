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

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 18
#define HC595_SER   B15
#define HC595_SRCLK B13
#define HC595_RCLK  B12
#define HC595_OE    B11

//#define MATRIX_ROWS 7
//#define MATRIX_COLS 24   // 3 片级联 → 24 列
//#define EEPROM_I2C_24LC128
#define EXTERNAL_EEPROM_I2C_BASE_ADDRESS 0b10100000
#define EXTERNAL_EEPROM_BYTE_COUNT       256
#define EXTERNAL_EEPROM_PAGE_SIZE        8
#define EXTERNAL_EEPROM_ADDRESS_SIZE     1   // 1 字节地址（≤256 B）
#define EXTERNAL_EEPROM_WRITE_TIME       5   // ms
//#define IS31FL3731_I2C_ADDRESS_1 IS31FL3731_I2C_ADDRESS_GND
//#define IS31FL3731_I2C_ADDRESS_2 IS31FL3731_I2C_ADDRESS_VCC

#define MUSIC_MAP

#define FIRMWARE_VERSION_SIZE 17
#define DYNAMIC_KEYMAP_EEPROM_ADDR (EECONFIG_SIZE + FIRMWARE_VERSION_SIZE)

#define AUDIO_PIN A5
#define AUDIO_PIN_ALT A4
#define AUDIO_PIN_ALT_AS_NEGATIVE
