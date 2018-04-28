顺手把map封装了一下，感觉还有模有样的     
     
其中故意写了一个地方：ValueProxy，用来区分读和写，      
当然这个是不需要的，我只是模拟一下《More Effective C++》Item 30 上面的手法。     
     
不过的确是碰到了一个问题，在形如     
cout << mp[key];      
的语句中，会出现编译错误。    
正解是这样：` std::operator<<<char, char_traits<char>, allocator<char>(std::cout, mp[key]);`      
原因是模板实参推导失败，多的就不说了。。。     
