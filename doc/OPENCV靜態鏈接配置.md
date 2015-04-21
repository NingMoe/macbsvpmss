# OPENCV靜態鏈接配置 #
調試->macbsvpmss屬性(工程名屬性)  
配置屬性-> 平台工具集 修改成Visual Studio 2012 - Windows XP (v110_xp)  
配置屬性-> C/C++-> 代碼生成->運行庫修改成 多线程调试 (/MTd)  
配置屬性-> 鏈接器-> 常規 ->附加庫目錄 修改成本機的靜態庫文件夾位置 即 ....\\opencv\build\x86\vc11\staticlib;%(AdditionalLibraryDirectories)  
注意由於本項目用的是V11的版本 所以注意不選錯版本 

配置屬性-> 鏈接器-> 輸入 ->附加依賴項 增加
vfw32.lib
comctl32.lib
IlmImfd.lib
libjasperd.lib
libjpegd.lib
libpngd.lib
libtiffd.lib
opencv_contrib249d.lib
opencv_core249d.lib
opencv_features2d249d.lib
opencv_flann249d.lib
opencv_gpu249d.lib
opencv_highgui249d.lib
opencv_imgproc249d.lib
opencv_legacy249d.lib
opencv_ml249d.lib
opencv_nonfree249d.lib
opencv_objdetect249d.lib
opencv_ocl249d.lib
opencv_photo249d.lib
opencv_stitching249d.lib
opencv_superres249d.lib
opencv_ts249d.lib
opencv_video249d.lib
opencv_videostab249d.lib
opencv_calib3d249d.lib
Zlibd.lib 

其中vfw32.lib comctl32.lib 必須有(這2個文件WINDOW自帶 添加名即可) 
而其他的.lib 文件 如果生成DEBUG版則如上添加 *d.lib  如果生成RELEASE版則需要添加*.lib 而非上述的*d.lib文件

最後生成即可