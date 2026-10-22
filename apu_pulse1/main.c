// pulse test for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // ライブラリ
#include "pattern.h"   // タイルセット

extern void waitvsync(void); // Vブランク待ち

#define INIT 0
#define RANGE 1
#define KETA 2
#define MENUMAX 9
const unsigned char btnname[MENUMAX][12] = {
    "  NOTE 60+:", // 0
    " SWEEP VAL:", // 1
    "SWP PERIOD:", // 2
    "SWP NEGATE:", // 3
    "  VOL FLAG:", // 4
    "VOL/ENV_DV:", // 5
    "  ENV LOOP:", // 6
    "LENGTH CNT:", // 7
    "      DUTY:"  // 8
};

// 初期値、レンジ、ケタ
const char menus[MENUMAX][3]={
    { 9,13,2}, // 0:note 60+(0～12)
    { 4, 8,1}, // 1:スイープ変化量(0～7)
    { 4, 8,1}, // 2:スイープ時間(0～7)
    { 1, 2,1}, // 3:スイープ音程(0～1)
    { 1, 2,1}, // 4:ボリュームフラグ(0～1)
    { 8,16,2}, // 5:音量/ループ間隔(0～15)
    { 1, 2,1}, // 6:エンベロープループ(0～1)
    { 0,32,2}, // 7:長さカウンタ(0～31)
    { 2, 4,1}  // 8:デューティ(0～3)
};

char arg[MENUMAX];

// 周波数テーブル
const unsigned short freq_table[] = {
    262,  // C (ド)   60
    277,  // C#       61
    294,  // D (レ)   62
    311,  // D#       63
    330,  // E (ミ)   64
    349,  // F (ファ) 65
    370,  // F#       66
    392,  // G (ソ)   67
    415,  // G#       68
    440,  // A (ラ)   69
    466,  // A#       70
    494,  // B (シ)   71
    523   // C (ド)   72
};

//---- パラメータのデクリメント
char dec_value(char value,char range)
{
    if(value == 0) return(range-1);

    return(value -1);
}

//---- メイン関数
char NesMain()
{
    unsigned short hz,tmpfreq;
    char enveloop,length,duty;
    char volflag, vol_div;   
    char sw_enable, sw_value, sw_period, sw_negate; 
    char idx=0,idx_old=1;
    char i;
    unsigned char button,button_old=0;
    char tempstr[6];

    fc_init(); // 全機能の初期化
    // パターン設定 & BG描画
    ppu_pattern((unsigned char *)pattern,0,512);
    bg_printstr(2,2,(char *)"PULSE TEST  [A]PLAY");
    for(i=0; i<MENUMAX; i++){
        bg_printstr(5,4+i,(char *)btnname[i]);
        arg[i] = menus[i][INIT];
    }
    waitvsync();
    ppu_enable(1);    // 画面表示を有効化
    while (1){
        button = controller1();   // コントローラー1入力
        if(button != button_old){
            button_old = button;
            switch(button){
            case BUTTON_UP:
                idx = dec_value(idx,MENUMAX);
                break;
            case BUTTON_DOWN:
                idx = (idx + 1) % MENUMAX;
                break;
            case BUTTON_RIGHT:
                arg[idx] = (arg[idx]+1) % menus[idx][RANGE];
                break;
            case BUTTON_LEFT:
                arg[idx] = dec_value(arg[idx],menus[idx][RANGE]);
                break;
            case BUTTON_A:    // 再生
                hz = freq_table[arg[0]];     // 周波数
                sw_value = arg[1];  // スイープ変化量(0～7)
                sw_period = arg[2]; // スイープ時間(0～7)
                sw_negate = arg[3]; // スイープ音程(0=低音化/1=高音化) 
                volflag = arg[4];   // ボリュームフラグ(0=ループ間隔/1=ボリューム)
                vol_div = arg[5];   // ループ間隔/ボリューム(0～15)
                enveloop = arg[6];  // エンベロープループ(0=OFF/1=ON＆長さカウンタ無効)
                length = arg[7];    // 長さカウンタ(0～31)
                duty = arg[8];      // デューティ比(0～3)
                if(sw_value==0){
                    sw_enable = 0;  // スイープOFF
                }else{
                    sw_enable = 1;  // スイープON
                }
                // 矩形波1制御レジスタ1,2
                PULSE1CTR1 = (duty<<6)+(enveloop<<5)+(volflag<<4)+vol_div;
                PULSE1CTR2 = (sw_enable<<7)+(sw_period<<4)+(sw_negate<<3)+sw_value;

                // 矩形波1周波数1,2
                tmpfreq = (unsigned short)((1790000/16)/hz)-1;
                PULSE1FREQ1 = tmpfreq & 0xff; 
                PULSE1FREQ2 = (tmpfreq >> 8)+(length << 3);
                break;
            }
        }
        i = (i+1) % MENUMAX;
        num_to_str(tempstr,arg[i],menus[i][KETA]);
        waitvsync();     // Vブランク待ち
        bg_printstr(19,4+i,tempstr);
        if(idx_old != idx){
            bg_printch(3,4+idx_old,' ');
            idx_old = idx;
            bg_printch(3,4+idx    ,'>');
        }
        bg_scroll(0,0);  // BGスクロール
    }
    return 0;
}
