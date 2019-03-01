# exerciseformuduo
基于Reactor模式，使用了部分C++的新特性进行编写。现在的设计思路接近Libuv和muduo。后续会考虑添加异步模式。
代码说明：base中为源码，unittest中是单元测试，后续会把单元测试逐步完善并编写小型的服务器做整体测试。
base为源文件文件夹，现主要模块包括：
  Eventloop——事件循环器，负责主循环。
  Poller——封装Epoll，IO复用器。
  Timer——利用timerfd实现定时器功能，可以被注册在循环中。
  Channel——基础的事件管理者，绑定（非持有）一个文件描述符，并注册相关事件。
  Tcpconnection——链接管理者，持有一个socket连接及相关回调。
  Logger——多线程日志系统，尚未大规模测试。
  Buffer——收发缓冲区，内部环形实现。
unittest为单元测试，对上述模块进行测试。
现阶段仍为单线程操作，但考虑到未来的实现，在里面加入了简单的线程池接口。
