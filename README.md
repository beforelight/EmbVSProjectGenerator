# EmbVSProjectGenerator

#### 介绍
嵌入式开发的IDE要么古老，要么功能不全，这个项目的用途就是将这些嵌入式项目转换(读写xml文档)成vs\vscode等能识别的格式，这样可以使用代码补全等等一系列加速编写代码的功能（ **编译调试还是得在原来的IDE** ）。

支持：
- Keil
- IAR
- MCUXpresso IDE
- STM32CubeIDE
- Code Composer Studio
- AURIX Development Studio

转换支持：
- vs
- vscode
- Clion(CMake)

#### 编译
1.  git clone & git submodule init  
2.  使用CMake编译
3.  打包exe和resource文件夹


#### 使用说明

使用exe打开项目相应的.cproject/.uvprojx/.ewp文件

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
