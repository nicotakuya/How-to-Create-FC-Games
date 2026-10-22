// スプライトの衝突判定 for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

const char palette[] = { // カラーパレット
    C_BLACK, C_BLUE  , C_RED   , C_YELLOW, // SPパレット0
    0      , C_BLACK , C_GRAY1 , C_WHITE   // SPパレット1
};

#define THRESHOLD 15  // 衝突判定のしきい値

// メイン関数
char NesMain(void)
{
    unsigned char x,y,hit;
    char x1=-1;
    unsigned char player,enemy;
    char tmpstr[3];
    fc_init();      // 全機能の初期化
    // パターンテーブル設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);
    ppu_palette((char *)palette,PAL_0+4,2);    // カラーパレット設定
    player = sp_append(64,96,0+1,PAL_0);    // プレイヤー追加
    enemy = sp_append(128,96,16+1,PAL_1);   // オバケ追加
    bg_printstr(8,20,"SPRITE HIT:");
    waitvsync();          // Vブランク待ち
    ppu_enable(1);
    while (1){
        x = sp_getx(player);
        y = sp_gety(player);
        x += x1;
        if((x<64)||(x>192)) x1 = -x1;
        sp_ofs(player,x,y); // スプライトの移動
        // スプライトの衝突判定
        hit = sp_hitcheck(enemy,x,y,THRESHOLD);
        num_to_hex(tmpstr,hit);
        waitvsync();          // Vブランク待ち
        sp_dmastart();        // OMA DMA転送開始
        bg_printstr(19,20,tmpstr);  // 衝突結果
        bg_scroll(0,0);       // BGスクロール
    }
    return 0;
}

