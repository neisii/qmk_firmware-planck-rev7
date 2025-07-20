/* Copyright 2015-2021 Jack Humbert
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

/*
    Planck/Rev7
    version: 2.2
*/
#include QMK_KEYBOARD_H
#include <string.h> // memset

uint8_t current_layer = 0;
bool keyboard_lock_on = false;
bool capslock_on = false;
bool ck_fn_on = false;
bool ck_fn2_on = false;

// // 입력 언어 전환
// bool is_hangul = false;

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     if (keycode == KC_LANG1 && record->event.pressed) {
//         is_hangul = true;
//     } else if (keycode == KC_LANG2 && record->event.pressed) {
//         is_hangul = false;
//     }
//     return true;
// }


enum planck_layers {
  LR_0, LR_1, LR_2
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER_LAYER, // prev layer
  RAISE_LAYER, // next layer
  KC_LR_CYCLE, // layer cycle
  SET_DEFAULT_LAYER, // layer toggle to LR_0
  MOD_CK,
  CK_CTL_ALT,
  CK_FN,
  CK_FN2,
  SEL_LINE
};

enum tap_dance_steps {
    TD_MINS_GUI,
    TD_EQL_APP,
    TD_ESC_CK,
    TD_CTL_ALT,
    TD_P_PSCR,
    TD_TAB,
    TD_SPACE_LOCK,
    TD_TOGG_MS
};

typedef struct {
    bool ck_fn;
    bool ck_fn2;
    bool kc_del;
    bool kc_pgdn;
    bool kc_end;
    bool kc_t;
    bool kc_y;
    bool kc_g;
    bool kc_h;
    bool kc_b;
    bool kc_n;
    bool kc_slsh;
    bool kc_quot;
} custom_state_t;
custom_state_t custom_state = {0}; // All fields set to false

typedef struct {
    bool ck_fn;
    bool ck_fn2;
    bool kc_ins;
    bool kc_pgup;
    bool kc_home;
    bool kc_l_round_brc;
    bool kc_r_round_brc;
    bool kc_l_square_brc; 
    bool kc_r_square_brc;
    bool kc_l_culry_brc;
    bool kc_r_culry_brc;
    bool kc_bsls; // backslash
    bool kc_grv;
} custom_sending_t;
custom_sending_t custom_sending = {0}; // All fields set to false


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [LR_0] = LAYOUT_planck_mit(
        TD(TD_ESC_CK),  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,  KC_Y,  KC_U,  KC_I,  KC_O,  TD(TD_P_PSCR),  KC_BSPC,
        TD(TD_TAB),  KC_A,  KC_S,  KC_D,  KC_F,  KC_G,  KC_H,  KC_J,  KC_K,  KC_L,  KC_SCLN,  KC_QUOT,
        KC_LSFT,  KC_Z,   KC_X,  KC_C,  KC_V,  KC_B,  KC_N,  KC_M,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_ENT,
        CK_FN2,  TD(TD_MINS_GUI),  TD(TD_EQL_APP),  TD(TD_CTL_ALT),  TT(1),  TD(TD_SPACE_LOCK),  TT(2),  KC_PGDN,  KC_END,  CK_FN,  KC_DEL
    ),

    [LR_1] = LAYOUT_planck_mit(
        KC_TRNS,  KC_1,  KC_2,  KC_3,  KC_4,  KC_5,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_TRNS,
        KC_TRNS,  KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_UP,  KC_DOWN,  KC_RGHT,  KC_TRNS,  KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  SET_DEFAULT_LAYER
    ),

    [LR_2] = LAYOUT_planck_mit(
        KC_TRNS,  QK_BOOT,  DB_TOGG,  EE_CLR,  XXXXXXX,  AU_ON,  AU_OFF,  AU_TOGG,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,
        KC_TRNS,  MS_WHLL,  MS_WHLU,  MS_WHLD,  MS_WHLR,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  MS_LEFT,  MS_UP,  MS_DOWN,  MS_RGHT,  MS_BTN1,  MS_BTN2,  MS_BTN3,  XXXXXXX,  KC_BRID,  KC_BRIU,  XXXXXXX,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  SET_DEFAULT_LAYER
    )

};

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
float caps_on_song[][2]    = SONG(CAPS_LOCK_ON_SOUND);
float caps_off_song[][2]    = SONG(CAPS_LOCK_OFF_SOUND);
float major_song[][2]    = SONG(MAJOR_SOUND);
float minor_song[][2]    = SONG(MINOR_SOUND);
float guitar_song[][2]    = SONG(GUITAR_SOUND);
float violin_song[][2]    = SONG(VIOLIN_SOUND);
#endif

bool play_encoder_melody(uint8_t index, bool clockwise);
/*
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
} */


/*************************** 사용자 정의 함수 ***************************/
void keyboard_init_user(void) {
    print("keyboard init\n");
    clear_keyboard(); // 모든 키 입력 Hold 및 mod 해제
    layer_clear();
    layer_on(LR_0); // 부팅 시 기본 레이어를 0으로 설정
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_BLACK);
    keyboard_lock_on = false;
    capslock_on = false;
    ck_fn_on = false;
    ck_fn2_on = false;
}

void init_capslock_on(void) {
    rgblight_sethsv_at(255, 60, 100,  6);  // 좌측 상단
}

void toggle_capslock_on(void) {
    capslock_on = !capslock_on;
    printf("capslock_on is %d\n", capslock_on);

    if (capslock_on) {
        #ifdef AUDIO_ENABLE
        PLAY_SONG(caps_on_song);
        #endif
        init_capslock_on();
    } else {
        #ifdef AUDIO_ENABLE
        PLAY_SONG(caps_off_song);
        #endif
        rgblight_sethsv_at(255, 0, 0, 6);
    }
}

void init_ck_fn_on(void) {
    rgblight_sethsv_at(255, 60, 100,  1);  // 우측 하단
}

void toggle_ck_fn_on(void) {
    ck_fn_on = !ck_fn_on;
    printf("ck_fn_on is %d\n", ck_fn_on);

    if (ck_fn_on) {
        // #ifdef AUDIO_ENABLE
        // PLAY_SONG(guitar_song);
        // #endif
        init_ck_fn_on();
    } else {
        // #ifdef AUDIO_ENABLE
        // PLAY_SONG(minor_song);
        // #endif
        // toggle_ck_fn_on();
        rgblight_sethsv_at(255, 0, 0, 1);
    }
}

void init_ck_fn2_on(void) {
    rgblight_sethsv_at(255, 60, 100,  1);  // 우측 하단
}

void toggle_ck_fn2_on(void) {
    ck_fn2_on = !ck_fn2_on;
    printf("ck_fn2_on is %d\n", ck_fn2_on);

    if (ck_fn2_on) {
        // #ifdef AUDIO_ENABLE
        // PLAY_SONG(guitar_song);
        // #endif
        init_ck_fn2_on();
    } else {
        // #ifdef AUDIO_ENABLE
        // PLAY_SONG(minor_song);
        // #endif
        // toggle_ck_fn2_on();
        rgblight_sethsv_at(255, 0, 0, 1);
    }
}


void init_keyboard_lock_on(void) {
    rgblight_sethsv_at(210, 60, 50,  3); // 우측 하단
}

void toggle_keyboard_lock_on(void) {
    keyboard_lock_on = !keyboard_lock_on;
    printf("keyboard_lock_on is %d\n", keyboard_lock_on);

    if (keyboard_lock_on) {
        init_keyboard_lock_on();
        toggle_ck_fn_on();
        #ifdef AUDIO_ENABLE
        PLAY_SONG(major_song);
        #endif
    } else {
        rgblight_sethsv_at(255, 0, 0, 3);
        #ifdef AUDIO_ENABLE
        PLAY_SONG(minor_song);
        #endif
    }
}


// 탭 댄스 인식 시간
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (QK_TAP_DANCE <= keycode && keycode <= QK_TAP_DANCE_MAX) {
        return 150; // TD_로 시작하는 모든 Tap Dance 키는 150ms로 설정
    } else if (KC_LCTL == keycode || KC_RCTL == keycode) { // KC_LSFT == keycode || CK_FN == keycode ||
        return TAPPING_TERM + 100; // 모디키는 TAPPING_TERM + 100ms로 설정
    }
    return TAPPING_TERM; // 나머지 키는 기본값 사용
}

void esc_cycle_td_finished(tap_dance_state_t *state, void *user_data) {
     if (ck_fn_on && state->count == 1) { // LOWER
        printf("move prev Layer\n");
        switch (current_layer) {
          case 0:
              layer_move(LR_2);  // LR_0 -> LR_2
              current_layer = 2;
              break;
          case 1:
              layer_move(LR_0);  // LR_1 -> LR_0
              current_layer = 0;
              break;
          case 2:
              layer_move(LR_1);  // LR_2 -> LR_1
              current_layer = 1;
              break;
        }
    } else if (ck_fn2_on && state->count == 1) { // KC_GRV
        printf("grave\n");
        register_code(KC_GRV);

    } else if (state->count == 1) { // 표준 입력
        register_code(KC_ESC);
    }
}
void esc_cycle_td_reset(tap_dance_state_t *state, void *user_data) {
     if (!ck_fn2_on && state->count == 1) { // 표준 입력
        unregister_code(KC_GRV);
    }
     if (!ck_fn_on && state->count == 1) { // 표준 입력
        unregister_code(KC_ESC);
    }
}

void mins_gui_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 3) { // GUI lock
        tap_code16(LGUI(KC_L));
    } else if (state->count == 2) { // gui
        register_code(KC_LGUI);
    } else if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 언더스코어
        register_code16(KC_UNDERSCORE);
    } else if (state->count == 1) { // 마이너스
        register_code(KC_MINS);
    }
}
void mins_gui_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // gui
        unregister_code(KC_LGUI);
    } else if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 언더스코어
        unregister_code16(KC_UNDERSCORE);
    } else if (state->count == 1) { // 마이너스
        unregister_code(KC_MINS);
    }
}

void eql_app_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 메뉴
        tap_code(KC_APP);
    } else if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 플러스
        register_code16(KC_PLUS);
    } else if (state->count == 1) { // 이퀄
        register_code(KC_EQL);
    }
}
void eql_app_td_reset(tap_dance_state_t *state, void *user_data) {
    if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 플러스
        unregister_code16(KC_PLUS);
    } else if (state->count == 1) { // 이퀄
        unregister_code(KC_EQL);
    }
}
void d_row_copy_paste_finished(tap_dance_state_t *state, void *user_data) {
    if ((get_mods() & MOD_MASK_SHIFT) && state->count == 2) { // 한줄 복사
        tap_code(KC_HOME);
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
        SEND_STRING(SS_LCTL("c"));

        tap_code(KC_ESC);
        tap_code(KC_END);
        tap_code(KC_ENTER);
        SEND_STRING(SS_LCTL("v"));

    } else if (state->count == 1) { // 표준 입력
        tap_code(KC_D);
    }
}

void ctrl_alt_td_finished(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 1) {
        printf("lclt lalt hold\n");
        register_code(KC_LCTL);
        register_code(KC_LALT);
    } else if (state->count == 2) {
        register_code(KC_LALT);
    } else if (state->count == 1) {
        register_code(KC_LCTL);
    } 
}
void ctrl_alt_td_reset(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 1) {
        printf("lclt lalt realize\n");
        unregister_code(KC_LCTL);
        unregister_code(KC_LALT);
    } else if (state->count == 2) {
        unregister_code(KC_LALT);
    } else if (state->count == 1) {
        unregister_code(KC_LCTL);
    } 
}

void tab_capslock_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 캡스락
        toggle_capslock_on();
        tap_code(KC_CAPS);
    } else if (ck_fn_on && state->count == 1) { // 둥근 괄호 열기
        SEND_STRING("(");
    } else if (!ck_fn_on && state->count == 1) { // 탭
        register_code(KC_TAB);
    }
}
void tab_capslock_td_reset(tap_dance_state_t *state, void *user_data) {
    if (!ck_fn_on && state->count == 1) { // 탭
        unregister_code(KC_TAB);
    }
}

void space_kblock_td_finished(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 2) { // 키보드 잠금
        toggle_keyboard_lock_on();
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_SPACE);
    }
}
void space_kblock_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { // 표준 입력
        unregister_code(KC_SPACE);
    }
}

// tap_dance_actions 정의
tap_dance_action_t tap_dance_actions[] = {
    [TD_P_PSCR] = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_PSCR),  // P 두번 Print Screen

    // functional
    [TD_ESC_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_cycle_td_finished, esc_cycle_td_reset),

    [TD_MINS_GUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mins_gui_td_finished, mins_gui_td_reset),

    [TD_EQL_APP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, eql_app_td_finished, eql_app_td_reset),

    [TD_CTL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctrl_alt_td_finished, ctrl_alt_td_reset),

    [TD_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tab_capslock_td_finished, tab_capslock_td_reset),
    [TD_SPACE_LOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, space_kblock_td_finished, space_kblock_td_reset)
};

//static uint16_t limit_timer = 185; // KC 키 타이머
//static uint16_t kc_timer; // KC 키 타이머
//static bool kc_pressed = false; // KC 키 눌림 상태

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    /* 키보드 잠금 모드 */
    if (keyboard_lock_on) {
        if (keycode == KC_ENT) { // 잠금 해제
            if (record->event.pressed) {
                toggle_keyboard_lock_on();
                layer_move(LR_0);
                current_layer = 0;
            }
        }
        return false; // 나머지 키 입력 무시
    }

    // 커스텀 키코드 처리
    switch (keycode) {
      case LOWER_LAYER: // 이전 레이어
        if (record->event.pressed) {
          printf("move prev Layer");
          switch (current_layer) {
              case 0:
                  layer_move(LR_2);  // LR_0 -> LR_2
                  current_layer = 2;
                  break;
              case 1:
                  layer_move(LR_0);  // LR_1 -> LR_0
                  current_layer = 0;
                  break;
              case 2:
                  layer_move(LR_1);  // LR_2 -> LR_1
                  current_layer = 1;
                  break;
          }
        }
        return false;
        break;
      case RAISE_LAYER: // 다음 레이어
        if (record->event.pressed) {
          printf("move next Layer");
          switch (current_layer) {
              case 0:
                  layer_move(LR_1);  // LR_0 -> LR_1
                  current_layer = 1;
                  break;
              case 1:
                  layer_move(LR_2);  // LR_1 -> LR_2
                  current_layer = 2;
                  break;
              case 2:
                  layer_move(LR_0);  // LR_2 -> LR_0
                  current_layer = 0;
                  break;
          }
        }
        return false;
        break;
      case KC_LR_CYCLE: // 레이어 순환
        if (record->event.pressed) {
          printf("layer cycle, current_layer is %d\n", current_layer);
              // 현재 레이어에 따라 다음 레이어로 순환
              switch (current_layer) {
                  case 0:
                      layer_move(LR_1);  // LR_0 -> LR_1
                      current_layer = 1;
                      break;
                  case 1:
                      layer_move(LR_2);  // LR_1 -> LR_2
                      current_layer = 2;
                      break;
                  case 2:
                      layer_move(LR_0);  // LR_2 -> LR_0
                      current_layer = 0;
                      break;
              }
        }
        return false;
        break;
      case SET_DEFAULT_LAYER: // 현재 레이어를 LR_0 레이어로 전환
        if (record->event.pressed) {
              printf("layer to LR_0, current_layer is %d\n", current_layer);
              layer_move(LR_0);
              current_layer = 0;
        }
        return false;
        break;

      case CK_FN:// User Defined Functional Keycode
        custom_state.ck_fn = record->event.pressed;
        return false; // 키 자체는 전송 안 함
      case CK_FN2:
        custom_state.ck_fn2 = record->event.pressed;
        return false;

      case KC_DEL: // 딜리트인서트 전환
            custom_state.kc_del = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_DEL);
            } else {
                unregister_code(KC_DEL);
            }
            return false;
      case KC_PGDN: // 페이지업다운 전환
            custom_state.kc_pgdn = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_PGDN);
            } else {
                unregister_code(KC_PGDN);
            }
            return false;
      case KC_END: // 홈앤드 전환
            custom_state.kc_end = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_END);
            } else {
                unregister_code(KC_END);
            }
            return false;

      case KC_T: // 소괄호
            custom_state.kc_t = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_T);
            } else {
                unregister_code(KC_T);
            }
            return false;
      case KC_Y:
            custom_state.kc_y = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_Y);
            } else {
                unregister_code(KC_Y);
            }
            return false;
      case KC_G: // 대괄호
            custom_state.kc_g = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_G);
            } else {
                unregister_code(KC_G);
            }
            return false;
      case KC_H:
            custom_state.kc_h = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_H);
            } else {
                unregister_code(KC_H);
            }
            return false;
      case KC_B: // 중괄호
            custom_state.kc_b = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_B);
            } else {
                unregister_code(KC_B);
            }
            return false;
      case KC_N:
            custom_state.kc_n = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_N);
            } else {
                unregister_code(KC_N);
            }
            return false;

      case KC_SLSH:
            custom_state.kc_slsh = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_SLSH);
            } else {
                unregister_code(KC_SLSH);}
            return false;
      case KC_QUOT:
            custom_state.kc_quot = record->event.pressed;
            if (record->event.pressed && !custom_state.ck_fn) {
                register_code(KC_QUOT);
            } else {
                unregister_code(KC_QUOT);
            }
            return false;
            
      case KC_A: // 전체 선택
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("select all triggered\n");
                tap_code16(LCTL(KC_A));
              } else {
                printf("KC_A pressed\n");
                register_code(KC_A);
              }
        } else {
              if (!ck_fn_on) {
                printf("KC_A unpressed\n");
                unregister_code(KC_A);
              }
        }
        return false;
        break;
      case KC_X: // 잘라내기
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("cut triggered\n");
                tap_code16(LCTL(KC_X));
              } else {
                printf("KC_X pressed\n");
                register_code(KC_X);
              }
        } else {
              if (!ck_fn_on) {
                printf("KC_X unpressed\n");
                unregister_code(KC_X);
              }
        }
        return false;
        break;     
      case KC_C: // 복사하기
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("copy triggered\n");
                tap_code16(LCTL(KC_C));
              } else {
                printf("KC_C pressed\n");
                register_code(KC_C);
              }
        } else {
              if (!ck_fn_on) {
                printf("KC_C unpressed\n");
                unregister_code(KC_C);
              }
        }
        return false;
        break;
      case KC_V: // 붙여넣기
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("paste triggered\n");
                tap_code16(LCTL(KC_V));
              } else {
                printf("KC_L pressed\n");
                register_code(KC_V);
              }
        } else {
              if (!ck_fn_on) {
                printf("KC_L unpressed\n");
                unregister_code(KC_V);
              }
        }
        return false;
        break;
        
      case KC_BSPC: // 백스페이스 매크로
        if (record->event.pressed) {
            if (ck_fn_on) { // RAISE
                printf("move next Layer\n");
                switch (current_layer) {
                    case 0:
                        layer_move(LR_1);  // LR_0 -> LR_1
                        current_layer = 1;
                        break;
                    case 1:
                        layer_move(LR_2);  // LR_1 -> LR_2
                        current_layer = 2;
                        break;
                    case 2:
                        layer_move(LR_0);  // LR_2 -> LR_0
                        current_layer = 0;
                        break;
                }
            } else if ((get_mods() == MOD_BIT(KC_LCTL))) { //TODO: 콤보키 구현 Ctrl Shift Bspc
                if (get_mods() == MOD_BIT(KC_LSFT)) { // 현재 줄 끝까지 블록 삭제
                    tap_code(KC_HOME);
                    register_code(KC_LSFT);
                    tap_code(KC_END);
                    unregister_code(KC_LSFT);
                    tap_code(KC_DEL);
                    return false;
                }
                // 현재 커서 위치에서 끝까지 블록 삭제
                register_code(KC_LSFT);
                tap_code(KC_END);
                unregister_code(KC_LSFT);
                tap_code(KC_DEL);
            } else { // 표준 입력
                register_code(KC_BSPC);
            }
        } else {
            if (!ck_fn_on) {
                printf("KC_BSPC unpressed\n");
                unregister_code(KC_BSPC);
            }
        }
        return false;
        break;
        
      case SEL_LINE: // 한줄 선택
        if (record->event.pressed) {
          printf("select line triggered\n");
          tap_code(KC_HOME);
          register_code(KC_LSFT);
          tap_code(KC_END);
          unregister_code(KC_LSFT);
        }
        return false;
        break;
        
      default:
         return true;  // 기본 동작 수행
    }
    return true;
}

// 레이어 변경에 따른 RGB LED 설정
layer_state_t layer_state_set_user(layer_state_t state) {
    printf("layer state: %d\n", get_highest_layer(state));
    switch (get_highest_layer(state)) {
        case LR_0:
            rgblight_sethsv_noeeprom(HSV_BLACK);
            break;
        case LR_1:
            rgblight_sethsv_noeeprom(HSV_BLACK);
            rgblight_sethsv_at(100, 100, 50,  8); // cyan
            break;
        case LR_2:
            rgblight_sethsv_noeeprom(HSV_BLACK);
            rgblight_sethsv_at(255, 100, 50,  8); // purple
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_BLACK);
            break;
    }

    if (capslock_on) {
        init_capslock_on();
    }

    if (keyboard_lock_on) {
        init_keyboard_lock_on();
    }

    if (ck_fn_on) {
        init_ck_fn_on();
    }

    return state;
}




void matrix_scan_user(void) {
    // RGB logic
    if (custom_state.ck_fn) {
        init_ck_fn_on();
    } else if (custom_state.ck_fn2) {
        init_ck_fn2_on();
    } else {
        rgblight_sethsv_at(255, 0, 0, 1);  // 기본 RGB 상태
    }

    // repeat logic
    if (custom_state.ck_fn && custom_state.kc_del) { // 인서트
        if (!custom_sending.kc_ins) {
            register_code(KC_INS);
            custom_sending.kc_ins = true;
        }
    } else {
        if (custom_sending.kc_ins) {
            unregister_code(KC_INS);
            custom_sending.kc_ins = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_pgdn) { // 페이지업
        if (!custom_sending.kc_pgup) {
            register_code(KC_PGUP);
            custom_sending.kc_pgup = true;
        }
    } else {
        if (custom_sending.kc_pgup) {
            unregister_code(KC_PGUP);
            custom_sending.kc_pgup = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_end) { // 홈
        if (!custom_sending.kc_home) {
            register_code(KC_HOME);
            custom_sending.kc_home = true;
        }
    } else {
        if (custom_sending.kc_home) {
            unregister_code(KC_HOME);
            custom_sending.kc_home = false;
        }
    }

    if (custom_state.ck_fn && custom_state.kc_t) { // 좌 소괄호
        if (!custom_sending.kc_l_round_brc) {
            // Press and hold '(' (Shift + 9)
            register_code(KC_LSFT);
            register_code(KC_9);
            custom_sending.kc_l_round_brc = true;
        }
    } else {
        if (custom_sending.kc_l_round_brc) {
            // Release '('
            unregister_code(KC_9);
            unregister_code(KC_LSFT);
            custom_sending.kc_l_round_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_y) { // 우 소괄호
        if (!custom_sending.kc_r_round_brc) {
            // Press and hold ')' (Shift + 0)
            register_code(KC_LSFT);
            register_code(KC_0);
            custom_sending.kc_r_round_brc = true;
        }
    } else {
        if (custom_sending.kc_r_round_brc) {
            // Release ')'
            unregister_code(KC_0);
            unregister_code(KC_LSFT);
            custom_sending.kc_r_round_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_g) { // 좌 대괄호
        if (!custom_sending.kc_l_square_brc) {
            // Press and hold '[' (KC_LBRC)
            register_code(KC_LBRC);
            custom_sending.kc_l_square_brc = true;
        }
    } else {
        if (custom_sending.kc_l_square_brc) {
            // Release '['
            unregister_code(KC_LBRC);
            custom_sending.kc_l_square_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_h) { // 우 대괄호
        if (!custom_sending.kc_r_square_brc) {
            // Press and hold ']' (KC_RBRC)
            register_code(KC_RBRC);
            custom_sending.kc_r_square_brc = true;
        }
    } else {
        if (custom_sending.kc_r_square_brc) {
            // Release ']'
            unregister_code(KC_RBRC);
            custom_sending.kc_r_square_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_b) { // 좌 중괄호
        if (!custom_sending.kc_l_culry_brc) {
            // Press and hold '{' (Shift + KC_LBRC)
            register_code(KC_LSFT);
            register_code(KC_LBRC);
            custom_sending.kc_l_culry_brc = true;
        }
    } else {
        if (custom_sending.kc_l_culry_brc) {
            // Release '}'
            unregister_code(KC_LBRC);
            unregister_code(KC_LSFT);
            custom_sending.kc_l_culry_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_n) { // 우 중괄호
        if (!custom_sending.kc_r_culry_brc) {
            // Press and hold '}' (Shift + KC_RBRC)
            register_code(KC_LSFT);
            register_code(KC_RBRC);
            custom_sending.kc_r_culry_brc = true;
        }
    } else {
        if (custom_sending.kc_r_culry_brc) {
            // Release '}'
            unregister_code(KC_RBRC);
            unregister_code(KC_LSFT);
            custom_sending.kc_r_culry_brc = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_slsh) { // 백슬래시
        if (!custom_sending.kc_bsls) {
            register_code(KC_BSLS);
            custom_sending.kc_bsls = true;
        }
    } else {
        if (custom_sending.kc_bsls) {
            unregister_code(KC_BSLS);
            custom_sending.kc_bsls = false;
        }
    }
    if (custom_state.ck_fn && custom_state.kc_quot) { // 백틱
        if (!custom_sending.kc_grv) {
            register_code(KC_GRV);
            custom_sending.kc_grv = true;
        }
    } else {
        if (custom_sending.kc_grv) {
            unregister_code(KC_GRV);
            custom_sending.kc_grv = false;
        }
    }
}


// 함수 끝


bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(LR_2)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
    return true;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(LR_0);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(LR_0);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
    return true;
}


bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case KC_END:
    case KC_ESC:
      return false;
    default:
      return true;
  }
}
