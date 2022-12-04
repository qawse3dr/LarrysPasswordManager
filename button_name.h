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
#pragma once

#include <vector>

// BUTTONS for password keypad
// Version 2
std::vector<int> key_button_maps = {2, 10, 11, 3, 4, 6, 1, 5, 7};
// Versions 1
// std::vector<int> key_button_maps = {13, 12, 10, 18, 19, 20};

// Version 2
#define BUTTON_SUBMIT 12

// Version 1
// #define BUTTON_SUBMIT 2

// Version 2
#define BUTTON_CANCEL 8
#define BUTTON_FLASH 9
#define LED_PIN 0

// Version 1 Cancel and flash aren't support set to -1
// #define BUTTON_CANCEL -1
// #define BUTTON_FLASH -1
// #define LED_PIN 5
