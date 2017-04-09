## 概要

本工程为清华大学计算机系大一小学期第一周Qt的大作业

## 构建指南

需要安装Qt5

下载后删除Calendar.pro.user，其中包含Build文件的输出位置，否则Build之后容易找不到输出。

随后打开Calendar.pro，待配置Build目录之后，将chinese.qm复制到Debug/Release目录下，最后并点击“运行”即可

## 文件说明

运行Calendar.exe后会生成的文件及功能如下

-   data.xml  保存每日的事件、颜色信息
-   settings.ini  保存语言、文件拖拽设置

更换语言需要重新启动