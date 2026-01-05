1.html 需要go环境或者用其他的http服务器都可以，他适配20*48的点阵屏幕
2.spi的例子是借鉴网上开源项目的
3.inlcude里的spi.h 是因为我的3588libc有点老没有spi.h 所以我从其他地方复制的
4.test.h 为html生成的
5.非常重要一点，3588的spi默认单次最大发送4096个字节，因为spi模拟ws2812一个点阵要24个字节，所以170个点之后就无法发送所以需要修改内核spi的驱动修改地方在kernel-6.1/drivers/spi/spidev.c 修改static unsigned bufsiz = 4096;改为需要的大小