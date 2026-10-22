// スプライトとBGとの衝突 for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

const char palette_bg[] = { // カラーパレット
    C_BLACK, C_GRAY , C_GREEN , C_WHITE  // BGパレット0
};

const char palette_sp[] = { // カラーパレット
    C_BLACK, C_BLUE , C_RED   , C_YELLOW // SPパレット0
};

// メイン関数
char NesMain(void)
{
    unsigned char x,y,hit;
    char x1 = 1;
    unsigned char player;
    unsigned short adr;
    char tmpstr[3];
    fc_init();      // 全機能の初期化
    // パターンテーブル設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);
    // カラーパレット設定
    ppu_palette((char *)palette_bg,PAL_0  ,1);
    ppu_palette((char *)palette_sp,PAL_0+4,1);
    // BGに描画
    bg_printstr(8,20,"BG HIT:");
    for(y=10; y<18; y+=2){
        for(x=16; x<24; x+=2){
            bg_print2x2(x,y,0xe2);
        }
    }
    // スプライト追加
    player = sp_append(64,96,0+1,PAL_0);
    waitvsync();          // Vブランク待ち
    ppu_enable(1);
    while (1){
        x = sp_getx(player);
        y = sp_gety(player);
        x += x1;
        if((x<64)||(x>192))x1 = -x1;
        sp_ofs(player,x,y); // スプライトの移動
        x /= 8;
        y /= 8;
        adr = 0x2000+x+((unsigned short)y*32);
        waitvsync();          // Vブランク待ち
        sp_dmastart();        // OMA DMA転送開始
        ppu_address( adr );
        hit = PPUDATA;        // ネームテーブルの読み取り
        num_to_hex(tmpstr,hit);
        bg_printstr(19,20,tmpstr);  // 衝突結果
        bg_scroll(0,0);       // BGスクロール
    }
    return 0;
}

