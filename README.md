# zkSupersu
使用ndk-build编译生成su_server,su_client.

在系统源码目录 
/system/core/rootdir/init.rc
末尾加上

    service su_server /system/bin/su_server
    class main
    user root
    group root
    oneshot
    
重新编译刷入image

然后

    adb push su_server /system/bin

    adb push su_client /system/bin
    
    adb shell reboot

通过命令su_client <command> 来执行root指令
