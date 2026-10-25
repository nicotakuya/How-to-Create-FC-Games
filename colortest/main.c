// Color Test for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void);  // Vブランク待ち

char palette[16];  // BG用パレット(4×4色)

//---- BG塗りつぶし(x,y,幅,高さ)
void bg_fill(char x,char y,char w,char h,char ch)
{
    char x1,y1;

    for (y1=0; y1<h; y1++){
        for (x1=0; x1<w; x1++){
            bg_printch(x+x1,y+y1,ch);
        }
    }
}

//---- スクリーン初期化
void screen_init(void)
{
    char x,y;
    char pal,i;
    unsigned char attr;
    char tempstr[6]="PAL._";

    ppu_enable(0);    //表示OFF
    // BGに描画
    for (pal=0; pal<4; pal++){
        x = pal*8;
        for (i=0; i<3; i++){
            // BG塗りつぶし4×4
            y = (i*8)+8;
            bg_fill(x,y,4,4,i+1);
            // BGアトリビュート設定
            attr = 0b01010101 * pal;
            bg_attrib(x,y,attr);
        }
        // パレット番号表示
        y = 4;
        tempstr[4] = '0'+pal;
        bg_printstr(x,y,tempstr);
    }
    waitvsync();      // Vblank待ち
    ppu_enable(1);    // 表示ON
}

//---- パレット変更(色の種類,明るさのオフセット)
void palette_change(char htemp,char btemp)
{
    char i,pal,color;
    char x,y;
    char tempstr[5]="0x__";

    waitvsync();      // Vblank待ち
    ppu_enable(0);    // 表示OFF
    for(pal=0; pal<4; pal++){
        for(i=0; i<3; i++){
            color = (i*0x10)+htemp+btemp; // カラーコード
            palette[(pal*4)+i+1] = color; // パレットに格納
            x = pal*8;
            y = (i*8)+7;
            num_to_hex(tempstr+2,color); // 16進数表示
            bg_printstr(x,y,tempstr);
        }
        htemp = (htemp+1) % 13;  // 色の種類(0-12)
    }
    // パレット設定
    palette[0] = 0x0F;
    ppu_palette((char *)palette,PAL_0,4);
    waitvsync();      // Vblank待ち
    ppu_enable(1);    // 表示ON
}

// メイン
char NesMain()
{
    unsigned char loopcnt = 0;
    char hue = 0;     // 色の種類
    char bright = 0;  // 明るさ

    fc_init();   // 全機能を初期化
    ppu_pattern((unsigned char *)pattern,0,512);
    screen_init();               // BGを描画
    palette_change(hue,bright);  // パレット変更
    while (1){
        loopcnt++;
        if(loopcnt >= 180){ // 3秒経過
            loopcnt = 0;
            hue++;  // 色の種類を循環
            if(hue >= 13){
                hue = 0;
                bright ^= 0x10; // 明るさを変更
            }
            palette_change(hue,bright);
        }
        waitvsync();     // Vblank待ち
        bg_scroll(0, 0);
    }
    return 0;
}
