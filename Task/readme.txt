任务文件夹主要存放各个小功能（部分组件）组成的任务，任务的划分，根据功能的类似性和属性划分。
原则上，除了message和Tasksystem会被其他任务引用或者引用其他任务，其他任务之间拒绝引用，减少耦合性，需要通过消息进行任务与任务之间的通信；
组件之间，可以直接调用其他组件get/set函数来进行数据传递；

根据对输入信息的特性和响应速度需求，有几种方式可以进行：

输入信息            处理方式                            响应速度（1...2...3...,最快...慢...最慢...）  这里规定下
中断形式            中断中处理完                            1
中断形式            中断中置标志位，主循环中处理              2
ms级定时查询        立即处理                                3
ms级定时查询        置标志位，主循环中处理                   4                                          采用消息传递
主循环中不定时查询   置标志位，主循环中处理                   4                                          采用消息传递
s级定时查询         立即处理                                5                                          
s级定时查询         置标志位，主循环中处理                   6                                          尽量减少用，而采用立即处理

