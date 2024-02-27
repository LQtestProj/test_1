未跟过C++项目乱写的东西,使用了XP时代的技术和思想
release目录下有生成测试文件
简单的使用FFT画图,选择启动项Bezeir编译生成.

![GIF 2024-2-27 10-33-08](https://github.com/LQtestProj/test_1/assets/161401866/c7c554b6-2e6d-4493-b75c-23d3533182e4)



![截图未命名0227101312](https://github.com/LQtestProj/test_1/assets/161401866/ffc2f3a7-0d4b-4c36-8271-cde5aa8eaff7)

当运行目录内有b.bmp时会直接读取,注意画布大小不要修改,否则会出现绘制出界内存申请失败等问题
可使用"打开图片"来选择图片

![截图未命名0227101758](https://github.com/LQtestProj/test_1/assets/161401866/3840b6b5-fe6b-4e92-8a5f-a10662f5ec85)

图片可以随便画,只要保证单色以及控制画布大小就好

![截图未命名0227102106](https://github.com/LQtestProj/test_1/assets/161401866/1b11bfe4-57fd-43f9-b652-93a74eb6ec3c)

计算FFT算法代码未经过优化,优化代码满世界都是,所以运行时要看看自己的计算机有没有性能,不然会卡爆.我这是根据数学计算思路进行的代码运行顺序,懂公式的人应该理解的会很快.

如有需要更完美漂亮的FFT画图代码,前往https://www.jezzamon.com/fourier/
