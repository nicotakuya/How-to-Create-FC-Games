; startup.asm
.setcpu        "6502"
.autoimport    on
.IMPORTZP    c_sp
.global    _NesMain
.export    _gettick

.segment "ZEROPAGE"

tickcnt:    .res 1     ; チック時間

.segment "HEADER"           ; iNESヘッダ(16 Bytes)
    .byte   $4E,$45,$53,$1A ; "NES" 
    .byte   $02             ; PRG-ROMバンク数(16KB x 2)
    .byte   $00             ; CHR-ROMバンク数(8KB x 0)
    .byte   $01             ; Mapper 0(low)/V-Mirroring
    .byte   $00             ; Mapper 0(high)
    .byte   $00             ; PRG-RAMサイズ
    .byte   $00,$00,$00,$00, $00, $00, $00

.segment "STARTUP"     ; リセット割り込み
.proc    Reset
    sei                ; 割り込み禁止
    ldx    #$ff        ; スタックポインタ初期化1
    txs
    lda    #$ff        ; スタックポインタ初期化2
    sta    c_sp
    lda    #$03
    sta    c_sp + 1
    jsr    _NesMain    ; メイン関数を呼び出す
.endproc

.proc    Nmi           ; NMI（Non Maskable Interrupt)
    inc    tickcnt ; チック時間+1
    rti
.endproc

; チック時間の取得
; unsigned char gettick(void);
_gettick:
    lda tickcnt
    ldx #$00   ; int化に備えてX=0
    rts

.segment "VECINFO"     ; ベクタテーブル
    .word    Nmi       ; NMIアドレス
    .word    Reset     ; リセット割り込みアドレス
    .word    $0000     ; IRQ割り込みアドレス

