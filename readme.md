# 面向对象表达式计算器

## 基本功能
#### 使用面向对象的程序设计方式，实现一个表达式计算器，支持表达式计算，函数，变量，简单的命令

#### 例子
```c++
a = 6  b = 8  
b = 1 + 2 * 3 - ( 8 / 4 * sqrt(a * a) + -b )
结果 b = 3
```

## 如何解析表达式  
### 1.逆波兰表达式  
    先把中缀表达式转换成后缀表达式，然后再计算表达式的值。灵活性不够，很难支持函数功能？表达能力不够强。
### 2.巴科斯范式（BNF）  
```c++
表达式的BNF表示
一个表达式分解为：表达式（Expr）、项 （Term）、因子（Factor）
Expr ::= Term{('+' | '-') Term }    1+2-3    a+2*2   -1+3  -a+4 *(1+2)*2    3+fun(5)
Expr ::= Term = Expr        a=5   x=y=1+2-6/3  这个Term必须是一个左值节点
Term ::= Factor {('*' | '/') Factor }    1*2    -4 *(1+2)*2   2*fun(3)  3*b
Factor ::=
              1.Number 
              2.Identifier
                   2.1.Function
                   2.2.Variable 
              3.'-'Factor
              4.'('Expr ')'
```


### 3.表达式解析列子   
```c++
a = 6
b = 8
b = 1 + 2 * 3 - ( 8 / 4 * sqrt( a * a ) + -b)
结果 b = 3

按照BNF表达式
a = 6  b = 8 被解析成    Term = Expr    Term必须是一个左值节点
b = 1 + 2 * 3 - ( 8 / 4 * sqrt( a * a ) + -b) 被解析成  Term = Expr
1 + 2 * 3 - ( 8 / 4 * sqrt( a * a ) + -b) 被解析成  Term{('+' | '-') Term }
2 * 3  被解析成 Factor {('*' | '/') Factor }
( 8 / 4 * sqrt( a * a ) + -b)  被解析成 Term -> Factor ->  '('Expr ')'
8 / 4 * sqrt( a * a ) + -b 被解析成  Term{('+' | '-') Term }
8 / 4 * sqrt( a * a )  被解析成  Factor {('*' | '/') Factor }
-b    被解析成          '-'Factor 
    
根据上面的分析可知，可以利用递归下降法，解析任意合法的表达式，为了能够实现上述表达式的实际计算，需要设计一个Node（计算节点）继承体系，来为各种运算关系建模。
```

## 设计实现  
### 1.Node继承体系  
![图片](C:\Users\lenovo\Desktop/node.png "Node继承体系")  
### 2.Node节点类  
```c++
Noncopyable  tag类 表示所有的Node都是不可以拷贝的
Node 抽象类 提供一个 Calc()接口 返回表达式的值

MultipleNode 抽象类
ProductNode 具体类 继承自MultipleNode 为Factor {('*' | '/') Factor }建模
SumNode 具体类 继承自MultipleNode 为Term{('+' | '-') Term }建模

BinaryNode 抽象类
AssignNode 具体类 继承自BinaryNode 为Term = Expr建模

NumberNode 具体类 为Number建模

UnaryNode 抽象类
UMinusNode 具体类 继承自UnaryNode 为'-'Factor建模
FunctionNode 具体类 继承自UnaryNode 为Function建模
```
### 3.几个重要的类  
```c++
Scanner		读取输入表达式，并存取当前的解析状态
Parser		实现递归下降算法，解析表达式，根据Scanner的解析状态，动态创建计算节点
Calc		表示计算器，存有符号表，函数表，变量表，支持序列化
```

### 4.其它的类  
```C++
SymbolTable		 	存储符号信息，比如，函数，变量
FunctionTable	  	存储函数表，大小固定
Storage			    存储变量表，大小不固定
CommandParser	    支持简单命令，例如，help,显示函数表，变量表，序列化命令
Exception  	         简单异常类
Serial		        简单序列化类，主要用来保存，函数，变量的信息（用处不是很大）
Tracer		        自定义简单的动态内存使用情况跟踪器，用于调试，检测程序是否有内存泄露
```
### 5.其他特点  
```c++
使用C++11特性
使用智能指针，无需显示的delete
自定义简单的动态内存使用情况跟踪器，检测是否有内存泄露

前向声明的好处？
减少编译时间，实际比较过没？
```
### 代码统计  
![图片](C:\Users\lenovo\Desktop/code_lines.png "1")  