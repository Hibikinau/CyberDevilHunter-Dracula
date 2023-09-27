#pragma once

//player
#define PL_IDLE 0
#define PL_RUN 1
#define PL_DAMAGE 2
#define PL_DEATH 3
#define PL_BLOWN_AWAY_1 40
#define PL_BLOWN_AWAY_2 41
#define PL_STAGGER 42
//回避
#define PL_DODGE_F 4
#define PL_DODGE_R 5
#define PL_DODGE_L 6
#define PL_DODGE_B 7
//弱攻撃
#define PL_JAB_1 8
#define PL_JAB_1E 9
#define PL_JAB_2 10
#define PL_JAB_2E 11
#define PL_JAB_3 12
#define PL_JAB_3E 13
#define PL_JAB_4_1 14
#define PL_JAB_4_2 15
#define PL_JAB_4_3 16
#define PL_JAB_4_4 17
#define PL_JAB_4_4E 18
//強攻撃
#define PL_FIERCE_1 19
#define PL_FIERCE_1E 20
#define PL_FIERCE_2 21
#define PL_FIERCE_2E 22
#define PL_FIERCE_3 23
#define PL_FIERCE_3E 24
#define PL_FIERCE_4_1 25
#define PL_FIERCE_4_2 26
#define PL_FIERCE_4_3 27
#define PL_FIERCE_4_3E 28
//防御系
#define PL_GUARD_1 29
#define PL_GUARD_2 30
#define PL_COUNTER 31
//入れ替え技
#define PL_ARTS_THRUST_1 32
#define PL_ARTS_THRUST_2 33
#define PL_ARTS_THRUST_3 34
#define PL_ARTS_MOWING_R 35
#define PL_ARTS_MOWING_L 36
#define PL_ARTS_CUT 37
#define PL_ARTS_REMOTE 38
//必殺
#define PL_MOTION_SPECIAL 31

//ラスボス
#define BOSS_IDLE_1 32 //仰ぎ
#define BOSS_IDLE 33 //爪
#define BOSS_KICK 34 //ハイキック
#define BOSS_SPIN 35 //回転切り
#define BOSS_JUMP_ATTACK 36 //ジャンプ攻撃
#define BOSS_MAGIC_1 37 //魔法　片手
#define BOSS_MAGIC_2 38 //魔法　両手
#define BOSS_ARTS_THRUST_1 26
#define BOSS_ARTS_THRUST_2 27
#define BOSS_ARTS_THRUST_3 28

//騎士
#define BOSS1_IDLE 0
#define BOSS1_RUN 23
#define BOSS1_DEAD 2
#define BOSS1_DAMAGE 24
//攻撃(薙ぎ払い
#define BOSS1_MOWING 1
//攻撃(たたきつけ
#define BOSS1_SLAP_L1 25
#define BOSS1_SLAP_L2 26
#define BOSS1_SLAP_L3 27
#define BOSS1_SLAP_R1 7
#define BOSS1_SLAP_R2 8
#define BOSS1_SLAP_R3 9
#define BOSS1_SLAP_M1 10
#define BOSS1_SLAP_M2 11
#define BOSS1_SLAP_M3 12
//攻撃(突き
#define BOSS1_THRUST_1 13
#define BOSS1_THRUST_2 14
#define BOSS1_THRUST_3 15
#define BOSS1_THRUST_4 16
//攻撃(袈裟斬り
#define BOSS1_CUT_1 17
#define BOSS1_CUT_2 18
#define BOSS1_CUT_3 19
//攻撃(ジャンプ攻撃
#define BOSS1_JUMP_A1 20
#define BOSS1_JUMP_A2 21
#define BOSS1_JUMP_A3 22
//回避
#define BOSS1_DODGE_R 3
#define BOSS1_DODGE_F 4
#define BOSS1_DODGE_L 5
#define BOSS1_DODGE_B 6

//ライオンマン
#define BOSS2_IDLE 0
#define BOSS2_RUN 1
#define BOSS2_DAMAGE 2
#define BOSS2_DOWN 3
#define BOSS2_DEAD 4
#define BOSS2_DODGE 5
//攻撃
#define BOSS2_ATTACK_1 6
#define BOSS2_ATTACK_2 7
#define BOSS2_ATTACK_3 8
#define BOSS2_KNEE 9
#define BOSS2_TACKLE 10
#define BOSS2_SLAP 11
#define BOSS2_DIVE 12