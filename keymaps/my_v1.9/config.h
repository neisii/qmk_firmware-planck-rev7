/* Copyright 2015-2023 Jack Humbert
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

#ifdef AUDIO_ENABLE
#    define STARTUP_SONG SONG(PLANCK_SOUND)
// #define STARTUP_SONG SONG(NO_SOUND)

#    define DEFAULT_LAYER_SONGS \
        { SONG(QWERTY_SOUND), SONG(COLEMAK_SOUND), SONG(DVORAK_SOUND) }
#endif

/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
// #define MIDI_ADVANCED

/*
 * Encoder options
 */
// #define PLANCK_ENCODER_SETTLE_PIN_STATE_DELAY 20
// #define ENCODER_MAP_KEY_DELAY 10
// #define ENCODER_RESOLUTION 4


#define BACKLIGHT_LEVELS 2
// #define BACKLIGHT_BREATHING
// #define BREATHING_PERIOD 5

#define TAPPING_TERM 50
#define TAPPING_TERM_PER_KEY

// #define AUDIO_INIT_DELAY