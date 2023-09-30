// Copyright 2023 Matthew Smith
// SPDX-License-Identifier: BSD-3-Clause

#include <hardware/flash.h>
#include <pico/stdlib.h>
#include <pico/unique_id.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Store the 8-byte flash id 256Ki into flash. You could modify the
// linker script to make space for this so that you never accidentally
// trample on the code/data in the application.
#define BOARD_ID_OFFSET (64 * FLASH_PAGE_SIZE)
#define BOARD_ID_LEN 8

static const uint8_t *s_stored_board_id =
    (const uint8_t *)(XIP_BASE + BOARD_ID_OFFSET);

int main() {
  pico_unique_board_id_t id;
  bool valid = true;
  pico_get_unique_board_id(&id);
  if (memcmp(s_stored_board_id, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
             BOARD_ID_LEN) == 0) {
    // This is a fresh board, store the measured flash id into flash.
    flash_range_erase(BOARD_ID_OFFSET, FLASH_PAGE_SIZE);
    flash_range_program(BOARD_ID_OFFSET, id.id, BOARD_ID_LEN);

    // Verify programming success.
    if (memcmp(s_stored_board_id, id.id, BOARD_ID_LEN) != 0) {
      // TODO: Don't just abort.
      abort();
    }
  } else {
    // An id has been written to flash. Compare it against the measured id.
    valid = memcmp(s_stored_board_id, id.id, BOARD_ID_LEN) == 0;
  }

  // Blink slowly if valid, blink rapidly if we think that we've been
  // cloned.
  int blink_delay = valid ? 500 : 50;
  gpio_init(PICO_DEFAULT_LED_PIN);
  while (true) {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(blink_delay);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(blink_delay);
  }
}
