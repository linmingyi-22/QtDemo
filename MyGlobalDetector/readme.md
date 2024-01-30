这只是个案例项目

如果需要在其他项目中引用

可以将MyGlobalEvent这个文件夹拷贝到对应的pro文件夹下（也可以是其他位置，但是要确保能在pro文件中引用正确。）

然后在pro文件中引入

![image-20240131000953938](C:\Users\12465\AppData\Roaming\Typora\typora-user-images\image-20240131000953938.png)

添加头文件

![image-20240131001019792](C:\Users\12465\AppData\Roaming\Typora\typora-user-images\image-20240131001019792.png)

新建接收的槽函数并且绑定信号

<img src="C:\Users\12465\AppData\Roaming\Typora\typora-user-images\image-20240131001058350.png" alt="image-20240131001058350" style="zoom:200%;" />

然后就可以正确使用了