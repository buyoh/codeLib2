# %=BEGIN DOC
# %title
# ccw(進行方向)
#
# %overview
# 二次元座標上の点を表す構造体
# a -> b -> c と進むときの方向を判定する。
#
# %usage
# %require
# %verified
#
# %references
# http://www.prefield.com/algorithm/geometry/ccw.html
# %words
# ccw
# %=END DOC
# %=BEGIN CODE

def ccw(a,b,c)
  ax,ay = a
  bx,by = by
  cx,cy = c
  bx -= ax
  by -= ay
  cx -= ax
  cy -= ay
  bx*cy - by*cx > 0 ?  1 : # 半時計周り
  bx*cy - by*cx < 0 ? -1 : # 時計回り
  bx*cx + by*cy < 0 ?  2 :       # c - a - b
  bx*bx+by*by < cx*cx+cy*cy ? -2 # a - b - c
                            :  0 # a - c - b
end

# %=END CODE
