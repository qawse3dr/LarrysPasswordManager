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
std::vector<int> key_button_maps = {2, 10, 11, 3, 4, 6, 1, 5, 7};

#define BUTTON_SUBMIT 12
#define BUTTON_CANCEL 8  // TODO
#define BUTTON_FLASH 9   // TODO
#define LED_PIN 0
