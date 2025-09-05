// Top-level build file where you can add configuration options common to all sub-projects/modules.
buildscript {
    repositories {
        //google()
        mavenCentral()
        maven(url="https://plugins.gradle.org/m2/")
        maven(url="https://maven.aliyun.com/nexus/content/repositories/google")
        maven (url="https://maven.aliyun.com/nexus/content/repositories/jcenter")
    }
    dependencies {
        classpath("com.android.tools.build:gradle:" +
                "7.4.2")
        classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:1.9.0")
        classpath("org.tboox:gradle-xmake-plugin:1.2.2")
        classpath(files("./ImiDocX.jar"))
        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}

allprojects {
    repositories {
        //todo
        mavenCentral()
        maven(url="https://plugins.gradle.org/m2/")
        maven(url="https://maven.aliyun.com/nexus/content/repositories/google")
        maven (url="https://maven.aliyun.com/nexus/content/repositories/jcenter")
    }
}