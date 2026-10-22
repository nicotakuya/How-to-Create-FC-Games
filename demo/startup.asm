; startup.asm

.setcpu        "6502"
.autoimport    on
.IMPORTZP    sp

.global    _NesMain

; iNESヘッダ(16 Bytes)
.segment "HEADER"
    .byte   $4E, $45, $53, $1A ; "NES" 
    .byte   $02                ; PRG-ROMバンク数(16KB x 2)
    .byte   $00                ; CHR-ROMバンク数(8KB x 0)
    .byte   $01                ; Mapper 0(low)/Vetrical Mirroring
    .byte   $00                ; Mapper 0(high)
    .byte   $00                ; PRG-RAMサイズ
    .byte   $00,$00,$00,$00, $00, $00, $00

.segment "STARTUP"
; リセット割り込み
.proc    Reset
    sei
    ldx    #$ff        ; スタックポインタ初期化1
    txs 

    lda    #$ff        ; スタックポインタ初期化2
    sta    sp
    lda    #$03
    sta    sp + 1

    jsr    _NesMain    ; メイン関数を呼び出す
.endproc

.segment "VECINFO"     ; ベクタテーブル
    .word    $0000     ; NMI割り込みアドレス
    .word    Reset     ; リセット割り込みアドレス
    .word    $0000     ; IRQ割り込みアドレス

