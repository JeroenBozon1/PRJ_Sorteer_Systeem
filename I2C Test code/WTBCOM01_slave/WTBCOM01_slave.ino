#include <Wire.h>

#define BUTTON_PIN 2
#define SWITCH_PIN_I 3
#define SWITCH_PIN_II 4
#define WIRE_ID 8

#define SWITCH_PIN_OFF_STATUS 0
#define SWITCH_PIN_I_STATUS 1
#define SWITCH_PIN_II_STATUS 2

// In case the toggle switch is turned off
#define CHOICE_OFF 255
// Inclusive minimum
#define CHOICE_MIN 0
// Inclusive maximum
#define CHOICE_MAX 3

#define SAFETY_BUFFER 100

const boolean log_important_enabled = true;
const boolean log_info_enabled = false;
const boolean log_verbose_enabled = false;

int choice = -1;
int choice_memory = -1;

int unit = 0;


void setup() {
  Serial.begin(115200);
  Wire.begin(WIRE_ID);
  Wire.onRequest(requestEvent);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(SWITCH_PIN_I, INPUT);
  pinMode(SWITCH_PIN_II, INPUT);

  choice = CHOICE_MIN - 1;
  choice_memory = CHOICE_MIN;

  int state = switch_status();
  switch (state) {
    case SWITCH_PIN_I_STATUS:
      sequential_choice();
      log_important("Booting in SEQUENTIAL mode. Generated sequential number: " + String(choice) + " | Unit: " + String(unit));
      break;
    case SWITCH_PIN_II_STATUS:
      random_choice();
      log_important("Booting in RANDOM mode. Generated random number: " + String(choice) + " | Unit: " + String(unit));
      break;
    case SWITCH_PIN_OFF_STATUS:
      log_important("Booting in OFF mode.");
      // Choice is not set here as it is automatically set in the loop() function.
      break;
    default:
      log_important("ERROR when booting! Switch_status unknown! State: " + String(state));
  }

}

void loop() {
  // SEQUENTIAL MODE:
  if (switch_status() == SWITCH_PIN_I_STATUS) {
    log_info("Entering SEQUENTIAL mode.");

    // Wait for either the switch to be toggled or the button to be turned on
    while (switch_status() == SWITCH_PIN_I_STATUS && !button_high()) {}

    // If the button is pressed:
    if (button_high()) {
      log_info("Button is HIGH.");

      // Generate sequential number based off of previous number and within CHOICE_MIN and CHOICE_MAX
      sequential_choice();
      log_important("Generated sequential number: " + String(choice) + " | Unit: " + String(unit));

      // Find out whether we released the button/switch properly.
      check_release(SWITCH_PIN_I_STATUS);
    }

    // Switch was toggled somewhere in this process.
    if (switch_status() != SWITCH_PIN_I_STATUS) {
      log_info("Switch is toggled to: " + String(switch_status()));
      log_status();

      // Store the sequential position in memory and pick a random choice.
      choice_memory = choice;
    }
  } else if (switch_status() == SWITCH_PIN_II_STATUS) {
    // RANDOM MODE
    log_info("Entering RANDOM mode.");

    // Wait for either the switch to be toggled or the button to be turned on
    while (switch_status() == SWITCH_PIN_II_STATUS && !button_high()) {}

    // If the button is pressed:
    if (button_high()) {
      log_info("Button is HIGH.");

      // Generate random number between CHOICE_MIN and CHOICE_MAX
      random_choice();

      log_important("Generated random number: " + String(choice) + " | Unit: " + String(unit));

      // Find out whether we released the button/switch properly.
      check_release(SWITCH_PIN_II_STATUS);
    }

    // Switch was toggled somewhere in this process.
    if (switch_status() != SWITCH_PIN_II_STATUS) {
      log_info("Switch is toggled to: " + String(switch_status()));
      log_status();
    }
  } else if (switch_status() == SWITCH_PIN_OFF_STATUS) {
    // SWITCH IS OFF
    log_info("Switch is OFF");

    choice = CHOICE_OFF;
    update_unit();
    log_important("Choice set OFF to: " + String(choice) + " | Unit: " + String(unit));

    // Wait for the switch to be toggled on to either status
    while (switch_status() == SWITCH_PIN_OFF_STATUS) {}

    log_info("Switch status changed. Switching modes.");
    log_status();

    // Reset back the choice to memory if sequential mode or to a random value if
    if (switch_status() == SWITCH_PIN_I_STATUS) {
      choice = choice_memory;
      update_unit();

      log_important("Choice set to MEMORY: " + String(choice) + " | Unit: " + String(unit));
    } else if (switch_status() == SWITCH_PIN_II_STATUS) {
      random_choice();
      log_important("Choice toggled to a RANDOM value: " + String(choice) + " | Unit: " + String(unit));
    }
  }
}

/*
   =========================
       I2C COMMUNICATION
   =========================
*/

void requestEvent() {
  byte buff[2];
  buff[0] = unit;
  buff[1] = choice;
  Wire.write(buff, 2);
}

/*
   ================================
     BUTTON/SWITCH RELEASE CHECKS
   ================================
*/

void check_release(int switch_pin_status) {
  // Repeat endlessly until a safety buffer is breached.
  while (true) {

    log_verbose("Entering endless loop.");

    // Wait for either the switch to be toggled or the button to be released
    while (switch_status() == switch_pin_status && button_high()) {}

    log_info("Switch is toggled or button is LOW.");

    int safety_count = 0;

    if (switch_status() != switch_pin_status) {
      safety_count = safety_check(switch_status() != switch_pin_status);
    } else if (!button_high()) {
      safety_count = safety_check(!button_high());
    }

    // We reached SAFETY_BUFFER.
    if (safety_count == SAFETY_BUFFER) {
      log_info("Reached SAFETY_BUFFER! (" + String(safety_count) + "/" + String(SAFETY_BUFFER) + ")");
      break;
    } else {
      log_info("SAFETY BUFFER not reached! (" + String(safety_count) + "/" + String(SAFETY_BUFFER) + ")");
    }
  }
}

int safety_check(boolean check) {
  // Starts at two because of the checks before and after this line.
  int count = 2;

  for (int i = 0; i < SAFETY_BUFFER - 2; i++) { // SAFETY_BUFFER - 2, because we have already checked twice.
    if (check) {
      count++;
    }
    log_verbose("Switch Safety Count: " + String(count));
  }

  return count;
}

/*
   ========================
       UTILITY FUNCTIONS
   ========================
*/

void update_unit() {
 unit == 255 ? unit = 0 : unit++;
}

boolean button_high() {
  return digitalRead(BUTTON_PIN) == HIGH;
}

int switch_status() {
  if (digitalRead(SWITCH_PIN_I) == HIGH) {
    return SWITCH_PIN_I_STATUS;
  } else if (digitalRead(SWITCH_PIN_II) == HIGH) {
    return SWITCH_PIN_II_STATUS;
  }

  return SWITCH_PIN_OFF_STATUS;
}

void sequential_choice() {
  // Add 1 to our choice and reset back to min if it reaches max.
  choice = (choice + 1) % (CHOICE_MAX + 1) + CHOICE_MIN;
  update_unit();
}

void random_choice() {
  // Reset choice: lower bound is inclusive, upper bound is exclusive.
  choice = random(CHOICE_MIN, CHOICE_MAX + 1);
  update_unit();
}

String booleanToString(boolean b) {
  return b ? "TRUE" : "FALSE";
}

String highlowToString(int hl) {
  return hl == 0 ? "LOW" : hl == 1 ? "HIGH" : "UNDEFINED";
}

/*
   =========================
       LOGGING FUNCTIONS
   =========================
*/

void log_status() {
  log_info("UNIT: " + String(unit) + " | CHOICE: " + String(choice) + " | SWITCH STATUS: " + String(switch_status()) + " | BUTTON STATUS: " + highlowToString(button_high()));
}

void log_verbose(String msg) {
  if (log_verbose_enabled) {
    Serial.println("[V] " + msg);
  }
}

void log_info(String msg) {
  if (log_info_enabled) {
    Serial.println("[INFO] " + msg);
  }
}

void log_important(String msg) {
  if (log_important_enabled) {
    Serial.println("===========================================");
    Serial.println("[IMPORTANT] " + msg);
    Serial.println("===========================================");
  }
}
