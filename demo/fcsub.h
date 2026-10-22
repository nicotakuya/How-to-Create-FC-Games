// FC LIBRARY (fcsub.h)
// by Takuya Matsubara
#define PPUCTRL      *(char*)0x2000    // PPU制御レジスタ
#define PPUMASK      *(char*)0x2001    // PPUマスクレジスタ
#define PPUSTATUS    *(char*)0x2002    // PPUステータスレジスタ
#define OAMADDR      *(char*)0x2003    // OAMアドレスポート
#define OAMDATA      *(char*)0x2004    // OAMデータポート
#define PPUSCROLL    *(char*)0x2005    // PPUスクロールレジスタ
#define PPUADDR      *(char*)0x2006    // PPUアドレスレジスタ
#define PPUDATA      *(char*)0x2007    // PPUデータレジスタ
#define JOYPAD1      *(char*)0x4016    // コントローラー1
#define JOYPAD2      *(char*)0x4017    // コントローラー2
#define PULSE1CTR1   *(char*)0x4000    // 矩形波1制御レジスタ1
#define PULSE1CTR2   *(char*)0x4001    // 矩形波1制御レジスタ2
#define PULSE1FREQ1  *(char*)0x4002    // 矩形波1周波数1
#define PULSE1FREQ2  *(char*)0x4003    // 矩形波1周波数2
#define OAMDMA       *(char*)0x4014    // OAM DMAレジスタ
#define SNDCHANNEL   *(char*)0x4015    // サウンドチャンネル制御
#define SPWADDR      0x700             // スプライトワークの格納アドレス

#define C_BLACK      0x0f  // カラーコード 黒色
#define C_RED        0x16  // カラーコード 赤色
#define C_RED0       0x06  // カラーコード 暗い赤色
#define C_RED1       0x16  // カラーコード 赤色
#define C_RED2       0x26  // カラーコード 明るい赤色
#define C_BLUE       0x12  // カラーコード 青色
#define C_BLUE0      0x02  // カラーコード 暗い青色
#define C_BLUE1      0x12  // カラーコード 青色
#define C_BLUE2      0x22  // カラーコード 明るい青色
#define C_GREEN      0x1A  // カラーコード 緑色
#define C_GREEN0     0x0A  // カラーコード 暗い緑色
#define C_GREEN1     0x1A  // カラーコード 緑色
#define C_GREEN2     0x2A  // カラーコード 明るい緑色
#define C_YELLOW     0x28  // カラーコード 黄色
#define C_YELLOW0    0x08  // カラーコード 暗い黄色
#define C_YELLOW1    0x18  // カラーコード 黄色
#define C_YELLOW2    0x28  // カラーコード 明るい黄色
#define C_SKYBLUE    0x22  // カラーコード 空色
#define C_LIMEGREEN  0x29  // カラーコード 黄緑色
#define C_TEAL       0x1C  // カラーコード ティール
#define C_TEAL0      0x0C  // カラーコード 暗いティール
#define C_TEAL1      0x1C  // カラーコード ティール
#define C_TEAL2      0x2C  // カラーコード 明るいティール
#define C_GRAY       0x00  // カラーコード 灰色
#define C_LIGHTGRAY  0x10  // カラーコード 明るい灰色
#define C_GRAY0      0x00  // カラーコード 暗い灰色
#define C_GRAY1      0x10  // カラーコード 灰色
#define C_GRAY2      0x20  // カラーコード 明るい灰色
#define C_PURPLE     0x14  // カラーコード 紫色
#define C_PURPLE0    0x04  // カラーコード 暗い紫色
#define C_PURPLE1    0x14  // カラーコード 紫色
#define C_PURPLE2    0x24  // カラーコード 明るい紫色
#define C_MAGENTA    0x15  // カラーコード マゼンタ
#define C_MAGENTA0   0x05  // カラーコード 暗いマゼンタ
#define C_MAGENTA1   0x15  // カラーコード マゼンタ
#define C_MAGENTA2   0x25  // カラーコード 明るいマゼンタ
#define C_WHITE      0x30  // カラーコード 白色

#define BUTTON_A      0x80   // ボタン A
#define BUTTON_B      0x40   // ボタン B
#define BUTTON_SELECT 0x20   // ボタン SELECT
#define BUTTON_START  0x10   // ボタン START
#define BUTTON_UP     0x08   // ボタン 上
#define BUTTON_DOWN   0x04   // ボタン 下
#define BUTTON_LEFT   0x02   // ボタン 左
#define BUTTON_RIGHT  0x01   // ボタン 右

#define OUTSIDE      240     // 画面外の座標

#define PCTR_SP8X16 (1<<5)   // SP 8x16モード
#define PCTR_BGP1 (1<<4) // BGパターンテーブルを0x1000番地に
#define PCTR_SPP1 (1<<3) // SPパターンテーブルを0x1000番地に

// PPUMASK用
#define PMSK_SHOWSP  (1<<4)  // SPの表示
#define PMSK_SHOWBG  (1<<3)  // BGの表示
#define PMSK_SHOWLSP (1<<2)  // 左8pixelのSP表示
#define PMSK_SHOWLBG (1<<1)  // 左8pixelのBG表示
#define PMSK_SHOWALL 0x1E    // SP&BGを表示

// スプライト用アトリビュート
#define ATR_BEHIND (1<<5)  // SPをBGの奥に表示
#define ATR_FLIPH  (1<<6)  // SPを左右反転
#define ATR_FLIPV  (1<<7)  // SPを上下反転

// パレット番号
#define PAL_0  0   // パレット番号0
#define PAL_1  1   // パレット番号1
#define PAL_2  2   // パレット番号2
#define PAL_3  3   // パレット番号3

// 全機能の初期化
void fc_init(void);
// PPUアドレス設定(アドレス0x0000～0x3fff)
void ppu_address(unsigned short adr);
// BGの初期化
void bg_cls(void);
// スプライトの初期化
void sp_cls(void);
// 疑似乱数の取得
unsigned int random(void);
// コントローラー1入力
unsigned char controller1(void);
// サウンド初期化
void sound_init(void);
// 矩形波1再生(周波数)
void pulse1(unsigned short hz);
// カラーパレット設定(格納先ポインタ,オフセット0～7,パレット数1～8)
void ppu_palette(char *ptable,char ofs,char size);
// パターンテーブル設定(格納先ポインタ,オフセット0～511,枚数1～512)
void ppu_pattern(unsigned char *ptable,short ofs,short size);
// 8x16パターンテーブル設定(格納先ポインタ,オフセット0～511,枚数1～512)
void ppu_pattern8x16(unsigned char *ptable,short ofs,short size);
// 画面表示の有効化(1=表示ON / 0=表示OFF)
void ppu_enable(char on);
// Vブランクを待つ
void ppu_vsync(void);
// BGスクロール設定(X座標オフセット0～511,Y座標オフセット0～239)
void bg_scroll(unsigned short x, unsigned char y);
// BGキャラクタ2x2セル表示(X座標0～31,Y座標0～29,タイル番号0～255)
void bg_print2x2(char x,char y,unsigned char c);
// BGキャラクタ表示(X座標0～31,Y座標0～29,タイル番号0～255)
void bg_printch(char x,char y,unsigned char c);
// BG文字列表示(X座標0～31,Y座標0～29,文字列ポインタ)
void bg_printstr(char x,char y,char *str);
// BGアトリビュート設定(X座標0～31,Y座標0～29,アトリビュート0～255)
void bg_attrib(char x,char y,unsigned char atr);
// 10進数nケタ変換(格納先ポインタ,数値0～65535,桁数)
void num_to_str(char *str,unsigned short num,char keta);
// 16進数2ケタ変換(格納先ポインタ,数値0～255)
void num_to_hex(char *str,unsigned char num);
// スプライトワークのDMA転送を開始する
void sp_dmastart(void);
// スプライト 追加(X座標,Y座標,タイル0～255,アトリビュート0～255)
char sp_append(char x,char y,char tile,char attr);
// スプライト 消去(スプライト番号0～31)
void sp_remove(char i);
// スプライト 表示変更(スプライト番号0～31,タイル0～255,アトリビュート0～255)
void sp_chr(char num,unsigned char tile,unsigned  char attr);
// スプライト アニメ処理(スプライト番号0～31)
void sp_anime(char num);
// スプライト 移動(スプライト番号0～31,X座標0～255,Y座標0～255)
void sp_ofs(char num,unsigned char x,unsigned  char y);
// スプライトX座標取得(スプライト番号0～63)
unsigned char sp_getx(char num);
// スプライトY座標取得(スプライト番号0～63)
unsigned char sp_gety(char num);
// スプライトのタイル取得(スプライト番号0～63)
unsigned char sp_gettile(char num);
// スプライト衝突判定(スプライト番号,X座標,Y座標,しきい値)
char sp_hitcheck(char num,unsigned char x,unsigned char y,unsigned char diff);
