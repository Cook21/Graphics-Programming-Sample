opengl32.lib;freeglut.lib;glew32.lib;

这个是从博客：
VS2015安装+OpenGL环境配置及测试
https://blog.csdn.net/wizen641372472/article/details/52972231
中所附带提供的包来的。不过其提供的配置文件中缺乏：glew32.dll，我就又从别的地方找来了x64的glew32而补充之。
注意：其中的库文件都是针对x64工程的。
该博客其实讲的是如何使自己的程序能够在没有按照freeglut和glew的机器上也能运行。
