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
#include "pico/stdlib.h"

class Gpio {
 protected:
  int8_t gpio_pin_;

 public:
  explicit Gpio(int gpio_pin);
  ~Gpio();

  inline int8_t getPin() { return gpio_pin_; }
};

class GpioInputButton : public Gpio {
 private:
  bool pull_up_;

 public:
  GpioInputButton(int gpio_pin, bool pull_up = false);

  bool pressed();
};

class GpioOutput : public Gpio {
 private:
  bool state_;

 public:
  explicit GpioOutput(int gpio_pin, bool start_state = false);

  bool put(bool state);
  bool toggle();
};