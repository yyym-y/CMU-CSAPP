# CSAPP-第二章练习题

## 课堂练习

### 练习 2.1

二进制和十六进制对应的表为

|  0   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| 0000 | 0001 | 0010 | 0011 | 0100 | 0101 | 0110 | 0111 |


|  8   |  9   |  A   |  B   |  C   |  D   |  E   |  F   |
| :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| 1000 | 1001 | 1010 | 1011 | 1100 | 1101 | 1110 | 1111 |

我们按照上表, 一一对应即可生成答案

> $A$ : 	$001110011010011111111000_2$
>
> $B$ : 	$\mathrm{0xC97B}$
>
> $C$ : 	$11010101111001001100$
>
> $D$ : 	$\mathrm{0x26E7B5}$



### 练习 2.2

如书上所述, 如果我们要转换的数是可以转换为 $2^n$ 

那么对于二进制来说 : 表示为一个 $1$ 后面跟着 $n$ 个 $0$

对于十六进制来说 : 我们将 $n$ 表示为 $i + 4j$ 的形式 , 那么十六进制表示为 $\alpha$ 后跟着 $j$ 个 $0$

$\alpha$ 根据 $i$ 的值来确定 : $\alpha = 1 (i= 0) \quad \alpha = 2 (i= 1) \quad \alpha = 4 (i= 2) \quad \alpha = 8 (i= 3)$

所以答案为 : 

| $n$  | $2^n$(十进制) |  $2^n$(十六进制)   |
| :--: | :-----------: | :----------------: |
| $9$  |     $512$     |  $\mathrm{0x200}$  |
| $19$ |   $524288$    | $\mathrm{0x80000}$ |
| $15$ |    $16384$    | $\mathrm{0x4000}$  |
| $16$ |    $65536$    | $\mathrm{0x10000}$ |
| $17$ |   $131072$    | $\mathrm{0x20000}$ |
| $5$  |     $32$      |  $\mathrm{0x20}$   |
| $7$  |     $128$     |  $\mathrm{0x80}$   |



### 练习 2.3

注意, 从十进制转化为十六进制的时候, 余数是从后向前排列的

| 十进制 |    二进制    |    十六进制     |
| :----: | :----------: | :-------------: |
|  $0$   |    $00_2$    | $\mathrm{0x00}$ |
| $167$  | $10100111_2$ | $\mathrm{0xA7}$ |
|  $62$  | $00111110_2$ | $\mathrm{0x3E}$ |
| $188$  | $10111100_2$ | $\mathrm{0xBC}$ |
|  $55$  | $00110111_2$ | $\mathrm{0x37}$ |
| $136$  | $10001000_2$ | $\mathrm{0x88}$ |
| $115$  | $1110011_2$  | $\mathrm{0x73}$ |
|  $82$  | $01010010_2$ | $\mathrm{0x52}$ |
| $172$  | $10101100_2$ | $\mathrm{0xAC}$ |
| $231$  | $11100111_2$ | $\mathrm{0xE7}$ |



### 练习 2.4

在十六进制相加减的时候, 把它类比成我们平时的十进制加减法即可, 也要注意进位的问题

$A$ : 	$\mathrm{0x503c + 0x8 = 0x5044}$

$B$ :	$\mathrm{0x503C - 0x40 = 0x4FFC}$

$C$ :	$\mathrm{0x503C + 64 = 0x507C}$

$D$ : 	$\mathrm{0x50EA - 0x503C = 0xAE}$



### 练习 2.5

对于 $\mathrm{val = 0x87654321}$ 来说 , 我们用大端法和小端法完整的表示一遍

> 大端法 : 	$87 \quad 65 \quad 43 \quad 21$
>
> 小端法 : 	$21 \quad 43 \quad 65 \quad 87$
>
> 值得注意的是, 因为 val 定义的类型为 `int` , 所以占得字节数为 $4$

所以我们能够得出答案 : 

* 小端法 : 

  > $A$ :	$21$
  >
  > $B$ :	$21 \quad 43$
  >
  > $C$ :	$21 \quad 43 \quad 65$

* 大端法 : 

  > $A$ :	$87$
  >
  > $B$ :	$87 \quad 65$
  >
  > $C$ :	$87 \quad 65 \quad 43$



### 练习 2.7

结果为 : $\mathrm{0x61 \quad 0x62 \quad 0x63 \quad 0x64 \quad 0x65 \quad 0x66 }$



### 练习 2.8

$\sim a = [10010110]\quad \sim b = [10101010]$

$a \& b = [01000001] \quad a | b = [01111101] \quad a \oplus b = [00111100]$



### 练习 2.13

```c
int bool_or(int x, int y) {
    int result = bis(x, y);
    return result;
}
```

$\mathrm{a \oplus b = (a \:\& \: \sim b) | (\sim a \:\& \:b)}$

```c
int bool_xor(int x, int y) {
    int result = bis(bic(x, y), bic(y, x));
    return result;
}
```



### 练习 2.15

```c
bool if_equal(int x, int y) {
    // return !(~(x ^ (~ y))); 法一
    return !(x ^ y);
}
```



### 练习 2.17

<img src="C:/Users/%25E9%2598%25B4%25E9%2593%25AD%25E6%25B4%258B/AppData/Roaming/Typora/draftsRecover/img/2.17.png" alt="2.17" style="zoom:80%;" />

### 练习 2.19

关键点在于, 如果 $x$ 为非负数, 那么 $\mathrm{T2U_4}(x) = x$ , 如果为负数, 那么 $\mathrm{T2U_4}(x) = x + 2^4$

<img src="C:/Users/%25E9%2598%25B4%25E9%2593%25AD%25E6%25B4%258B/AppData/Roaming/Typora/draftsRecover/img/2.19.png" alt="2.19" style="zoom:80%;" />

### 练习 2.21

只要基本算数操作中出现了无符号数, C 语言都会强转为无符号数, 否则为有符号数

> tip : `size_t` 被定义为 `unsigned int`

<img src="C:/Users/%25E9%2598%25B4%25E9%2593%25AD%25E6%25B4%258B/AppData/Roaming/Typora/draftsRecover/img/2.21.png" alt="2.21" style="zoom:80%;" />



### 练习 2.23

值得注意的是 : 无符号数执行逻辑移, 有符号数执行算数移

所以对于 `fun1`, 其作用就是清空高24位, 另其为0

对于 `fun2` 来说, 起作用是, 清空高 24 位, 如果高25为为 1, 则填充1, 否则填充0

<img src="C:/Users/%25E9%2598%25B4%25E9%2593%25AD%25E6%25B4%258B/AppData/Roaming/Typora/draftsRecover/img/2.23.png" alt="2.23" style="zoom:80%;" />

### 练习 2.24

截断的公式为 : 如果 $m$ 位截断到 $k$ 位

无符号数 : $x = x \bmod 2^k$

有符号数 : $x = \mathrm{U2T_k}(\mathrm{T2U_m}(x) \bmod 2^k)$

$k$ 位无符号转有符号的公式为 :

$x_U \ge 2^{k-1} , x_T = x_U - 2^k$ ,  $x_U < 2^{k-1}, x_T = x_U$

<img src="C:/Users/%25E9%2598%25B4%25E9%2593%25AD%25E6%25B4%258B/AppData/Roaming/Typora/draftsRecover/img/2.24.png" alt="2.24" style="zoom:80%;" />

### 练习 2.25

传参传入的时候, `length` 是 `unsigned` 类型的, 如果输入是 $0$ , 那么最后 `length - 1` 的结果会是 $[1,1,\dots, 1]$

转化为无符号数是一个巨大的数字, 会导致内存泄漏

修改建议, 增加下面的判定代码 : 

```c
if((int)length <= 0)
    return NULL;
```



### 练习 2.26

A , B : 当 `strlen(s) < strlen(t)` 的时候, 此时结果为负数, 但是 `size_t` 是 `unsigned` 类型的, 所以负数会转变成一个非常大的正数, 从而得到错误的结果

C : 改为 : `strlen(s) > strlen(t)`



### 练习 2.27

无符号加法溢出的检验条件为 : 

$a_u + b_u = c_u$ , 如果 $c_u < a_u \or c_u < b_u$ 则溢出

实际在判断时候, 如果大于等于 x, y 中任意一个就可以了

```c
int uadd_ok(unsigned x, unsigned y) {
    unsigned ans = x + y;
    return x >= x
}
```



### 练习 2.30

有符号数加法的溢出检验条件为 :

如果 $a_t + b_t = c_t$

如果 $a_t, b_t < 0$ , $c_t > 0$ , 如果 $a_t, b_t > 0$ , $c_t < 0$ , 则发生溢出

```c
int tadd_ok(int x, int y) {
    int ans = x + y;
    if(x < 0 && y < 0) return ans < 0;
    if(x > 0 && y > 0) return ans > 0;
    return 1;
}
```

 

### 练习 2.31

我们假设发生正溢出, 那么 `sum = x + y - 2^32`

那么 `(sum - x == y) && (sum - y == x)` 可以转化为 `(y - 2^32 == y) && (x - 2^32 == x)`

这是恒等的, 所以不成立



### 练习 2.32

我们考虑特殊值 : 我们假设 $x = 10, y = \mathrm{T_{min}}$

很显然 $x - y$ 是发生正溢出的, 但是因为 $y = -y = \mathrm{T_{min}}$, 所以 `tadd_ok` 返回 1, 发生了错误

我们可以对 $\mathrm{T_{min}}$ 进行特判

```c
int tsub_ok(int x, int y) {
    if(y == -y) {
        if(x < 0) return 1;
        return 0;
    }
    return tadd_ok(x, y);
}
```



### 练习 2.36

$w$ 位的两个数相乘会产生小于 $2w$ 位的数, 所以我们只需要简单判断是否相等即可

```c
int tmul_ok(int x, int y) {
    int64_t real_ans = (int64_t)x * y;
    int ans = x * y;
    return (int64_t)ans == real_ans;
}
```



### 练习 2.38

一条 `LEA` 指令, 如果 `b = 0` 的话, 可以计算 2 的幂次倍 a

如果 `b = 1` 的话, 可以计算 2 的幂次倍 +1 a



### 练习 2.40

我们将这些数用二进制展开 : 

$6 \Rightarrow [00110]$ , 发现有两个 $1$ 

所以可以转化为 : $x \times 6 \Rightarrow (x << 1) + (x << 2)$ , 或者 $(x << 3) - (x << 1)$

$31 \Rightarrow [011111]$ 

所以可以转化为 : $x \times 31 \Rightarrow (x << 5) - x$

$-6 \Rightarrow [111\dots1010]$ 

所以可以转化为 : $x \times -6 \Rightarrow (x << 1) - (x << 3)$

$55 \Rightarrow [0011 0111]$ , 我们将这两块 $1$ 合在一起处理 , 之后再单独减去 $0$

所以可以转化为 : $x \times 55 \Rightarrow (x << 6) - x - (x << 3)$



### 练习 2.42

关键在于判断输入的数是不是正数, 我们分两种情况讨论 : 

如果输入的数是正数, 那么 : $x / 16 = (x >> 4)$

如果输入的数是负数, 那么 : $x / 16 = (x + (1 << 4) - 1) >> 4$

我们发现两者的区别仅仅在于偏置, 所以我们考虑是不是能让输入数为负数的时候, 偏置为0

于是我们得到了下面的代码 :

```c
int div16(int x) {
    int bias = x >> 31 & 1;
    return (x + (bias << 4) - bias) >> 4;
}
```



### 练习 2.43

对 $x$ 的操作为 $(x << 5) - x$ , 翻译过来 $x \times (2^5 - 2^0) = x \times 31$ , 所以 $M = 31$

对 $y$ 的操作为 : 通过 $y >>= 3$, 我们不难猜出 $N = 2^3 = 8$, 同时偏置量为 $7$

