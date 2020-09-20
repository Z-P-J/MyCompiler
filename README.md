# 编译原理课程设计 MyCompiler

 编译原理课程设计：自制C-语言词法分析和语法分析器

### C- 的词法规则
- 1、关键字：
    else  if  int  return  void  while  
    所有的关键字均为保留字，并且全小写。

- 2、专用符号：
    \+  -  *  /  <  <=  >  >=  ==  !=  =  ;  ,  (  )  [  ]  {  }  /*  */  

- 3、ID和NUM，通过下列正规表达式定义： 
    ID = letter letter* 
    NUM = digit digit* 
    letter = a|…|z|A|...|Z 
    digit = 0|…|9  
    C- 中字母是区分大小写的。

- 4、空格由空白、换行符和制表符组成。除了在必须分割ID、NUM关键字之外，其他情况下空格常被忽略。

- 5、注释由/*...*/表示，它可以被放在任何空白位置，且可以超过一行，但是不能嵌套。

### 词法分析器设计要求及方法

##### 要求：
- (1) 具备预处理功能。将文本格式符号等先滤掉，只保留要分析的源代码串，即要求设计一个供词法分析调用的预处理子程序；
- (2) 拼出源程序中的各合法单词符号串，并以(种别码, 属性值）方式返回；
- (3) 分析中出现非法符号串，要进行错误报告。

##### 方法：
为不同的单词设计种别码；
根据各类单词的定义，设计各单词的状态转换图，再合并成一个总的状态转换图；
根据状态转换总图编写代码实现C- 的扫描器。
现阶段先设计成一个可独立运行的扫描器，下一个阶段做恰当修改，将词法分析器设计改成供语法分析器调用的子程序。

##### 编程语言：
C或者C++

### C- 的BNF语法规则

- 1.program → declaration-list
- 2.declaration-list → declaration-list declaration|declaration
- 3.declaration → var-declaration|fun-declaration
- 4.var-declaration → type-specifier ID;|type-specifier ID[NUM];
- 5.type-specifier → int|void
- 6.fun-declaration → type-specifier ID(params)|compound-stmt
- 7.params → params-list|void
- 8.param-list → param-list,param|param
- 9.param → type-specifier ID|type-specifier ID[]
- 10.compound-stmt → {local-declarations statement-list}
- 11.local-declarations → local-declarations var-declaration|empty
- 12.statement-list → statement-list statement|empty
- 13.statement → expression-stmt|compound-stmt|selection-stmt|
iteration-stmt|return-stmt
- 14.expression-stmt → expression;|;
- 15.selection-stmt → if(expression) statement | if(expression) statement else statement
- 16.iteration-stmt → while(expression) statement
- 17.return-stmt → return;|return expression;
- 18.expression → var=expression|simple-expression
- 19.var → ID|ID[expression]
- 20.simple-expression → additive-expression relop additive-expression | additive-expression
- 21.relop → <=|<|>|>=|==|!=
- 22.additive-expression → additive-expression addop term|term
- 23.addop → +|-
- 24.term → term mulop factor|factor
- 25.mulop → *|/
- 26.factor → (expression)|var|call|NUM
- 27.call → ID(args)
- 28.args → arg-list|empty
- 29.arg-list → arg-list, expression|expression

### 对以上每条文法规则，给出了相关语义的简短解释。
- 1.program → declaration-list
- 2.declaration-list → declaration-list declaration|declaration
- 3.declaration → var-declaration|fun-declaration
程序由声明的列表(或序列)组成，声明可以是函数或变量声明，顺序是任意的。至少必须有一个声明。接下来是语义限制(这些在C中不会出现)。所有的变量和函数在使用前必须声明(这避免了向后backpatching引用)。程序中最后的声明必须是一个函数声明，名字为main。注意，C－缺乏原型，因此声明和定义之间没有区别(像C一样)。
- 4.var-declaration → type-specifier ID;|type-specifier ID[NUM];
- 5.type-specifier → int|void
变量声明或者声明了简单的整数类型变量，或者是基类型为整数的数组变量，索引范围从0到NUM-1。注意，在C－中仅有的基本类型是整型和空类型。在一个变量声明中，只能使用类型指示符int。void用于函数声明(参见下面)。也要注意，每个声明只能声明一个变量。
- 6.fun-declaration → type-specifier ID(params)compound-stmt
- 7.params → param-list|void
- 8.param-list → param-list,param|param
- 9.param → type-specifier ID|type-specifier ID[]
函数声明由返回类型指示符、标识符以及在圆括号内的用逗号分开的参数列表组成，后面跟着一个复合语句，是函数的代码。如果函数的返回类型是void，那么函数不返回任何值(即是一个过程)。函数的参数可以是void(即没有参数)，或者一列描述函数的参数。参数后面跟着方括号是数组参数，其大小是可变的。简单的整型参数由值传递。数组参数由引用来传递(也就是指针)，在调用时必须通过数组变量来匹配。注意，类型“函数”没有参数。一个函数参数的作用域等于函数声明的复合语句，函数的每次请求都有一个独立的参数集。函数可以是递归的(对于使用声明允许的范围)。
- 10.compound-stmt → {local-declarations statement-list}
复合语句由用花括号围起来的一组声明和语句组成。复合语句通过用给定的顺序执行语句序列来执行。局部声明的作用域等于复合语句的语句列表，并代替任何全局声明。
- 11.local-declarations → local-declarations var-declaration|empty
- 12.statement-list → statement-list statement|empty
注意声明和语句列表都可以是空的(非终结符empty表示空字符串，有时写作ε)
- 13.statement → expression-stmt|compound-stmt|selection-stmt|iteration-stmt|return-stmt
- 14.expression-stmt → expression;|;
表达式语句有一个可选的且后面跟着分号的表达式。这样的表达式通常求出它们一方的结果。因此，这个语句用于赋值和函数调用。
- 15.selection-stmt → if(expression) statement|if(expression) statement else statementif
语句有通常的语义：表达式进行计算；非0值引起第一条语句的执行；0值引起第二条语句的执行，如果它存在的话。这个规则导致了典型的悬挂else二义性，可以用一种标准的方法解决：else部分通常作为当前if的一个子结构立即分析(“最近嵌套”非二义性规则)。
- 16.iteration-stmt → while(expression) statement
while语句是C－中唯一的重复语句。它重复执行表达式，并且如果表达式的求值为非0，则执行语句，当表达式的值为0时结束。
- 17.return-stmt → return;|return expression;返回语句可以返回一个值也可无值返回。函数没有说明为void就必须返回一个值。函数声明为void就没有返回值。return引起控制返回调用者(如果它在main中，则程序结束)。
- 18.expression → var=expression|simple-expression
- 19.var → ID|ID[expression]
表达式是一个变量引用，后面跟着赋值符号(等号)和一个表达式，或者就是一个简单的表达式。赋值有通常的存储语义：找到由var表示的变量的地址，然后由赋值符右边的子表达式进行求值，子表达式的值存储到给定的地址。这个值也作为整个表达式的值返回。var是简单的(整型)变量或下标数组变量。负的下标将引起程序停止(与C不同)。然而，不进行下标越界检查。var表示C－比C的进一步限制。在C中赋值的目标必须是左值(l-value)，左值是可以由许多操作获得的地址。在C－中唯一的左值是由var语法给定的，因此这个种类按照句法进行检查，代替像C中那样的类型检查。故在C－中指针运算是禁止的。
- 20.simple-expression → additive-expression relop additive-expression|additive-expression
- 21.relop → <=|<|>|>=|==|!=
简单表达式由无结合的关系操作符组成(即无括号的表达式仅有一个关系操作符)。简单表达式在它不包含关系操作符时，其值是加法表达式的值，或者如果关系算式求值为ture，其值为1，求值为false时值为0。
- 22.additive-expression → additive-expression addop term|term
- 23.addop → +|-
- 24.term → term mulop factor|factor
- 25.mulop → *|/
加法表达式和项表示了算术操作符的结合性和优先级。符号表示整数除；即任何余数都被截去。
- 26.factor → (expression)|var|call|NUM
因子是围在括号内的表达式；或一个变量，求出其变量的值；或者一个函数调用，求出函数的返回值；或者一个NUM，其值由扫描器计算。数组变量必须是下标变量，除非表达式由单个ID组成，并且以数组为参数在函数调用中使用(如下所示)。
- 27.call → ID(args)
- 28.args → arg-list|empty
- 29.arg-list → arg-list,expression
函数调用的组成是一个I D (函数名 )，后面是用括号围起来的参数。参数或者为空，或者由逗号分割的表达式列表组成，表示在一次调用期间分配的参数的值。函数在调用之前必须声明，声明中参数的数目必须等于调用中参数的数目。 函数声明中的数组参数必须和一个表达式匹配，这个表达式由一个标识符组成表示一个数组变量。

### 词法分析

 ```

int main()
{
    cout << "Please enter the file path:" << endl;
	string path;
	cin >> path;
    LexicalAnalyzer analyzer(path);
    analyzer.analyze();

    system("pause");
    return 0;
}

 ```


 ### 语法分析

 ```

int main()
{
	cout << "Please enter the file path:" << endl;
	string path;
	cin >> path;
	LexicalAnalyzer analyzer(path);
	SyntaxParser parser(analyzer.analyze());
	parser.parse();
	system("PAUSE");
	return 0;
}

 ```