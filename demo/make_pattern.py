# make pattern (16色BMPファイルをソースファイルに変換)
cnv = [ 0, 128, 192, 255 ]      # 4諧調カラー変換テーブル
inputname = 'pattern.bmp'       # 入力ファイル名
outputname = 'pattern.h'        # 出力ファイル名
print("load:"+inputname)
infile = open(inputname, 'rb')  # ファイルの読み込み
indata = infile.read()
infile.close()
header = int.from_bytes(indata[10:14], 'little') # ヘッダ長
bmpw = int.from_bytes(indata[18:22], 'little')   # 画像の幅
bmph = int.from_bytes(indata[22:26], 'little')   # 画像の高さ
depth = int.from_bytes(indata[28:30], 'little')  # カラーbit
if depth != 4:
    print('ERROR:カラーが16色(4bit)ではありません')

pallete = []      # 16色カラーパレットを読み込み
for i in range(16):
    idx = 54+(4*i)
    color = int.from_bytes(indata[idx:idx+4], 'little')
    pallete.append(color)

cellsize = 8  # セルの大きさ[ピクセル]
cellw = int(bmpw / cellsize)  # 画像の幅[セル]
cellh = int(bmph / cellsize) # 画像の高さ[セル]
comma = ' '
fbuf = '// '+ outputname +'\n'
fbuf += 'const unsigned char pattern[] = {\n'
for i in range(cellw * cellh):
    ofsx = (i % cellw)*cellsize
    ofsy = int(i / cellw)*cellsize
    for plane in range(2):
        for y  in range(cellsize):
            tmp = 0
            for x  in range(cellsize):
                ry = ((bmph-1)-(y+ofsy))
                idx = int(((x+ofsx)+(ry*bmpw))/2) + header
                if (x & 1)==0 :
                    coloridx = (indata[idx] >> 4)& 0xf
                else:
                    coloridx = indata[idx] & 0xf

                color = pallete[coloridx]     # 指定座標の色
                r = (color >> 16) & 0xff
                g = (color >> 8) & 0xff
                b = color & 0xff
                v = int((r + g + b)/3)        # 256諧調に変換
                for code in range(4):
                    if v <= cnv[code]: break  # 4諧調に変換

                if ((1 << plane) & code) != 0:
                    tmp += (0x80 >> x)        # バイナリに変換

            fbuf += comma + '0x' + '{:02x}'.format(tmp)
            comma = ','

    fbuf += '\n'

fbuf += '};\n'
outfile = open(outputname, 'w')  # ファイルの出力
outfile.write(fbuf)
outfile.close()
print("save:"+outputname)
