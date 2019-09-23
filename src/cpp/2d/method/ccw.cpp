
// http://www.prefield.com/algorithm/geometry/convex_hull.html
int ccw(P a, P b, P c) {
  b.x() -= a.x();
  b.y() -= a.y();
  c.x() -= a.x();
  c.y() -= a.y();
  if (b.x() * c.y() - b.y() * c.x() > 0)
    return +1;  // counter clockwise
  if (b.x() * c.y() - b.y() * c.x() < 0)
    return -1;  // clockwise
  if (b.x() * c.x() + b.y() * c.y() < 0)
    return +2;  // c--a--b on line
  if (b.hypot() < c.hypot())
    return -2;  // a--b--c on line
  return 0;
}