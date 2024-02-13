# CSAPP-第三章课堂练习

## 练习 3.1



## 练习 3.2

由上到下依次为 : 

```
movl
movw
movb
movb
movq
movw
```

取两个寄存器所存储位数的最小值即可



## 练习 3.3

<img src="img/3.3.png" alt="3.3" style="zoom:80%;" />



## 练习 3.4



## 练习 3.5

```c
void decode1(long* xp, long* yp, long* zp) {
    long a, b, c = *xp, *yp, *zp;
    *yp = a;
    *zp = b;
    *xp = c;
}
```



## 练习 3.6



## 练习 3.7

```c
long scale2(long x, long y, long z) {
    long t = 8 * z + 2 * y + 5 * x;
    return t;
}
```



## 练习 3.8



## 练习 3.9

```assembly
shift_left4_rightn:
	movq	%rdi, %rax
	sal		$4, %rax
	movl	%esi, %ecx
	sarq	%cl, %rax
```



## 练习 3.10

```c
long arith2(long x, long y, long z) {
    long t1 = x | y;
    long t2 = t1 >>= 3;
    long t3 = ~ t2;
    long t4 = z - t3;
    return t4;
}
```



## 练习 3.11

$A :$ 它将 `%rdx` 赋值为 $0$

$B :$ `movq $0, %rdx`

$C :$ 我们编码立即数 $0$ 需要 $8$ 个字节, 而使用异或的方法可以节省这片空间



## 练习 3.12

```assembly
uremdiv:
	movq 	%rdx, %r8
	movq 	%rdi, %rax
	xorl	%edx, %edx
	divq	%rsi
	movq	%rax, (%r8)
	movq	%rdx, (%rcx)
	ret
```



## 练习 3.13

$A :$ 

`data_t` : 	`int` 

`COMP` : 		`<`

$B :$

`data_t` : 	`short`

`COMP` : 		`>=`

$C :$

`data_t` : 	`unsigned char`

`COMP` : 		`<=`

$D :$

`data_t` : 	`long` , `unsigned long` , 指针

`COMP` : 		`!=`



## 练习 3.14

$A :$ 

`data_t` : 	`long` 

`COMP` : 		`>=`

$B :$

`data_t` : 	`short` , `unsigned short`

`COMP` : 		`==`

$C :$

`data_t` : 	`unsigned char`

`COMP` : 		`>`

$D :$

`data_t` : 	`int` 

`COMP` : 		`<=`



## 练习 3.15

$A : \mathrm{0x40 03FE}$ 	$B : \mathrm{0x40 0425}$	

$C: \mathrm{addr_ja = 0x400543\quad addr_{pop} = 0x400545}$	

$D : \mathrm{0x40 0560}$



## 练习 3.16

```c
void cond(long a, long *p) {
    if(!p)
        goto ret;
    if(a <= *p)
        goto ret;
    *p = a;
    return;
ret:
    return;
}
```



## 练习 3.17

```c
int xLarge = 0, yLarge = 0;
long absdiff_se(long x, long y) {
    bool t = x < y;
    if(t)
        goto True;
    xLarge ++;
    return x - y;
True:
    yLarge ++;
    return y - x;
}
```



## 练习 3.18

```c
long test(long x, long y, long z) {
    long val = x + y + z;
    if(x < -3) {
        if(y >= z) 
            val = y * z;
        else 
            val = x * y;
    }else if(x > 2)
        val = x * z;
    return val;
}
```



## 练习 3.19

`