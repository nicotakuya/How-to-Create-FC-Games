// Jump Game V1.2 for FC (main.c)
// by Takuya Matsubara
#include "fcsub.h"     // FCライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

void pushbutton(void);
void title(void);
void result(void);
void player(char num,unsigned char x,unsigned char y);
void coin(char num,unsigned char x,unsigned char y);
void monster(char num,unsigned char x,unsigned char y);
char NesMain(void);

const char palette_bg[] = { // カラーテーブル
    C_SKYBLUE, C_BLACK , C_GREEN, C_WHITE  // BGパレット0
};

const char palette_sp[] = { // カラーテーブル
    C_SKYBLUE,C_BLUE  ,C_RED     ,C_YELLOW, // SPパレット0
    0        ,C_BLACK ,C_MAGENTA1,C_WHITE   // SPパレット1
};

const char strtitle[] = "JUMP GAME";
const char strcopyright[] = "(C)2021 Takuya Matsubara";
const char strpush[]  = "PUSH ANY BUTTON";
const char strgover[] = "GAME OVER";
const char strscore[] = "SCORE";
const char strlevel[] = "LEVEL";

// グローバル変数
short score;            // スコア
unsigned char gameover; // ゲームオーバーカウンタ
signed char jump;       // プレイヤーのジャンプ移動量
unsigned short bgx;     // BGオフセット座標
unsigned char loopcnt;  // ループカウンタ
unsigned char level;    // 難易度（敵の出現度数）

// タイル番号+(0x1000以降は+1)
#define TILE_MAN   (0x00+1)
#define TILE_COIN  (0x08+1)
#define TILE_ENEMY (0x10+1)

// ボタンの入力待ち
void pushbutton(void)
{
    unsigned char newbtn,oldbtn,edge;

    bg_printstr(8,20,(char *)strpush);   // PUSH
    waitvsync();      // Vブランク待ち
    sp_dmastart();    // OMA DMA転送開始
    ppu_enable(1);    // 表示を有効化
    oldbtn = 0;
    edge = 0;
    while(edge < 2){
        newbtn = controller1();   // コントローラー1入力
        if(oldbtn != newbtn) edge++;  // ボタンのエッジ検出
        oldbtn = newbtn;
        waitvsync();     // Vブランク待ち
        bg_scroll(0,0xef);  // BGスクロール
    }
    ppu_enable(0);   // 表示を無効化
    pulse1(300);     // 矩形波1サウンド再生
    bg_cls();        // BGを全消去
    sp_cls();        // スプライトを全消去
}

// タイトル＆ゲーム開始
void title(void)
{
    char x,y,tile;

    bg_printstr(11,10,(char *)strtitle);  // タイトル
    bg_printstr(4,25,(char *)strcopyright);
    pushbutton();    // ボタンの入力待ち
    for(y=20; y<30; y+=2){
        for(x=0; x<32; x+=2){
            tile = 226+((x+y)% 4);
            bg_print2x2(x,y   ,tile); // BG Aに描画
            bg_print2x2(x,y+32,tile); // BG Bに描画
        }
    }
    sp_append(64,120, TILE_MAN,PAL_0); // プレイヤーを追加
    score = 0;     // スコア
    jump = 0;      // ジャンプ移動量
    gameover = 0;  // ゲームオーバーカウンタ
    bgx = 0;       // BGオフセット座標
    loopcnt = 0;   // ループカウンタ
    level = 50;    // 難易度
    ppu_enable(1); // 表示を有効化
}

// リザルト画面
void result(void)
{
    char tmpstr[6];

    ppu_enable(0);    // 表示を無効化
    bg_cls();         // BGを全消去
    sp_cls();         // スプライトを全消去
    bg_printstr(11,5,(char *)strgover);  // GAME OVER
    bg_printstr(10,10,(char *)strscore); // SCORE表示
    bg_printstr(10,15,(char *)strlevel); // 難易度表示
    num_to_str(tmpstr,score,4);  // スコアを文字列化
    bg_printstr(16,10,tmpstr);   // スコア表示
    num_to_str(tmpstr,level,3);  // 難易度を文字列化
    bg_printstr(16,15,tmpstr);   // 難易度表示
    pushbutton();    // ボタンの入力待ち
}

// プレイヤー移動処理
void player(char num,unsigned char x,unsigned char y)
{
    unsigned char btn;     // ボタン情報

    if(gameover){
        sp_chr(0,TILE_MAN,ATR_FLIPH + ATR_FLIPV);
        if(y < OUTSIDE)y += 2;
        sp_ofs(num,x,y);
        return;
    }
    if((loopcnt % 32)==num)sp_anime(num);
    btn = controller1();   // コントローラー1入力
    if(((btn & BUTTON_LEFT ) != 0)&&(x > 8  )) x -= 2;  // 左
    if(((btn & BUTTON_RIGHT) != 0)&&(x < 228)) x += 2;  // 右
    if((y >= 144)&&((btn & BUTTON_A)!=0)) jump = -31;   // ジャンプ
    if(jump < 31){
        jump++;                          // 重力による加速
        if((btn & BUTTON_A)==0)jump+=2;  // Aボタンを押していない場合
    }
    y += (jump/4);
    if(y > 144) y = 144; // 地面のめり込み防止
    sp_ofs(num,x,y);
}

// コイン移動処理
void coin(char num,unsigned char x,unsigned char y)
{
    if((loopcnt % 32)==num)sp_anime(num);
    x--;
    if((gameover==0)&&(sp_hitcheck(0,x,y,15)==1)){ //プレイヤーと衝突
        y = OUTSIDE;  // コインを画面外へ
        pulse1(500);  // 矩形波1サウンド再生
        score++;      // スコアを加算
    }
    sp_ofs(num,x,y);  // スプライトの移動
}

// オバケ移動処理
void monster(char num,unsigned char x,unsigned char y)
{
    if((loopcnt % 32)==num)sp_anime(num);
    x--;
    x-=(num % 3);
    sp_ofs(num,x,y);
    if((gameover==0)&&(sp_hitcheck(0,x,y,10)==1)){ //プレイヤーと衝突
        pulse1(250);     // 矩形波1サウンド再生
        gameover = 1;    // ゲームオーバー
    }
}

// スコア表示 60～63番のスプライトを使用
void num_to_sp(unsigned short num)
{
    char i;
    unsigned char *ptr;
    char x,y;
    x = 144;
    y = 178;
    ptr = (unsigned char *)SPWADDR+(60*4); // スプライトワーク
    for(i=0; i<4; i++){
        *ptr++ = y;  // Y座標
        *ptr++ = ((num % 10)*2)+0xec+1; // タイル番号
        *ptr++ = 1;  // アトリビュート
        *ptr++ = x;  // X座標
        num /= 10;   // 1つ上のケタへ
        x -= 8;
    }
}

// メイン関数
char NesMain(void)
{
    unsigned char i,x,y;

    fc_init();      // 全機能の初期化
    // パターンテーブル & カラーパレット設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);
    ppu_palette((char *)palette_bg,PAL_0  ,1);
    ppu_palette((char *)palette_sp,PAL_0+4,2);
    while (1){
	title();   // タイトル＆ゲーム開始
        while (gameover < 180){
            for(i=0; i<32; i++){
                y = sp_gety(i);
                if(y==OUTSIDE)continue; // 画面外のスプライト
                x = sp_getx(i);
                if(x <= 1){
                    sp_remove(i); // 画面外は消去
                    continue;
                }
                switch(sp_gettile(i)/8){
                case 0:
                    player(i,x,y);  // プレイヤー
                    break;
                case 1:
                    coin(i,x,y);    // コイン
                    break;
                case 2:
                    monster(i,x,y); // オバケ
                    break;
                }
            }
            loopcnt++;
            if((loopcnt & 63)==0){
                y = (random() % 8)*16 + 32;
                sp_append(240,y,TILE_COIN,PAL_0); // コイン出現
                if(level < 255)level++;  // 難易度を上げる
            }
            if((loopcnt & 15)==0 ){
                num_to_sp(score);        //スコア表示
                if((random() & 255) <= level){
                    y = (random() % 17)*8 + 16;
                    sp_append(240,y,TILE_ENEMY,PAL_1); // オバケ出現
                }
            }
            if(gameover)gameover++;
            bgx = (bgx+1) & 511;  // BGオフセット座標
            waitvsync();          // Vブランク待ち
            sp_dmastart();        // OMA DMA転送開始
            bg_scroll(bgx,0xef);  // BGスクロール
        }
        result();   // リザルト
    }
    return 0;
}

