"# CS2Cheat-Trans"  
一个简单的内存作弊。
对于俯仰角pitch，范围 89 ~ -89 
基础地址 cheat::clientAddress + 0x1836BB8
各偏移量可在out中找到
偏移量： https://github.com/a2x/cs2-dumper
矩阵： constexpr std::ptrdiff_t dwViewMatrix = 0x1A33E30;

Pawn：constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1836BB8;

bot_kick 踢出机器人
