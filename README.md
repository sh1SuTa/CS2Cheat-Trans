﻿# CS2Cheat-Trans
一个简单的外部作弊。
对于俯仰角pitch，范围 89 ~ -89 
基础地址 cheat::clientAddress + 0x1836BB8
各偏移量可在out中找到
偏移量： https://github.com/a2x/cs2-dumper  
矩阵：  
`constexpr std::ptrdiff_t dwViewMatrix = 0x1A33E30;`  
`Pawn：constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1836BB8;`

对象玩家地址[0]获取处  
`if (!mem::Read(cheat::g_handle, cheat::clientAddress + 0x01A2A478, &cheat::ActorPlayer.Address[0],8)) {  
	continue;  
}`  
如何获取对象玩家坐标：
找到敌人的生命值地址，一般最后筛选出4个，找到344偏移的，对这个地址进行指针扫描，得到很多，
然后退出对局重新进入，点击 指针扫描器 -> 重新扫描内存 -> 要查找的数值 100 4字节，
找到偏移1：10，偏移2：0，偏移3：1C8，偏移4：344

在 高级保存选项中保存为 Unicode代码1200
## 游戏控制台指令
`bot_kick` 踢出机器人

更多带阅读文档 [nfk.putileaf.top](http://nfk.putileaf.top/index.php/2024/11/05/%e8%ae%b0%e4%b8%80%e6%ac%a1%e7%94%a8ce%e5%af%bb%e6%89%becs2%e5%9f%ba%e5%9d%80/)

