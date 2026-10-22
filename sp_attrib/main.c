// スプライトのアトリビュート設定 for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

const char palette[] = { // カラーパレット
    C_BLACK, C_BLUE  , C_RED   , C_YELLOW // SPパレット0
};

// メイン関数
char NesMain(void)
{
    unsigned char x,y;
    unsigned char sp1,sp2,sp3,sp4,sp5;
    fc_init();      // 全機能の初期化
    // パターンテーブル設定 &  カラーパレット設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);
    ppu_palette((char *)palette,PAL_0+4,1);
    // BGに描画
    for(y=3; y<27; y++){
        for(x=2; x<30; x++){
            bg_printch(x,y,'Z');
        }
    }
    // スプライト追加
    sp1 = sp_append(64,64,0+1,PAL_0);

    // スプライト追加 左右反転
    sp2 = sp_append(96,64,0+1,PAL_0+ATR_FLIPH);

    // スプライト追加 上下反転
    sp3 = sp_append(64,96,0+1,PAL_0+ATR_FLIPV);

    // スプライト追加 左右反転+上下反転
    sp4 = sp_append(96,96,0+1,PAL_0+ATR_FLIPH+ATR_FLIPV);

    // スプライト追加 BGの奥に表示
    sp5 = sp_append(64,128,0+1,PAL_0+ATR_BEHIND);

    waitvsync();          // Vブランク待ち
    ppu_enable(1);        // 表示有効化
    while (1){
        waitvsync();      // Vブランク待ち
        sp_dmastart();    // OMA DMA転送開始
        bg_scroll(0,0);   // BGスクロール
    }
    return 0;
}
