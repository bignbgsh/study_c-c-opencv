fgets(要输入的参数；允许输入的长度；输入器)
get（要输入的参数）#不限制字符节，最好别用
字符串常用函数：
    strlen：字符串长度测量
    strcpy（a，b）把b字符串复制到a
    strncpy（a，b，n）最多把指定数量的字符串复制到a
    strcat（a，b）把a，b两个字符串拼接起来
    strncat多了一个指定数量
    strcmp（a，b）比较两个字符串小于0 a<b 大于零 a>b
    strcspn(a,"b")遇到b之前字符串长度常用来
        str[strcspn(str, "\n")] = '\0'; // 去掉换行符


    puts（a）输出字符串a并自动换行
    
