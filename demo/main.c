// FC Library Demo for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

char x1[32],y1[32];   // 移動量

// メイン関数
char NesMain(void)
{
    char num;
    unsigned char x,y,tile;
    short bgx;

    fc_init();      // 全機能の初期化
    // パターンテーブル設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);

    // BGの描画
    for(y=0; y<30; y+=2){
        for(x=0; x<32; x+=2){
            if((x>4)&&(x<26)&&(y>4)&&(y<24)) continue;
            tile = (random() % 3)*2 + 0xe0;
            bg_print2x2(x,y,tile);
        }
    }
    bg_printstr(8,15,"FC LIBRARY DEMO");

    // スプライト追加
    for(num=0; num<32; num++){
        // 座標、移動方向
        x = (num % 4)*32+16;
        y = (num / 4)*24+16;
        x1[num] = (char)(random() % 5)-2;
        y1[num] = (char)(random() % 5)-2;
        // SP追加(0x1000番地以降のタイル画像は+1)
        sp_append(x,y,(num*8)+1,PAL_0);
    }
    waitvsync();      // Vブランク待ち
    ppu_enable(1);    // 表示を有効化

    num = 0;  // スプライト番号
    while (1){
        x = sp_getx(num);
        y = sp_gety(num);
        x += x1[num];
        y += y1[num];
        if((x <= 8)||(x >= 240)){
            x1[num] = -x1[num];
        }
        if((y <= 8)||(y >= 216)){
            y1[num] = -y1[num];
        }
        sp_ofs(num,x,y);  // スプライト移動
        sp_anime(num);    // アニメ表示
        num = (num+1) % 32;  // スプライト番号を切り替え
        bgx = (bgx+1) & 511; // BGオフセット座標
        waitvsync();        // Vブランク待ち
        sp_dmastart();      // OMA DMA転送開始
        bg_scroll(bgx,0);   // BGスクロール
    }
    return 0;
}
