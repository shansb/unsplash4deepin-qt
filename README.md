# unsplash4deepin-qt
>  Deepin下的unsplash壁纸更换程序，因为swing版本内存占用达50M左右，所以用QT写了一个，本程序内存占用10M，个人觉得挺完美的了，适配至deepin 19.5。


![Logo](https://github.com/shansb/unsplash4Deepin/blob/master/unsplash4Deepin/src/resource/Taskbar.png?raw=true)


### 安装

对于安装过swing版本的同学，先用swing安装包中的uninstall.sh进行卸载。然后使用qt版本的安装包进行安装。如遇无法启动无法设置开机自启动等问题请卸载后重新安装即可。

### 使用

安装好之后需要手动启动，需要手动右击图标添加开机自启动。

1. 手动刷新：点击`手动刷新`菜单进行刷新。
2. 设置刷新时间：右击托盘图标，点击`刷新时间`进入子菜单即可勾选指定的刷新时间。
   1. 设置刷新时间后自动在下一次刷新之后生效，也可通过手动刷新即时生效。
   2. 如需自定义其他刷新时间，可以打开`~/.config/unplash4deepin/setting.ini`，更改`CycleTime` 的值，重启应用即可生效。
3. 关于：跳转github项目主页。
4. 退出：退出程序。

### 截图

![Logo](https://github.com/shansb/unsplash4deepin-qt/blob/master/screenshot.png?raw=true)