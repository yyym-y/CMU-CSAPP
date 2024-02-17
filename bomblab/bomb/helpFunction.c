void phase_1(input) {
    // input = Border relations with Canada have never been better.
    int tem = 0x402400;
    int result = string_not_equal(input, tem);
    if(result == 0)
        explode_bomb();
    return ;
}
// rdi, rsi
int string_not_equal(char* rdi, char* rsi) {
    char* rbx = rdi; char* rbp = rsi;
    int len1 = string_length(rdi);
    int len2 = string_length(rdi);
    if(len1 != len2)
        return 1;

    char rax = *rbx;
    if(rax == 0) return 1;
    if(rax != *rbp) return 1;
    
    do {
        rbx ++; rbp ++;
        rax = *rbx;
        if(rax != 0 && rax != *rbp)
            return 1;
    }while (rax != 0);

    return 0;

}

int string_length(char* input) {
    if(*input == 0)
        return 0;
    
    int result;
    char* rdx = input;

    do {
        rdx += 1;
        result = rdx - input;
    } while (*rdx != 0);

    return result;
}

void phase_2(input) {
    void* rsp;
    int a, b, c, d, e, f;
    read_six_number(?, rsp);
    
    if (*((int*)rsp) != 1)
        explode_bomb();
    
    void* rbx = rsp + 4;
    void* rbp = rsp + 24;
    
    do {
        int eax = *((int*)(rbx - 4));
        eax *= 2;
        if(eax != *((int*)rbx))
            explode_bomb();
        rbx += 4;
    } while (rbp != rbx);
    // 1 2 4 8 16 32
}

// 六个数字分别在 rsi, rsi + 4, rsi + 8, rsi + 12, rsi + 16, rsi + 20, 
void read_six_number(void* rdi, void* rsi) {
    void* rsp;

    void* rdx = rsi;
    void* rcx = rsi + 4;
    ((int*)(rsp + 8)) = rsi + 20;
    rsp = rsi + 16;
    void* r9 = rsi + 12;
    void* r8 = rsi + 8;
    int eax = scanf("%d %d %d %d %d %d", rdx, rcx, r8, r9, rsp, rsp + 8);
    if(eax <= 5) explode_bomb();
}

void phase_3(input) {
    void* rsp;
    void* rcx = rsp + 12;
    void* rdx = rsp + 8;
    // 读取的两个数字保存在上面对应的地址中
    int eax = scanf("%d %d", rdx, rcx);
    if(eax <= 1)
        explode_bomb();

    if(*((int*)(rsp + 8)) > 7) {
        explode_bomb();
        eax = 0;
    }else {
        int rax = *((int*)(rsp + 8));
        switch (rax) {
            case 0:
                eax = 207;
                break;
            case 1:
                eax = 311;
                break;
            case 2:
                eax = 707;
                break;
            case 3:
                eax = 256;
                break;
            case 4:
                eax = 389;
                break;
            case 5:
                eax = 206;
                break;
        }
    }

    if(eax != *((int*)(rsp + 12)))
        explode_bomb();
}
/*
(gdb) x/6a 0x402470
0x402470:       0x400f7c <phase_3+57>   0x400fb9 <phase_3+118>
0x402480:       0x400f83 <phase_3+64>   0x400f8a <phase_3+71>
0x402490:       0x400f91 <phase_3+78>   0x400f98 <phase_3+85>
*/
// 0 207

void phase_4(input) {
    void* rsp;
    void* rcx = rsp + 12;
    void* rdx = rsp + 8;
    // 读取的两个数字保存在上面对应的地址中
    int eax = scanf("%d %d", rdx, rcx);
    if(eax != 2)
        explode_bomb();
    if(*((int*)(rdx)) > 14)
        explode_bomb();
    
    int rdx = 14;
    int rsi = 0;
    int rdi = *((int*)(rsp + 8));
    eax = func4(rdi, rsi, rdx);
    if(eax != 0)
        explode_bomb();
    if(*((int*)(rsp + 12)) != 0)
        explode_bomb();
}

int func4(rdi, rsi, rdx) { // 初始时 rsi = 0, rdx = 14;
    int rax = rdx;
    rax -= rsi;
    int rcx = rax;
    rcx = (unsigned)(rcx) >> 31;
    rax += rcx;
    rax >>= 1;
    rcx = rsi + rax;

    if(rcx <= rdi) {
        rax = 0;
        // rcx == rdi 时结束, 发现 rdi 其实一直不会改变
        // 列出等式 : 解设我们的输入为 x;
        // rcx = rax >> 1 => (rax + rcx) >> 1 => 
        // (rdx + (unsigned)(rdx) >> 31) >> 1 => 14 >> 1 = 7
        if(rcx >= rdi) return rax;

        rsi = rcx + 1;
        rax = func4(rdi, rsi, rdx);
        rax = 2 * rax + 1;
        return rax;
    }else {
        rdx = rcx - 1;
        rax = fun4(rdi, rsi, rdx);
        rax += rax;
        return rax;
    }
}
// 7 0

void phase_5(char* rdi) {
    void* rsp;

    char* rbx = rdi;
    char* rax = rdi; // 经过debug, 发现%fs:0x28是输入字符串的地址
    *((int*)(rsp + 24)) = rax;
    rax = 0;
    rax = string_length(rdi);
    if(rax != 6)
        explode_bomb();
    
    for(int rax = 0 ; rax != 6 ; rax ++) {
        char rcx = *(rax + rbx);
        int cl = ((int)rcx) & 0xF; // 取rcx的0~7位作为值

        *((int*)rsp) = cl; // 放在栈 rsp上
        int rdx = *rsp;
        rdx &= 0xF;
        rdx = rdx + 0x4024b0;
        *(rax + rsp + 16) = rdx;
    }
    // 9 15 14 5 6 7
    // 0x9, 0xF , 0xE, 0x5, 0x6, 0x7
    // IONEFG
    
    *(rsp + 22) = 0;
    int rsi = 0x40245e; // "flyers"
    int rdi = rsp + 16;
    int eax = string_not_equal(rdi, rsi);
    if(eax != 0)
        explode_bomb();
}

void phase_6(input) {
    void* rsp;
    void* r13 = rsp;
    void* rsi = rsp;
    // 六个数字分别在 rsi, rsi + 4, rsi + 8, rsi + 12, rsi + 16, rsi + 20,
    read_six_number(?, rsi);
    void* r14 = rsp; 
    int r12d = 0;
->  .L1
    void* rbp = r13;
    int rax = *((int*)r13); // first number: N1
    rax -= 1;   // N1--;
    if(rax > 5) explode_bomb(); // [N1 > 6]
    r12d ++;


    if(r12d == 6) {
        rsi = *(rsp + 24);
        rax = r14;
        int rcx = 7;
        do {
            rdx = rcx;
            rdx -= *(rax) [int];
            *rax = rdx;
            rax += 4;            
        }while(rax != rsi);
        rsi = 0;

        // ---------------------
        .L2
        rcx = *(rsi + rsp);
        rdx = 0x6032d0;
        if(rcx <= 1) {
            .L3
            *(2 * rsi + rsp + 20) = rdx;
            rsi += 4;
            if(rsi == 24) {
                rbx = *(rsp + 32);
                rax = rsp + 40;
                rsi = rsp + 80;
                rcx = rbx;
                .L4
                rdx = *rax;
                *(rcx + 8) = rdx;
                rax += 8;
                if(rax == rsi) break;
                rcx = rdx;
                goto -> .L4


                *(rdx + 8) = 0;
                rbp = 5;
                .L5;
                rax = *(rbx + 8)
                rax = *(rax);
                if(*(rbx) <  rax) {
                    explode_bomb();
                }
                rbx = *(rbx + 8);
                rbp -= 1;
                if(rbp != 0)
                    goto .L5;
                
            }
            -> .L2
        }else {
            rax = 1;
            do {
                rdx = *(rdx + 8);
                rax + 1;
            }while(rax != rcx);
            -> .L3
        }

    }
}

void phase_6(input) {
    void* rsp;
    void* r13 = rsp;
    void* rsi = rsp;
    // 六个数字分别在 rsi, rsi + 4, rsi + 8, rsi + 12, rsi + 16, rsi + 20,
    read_six_number(?, rsi);
    void* r14 = rsp; 
    int r12d = 0;
    while (r12d != 6) {
        void* rbp = r13;
        int rax = *((int*)r13); // r13 处存放的数字 X
        rax -= 1;   // X--;
        if(rax > 5) explode_bomb(); // [X > 6]
        r12d ++; //当前坐标 + 1
        if(r12d == 6) break;
        int rbx = r12d; // 下一个数的坐标
        do {
            rax = rbx;
            rax = 4*rax + rsp;
            if(*rbp == rax) explode_bomb();
            rbx += 1;            
        }while (rbx <= 5);
        r13 += 4;
    }
    // 上面的要求是 : 这六个数字都要<=6, 并且不能重复
    // 使用了 jbe 代表是无符号数, 所以只可能是 [1,2,3,4,5,6] 中的数

    int rsi = *(int*)(rsp + 24);
    void* rax = r14; // rax = rsp;
    int rcx = 7;
    do {
        int rdx = rcx; // rdx = 7;
        rdx -= *(int*)(rax); // 7 - 当前循环到的数
        *(int*)rax = rdx;
        rax += 4;            
    }while(rax != rsi);
    // 上面的代码的影响是 : 将所有输入的数 x 变成 7-x

    int rsi = 0;
    do {
        int rcx = *(int*)(rsp + rsi); // 从0开始循环六个数字, rcx存储值
        void* rdx = 0x6032d0;
        if(rcx > 1) { // 如果值大于1, 也就是说原值 x < 6
            rax = 1;
            rdx = 0x6032d0;
            do {
                rdx = *(rdx + 8);
                rax + 1;
            }while(rax != rcx);
        }
        *(2 * rsi + rsp + 32) = rdx; // 
        rsi += 4;
    }while (rsi != 24);
    //这个模块后, 从 rsp + 32 后开始的内容由 7-x 决定
    // x = 6 : 0x6032d0
    // x = 5 : 0x6032e0 ....

    int rbx = *(int*)(rsp + 32); // 获取 rsp + 32 的值 (这个值是节点的地址)
    rax = rsp + 40; // rsp + 40 的地址
    rsi = rsp + 80; // rsp + 80 的地址, 不会到达
    rcx = rbx;
    int rdx;
    while(rax != rsi) {
        *(rcx + 8) = *rax; // 将 rcx 代表的节点的下一个地址替换成 rax
        rax += 8;
        if(rax == rsi) break;
        rcx = rdx;        
    }
    *(rdx + 8) = 0; // 将最后一个节点的后继节点设为 0
    // 这一部分的主要目的是, 假设我们从 rsp + 32 处的节点编号为 [2,5,1,3,0]
    // 那么经过这次操作将会将对应节点的下一个节点设置成我们想要的
    // 即 2 -> 5 -> 1 -> 3 -> 0
    

    for(int rbp = 5 ; rbp != 0 ; rbp--) {
        rax = *(rbx + 8); // 保存下一个节点的地址
        rax = *(rax);   // 将下一节点的值保存在 rax 中
        // 将当前节点的值 *(rbx) 与下一个节点的值 rax 比较
        if(*(rbx) < rax) explode_bomb();
        rbx = *(rbx + 8);
    }
    // 这个部分是判断整个链表的元素是不是降序的
    // 我们将初始状态的链表值附上
    // 0x014c -> 0x00a8 -> 0x039c -> 0x02b3 -> 0x01dd -> 01bb [6,5,4,3,2,1]

}
// 4 3 2 1 6 5

void phase_defused() {  // 简易版
    void* rsp;
    static int num_input_string; // 全局定义
    // num_input_string 意思是已经输入了几个字符串, 即第几个炸弹
    if(num_input_string != 6) return;
    
    void* r8 = rsp + 16;
    void* rcx = rsp + 12;
    void* rdx = rsp + 8;

    int eax = scanf("%d %d %s", rdx, rcx, r8);
    if(eax != 3) return;
    void rsi = 0x402622; // DrEvil
    int eax = strings_not_equal(rsi, rsp + 16);
    if(eax != 0) return ;
    
    secret_phase();
}

void secret_phase(input) {
    // 输入字符串的起始地址在rax中
    char* rax = read_line();
    int rdx = 10;
    int rsi = 0;
    // 将字符串转化为整数返回到 rax 中
    int rax = strtol(rax);
    int rbx = rax;
    rax = rax - 1;
    if(rax > 1000)
        explode_bomb();
    
    rsi = rbx;
    int rdi = 0x6030f0;
    int eax = fun7(rdi, rsi); // 0x6030f0, [input]
    if(eax != 2)
        explode_bomb();

    // "Wow! You've defused the secret stage!"
    rdi = 0x402438;
}

int fun7(void* rdi, int rsi) {
    if(rsi == 0) 
        return 0xFFFFFFFF;
    
    int rdx = *(int*)(rdi);
    if(rdx <= rsi) {
        if(rdx == rsi) return 0;
        
        rdi = * (rdi + 16); // rdi 变成了右节点的地址
        int rax = fun7(rdi, rsi);
        return 2 * rax + 1;
    }

    rdi = *(rdi + 8); // rdi 变成了左节点的地址
    int rax = fun7(rdi, rsi);
    return rax * 2;
}
// 先向左, 再向右, 最后相等返回 0  -> < > =
// < 36 > 8 => 22
