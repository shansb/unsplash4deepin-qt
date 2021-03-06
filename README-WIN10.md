# unsplash4deepin-qt windows版

> 因目前切换回win10做主力机，所以做了下移植，内存占用10M以下，没有实现windows下的气泡通知功能，以后再填坑。

[下载地址](https://github.com/shansb/unsplash4deepin-qt/releases)

### 说明

1. 采用unsplash图片源，有时候会比较慢，10分钟下载不成功会自动停止本次壁纸更新。
2. 适配屏幕分辨率。
3. 跟随系统显示中英文。
4. 多屏幕情况下取最大宽和最大长下载图片，所以如果分辨率不一样会很尴尬。

### 使用

1. 手动刷新
   - 双击托盘图标即可刷新（推荐）
   - 右击图标点击`立即刷新`菜单进行刷新。
2. 壁纸源：
      - unplash：国内访问慢，建议加入代理pac，现在可以通过api设置壁纸类别，比较爽。
      - Picsum：国内访问快，但是不是设置类别。
      - 风云4号：中国气象卫星实时图
3. 类别：当选择unplash时可用，建议使用`壁纸`类别。
4. 设置刷新时间：右击托盘图标，点击`刷新间隔`进入子菜单即可勾选指定的刷新时间。
      1. 设置刷新时间后自动在下一次刷新之后生效，也可通过手动刷新即时生效。
      2. 如需自定义其他刷新时间，可以打开用户目录下的`/.config/unplash4deepin/setting.ini`，更改`CycleTime` 的值，重启应用即可生效。
5. 保存壁纸：会将当前壁纸保存到用户目录/Pictures/unsplash/ 中，目前未实现系统通知。
6. 开机自启动：勾选后可开机自启。
7. 关于：跳转github项目主页。
8. 退出：退出程序。

### 截图

![Logo](https://github.com/shansb/unsplash4deepin-qt/blob/master/win-screenshot.png?raw=true)