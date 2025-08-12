# MR Agent 

## 目录结构

+ cpp：接口定义及C++实现代码
+ java_lang: Java接口（Android）
+ csharp：C#接口（Unity3D）


Android平台与U3D平台的接口定义，方法流程基本相同。

接口API由

#### 更新接口

**目前更新接口要在PC平台进行**

```
xmake config -p windows -a x64  --vs_runtime=MD --update_mragent_wrapped_code=yes
```
更新C#
```
xmake build mragent_cs
```
更新Java
```
xmake build mragent_jni
```

#### PC平台

```
xmake build mragent_cpp
```

#### Android平台

先编译so库

```
xmake config -p android -a arm64-v8a --build_java_api=yes --update_mragent_wrapped_code=yes
xmake build mragent_jni
```

```
xmake config -p android -a armeabi-v7a --build_java_api=yes --update_mragent_wrapped_code=yes
xmake build mragent_jni
```

在编译AAR

```
<path/to>/gradle.bat :app:assembleRelease
```


#### U3D平台

1. 先编译Android平台的so库

```
xmake config -p android -a arm64-v8a 
xmake build mragent_cs
```

```
xmake config -p android -a armeabi-v7a 
xmake build mragent_cs
```

2. 再编译PC平台的dll库

```
xmake config -p windows -a x64 --update_mragent_wrapped_code=no
xmake build mragent_cs
```

或者Linux平台的so库（如果需要的话）

```
xmake config -p linux -a x86_64 --update_mragent_wrapped_code=no
xmake build mragent_cs
```

3. 打包输出


## 示例代码

[]()





