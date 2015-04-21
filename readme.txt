NDK 编译会报错：
error: format not a string literal and no format arguments [-Werror=format-security] 

原因：
as of Android NDK revision 9, the following should be added to Android.mk as a work-around for the quoted error.

解决方法为：
在Android.mk 下添加：
"LOCAL_DISABLE_FORMAT_STRING_CHECKS := true"