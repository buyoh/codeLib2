package main

import (
    "fmt"
    "os"
    "bufio"
    "strconv"
    // "sort"
)
var sc = bufio.NewScanner(os.Stdin)


func main() {
    sc.Split(bufio.ScanWords)
    // code goes here
}



func nextInt() int {
    sc.Scan()
    v, e := strconv.Atoi(sc.Text())
    if e != nil { panic(e) }
    return v
}
func nextInt64() int64 {
    sc.Scan()
    v, e := strconv.ParseInt(sc.Text(), 10, 64)
    if e != nil { panic(e) }
    return v
}
func nextString() string {
    sc.Scan()
    return sc.Text()
}
func nextFloat() float64 {
    sc.Scan()
    v, e := strconv.ParseFloat(sc.Text(), 64)
    if e != nil { panic(e) }
    return v
}
func Max(x, y int) int { if x > y { return x }; return y }
func Min(x, y int) int { if x < y { return x }; return y }
func Abs(x int) int { if x > 0 { return x }; return -x }

