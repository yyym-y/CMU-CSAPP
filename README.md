# CMU - CSAPP

> ### **项目介绍**

CMU [卡内基梅隆大学] 镇校深课 ： 深入理解计算机系统 CSAPP 每章节附带 lab 实验

内容是通过手写计算机系统的相关代码提升学生对于计算机体系的整体理解和细节处理，采用 C语言和汇编

- 课程网站：http://csapp.cs.cmu.edu/
- 课程视频：https://www.bilibili.com/video/BV1iW411d7hd
- 课程教材：Computer Systems: A Programmer's Perspective, 3/E
- 课程作业：11 个 Project，[代码框架全部开源](http://csapp.cs.cmu.edu/3e/labs.html)

值得一提的是, CSAPP 也是 HIT(哈尔并工业大学), PKU(北京大学) 计算机考研的考试书籍

HIT 课程资料取自仓库 : https://github.com/LMC117/HIT-CSAPP-2021Spring



> ### **开始**

**使用虚拟机部署实验环境 (推荐)**

如果你有一台运行 `Linux` 系统的 64 位虚拟机, 那么部署环境将会变得非常简单

>  *如果你没有虚拟机 , 可以自行搜索网上的资料学习安装*

步骤如下 : 

1. 在你的虚拟机上安装 `git`

   ```
   # debian/ubuntu
   sudo apt install git -y
   # centos/rehl
   sudo yum install git -y
   ```

2. 克隆本仓库

   ```
   git clone https://github.com/yyym-y/CMU-CSAPP.git
   ```

3. 进入每一个 `lab` , 删掉 `lab` 中的 `handout` 文件夹

   > 我们拿 `datalab` 举例, 进入 `datalab` 文件夹, 删除 `datalab-handout` 文件夹
   >
   > 所有 `handout` 文件夹是我本人对于这个 `lab` 的实现方式

4. 解压对应的 `handout.tar` 压缩包

   > 进行玩这一步后, 会有一个全新的对应 `lab` 的 `handout` 文件夹

5. 之后见招拆招即可

   > 在 `lab` 的 `handout` 文件夹内执行 `make` 命令
   >
   > 缺少什么资源, 库, 包 就安装什么即可



**使用 docker 部署环境**


项目的环境部署较为繁琐， 推荐使用 docker 导入配置


参考自知乎文章 ： [CSAPP一键环境配置、完成8个lab总结 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/505497911)

首先你需要有一台 Linux 计算机或者一台搭载 Linux 环境的虚拟机

其次在这台 Linux 机器上， 你需要有可以运行的 Docker 软件， 接着只需要一步步执行一下的代码

```dockerfile
docker pull linxi177229/csapp:latest
docker images
docker run --name csapp -itd linxi177229/csapp 
docker attach csapp
```

这样你就进入了实验的环境了，以后再想要进入只需要执行两个命令

```dockerfile
docker start csapp
docker attach csapp
```

> 有些时候会发现 docker 命令无法使用， 可能是docker 没有启动， 这时候执行以下命令
>
> ```linux
> systemctl start docker
> ```

退出环境使用 ：

```dockerfile
exit
```

本实验环境包含以下软件 ： `git` , `gcc` , `tar`

不包括  `vim`,  `vi`,  `yum`

如何更新你的文件, 这就八仙过海各显神通了



> ### 测试你的答案

每一个 `lab` 对应的测试方法不同, 需要查看对应的实验文档

如果你对英文阅读有障碍, 也可以看我的实验笔记, 会详细说明如何测试



> ### 笔记

各个 lab 的笔记都在 note文件夹里（包括原文档 / 我的笔记[.md] / 我的笔记 [.pdf]）

同时笔记也会发布到知乎上

* CSAPP - 信息的表示和处理 : https://zhuanlan.zhihu.com/p/666331015
* CSAPP-链接 https://zhuanlan.zhihu.com/p/671477739



> ### 时间线

并不是持续的完成本项目，所以完成时间不代表持续时间

- [x] **datalab**			${\color{Orange} 100}$ / ${\color{red} 100}$

  > 完成 datalab 全部内容	`2023-11-11`

- [x] **bomblab**          ${\color{Orange} 85.7}$ / ${\color{red} 100}$

  > bomblab 基础炸弹全部拆除 [最后的菜单没有拆除]

- [ ] **attacklab**         ${\color{Orange} 0}$ / ${\color{red} 100}$

- [ ] **archlab**            ${\color{Orange} 39.3}$ / ${\color{red} 100}$

  > 完成 archlab PartI     `2024-2-4`
  >
  > 完成 archlab PartII     `2024-2-4`

- [x] **cachelab**          ${\color{Orange} 100}$ / ${\color{red} 100}$

  > 完成 cachelab 的 PartA 部分    `2024-1-16`
  >
  > 完成 cachelab 全部内容      `2024-1-19`

- [x] **tshlab**               ${\color{Orange} 100}$ / ${\color{red} 100}$

  > 完成 tshlab 的 eval , builtin_cmd 和 sigchld_handler      `2023-12-28`
  >
  > 完成 tshlab 的 SIGINT 和 SIGSTP handler      `2023-12-28`
  >
  > 完成 tshlab 全部内容     `2023-12-28`

- [x] **malloclab**        ${\color{Orange} 53}$ / ${\color{red} 100}$

  > 初步完成所有内容, 但是没有完成 realloc    `2024-1-23`
  >
  > 完成 malloclab 所有内容    `2024-1-24`

- [x] **proxylab**          ${\color{Orange} 100}$ / ${\color{red} 100}$

  > 完成 proxylab PartI    `2024-1-28`
  >
  > 完成 proxylab PartII    `2024-1-28`
  >
  > 完成 proxylab PartIII     `2024-1-30`



