# %=BEGIN DOC
# %title
# GCD,LCM,組み合わせ計算
# 
# %overview
# todo
#
# %usage
# int func(int x);
# x : input
# @ret
#
# %verified
# problem.
# 
# %references
# thanks.
# %words
# math
# %require
# 
# %=END DOC
# %=BEGIN CODE

def pow_md(x, w, mod)
  y = 1
  x = x%mod
  while 0 < w
      y = (y*x) % mod if w.odd?
      x = (x*x) % mod
      w >>= 1
  end
  y
end


def nCr_md(n, r, mod)
  return nCr_md(n, n-r, mod) if n/2 < r
  num,den = 1,1
  1.upto(r) do |i|
      num = num*(n-i+1) % mod
      den = den*i % mod
  end
  num * pow_md(den, mod-2, mod) % mod
end

# %=END CODE
