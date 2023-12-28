# CMU - CSAPP

> 项目介绍

CMU [卡内基梅隆大学] 镇校深课 ： 深入理解计算机系统 CSAPP 每章节附带 lab 实验

内容是通过手写计算机系统的相关代码提升学生对于计算机体系的整体理解和细节处理，采用 C语言和汇编

- 课程网站：http://csapp.cs.cmu.edu/
- 课程视频：https://www.bilibili.com/video/BV1iW411d7hd
- 课程教材：Computer Systems: A Programmer's Perspective, 3/E
- 课程作业：11 个 Project，[代码框架全部开源](http://csapp.cs.cmu.edu/3e/labs.html)



> 开始

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



> 更新你的 lab

由于我们的环境中没有 `vim` 等编辑器， 所以我们采用本机进行编辑

首先将写好的文件导入你的虚拟机中

* 如果你使用 finalshell 可以直接远程连接上传， 具体的软件有具体的方法， 大家根据自己情况百度

将虚拟机的文件导入你的实验环境中

* 我们使用 docker 的命令

  > 首先你需要获取你的实验环境的 docker 容器 id
  >
  > ```dockerfile
  > docker ps -a
  > ```
  >
  > 找到形如 `25b487e40d0e ` 的即是 ID
  >
  > 接着在你的虚拟机中运行如下命令
  >
  > ```dockerfile
  > docker cp 虚拟机地址 docker容器ID:容器内你要放置的地址
  > ```
  >
  > 如 ` docker cp /home/yyym/kk.txt 25b487e40d0e:/root/csapplab/datalab/datalab-handout`

这样就实现了对文件的更新



> 测试你的答案

首先每次修改完内部文件后都要更新 btest 文件， 执行以下命令

```c
make btest
```

接着就是测试， 执行 `./btest` 查看分数

```
root@25b487e40d0e:~/csapplab/datalab/datalab-handout# ./btest  
.....
Total points: 0/36
```

更详细操作见 ： [CSAPP:Lab1 -DataLab 超详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/339047608)



> 笔记

各个 lab 的笔记都在 note文件夹里（包括原文档 / 我的笔记[.md] / 我的笔记 [.pdf]）

同时笔记也会发布到知乎上

* CSAPP - 信息的表示和处理 : https://zhuanlan.zhihu.com/p/666331015
* CSAPP-链接 https://zhuanlan.zhihu.com/p/671477739



> 时间线

并不是持续的完成本项目，所以完成时间不代表持续时间

* datalab   2023 - 11 - 10 【笔记待定】
* bomblab   2023 - 11 - 23 【笔记待定, 彩蛋没有拆除】
* shlab 2023 - 12 - 28
