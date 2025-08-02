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

#include "keyboard16.h"
#include "mcp23018.h"

#pragma GCC push_options
#pragma GCC optimize("-O3")


/*
#define MATRIX_ROW_PINS { B10, B11, B12, B13, B14, B15 } outputs
#define MATRIX_COL_PINS { A0, A1, A2, A3, A6, A7, B0 }   inputs
#define MCP23_ROW_PINS { GPB5, GBP4, GBP3, GBP2, GBP1, GBP0 }       outputs
#define MCP23_COL_PINS { GPA0, GBA1, GBA2, GBA3, GBA4, GBA5, GBA6 } inputs

 */
/* matrix state(1:on, 0:off) */
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values
//extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
//static matrix_row_t raw_matrix_right[MATRIX_ROWS];
 bool is_launching=false;
uint8_t         matrix_errors; 

static uint32_t hc595_shadow = 0;   // 24 位缓存
static const uint32_t empty_map[MATRIX_ROWS] = {
    0b001000000000000100,  // row0 无空位
    0b000000000000000010,  // row1 col8 空缺
    0b010000000000000100,  // row2 无空位
    0b000000000000000000, 
     0b010000000000000000, 
     0b010000000000000000, 
};
static inline void pulse_pin(pin_t pin) {
    writePinHigh(pin);
    wait_us(1);
    writePinLow(pin);
}
/* 发送 24 位数据并锁存 */
void en_ctrl_init(void) {
   gpio_set_pin_output(C13);
   gpio_write_pin_high(C13);
    gpio_set_pin_output(A15);
    gpio_write_pin_low(A15);

}
void hc595_out(uint32_t data) {
    hc595_shadow = data;
    for (int8_t i = 23; i >= 0; --i) {
        writePin(HC595_SER, (data >> i) & 1);
        pulse_pin(HC595_SRCLK);
    }
    pulse_pin(HC595_RCLK);   // 锁存
}

/* 初始化：脚设为输出，OE 拉低使能，全灭 */
void hc595_init(void) {
    setPinOutput(HC595_SER);
    setPinOutput(HC595_SRCLK);
    setPinOutput(HC595_RCLK);
    setPinOutput(HC595_OE);
    writePinLow(HC595_SER);
    writePinLow(HC595_SRCLK);
    writePinLow(HC595_RCLK);
    writePinLow(HC595_OE);   // 使能输出
    hc595_out(0);
}
void matrix_init_custom(void) {
    // outputs
    gpio_set_pin_output(B1);
    gpio_set_pin_output(B2);
    gpio_set_pin_output(B3);
    gpio_set_pin_output(B4);
    gpio_set_pin_output(B5);
    gpio_set_pin_output(B6);

    // inputs
    gpio_set_pin_input(C0); //K_COL1
    gpio_set_pin_input(A3); //K_COL_2
    gpio_set_pin_input(C1); //K_COL2
    gpio_set_pin_input(C2); //K_COL3
    gpio_set_pin_input(C3); //K_COL4
    gpio_set_pin_input(A0); //K_COL5
    gpio_set_pin_input(A1); //K_COL6
    gpio_set_pin_input(A2); //K_COL7
    
    gpio_set_pin_input(A4); //K_COL8
    gpio_set_pin_input(A5); //K_COL9
    gpio_set_pin_input(A6); //K_COL10
    gpio_set_pin_input(A7); //K_COL11
    gpio_set_pin_input(C4); //K_COL12
    gpio_set_pin_input(C5); //K_COL13
    gpio_set_pin_input(C6); //K_COL14
    gpio_set_pin_input(C7); //K_COL15
    gpio_set_pin_input(C9); //K_COL_15
    gpio_set_pin_input(C8); //K_COL16

   // if (!mcp23018_errors) {
        is_launching = true;
    }


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;
   
      

    matrix_row_t data = 0;
    // actual matrix
    for (uint8_t row = 0; row <= 6; row++) {
        // strobe row
        switch (row) {
            case 0:
                gpio_write_pin_low(B1);
              //  wait_us(550);
                break;
            case 1:
                gpio_write_pin_low(B2);
              //  wait_us(550);
                break;
            case 2:
                gpio_write_pin_low(B3);
             //   wait_us(550);
                break;
            case 3:
                gpio_write_pin_low(B4);
             //   wait_us(550);
                break;
            case 4:
                gpio_write_pin_low(B5);
            //    wait_us(550);
                break;
            case 5:
                gpio_write_pin_low(B6);
             //   wait_us(550);
                break;
            
        }

      
  

        // Reading the keyboard.
        if (row < 6) {
             wait_us(550);
            // read col data
            data = ( (matrix_row_t)(gpio_read_pin(C0) << 0) | (matrix_row_t)(gpio_read_pin(A3) << 1)| (matrix_row_t)(gpio_read_pin(C1) << 2) | (matrix_row_t)(gpio_read_pin(C2) << 3) | (matrix_row_t)(gpio_read_pin(C3) << 4) |(matrix_row_t) (gpio_read_pin(A0) << 5) 
            |(matrix_row_t) (gpio_read_pin(A1) << 6)| (matrix_row_t)(gpio_read_pin(A2) << 7)| (matrix_row_t) (gpio_read_pin(A4) << 8)| (matrix_row_t)(gpio_read_pin(A5) << 9)| (matrix_row_t)(gpio_read_pin(A6) << 10) |(matrix_row_t) (gpio_read_pin(A7) << 11)
            |(matrix_row_t) (gpio_read_pin(C4) << 12)| (matrix_row_t)(gpio_read_pin(C5) << 13) | (matrix_row_t)(gpio_read_pin(C6) << 14)| (matrix_row_t)(gpio_read_pin(C7) << 15)| (matrix_row_t)(gpio_read_pin(C9) << 16)| (matrix_row_t)(gpio_read_pin(C8) << 17));
            // unstrobe  row
            switch (row) {
                case 0:
                    gpio_write_pin_high(B1);
                    break;
                case 1:
                    gpio_write_pin_high(B2);
                    break;
                case 2:
                    gpio_write_pin_high(B3);
                    break;
                case 3:
                    gpio_write_pin_high(B4);
                    break;
                case 4:
                    gpio_write_pin_high(B5);
                    break;
                case 5:
                    gpio_write_pin_high(B6);
                    break;
               
            }
            data &= ~empty_map[row];
            if (current_matrix[row] != data) {
                current_matrix[row] = data;
                changed             = true;
            }
        }

    }
    return changed;
}

// DO NOT REMOVE
// Needed for proper wake/sleep
void matrix_power_up(void) {
    bool temp_launching = is_launching;

    matrix_init_custom();

    is_launching = temp_launching;
    if (!is_launching) {

     //   ML_LED_1(false);
      //  ML_LED_2(false);
      //  ML_LED_3(false);
      //  ML_LED_4(false);
      //  ML_LED_5(false);
      //  ML_LED_6(false);
       hc595_out(0x03FFFF); 
    }

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

bool is_transport_connected(void) {
    return (bool)(matrix_errors == 0);
}
#pragma GCC pop_options
