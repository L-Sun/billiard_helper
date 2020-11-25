# 2D斯诺克桌球辅助

## 前置条件
本辅助只适用于斯诺克桌球，并且将设置中的深度、阴影关闭，打开2D斯诺克球体。

## 使用方法
1. 提前设置好深度、阴影、球体
2. 进入桌球房间并打开该辅助
3. 鼠标移动至目标球，按下`Ctrl+D`会自动判断你想要的击球点
4. 按下`Ctrl+F`显示当前库边反射路径（误差大）

## 编译
### 环境
请确保安装了[Vcpkg](https://github.com/microsoft/vcpkg/)，CMake，以及C++17以上的编译环境。使用Vcpkg进行OpenCV安装，安装方式如下
```ps
vcpkg.exe install opencv:x64-windows
```
如遇到无法下载，大概率为网络问题，请自行搜索解决方法。

### 编译
1. 方法一**VSCode + CMake + Vcpkg** ：在VSCode中安装CMakeTools插件，并将`.vscode/settings.json`中的`DCMAKE_TOOLCHAIN_FILE`的值修改为`${pathToVcpkg}/scripts/buildsystems/vcpkg.cmake`，其中`${pathToVcpkg}`为Vcpkg的安装路径。然后使用VSCode编译即可。

2. 方法一：直接使用命令行编译，注意将`${pathToVcpkg}`换为Vcpkg的安装路径
```ps
mkdir build
cd bulid
cmake .. -DCMAKE_TOOLCHAIN_FILE=${pathToVcpkg}/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release -j 8
```