# unsplash4deepin-qt
>  Deepin下的unsplash壁纸更换程序，因为swing版本内存占用达50M左右，所以用QT写了一个，本程序内存占用10M，个人觉得挺完美的了，适配至deepin 20。因未系统了解C++及业余时间有限，代码结构会比较混乱。


![Logo](https://github.com/shansb/unsplash4Deepin/blob/master/unsplash4Deepin/src/resource/Taskbar.png?raw=true)

### 增加适配win10

win10版本说明详见[这里](https://github.com/shansb/unsplash4deepin-qt/blob/master/README-WIN10.md)

### 安装

如遇无法启动无法设置开机自启动等问题请卸载后重新安装即可。

### 使用

安装好之后需要手动启动，需要手动右击图标添加开机自启动。

1. 手动刷新：点击`立即刷新`菜单进行刷新。
2. 设置刷新时间：右击托盘图标，点击`刷新间隔`进入子菜单即可勾选指定的刷新时间。
   1. 设置刷新时间后自动在下一次刷新之后生效，也可通过手动刷新即时生效。
   2. 如需自定义其他刷新时间，可以打开`~/.config/unplash4deepin/setting.ini`，更改`CycleTime` 的值，重启应用即可生效。
3. 壁纸源：
   1. unsplash
   2. picsum
   3. 风云4号：实时卫星图
4. 分类：只有当壁纸源为unsplash时可用。
5. 保存壁纸：会将当前壁纸保存到~/Pictures/unsplash/ 目录里中。
6. 关于：跳转github项目主页。
7. 退出：退出程序。

### 截图

![Logo](https://github.com/shansb/unsplash4deepin-qt/blob/master/screenshot.png?raw=true)