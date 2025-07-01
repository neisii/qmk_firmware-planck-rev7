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
    version: 1.7
*/
#include QMK_KEYBOARD_H

uint8_t current_layer = 0;
bool keyboard_lock_on = false;
bool capslock_on = false;
bool ck_fn_on = false;

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
  CK_PGDN,
  CK_DEL,
  CK_END,
  CK_CTL_ALT,
  CK_FN
};

enum tap_dance_steps {
    TD_MINS_GUI,
    TD_EQL_APP,
    TD_ESC_CK,
    TD_BSPC_CK,
    //TD_LSFT,
    TD_CTL_ALT,
    TD_BSPC_RDEL,
    TD_GRV,
    TD_A,
    TD_C_COPY,
    TD_X_CUT,
    TD_V_PASTE,
    TD_Z_UNDO,
    TD_D_RCOPY,
    TD_DEL,
    TD_PGDN,
    TD_END,
    TD_P_PSCR,
    TD_TAB,
    TD_J_RB,
    TD_K_SB,
    TD_L_CB,
    TD_LBRC,
    TD_SPACE_LOCK,
    TD_SCLN,
    TD_QUOT,
    TD_COM_AB,
    TD_DOT_DOTS,
    TD_TOGG_MS,
    TD_SLSH,
    TD_Z_CK,
    TD_X_CK,
    TD_C_CK,
    TD_V_CK,
    // --- unused
    TD_LR_1_MUTE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [LR_0] = LAYOUT_planck_mit(
        TD(TD_ESC_CK),  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,  KC_Y,  KC_U,  KC_I,  KC_O,  TD(TD_P_PSCR),  TD(TD_BSPC_CK),
        TD(TD_TAB),  TD(TD_A),  KC_S,  KC_D,  KC_F,  KC_G,  KC_H,  KC_J,  KC_K,  KC_L,  TD(TD_SCLN),  KC_ENT,
        KC_LSFT,  KC_Z,  TD(TD_X_CK),  TD(TD_C_CK),  TD(TD_V_CK),  KC_B,  KC_N,  KC_M,  TD(TD_COM_AB),  TD(TD_DOT_DOTS),  TD(TD_SLSH),  CK_END,
        TD(TD_GRV),  TD(TD_MINS_GUI),  TD(TD_EQL_APP),  TD(TD_CTL_ALT),  TT(1),  TD(TD_SPACE_LOCK),  TD(TD_QUOT),  CK_PGDN,  CK_DEL,  CK_FN,  TT(2)
    ),

    [LR_1] = LAYOUT_planck_mit(
        KC_TRNS,  KC_1,  KC_2,  KC_3,  KC_4,  KC_5,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_TRNS,
        XXXXXXX,  KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_TRNS,
        KC_TRNS,  KC_LEFT,  KC_UP,  KC_DOWN,  KC_RGHT,  KC_TRNS,  KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_TRNS,
        SET_DEFAULT_LAYER,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS
    ),

    [LR_2] = LAYOUT_planck_mit(
        KC_TRNS,  QK_BOOT,  DB_TOGG,  EE_CLR,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MPLY,  KC_MPRV,  KC_MNXT,  XXXXXXX,
        KC_TRNS,  MS_LEFT,  MS_UP,  MS_DOWN,  MS_RGHT,  MS_BTN1,  MS_BTN2,  MS_BTN3,  XXXXXXX,  KC_BRID,  KC_BRIU,  XXXXXXX,
        SET_DEFAULT_LAYER,  MS_WHLL,  MS_WHLU,  MS_WHLD,  MS_WHLR,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_TRNS,  KC_TRNS
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
    layer_clear();
    layer_on(LR_0); // 부팅 시 기본 레이어를 0으로 설정
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_BLACK);
    keyboard_lock_on = false;
    capslock_on = false;
    ck_fn_on = false;
}

void init_keyboard_lock_on(void) {
    rgblight_sethsv_at(210, 60, 50,  3); // 우측 하단
}

void toggle_keyboard_lock_on(void) {
    keyboard_lock_on = !keyboard_lock_on;
    printf("keyboard_lock_on is %d\n", keyboard_lock_on);

    if (keyboard_lock_on) {
        init_keyboard_lock_on();
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

// 탭 댄스 인식 시간
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (QK_TAP_DANCE <= keycode && keycode <= QK_TAP_DANCE_MAX) {
        return 150; // TD_로 시작하는 모든 Tap Dance 키는 150ms로 설정
    } else if (KC_LCTL == keycode || KC_RCTL == keycode) { // KC_LSFT == keycode || CK_FN == keycode ||
        return TAPPING_TERM + 100; // 모디키는 TAPPING_TERM + 100ms로 설정
    }
    return TAPPING_TERM; // 나머지 키는 기본값 사용
}

void grave_tilde_td_finished(tap_dance_state_t *state, void *user_data) {
    if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 물결
        register_code16(KC_TILDE);
    } if (state->count == 2) { // 백틱 닫기
        SEND_STRING("``");
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_GRV);
    }
}

void grave_tilde_td_reset(tap_dance_state_t *state, void *user_data) {
    if ((get_mods() & MOD_MASK_SHIFT) && state->count == 1) { // 물결
        unregister_code16(KC_TILDE);
    } if (state->count == 2) { // 백틱 닫기
        tap_code(KC_LEFT);
    } else if (state->count == 1) { // 표준 입력
        unregister_code(KC_GRV);
    }
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
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_ESC);
    }
}
void esc_cycle_td_reset(tap_dance_state_t *state, void *user_data) {
     if (!ck_fn_on && state->count == 1) { // 표준 입력
        unregister_code(KC_ESC);
    }
}

void bspc_cycle_td_finished(tap_dance_state_t *state, void *user_data) {
     if (ck_fn_on && state->count == 1) { // RAISE
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
    } else if ((get_mods() == MOD_BIT(KC_LCTL)) && state->count == 1) { // 표준 입력
        // 현재 커서 위치에서 끝까지 블록 삭제
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
        tap_code(KC_DEL);
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_BSPC);
    }
}
void bspc_cycle_td_reset(tap_dance_state_t *state, void *user_data) {
    if ((get_mods() != MOD_BIT(KC_LCTL)) && !ck_fn_on && state->count == 1) { // 표준 입력
        unregister_code(KC_BSPC);
    }
}

void z_ck_td_finished(tap_dance_state_t *state, void *user_data) {
     /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // left
        register_code(KC_LEFT);
    } else  */if (state->count == 1) { // 표준 입력
        register_code(KC_Z);
    }
}
void z_ck_td_reset(tap_dance_state_t *state, void *user_data) {
     /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 2) { // left
        unregister_code(KC_LEFT);
    } else  */if (state->count == 1) { // 표준 입력
        unregister_code(KC_Z);
    }
}

void x_ck_td_finished(tap_dance_state_t *state, void *user_data) {
     /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // up
        printf("arrow up\n");
        register_code(KC_UP);
    } else  */if (state->count == 2) { // 잘라내기
        tap_code16(LCTL(KC_X));
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_X);
    }
}
void x_ck_td_reset(tap_dance_state_t *state, void *user_data) {
    /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // up
        unregister_code(KC_UP);
    } else  */if (state->count == 1) { // 표준 입력
        unregister_code(KC_X);
    }
}

void c_ck_td_finished(tap_dance_state_t *state, void *user_data) {
    /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // down
        printf("arrow down\n");
        register_code(KC_DOWN);
    } else  */if (state->count == 2) { // 복사하기
        tap_code16(LCTL(KC_C));
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_C);
    }
}
void c_ck_td_reset(tap_dance_state_t *state, void *user_data) {
    /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // down
        unregister_code(KC_DOWN);
    } else  */if (state->count == 1) { // 표준 입력
        unregister_code(KC_C);
    }
}

void v_ck_td_finished(tap_dance_state_t *state, void *user_data) {
     /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // right
        printf("arrow right\n");
        register_code(KC_RGHT);
    } else */ if (state->count == 2) { // 붙여넣기
        tap_code16(LCTL(KC_V));
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_V);
    }
}
void v_ck_td_reset(tap_dance_state_t *state, void *user_data) {
     /* if ((get_mods() == MOD_BIT(MOD_CK)) && state->count == 1) { // right
        unregister_code(KC_RGHT);
    } else  */if (state->count == 1) { // 표준 입력
        unregister_code(KC_V);
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

void backspace_row_delete_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 한줄 지우기
        tap_code(KC_HOME);
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
        tap_code(KC_DEL);
    } else if (state->count == 1) { // 백스페이스
        tap_code(KC_BSPC);
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
    } else if (state->count == 1) { // 탭
        register_code(KC_TAB);
    }
}
void tab_capslock_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { // 탭
        unregister_code(KC_TAB);
    }
}

void slash_ck_td_finished(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 2) { // 파이프
        register_code16(KC_PIPE);
    } else if (ck_fn_on && state->count == 1) { // 백슬래시
        register_code16(KC_BACKSLASH);
    } else if ((get_mods() == MOD_BIT(KC_LSFT)) && state->count == 1) { // 물음표
        register_code16(KC_QUESTION);
    } else if (state->count == 1) { // 슬래시
        register_code(KC_SLASH);
    }
}
void slash_bslash_td_reset(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 2) { // 파이프
        unregister_code16(KC_PIPE);
    } else if (ck_fn_on && state->count == 1) { // 백슬래시
        unregister_code16(KC_BACKSLASH);
    } else if ((get_mods() == MOD_BIT(KC_LSFT)) && state->count == 1) { // 물음표
        unregister_code16(KC_QUESTION);
    } else if (state->count == 1) { // 슬래시
        unregister_code(KC_SLASH);
    }
}

void semicolon_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 콜론
        register_code16(KC_COLON);
    } else if (state->count == 1) { // 세미콜론
        register_code16(KC_SEMICOLON);
    }
}
void semicolon_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 콜론
        unregister_code16(KC_COLON);
    } else if (state->count == 1) { // 세미콜론
        unregister_code16(KC_SEMICOLON);
    }
}

void quot_won_td_finished(tap_dance_state_t *state, void *user_data) {
    if (ck_fn_on && state->count == 1) { // 원 문자
        SEND_STRING("\\");
    } else if (ck_fn_on && state->count == 2) { // 파이프 문자
        tap_code16(KC_PIPE);
    } else if (state->count == 2) { // 쌍따옴표 닫기
        SEND_STRING("\"");
        // register_code(KC_LSFT);
        // tap_code(KC_QUOT);
        // tap_code(KC_QUOT);
        // unregister_code(KC_LSFT);
    } else if (state->count == 1) { // 따옴표 닫기
        SEND_STRING("\'");
    }
}
void quot_won_td_reset(tap_dance_state_t *state, void *user_data) {
    // if (state->count > 3) { // 원 문자 제외 나머지
    //     tap_code(KC_LEFT);
    // }
}

// void j_rbracket_td_finished(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 2) { // 둥근괄호
//         SEND_STRING("()");
//     } else if (state->count == 1) { // 표준 입력
//         register_code(KC_J);
//     }
// }
// void j_rbracket_td_reset(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 1) { // 표준 입력
//         unregister_code(KC_J);
//     }
// }

// void k_sbracket_td_finished(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 2) { // 대괄호
//         SEND_STRING("[]");
//     } else if (state->count == 1) { // 표준 입력
//         register_code(KC_K);
//     }
// }
// void k_sbracket_td_reset(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 1) { // 표준 입력
//         unregister_code(KC_K);
//     }
// }

// void l_cbracket_td_finished(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 2) { // 중괄호
//         SEND_STRING("{}");
//     } else if (state->count == 1) { // 표준 입력
//         register_code(KC_L);
//     }
// }
// void l_cbracket_td_reset(tap_dance_state_t *state, void *user_data) {
//     if (state->count == 1) { // 표준 입력
//         unregister_code(KC_L);
//     }
// }

void comma_abracket_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 꺾은괄호
        SEND_STRING("<>");
    } else if (state->count == 1) { // 표준 입력
        register_code(KC_COMMA);
    }
}
void comma_abracket_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { // 표준 입력
        unregister_code(KC_COMMA);
    }
}

void dot_td_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) { // 점 세개 입력
        SEND_STRING("...");
    } else if (state->count == 1) { // 표준 입력
       register_code(KC_DOT);
    }
}
void dot_td_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { // 표준 입력
        unregister_code(KC_DOT);
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
    [TD_A] = ACTION_TAP_DANCE_DOUBLE(KC_A, LCTL(KC_A)), // A SEL_ALL
    [TD_DEL] = ACTION_TAP_DANCE_DOUBLE(KC_DEL, KC_INS), // DEL INSERT
    [TD_PGDN] = ACTION_TAP_DANCE_DOUBLE(KC_PGDN, KC_PGUP), // PGDN PGUP
    [TD_END] = ACTION_TAP_DANCE_DOUBLE(KC_END, KC_HOME), // DEL INSERT
    [TD_P_PSCR] = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_PSCR),  // P 두번 Print Screen

    // functional
    [TD_ESC_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_cycle_td_finished, esc_cycle_td_reset),
    [TD_BSPC_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, bspc_cycle_td_finished, bspc_cycle_td_reset),

    // arrows
    [TD_Z_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, z_ck_td_finished, z_ck_td_reset),
    [TD_X_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_ck_td_finished, x_ck_td_reset),
    [TD_C_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, c_ck_td_finished, c_ck_td_reset),
    [TD_V_CK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, v_ck_td_finished, v_ck_td_reset),

    [TD_MINS_GUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mins_gui_td_finished, mins_gui_td_reset),

    [TD_EQL_APP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, eql_app_td_finished, eql_app_td_reset),

    [TD_CTL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctrl_alt_td_finished, ctrl_alt_td_reset),

    [TD_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tab_capslock_td_finished, tab_capslock_td_reset),
    [TD_SPACE_LOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, space_kblock_td_finished, space_kblock_td_reset),
    [TD_GRV]=  ACTION_TAP_DANCE_FN_ADVANCED(NULL, grave_tilde_td_finished, grave_tilde_td_reset),

    // [TD_J_RB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, j_rbracket_td_finished, j_rbracket_td_reset),
    // [TD_K_SB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, k_sbracket_td_finished, k_sbracket_td_reset),
    // [TD_L_CB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l_cbracket_td_finished, l_cbracket_td_reset),

    [TD_SCLN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, semicolon_td_finished, semicolon_td_reset),
    [TD_QUOT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, quot_won_td_finished, quot_won_td_reset),

    [TD_COM_AB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, comma_abracket_td_finished, comma_abracket_td_reset),
    [TD_DOT_DOTS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dot_td_finished, dot_td_reset),
    [TD_SLSH] =  ACTION_TAP_DANCE_FN_ADVANCED(NULL, slash_ck_td_finished, slash_bslash_td_reset),

    // --- unused
    [TD_BSPC_RDEL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, backspace_row_delete_finished, NULL),
    [TD_D_RCOPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, d_row_copy_paste_finished, NULL),
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

        case CK_FN: // User Defined Functional Keycode
            if (record->event.pressed) {
                printf("fn pressed\n");
                ck_fn_on = 0;
                toggle_ck_fn_on();
                
            } else {  
                printf("fn unpressed\n");
                ck_fn_on = 1;
                toggle_ck_fn_on();
            }
            return false;
            break;

      case CK_DEL:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("insert pressed\n");
                register_code(KC_INS);
              } else {
                printf("delete pressed\n");
                register_code(KC_DEL);
              }
        } else {              
              if (ck_fn_on) {
                printf("insert unpressed\n");
                unregister_code(KC_INS);
              } else {
                printf("delete unpressed\n");
                unregister_code(KC_DEL);
              }
        }
        return false;
        break;
      case CK_PGDN:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("pgup pressed\n");
                register_code(KC_PGUP);
              } else {
                printf("pgdn pressed\n");
                register_code(KC_PGDN);
              }
        } else {
              if (ck_fn_on) {
                printf("pgup unpressed\n");
                unregister_code(KC_PGDN);
              } else {
                printf("pgdn unpressed\n");
                unregister_code(KC_PGDN);
              }
        }
        return false;
        break;
      case CK_END:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("home pressed\n");
                register_code(KC_HOME);
              } else {
                printf("end pressed\n");
                register_code(KC_END);}
        } else {
              if (ck_fn_on) {
                printf("home unpressed\n");
                unregister_code(KC_HOME);
              } else {
                printf("end unpressed\n");
                unregister_code(KC_END);}
        }
        return false;
        break;

      case KC_J:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("rb pressed\n");
                SEND_STRING("()");
              } else {
                printf("kc_j pressed\n");
                register_code(KC_J);
              }
        } else {
              if (ck_fn_on) {
                printf("rb typed\n");
              } else {
                printf("kc_j unpressed\n");
                unregister_code(KC_J);
              }
        }
        return false;
        break;     
      case KC_K:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("sb pressed\n");
                SEND_STRING("[]");
              } else {
                printf("KC_K pressed\n");
                register_code(KC_K);
              }
        } else {
              if (ck_fn_on) {
                printf("sb typed\n");
              } else {
                printf("KC_K unpressed\n");
                unregister_code(KC_K);
              }
        }
        return false;
        break;     
      case KC_L:
        if (record->event.pressed) {
              if (ck_fn_on) {
                printf("cb pressed\n");
                SEND_STRING("{}");
              } else {
                printf("KC_L pressed\n");
                register_code(KC_L);
              }
        } else {
              if (ck_fn_on) {
                printf("cb typed\n");
              } else {
                printf("KC_L unpressed\n");
                unregister_code(KC_L);
              }
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
