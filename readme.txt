NDK ����ᱨ��
error: format not a string literal and no format arguments [-Werror=format-security] 

ԭ��
as of Android NDK revision 9, the following should be added to Android.mk as a work-around for the quoted error.

�������Ϊ��
��Android.mk ����ӣ�
"LOCAL_DISABLE_FORMAT_STRING_CHECKS := true"