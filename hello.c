#include <stdio.h>

const char *getHello() {
    return "Hello,";
}

const char *getWorld() {
    return "World!";
}

// 主函数
int main() {
    // 调用两个函数并打印结果
    const char *hello = getHello();
    const char *world = getWorld();

    printf("%s %s\n", hello, world);
    return 0;
}