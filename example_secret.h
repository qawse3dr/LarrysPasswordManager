/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once

#include <string>
#include <vector>

#include "button_name.h"

#define KEY(a) key_button_maps[a - 1]

std::vector<int> CODE = {KEY(1), KEY(2), KEY(3), KEY(4)};

std::vector<std::string> passwords = {"password1", "password2", "password3",
                                      "password4", "password5", "password6",
                                      "password7", "password8", "password9"};