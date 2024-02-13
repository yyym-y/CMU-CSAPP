# CSAPP-第二章家庭作业

## 作业 2.55-2.57

`show_bytes()` 以及相关函数代码 : 

```c
typedef unsigned char* byte_pointer;
void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++)
    printf("%.2x ", start[i]);
    printf("\n");
}
void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}
void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}
void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}
```

经过测试, 我的电脑为小端法



## 作业 2.58

经过分析, 我们只需要生成一个 `int` 类型变量 `x = 0xF`, 然后判断存储的第一个字节是不是 `F` 

如果是就是小端法, 否则为大端法

同时这个方法不限制机器的字长的多少

```c
bool is_little_endian() {
    int x = 0xF;
    char* begin = (char *)&x;
    return *begin == (char)(0xF);
}
```



## 作业 2.59

我们可以把 $x$ 的最后的字节置为 $0$, $y$ 除了最后的字节置为 $0$

之后两者取 `或` 即可

```c
int solve(int x, int y) {
    // for chapter 2 homework 2.59;
    return (x & 0xFF) | (y & (~(0xFF)));
}
```



## 作业 2.60

```c
unsigned replace_bytes(unsigned x, int i, unsigned char b) {
    int byte = (int) b;
    return (x & (~(0x0) ^ (0xFF << (i * 8)))) | (byte << (i * 8 ));
}
```



## 作业 2.61

```c
int solve(int x) {
    // for chapter 2 homework 2.61;
    bool A = !(~x);
    bool B = !x;
    bool C = !(x & 0xFF ^ 0xFF);
    bool D = !(x >> ((sizeof(int)-1) << 3));
    return A || B || C || D;
}
```



## 作业 2.62

```c
bool int_shifts_are_arithmetic() {
    int shift_val = ((sizeof(int) - 1) << 3) + 4;
    int x = 0x0 | (0xF << shift_val);
    x >>= 4;  // arithmetic : 0xFF000000, logic : 0x0F000000
    x &= 0xF0000000; // arithmetic : 0xF0000000, logic : 0x00000000
    return x;
}
```



## 作业 2.63

```c
unsigned srl(unsigned x, int k) {
    unsigned xsra = (int) x >> k;
    int shift_val = ((sizeof(int) - 1) << 3) + 4;
    int shift_ori = !(!k) << 3; // k == 0 ? 0 : 0x8
    int tem = 0x0 | (shift_ori << shift_val); // tem = k == 0 ? 0x0 : 0x80000000
    tem >>= k - 1; tem = ~tem;
    return xsra & tem;
}
unsigned sra(int x, int k) {
    int xsrl = (unsigned) x >> k;
    int shift_val = ((sizeof(int) - 1) << 3) + 4;
    int shift_ori = !(!k) << 3; // k == 0 ? 0 : 0x8
    int tem = 0x0 | (shift_ori << shift_val); // tem = k == 0 ? 0x0 : 0x80000000
    tem >>= k - 1;
    return xsrl | tem;
}
```



## 作业 2.64

```c
int any_odd_one(unsigned x) {
    return !!(x & 0xAAAAAAAA);
}
```



## 作业 2.65

这题主要是强化了对于异或的理解, 当一组 $0/1$ 数组异或的时候, 如果有奇数个 $1$ , 那么最后的结果为 $1$ , 反之为 $0$

这便是异或所带来的压缩效果, 本题, 我们不妨使用最低位作为结果存储的位

我们分析一下每次异或操作后, 最低位相当于哪些坐标相互异或

$0$

$0,16$	// `x ^= x >> 16`

$0, 8, 16, 24$	// `x ^= x >> 8`

$0, 4, 8, 12, 16, 20, 24, 28$	// `x ^= x >> 4`

$0, 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30$	// `x ^= x >> 2`

$0, 1,2,3, 4,5, 6,7, 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31$

所以代码为 : 

```c
int odd_one(unsigned x) {
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}
```

> 同理你也可以判断奇数位 $1$ 的个数是不是奇数? 偶数位 $1$ 的个数是不是奇数等等
>
> ```c
> /*
> 判断偶数位是否有奇数个 1
> @return 奇数 : 1; 偶数 : 0
> */
> int sample(unsigned x) {
>     x ^= x >> 16;
>     x ^= x >> 8;
>     x ^= x >> 4;
>     x ^= x >> 2;
>     return x & 1;
> }
> ```



## 作业 2.66

这一题和上一题有异曲同工之妙, 大家可以自行体会

```c
int leftmost_one(unsigned x) {
    x |= x >> 16;
    x |= x >> 8;
    x |= x >> 4;
    x |= x >> 2;
    x |= x >> 1;
    int tem = ~x >> 1;
    return tem & x;
}
```



## 作业 2.67

$A :$ $32$ 位机器，最多只能移动 $31$ 位，这个代码超出了机器能移动的位数，超出位无定义 

$B :$

```c
int int_size_is_32() {
	int set_msb = 1 << 31;
	int beyond_msb = set_msb << 1; //相当于移动了 32 位
	return set_msb && !beyond_msb;
}
```

$C :$

```c
int int_size_is_32() {
	int set_msb = 1 << 15;
    set_msb <<= 15;
	int beyond_msb = set_msb << 1; //相当于移动了 32 位
	return set_msb && !beyond_msb;
}
```



## 作业 2.68

```c
int lower_one_mask(int n) {
    int w = sizeof(int) << 3;
    return (unsigned)-1 >> (w - n);
}
```



## 作业 2.69

```c
unsigned rotate_left(unsigned x, int n) {
    int w = sizeof(int) << 3;
    int x1 = x << n;
    int x2 = (signed)x >> (w - n);
    return x1 | x2;
}
```



## 作业 2.70

```c
int fits_bits(int x, int n) {
    int b = !(x >> (n - 1)); // if n ~ 32 bits all equal to 0
    int c = !(~(x >> (n - 1))); // if n ~ 32 bits all equal to 1
    return b || c;
}
```



## 作业 2.71

$A :$ 无法提取负值, `unsigned` 会进行逻辑右移

$B :$

```c
typedef unsigned packed_t;

int xbyte(packed_t word,int bytenum){
    return ((int)(word << ((3 - bytenum) << 3))) >> 24;
}
```



## 作业 2.72

$A :$ `size_t` 是无符号型, 所以在判断的时候, 会隐式类型转换为无符号型, 所以得到的结果恒为正数

$B :$

```c
if(maxbytes >= sizeof(val)) 
```



## 作业 2.73

```c
int saturating_add(int x, int y) {
    int sum = x + y;
    int sig_mask = INT_MIN;

    int pos_over = !(x & sig_mask) && !(y & sig_mask) && (sum & sig_mask);
    int neg_over = (x & sig_mask) && (y & sig_mask) && !(sum & sig_mask);

    (pos_over && (sum = INT_MAX)) || (neg_over && (sum = INT_MIN));

    return sum;
}
```



## 作业 2.74

```c
int tsub_ok(int x, int y) {
    int res = 1;
    (y == (unsigned)-1) && (res = 0); // y equal to INT_MIN must overflow
    
    int sub = x - y;
    (x > 0) && (y < 0) && (sub < 0) && (res = 0); // positive overflow
    (x < 0) && (y > 0) && (sub > 0) && (res = 0); // negtive overflow

    return res;
}
```



## 作业 2.75

我们给定长度为 $w$ 的向量 $\overrightarrow{x}$ ,  $\overrightarrow{y}$  , 用这两个向量表示的无符号整数为 $x^\prime$ 和 $y^\prime$ , 表示的补码整数为 $x$ 和 $y$

我们知道 $x \times y$ 会产生 $0 \sim 2w$ 位的结果, 对于低 $w$ 位, 其结果为 $(x \times y) \bmod 2^w$

同理可得, 对于高 $w$ 位, 其结果为 $(x \times y) \div 2^w$ , 对于 `unsigned_high_prod` 函数, 我们要求的是 $(x^\prime \times y^\prime) \div 2^w$

我们直接套用书上的公式 : 
$$
(x^\prime \times y^\prime) \div 2^w = [(x + x_{w-1}2^w) \times (y + y_{w - 1}2^w)] \div 2^w
$$
进一步简化可得 : 
$$
(x^\prime \times y^\prime) \div 2^w = [x \times y + 2^w \cdot (x \cdot y_{w-1} + y\cdot x_{w-1}) + 2^{2w}\cdot x_{w-1} \cdot y_{w-1}] \div 2^w
$$

我们将 $2^w$ 除进方括号内
$$
(x^\prime \times y^\prime) \div 2^w = (x \times y) \div 2^w + (x \cdot y_{w-1} + y\cdot x_{w-1}) + 2^{w}\cdot (x_{w-1} \cdot y_{w-1})
$$

我们分析一下 :

 $(x \times y) \div 2^w$ 的结果为 `signed_high_prod` 的结果

$2^{w}\cdot (x_{w-1} \cdot y_{w-1})$ 超过了 $w$ 位可以被直接舍去

所以我们可以得到下面的代码 : 

```c
int signed_high_prod(int x, int y) {
    return (int64_t) x * y >> 32;
}
unsigned unsigned_high_prod(unsigned x, unsigned y) {
    int w = sizeof(int) << 3;
    int x_mask = ((int)x) >> (w - 1); // x_{w - 1}, change form to 111...1 or 000...0
    int y_mask = ((int)y) >> (w - 1); // y_{w - 1}
    return signed_high_prod(x, y) + (x & y_mask) + (y & x_mask);
}
```



## 作业 2.76

```c
void *calloc(size_t nmemb, size_t size) {
    int tem = 0; // any number is ok
    void* ptr = &tem;
    ((nmemb & size) == 0) && (ptr = NULL); // nmemb or size equal to 0, ptr = NULL
    
    size_t buf_size = nmemb * size;
    (nmemb != buf_size / size) && (ptr = NULL); // if overflow set ptr = NULL

    (ptr) && (ptr = malloc(buf_size)); // if ptr != NULL, malloc memory
    (ptr) && (memset(ptr, 0, buf_size)); // check twice to make sure malloc success

    return ptr;
}
```



## 作业 2.77

$K = 17 = [0001 0001]$ 

> `a * 17 = (a << 4) + a`

$K=-7 = [1111\dots1001]$

> `a * (-7) = a - (a << 3)`

$K = 60 = [0011 1100]$

> `a * 60 = (a << 6) - (a << 2)`

$K = -112 = [1111\dots 1001 0000]$

> `a * (-112) = (a << 4) - (a << 7)`



## 作业 2.78

```c
int divide_power2(int x, int k) {
    int mask = x >> ((sizeof(int) << 3) - 1) & 1;
    int bias = (mask << k) - 1;
    return (x + bias) >> k;
}
```



## 作业 2.79

```c
int mul3div4(int x) {
    x = (x << 1) + x;
    int k = 2;
    int mask = x >> ((sizeof(int) << 3) - 1) & 1;
    int bias = (mask << k) - 1;
    return (x + bias) >> k;
}
```



## 作业 2.80

// 别问, 抄的, 不想思考了, 鬼知道题目要求的顺序是什么, 开摆...打瓦去了...

```c
int threefourths(int x) {
    int low_2_bit_x = x & 0x3;
    int high_bit_x = x & ~0x3;
    int bias = (x >> (w - 1)) & 0x3;
    high_bit_x = (high_bit_x >> 2) + (high_bit_x >> 1);
    low_2_bit_x += low_2_bit_x << 1;
    low_2_bit_x = (low_2_bit_x + bias) >> 2;
    return high_bit_x + low_2_bit_x;
}
```



## 作业 2.81

$A :$ `(unsigned)-1 << k`

$B :$ `0x0 | (0x1 << j)`



## 作业 2.82

$A :$ 错误的

> 当 $x = \mathrm{INT\_MIN}$ , $y$ 为任意负数的时候 (假设 $y = -1$)
>
> $(x < y) = 1$ , 但是 $(-x > -y) = 0$ , 应为 $(\mathrm{INT\_MIN} < -1)$

$B :$ 正确的

$C :$ 正确的

> 注意 : $\sim x + x + 1 = 0 \Rightarrow \sim x = -x -1$
>
> $\sim x + \sim y + 1 = -x -1 -y-1+1 = -(x + y) - 1$
>
> $= \sim(x + y)$

$D :$ 正确的

> 有符号整数与无符号整数具有相同的位级表现

$E :$ 正确的



## 作业 2.83

