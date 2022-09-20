# SpineConvertGUI

基于 JUCE 和 pure01fx的 spine converter 控制台版本
      -> 项目地址:https://github.com/pure01fx/SpineConverter

# 放置转换器
程序目录新建一个conventer文件夹，在里面放置文件夹名为对应spine转换器版本号，在这个文件夹里放置spine转换器
如下图所示  
......  
|--spineConveter.exe  
+--conventer  
    |---3.5  
		|		|--xxx.exe  
		|		+--other files  
		|--3.6  
		|		|--xxx.exe  
		|		+--other files  
		|-- ...  
 	...  
	
# 使用方法
		#添加文件 -> 直接拖拽文件 或者 点击 添加文件 按钮
		#转换     -> 会转换勾选的文件，转换结果会显示在对应列，转换成功后自动取消勾选
		#移除文件 -> 勾选要删除的文件点击删除按钮
