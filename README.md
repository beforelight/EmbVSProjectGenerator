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
1. git clone 
   ```
   git clone https://gitee.com/beforeligeht/EmbVSProjectGenerator.git vspg
   cd vspg
   git submodule update --init
   ```
   
2. 使用CMake编译

   ```
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release .. 
   cmake --build ./ --target vspg
   ```

3. 复制生成的exe到你想要的位置


#### 使用说明

将项目相应的.cproject/.uvprojx/.ewp文件以此软件打开

#### 下载

[Releases · beforelight/EmbVSProjectGenerator (github.com)](https://github.com/beforelight/EmbVSProjectGenerator/releases)

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
