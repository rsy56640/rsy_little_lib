naive work
--------------------
之前看的More EC都快忘了，寻思着智能指针(不处理循环引用的简单版本)这玩意很简单，写起来才发现有很多地方不好写，比如 refCount 和 ponitee 是写在一起还是分开，new一次还是在堆里再new一次，而且自定义删除器(Deleter)还没想好怎么写。果然是2个月没写Cpp，现在已经是智障了。。。去看了VS2015的实现<memory>，感觉自己naive.  
鱼唇的我现在连模板都写不利索了