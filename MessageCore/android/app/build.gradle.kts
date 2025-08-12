import java.text.SimpleDateFormat
import java.util.Date
// if (rootProject.hasProperty("useGlobal") && "$useGlobal"=="true") {
//     println("Use global settings: ${globalValues}")
// }else {
//     ext.kotlin_version = '1.4.10'
//     ext.globalValues = [
//         TargetSdkVer:   30,
//         CompileSdkVer:  30,
//         BuildToolsVer:  '30.0.2'
//     ]
// }

//定义时间
// fun releaseTime() {
//     return Date().format("yyyyMMdd_HHmmss", TimeZone.getTimeZone("GMT+08:00"))
// }

// fun getCurrentGitBranch() {
//     val process = 'git symbolic-ref --short HEAD'.execute()
//     process.waitFor()
//     return process.text.trim()
// }

// fun getGitCommitId() {
//     return "git rev-parse --short HEAD".execute().text.trim()
// }

// fun get_version_info() {
//     return "ImiSDK_V${project.android.defaultConfig.versionName}_T${releaseTime()}_B_${getCurrentGitBranch()}_C_${getGitCommitId()}"
// }

import java.util.Properties
import java.io.FileNotFoundException

plugins {
    id("com.android.library")
    id("org.tboox.gradle-xmake-plugin")
}


fun getSdkDir(name:String) : String {
    val properties = Properties()
    val localPropertiesFile =  rootProject.file("local.properties")
    if (localPropertiesFile.exists()) {
        properties.load(localPropertiesFile.inputStream())
        return properties?.getProperty(name)?:""
    } else {
        throw FileNotFoundException("local.properties file not found")
    }
}

//fun getSdkDir() {
//    val properties = Properties()
//    val localPropertiesFile = rootProject.file("local.properties")
//    if (localPropertiesFile.exists()) {
//        properties.load(localPropertiesFile.newDataInputStream())
//        return properties.getProperty("sdk.dir")
//    } else {
//        throw FileNotFoundException("local.properties file not found")
//    }
//}

//
android {
    namespace = "com.sximi.mrmanager"
    compileSdk = 30 //globalValues.CompileSdkVer
    // buildTools = "30.0.2" //globalValues.BuildToolsVer



    defaultConfig {
        minSdk = (21)
//        targetSdk = (33)
        //targetSdk = 30 //globalValues.TargetSdkVer
//        versionCode = 1
//        versionName = "1.0.0"

        testInstrumentationRunner= "androidx.test.runner.AndroidJUnitRunner"

        // 支持的CPU配置
        ndk {
            abiFilters += listOf("arm64-v8a")
        }

        externalNativeBuild {
            xmake {
                // append the global cflags (optional)
                // cFlags "-DTEST"
                // // append the global cppflags (optional)
                // cppFlags "-DTEST", "-DTEST2"
                // switch the build mode to `debug` for `xmake f -m debug` (optional)
                buildMode = "release"
                // set abi filters (optional), e.g. armeabi, armeabi-v7a, arm64-v8a, x86, x86_64
                // we can also get abiFilters from defaultConfig.ndk.abiFilters
                abiFilters("arm64-v8a")   //"armeabi-v7a", 
            }
        }
    }

    externalNativeBuild {
        xmake {
            path = "../../../xmake.lua"  //使用xmake编译JNI
            // enable verbose output (optional), e.g. verbose, warning, normal
            logLevel = "verbose"
            // set c++stl (optional), e.g. c++_static/c++_shared, gnustl_static/gnustl_shared, stlport_static/stlport_shared
            stl = "c++_static"
            arguments("-P",".")
            targets("messagecore_jni")

            // set the given xmake program path (optional)
            // program /usr/local/bin/xmake
            // disable stdc++ library (optional)
            // stdcxx false
            // set the given ndk directory path (optional)
            // ndk "/Users/ruki/files/android-ndk-r20b/"
            // set sdk version of ndk (optional)
            // sdkver 21

        }
    }



    // buildTypes {
    //     release {
    //         buildConfigField 'String', 'VERSION_INFO', "\"" + get_version_info() + "_release\""
    //         minifyEnabled false
    //         proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
    //     }
    //     debug {
    //         buildConfigField 'String', 'VERSION_INFO', "\"" + get_version_info() + "_debug\""
    //         jniDebuggable true
    //         ndk {
    //             // 保留所有的调试符号信息
    //             debugSymbolLevel 'FULL'
    //         }
    //         packagingOptions{
    //             doNotStrip "**.so"
    //         }
    //     }
    // }

    sourceSets {

        getByName("androidTest") {
            manifest.srcFile("src/androidTest/AndroidManifest.xml")
            java.srcDirs("src/androidTest/java", "../java_lang/test/java")
            res.srcDirs("src/androidTest/res")
        }

        getByName("main") {
            // java.srcDirs("${projectDir}/../java_lang/java_4_android/java")
            // java.srcDirs "${projectDir}/../../swig0/java"
            // jniLibs.srcDirs "${projectDir}/../../../../out/android"
        }

    }

    lintOptions {
        isAbortOnError =  false
    }

    // repositories{
    //     flatDir{
    //         dirs 'libs'
    //     }
    // }

    // compileOptions {
    //     sourceCompatibility 1.8
    //     targetCompatibility 1.8
    // }
    compileOptions {
        sourceCompatibility (JavaVersion.VERSION_11)
        targetCompatibility (JavaVersion.VERSION_11)
    }

    //配置自定义打包名称
//    libraryVariants.all { variant ->
//        variant.outputs.all { output ->
//            def outputFile = output.outputFile
//            if (outputFile != null && outputFile.name.endsWith('.aar')) {
//                if (variant.buildType.name.equals('release')) {
//                    outputFileName = "ImiSDK_V${defaultConfig.versionName}_T${releaseTime()}_B_${getCurrentGitBranch()}_C_${getGitCommitId()}_release.aar"
//                } else if (variant.buildType.name.equals('debug')) {
//                    outputFileName = "ImiSDK_V${defaultConfig.versionName}_T${releaseTime()}_B_${getCurrentGitBranch()}_C_${getGitCommitId()}_debug.aar"
//                }
//            }
//        }
//    }

    packagingOptions {
        
    }

    libraryVariants.all {
        outputs.all {
            (this as com.android.build.gradle.internal.api.LibraryVariantOutputImpl).apply {
                val buildTime = SimpleDateFormat("yyyyMMddHHmmss").format(Date())
                outputFileName = "MessageCore-${name}-$buildTime.aar"
            }
        }
    }
}

// 定义删除特定文件的任务
tasks.register<Delete>("deleteSpecificFiles") {
    // 日志输出
    doFirst {
        println("开始删除旧的AAR和APK文件...")
    }

    // 删除build目录下所有的AAR和APK文件
    delete(fileTree("build") {
        include("**/*.aar", "**/*.apk")
    })

    doLast {
        println("旧文件删除完成")
    }
}

// 项目评估完成后配置任务依赖
project.afterEvaluate {
    // 查找preBuild任务
    val preBuildTask = tasks.findByName("preBuild")
    val preReleaseBuildTask = tasks.findByName("preReleaseBuild")
    val preDebugBuildTask = tasks.findByName("preDebugBuild")

    // 添加依赖关系
    preBuildTask?.dependsOn(tasks["deleteSpecificFiles"])
    preReleaseBuildTask?.dependsOn(tasks["deleteSpecificFiles"])
    preDebugBuildTask?.dependsOn(tasks["deleteSpecificFiles"])

    // 日志输出
    println("已配置删除任务依赖关系")
}

// dependencies {
//     //
//     // compile files("../sdklibs/RawImageLite.jar")

//     implementation files("libs/imimrapi_jni.jar")

//     compileOnly 'com.android.support:appcompat-v7:28.0.0'
//     compileOnly 'org.jetbrains:annotations:13.0'

//      testImplementation 'junit:junit:4.12'
//     // //for testing
// //     androidTestImplementation 'androidx.test:runner:1.1.0'
// //     androidTestImplementation 'androidx.test:rules:1.1.0'
// //    androidTestImplementation 'androidx.test.espresso:espresso-core:3.5.1'
// //    androidTestImplementation 'androidx.test.ext:junit:1.1.5'
//     androidTestImplementation 'androidx.test:runner:1.1.0'
//     androidTestImplementation 'androidx.test:rules:1.1.0'
//      androidTestImplementation "androidx.core:core-ktx:1.3.2"
//      androidTestImplementation "org.jetbrains.kotlin:kotlin-stdlib:$kotlin_version"
// }

//dependencies {
//    androidTestImplementation 'androidx.test.espresso:espresso-core:3.5.1'
//    androidTestImplementation 'androidx.test.ext:junit:1.1.5'
//    androidTestImplementation 'androidx.test:runner:1.5.2'
//    androidTestImplementation 'androidx.test:rules:1.5.2'
//}

dependencies {
    implementation (files("libs/messagecore_jni.jar"))


//    implementation ("androidx.appcompat:appcompat:1.6.1")
//    implementation ("com.google.android.material:material:1.9.0")
//    implementation ("androidx.constraintlayout:constraintlayout:2.1.4")
    testImplementation ("junit:junit:4.13.2")
    androidTestImplementation ("androidx.test.ext:junit:1.1.5")
    androidTestImplementation ("androidx.test.espresso:espresso-core:3.5.1")

//    compileOnly ("androidx.appcompat:appcompat:1.6.1")
//    compileOnly ("org.jetbrains:annotations:13.0")

     //     androidTestImplementation 'androidx.test:runner:1.1.0'
 //     androidTestImplementation 'androidx.test:rules:1.1.0'
 //    androidTestImplementation 'androidx.test.espresso:espresso-core:3.5.1'
 //    androidTestImplementation 'androidx.test.ext:junit:1.1.5'
//    androidTestImplementation ("androidx.test:runner:1.+")
//    androidTestImplementation ("androidx.test:rules:1.+")
//    androidTestImplementation ("androidx.core:core-ktx:1.3.2")
//    androidTestImplementation ("org.jetbrains.kotlin:kotlin-stdlib:1.9.0")

//    implementation ("androidx.appcompat:appcompat:1.6.1")
//    implementation ("com.google.android.material:material:1.9.0")
//    implementation ("androidx.constraintlayout:constraintlayout:2.1.4")
//    testImplementation ("junit:junit:4.13.2")
//    androidTestImplementation ("androidx.test.ext:junit:1.1.5")
//    androidTestImplementation ("androidx.test.espresso:espresso-core:3.5.1")
}

/*
apply plugin: "com.imi.plugins.imidoc"
imidoc {
    lang = "en"
    title = "ImiCamera API 参考文档"
    addEnv("image_dir","${projectDir}/doc/images")
    // 人脸SDK Base库文档，直接集成到人脸SDK中，Base库不再单独输出文档
    srcDirs("${projectDir}/../../swig/java")
    outputDir("${projectDir}/build/imidoc")
}*/
