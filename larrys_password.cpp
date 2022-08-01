/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#include <stdio.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bsp/board.h"
#include "button.h"
#include "button_name.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "secret.h"
#include "tusb.h"
#include "usb_descriptors.h"

enum class LED_STATE { BLINK, SOLID, ERROR_BLINK };

LED_STATE led_state = LED_STATE::BLINK;

// LED runtime loop
void ledHandler() {
  GpioOutput led(LED_PIN);

  while (true) {
    while (led_state == LED_STATE::BLINK) {
      led.toggle();
      sleep_ms(500);
    }
    if (led_state == LED_STATE::SOLID) {
      led.put(true);
      while (led_state == LED_STATE::SOLID) {
        sleep_ms(100);
      }
    }
    while (led_state == LED_STATE::ERROR_BLINK) {
      sleep_ms(100);
      led.toggle();
    }
  }
}

enum class RUN_STATE { INPUT, SELECT };

uint8_t const conv_table[128][2] = {HID_ASCII_TO_KEYCODE};

static void sendString(const std::string& str) {
  for (char c : str) {
    // Press key
    while (true) {
      sleep_ms(20);
      tud_task();
      if (!tud_hid_n_ready(ITF_NUM_KEYBOARD)) continue;

      uint8_t keycode[6] = {0};
      uint8_t modifier = 0;
      if (conv_table[c][0]) modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
      keycode[0] = conv_table[c][1];
      tud_hid_n_keyboard_report(ITF_NUM_KEYBOARD, 0, modifier, keycode);
      break;
    }
    // release key
    while (true) {
      sleep_ms(20);
      tud_task();
      if (!tud_hid_n_ready(ITF_NUM_KEYBOARD)) continue;
      tud_hid_n_keyboard_report(ITF_NUM_KEYBOARD, 0, 0, NULL);

      break;
    }
  }
}

int main() {
  board_init();
  tusb_init();

  std::vector<GpioInputButton> buttons;
  for (auto but : key_button_maps) {
    buttons.emplace_back(but, true);
  }
  buttons.emplace_back(BUTTON_SUBMIT, true);
  buttons.emplace_back(BUTTON_CANCEL, true);
  buttons.emplace_back(BUTTON_FLASH, true);

  multicore_launch_core1(ledHandler);

  RUN_STATE state = RUN_STATE::INPUT;
  std::vector<int> password = {};
  std::map<int, std::string> secret_passwords;
  for (int i = 0; i < key_button_maps.size(); i++) {
    secret_passwords.emplace(std::pair(key_button_maps[i], passwords[i]));
  }

  while (true) {
    tud_task();
    for (auto& button : buttons) {
      if (button.pressed()) {
        // wait for button to become unpressed
        while (button.pressed()) {
          sleep_ms(10);
        }

        if (button.getPin() == BUTTON_SUBMIT && state == RUN_STATE::SELECT) {
          password.clear();
          state = RUN_STATE::INPUT;
          led_state = LED_STATE::BLINK;
          break;
        } else if (button.getPin() == BUTTON_SUBMIT) {
          // Submit password
          if (password == CODE) {
            state = RUN_STATE::SELECT;
            led_state = LED_STATE::SOLID;
          } else {
            led_state = LED_STATE::ERROR_BLINK;
            sleep_ms(1000);
            led_state = LED_STATE::BLINK;
          }
          password.clear();
          break;
        } else if (button.getPin() == BUTTON_CANCEL) {
          state = RUN_STATE::INPUT;
          led_state = LED_STATE::ERROR_BLINK;
          sleep_ms(250);
          led_state = LED_STATE::BLINK;
          password.clear();
        } else if (button.getPin() == BUTTON_FLASH) {
          reset_usb_boot(0, 0);
        }

        if (state == RUN_STATE::INPUT) {
          password.push_back(button.getPin());
        } else {
          sendString(secret_passwords[button.getPin()] + "\n");
          led_state = LED_STATE::BLINK;
          state = RUN_STATE::INPUT;
        }
      }
    }
  }
  return 0;
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report,
                                uint8_t len) {}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t* buffer,
                               uint16_t reqlen) {
  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const* buffer,
                           uint16_t bufsize) {}

// Invoked when device is mounted
void tud_mount_cb(void) {}

// Invoked when device is unmounted
void tud_umount_cb(void) {}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {}