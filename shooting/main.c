// Jimi Shooting game(main.c) for FC
// by Takuya Matsubara
#include "fcsub.h"     // ファミコン用ライブラリ
#include "pattern.h"   // パターンデータ

extern void waitvsync (void); // Vブランク待ち

void pushbutton(void);
void title(void);
void result(void);
void player(char num,unsigned char x,unsigned char y);
void bakuhatu(char num,unsigned char x,unsigned char y);
void beam(char num,unsigned char x,unsigned char y);
void myshot(char num,unsigned char x,unsigned char y);
void enemy(char num,unsigned char x,unsigned char y);
char NesMain(void);

const char palette[] = { // カラーテーブル
    C_BLACK,C_BLUE    ,C_BLUE2   ,C_WHITE  , // SPパレット0
    0      ,C_GREEN1  ,C_GREEN2  ,C_TEAL2  , // SPパレット1
    0      ,C_MAGENTA1,C_PURPLE2 ,C_MAGENTA2,// SPパレット2
    0      ,C_PURPLE1 ,C_MAGENTA2,C_WHITE    // SPパレット3
};

const char strtitle[] = "JIMI SHOOTING GAME";
const char strcopyright[] = "(C)2026 Takuya Matsubara";
const char strpush[]  = "PUSH ANY BUTTON";
const char strgover[] = "GAME OVER";
const char strscore[] = "SCORE";
const char strlevel[] = "LEVEL";

// グローバル変数
short score;            // スコア
unsigned char gameover; // ゲームオーバーカウンタ
unsigned char bgy;      // BGオフセット座標
unsigned char loopcnt;  // ループカウンタ
unsigned char level;    // 難易度（敵の出現度数）
unsigned char tempdata[32];  // 汎用カウンタ

// タイル番号+（0x1000以降は+1）
#define TILE_SHIP   (0x88+1)
#define TILE_MYSHOT (0x38+1)
#define TILE_BEAM   (0x50+1)
#define TILE_ENEMY  (0x30+1)
#define TILE_BAKUHA (0x40+1)

// ボタンの入力待ち
void pushbutton(void)
{
    unsigned char newbtn,oldbtn,edge;
    bg_printstr(8,20,(char *)strpush);   // PUSH
    waitvsync();      // Vブランク待ち
    sp_dmastart();    // OMA DMA転送開始
    bg_scroll(0,0);   // BGスクロール
    ppu_enable(1);    // 表示を有効化
    oldbtn = 0;
    edge = 0;
    while(edge < 2){
        newbtn = controller1();   // コントローラー1入力
        if(oldbtn != newbtn) edge++;  // ボタンのエッジ検出
        oldbtn = newbtn;
        waitvsync();     // Vブランク待ち
        bg_scroll(0,0);  // BGスクロール
    }
    ppu_enable(0);    // 表示を無効化
    pulse1(300);      // 矩形波1サウンド再生
    bg_cls();         // BGを全消去
    sp_cls();         // スプライトを全消去
}

// タイトル＆ゲーム開始
void title(void)
{
    char x,y,i;
    bg_printstr(6,10,(char *)strtitle);     // タイトル
    bg_printstr(4,25,(char *)strcopyright); // Copyright
    pushbutton();    // ボタンの入力待ち
    for(i=0; i<30; i++){ // 星
        x = random() % 32;
        y = random() % 30;
        bg_printch(x,y,'.');
    }
    sp_append(128-8,240*2/3, TILE_SHIP,PAL_0); // プレイヤーを追加
    tempdata[0] = 0;
    score = 0;     // スコア
    gameover = 0;  // ゲームオーバーカウンタ
    bgy = 0;       // BGオフセット座標
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
    // スコア表示
    bg_printstr(10,10,(char *)strscore);
    num_to_str(tmpstr,score,4);
    bg_printstr(16,10,tmpstr);
    // 難易度表示
    bg_printstr(10,15,(char *)strlevel);
    num_to_str(tmpstr,level,3);   
    bg_printstr(16,15,tmpstr);  
    pushbutton();    // ボタンの入力待ち
}

// スプライト衝突判定(番号,パレット下限値,しきい値)
// *指定したパレット以上のスプライトにだけ反応
char sp_search(char num,char pal,unsigned char diff)
{
    unsigned char *ptr;
    char ret,tempx,tempy,x,y;
    ptr = (unsigned char *)SPWADDR+(4*num);
    x = ptr[3];     // X座標
    y = ptr[0];     // Y座標
    ptr = (unsigned char *)SPWADDR;
    for(ret=0; ret<32; ret++,ptr+=4){
        if(ret == num)continue;
        tempy = ptr[0];     // Y座標
        if(tempy == OUTSIDE)continue;
        if((ptr[2] & 3) < pal)continue;
        tempy += diff-y;
        if(tempy > diff*2)continue;  // 衝突なし
        tempx = ptr[3];     // X座標
        tempx += diff-x;
        if(tempx > diff*2)continue;  // 衝突なし
        break; // 衝突
    }
    return(ret);  // スプライト番号を返す
}

// プレイヤー移動処理
void player(char num,unsigned char x,unsigned char y)
{
    unsigned char btn;     // ボタン情報
    btn = controller1();   // コントローラー1入力
    if(btn & BUTTON_LEFT ){
        if(x > 8) x -= 2;     // 左
    }else if(btn & BUTTON_RIGHT){
        if(x < 232) x += 2;   // 右
    }
    if(btn & BUTTON_UP ){
        if(y > 32) y -= 2;    // 上
    }else if(btn & BUTTON_DOWN){
        if(y < 208) y += 2;   // 下
    }
    if((btn & BUTTON_A)&&(tempdata[0] >= 32)){  // Aボタン
        tempdata[0] = 0;
        sp_append(x,y-8,TILE_MYSHOT,ATR_FLIPV+PAL_1);
    }
    if(tempdata[0] < 64) tempdata[0]++;
    sp_ofs(num,x,y);
    // 当たり判定は4フレームに1回だけ
    if(loopcnt & 3)return;
    if(gameover==0){
        if(sp_search(0,2,10)< 32){  // 衝突
            pulse1(250);     // 矩形波1サウンド再生
            gameover = 1;    // ゲームオーバー
            sp_chr(num,TILE_BAKUHA,PAL_1); // 爆発
            tempdata[num]=0;
        }
    }
}

// 自機の弾の移動処理
void myshot(char num,unsigned char x,unsigned char y)
{
    char ret;
    if(y<8){
        y = OUTSIDE;  // 画面外へ
        sp_ofs(num,x,y);  // スプライトの移動
        return;
    }
    y -= 8;
    // 当たり判定は2フレームに1回だけ
    if(loopcnt & 1){
        ret = sp_search(num,3,15);
        if(ret < 32){  // 衝突
            sp_chr(ret,TILE_BAKUHA,PAL_1); // 爆発
            tempdata[ret] = 0;
            pulse1(500);  // 矩形波1サウンド再生
            score++;      // スコアを加算
            y = OUTSIDE;  // 画面外へ
        }
    }
    sp_ofs(num,x,y);  // スプライトの移動
}

// 爆発処理
void bakuhatu(char num,unsigned char x,unsigned char y)
{
    tempdata[num]++;
    if(tempdata[num] > 30){  // 一定時間経過
        y = OUTSIDE;
        sp_ofs(num,x,y);
    }
}

// 敵機の弾
void beam(char num,unsigned char x,unsigned char y)
{
    char x1,y1;
    x1 = (tempdata[num] >> 4)-8;
    y1 = (tempdata[num] & 0xf)-8;
    x += x1;
    y += y1;
    if((x<8)||(x>240)||(y<8)){
        y = OUTSIDE;
    }
    sp_ofs(num,x,y);
}

// 符号
int fnc_sgn(int a)
{
    if(a < 0)
        return(-1);

    return(1);
}

// 絶対値
int fnc_abs(int a)
{
    if(a < 0)
        return(-a);

    return(a);
}

// 敵の機体の移動処理
void enemy(char num,unsigned char x,unsigned char y)
{
#define SPEED 2   // 弾の速度
    char num2;
    int delta_x,delta_y;
    if(tempdata[num] & 0x40){
        x+=1;
    }else{
        x-=1;
    }
    y += ((tempdata[num] >> 4) & 3);
    if((x<8)||(x>240)){
        y = OUTSIDE;
    }
    sp_ofs(num,x,y);
    tempdata[num]++;
    if((tempdata[num] & 0x3f)!=0)return;
    if(gameover)return;
    if((random() & 0xff) > level)return;
    delta_x = sp_getx(0) - x;
    delta_y = sp_gety(0) - y;
    if((delta_x==0)&&(delta_y==0))return;
    num2 = sp_append(x,y,TILE_BEAM,PAL_2); // 敵の弾
    if(num>=32)return;
    if(fnc_abs(delta_x) > fnc_abs(delta_y)){
        delta_y = delta_y * SPEED / fnc_abs(delta_x);
        delta_x = SPEED * fnc_sgn(delta_x);
    }else{
        delta_x = delta_x * SPEED / fnc_abs(delta_y);
        delta_y = SPEED * fnc_sgn(delta_y);
    }
    delta_x += 8;
    delta_y += 8;
    tempdata[num2] = ((char)delta_x << 4)+(char)delta_y;
}

// メイン関数
char NesMain(void)
{
    unsigned char i,x,y;
    fc_init();      // 全機能の初期化
    // パターンテーブル設定 & カラーパレット設定
    ppu_pattern((unsigned char *)pattern,0,256);
    ppu_pattern8x16((unsigned char *)pattern+4096,256,256);
    ppu_palette((char *)palette,PAL_0+4,4);
    while (1){
	title();   // タイトル＆ゲーム開始
        while (gameover < 180){
            for(i=0; i<32; i++){
                y = sp_gety(i);
                if(y==OUTSIDE)continue; // 画面外
                x = sp_getx(i);
                if(y >= 232){
                    sp_remove(i); // 画面外は消去
                    continue;
                }
                switch(sp_gettile(i)/8){
                case TILE_SHIP/8:
                    player(i,x,y);  // プレイヤー
                    break;
                case TILE_MYSHOT/8:
                    myshot(i,x,y); // 自機の弾
                    break;
                case TILE_ENEMY/8:
                    enemy(i,x,y);  // 敵の機体
                    break;
                case TILE_BEAM/8:
                    beam(i,x,y);   // 敵の弾
                    break;
                case TILE_BAKUHA/8:
                    bakuhatu(i,x,y); // 爆発処理
                    break;
                }
            }
            loopcnt++;
            // 敵の出現
            if((loopcnt & 0x1f)==0){
                if((random() & 255) <= level){
                    x = (random() % 18)*8 + 60;
                    y = 0;
                    sp_append(x,y,TILE_ENEMY,PAL_3);
                }
                if(level<255)level++;//難易度を上げる
            }
            sp_anime(loopcnt & 31);
            if(gameover)gameover++;
            bgy--;
            if(bgy == 0xff)bgy=0xef;
            waitvsync();          // Vブランク待ち
            sp_dmastart();        // OMA DMA転送開始
            bg_scroll(0,bgy);     // BGスクロール
        }
        result();   // リザルト
    }
    return 0;
}
