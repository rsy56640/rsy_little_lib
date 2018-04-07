    
    红黑树：个人练习作品    
    参考：《算法导论》  ch13    
         
         
    2018-4-7 目前这个版本有问题，问题在于使用智能指针进行类型转换时会出现很迷的情况。      
    关于为什么要进行类型转换，我参考了stl里面的技巧，存放value的模板继承了node类，于是将所有value_node看作是node，    
    但是在Impl类里面root必须是value_node，所以会产生很多问题。     
    于是机智的我找到了一个solution：在Impl类里面加一个node引用，用root来初始化，之后 禁止使用root，而使用那个node类型的引用。     
    顺便找了函数叫 std::static_pointer_cast<_Ty>();    
    
