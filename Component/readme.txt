该文件夹作为功能模块层
组件文件夹主要是根据功能虚拟划分成各个组件,比如电量组件、记录组件、冻结组件等等；
另外各个组件的数据拒绝被其他组件直接调用，只能通过方法set进行设置、通过方法get进行读取；