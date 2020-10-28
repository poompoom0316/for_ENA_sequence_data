from pykakasi import kakasi
kakasi = kakasi()
kakasi.setMode('H', 'a')  # a(ローマ字) to H(Hiragana)
conv = kakasi.getConverter()

d_1="しょくじ"

if d_1.isalpha()==True:
　  d_1=conv.do(d_1)
    print(d_1)
