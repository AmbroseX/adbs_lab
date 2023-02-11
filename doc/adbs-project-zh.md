
# 简介
在这个项目中，我们将实现一个简单的存储和缓冲区管理器。本文档涉及存储和缓冲区管理器。
将讨论缓冲区和帧大小、缓冲区和帧存储结构、页面格式、页面存储结构、文档格式、缓冲技术、散列技术、文档存储结构以及磁盘空间和缓冲区模块的接口功能。特定技术是从类中涵盖的与缓冲区和存储管理器相关的材料中选择的。

## 说 明：

实验提交内容：

(1)  文档：提交一个说明文档，陈述你的设计和实现结果(包括设计结果、实现结果、运行结果等）。 格式要求：word或pdf文档。

(2)  源码以及运行说明。

(3)  提交截止时间：2月18日，11:59 PM（北京时间）

(4)  提交方式：待助教通知

## 要求
1. 按文档要求实现一个Storage and Buffer Manager，要求至少实现LRU算法。

2. 底层文件默认采用目录式堆文件。

3. 建议先构建一个5万个page（页号从0到49999）的堆文件（使用自己实现的FixNewPage()接口），然后再运行trace文件：data-5w-50w-zipf.txt（包含了50万次满足Zipfan分布-0.8的页面请求，即80%的请求集中在前20%的页号上），根据设计文档要求统计磁盘IO、Buffer命中率、运行时间等数据。

4. 下面的实验为选做内容，不做强制要求。如果完成了选做实验（一个或者多个），最后实验成绩会根据完成的情况适当加分：

    （1）实现CLOCK、LRU-2、LIRS等其它缓存置换算法（至少一种），并与LRU算法的性能进行对比；

    （2）加入缓存请求时的并发控制，通过内存锁(latch)解决缓存访问时的冲突；要求通过多线程方式执行trace并给出测试结果。


# 缓冲和帧

## 缓冲和帧大小
缓冲区是指主内存中的空间。CPU 只能访问主内存中存在的内容。缓冲区由帧数组组成。当请求页面时，它会加载到缓冲区中的内存中。大多数商业数据库管理系统使帧大小与页面大小相同，以防止外部碎片。项目中采用了相同的策略。默认情况下，项目的缓冲区大小将设置为 1024。


## 缓冲区和帧存储结构
缓冲区由称为帧的逻辑分区组成。 框架将存储在全局定义的结构中，描述框架的外观。此结构将定义为
```c++
#define PAGE_SIZE 4096  
struct bFrame  
{  
Char field [PAGE_SIZE];  
};
```
缓冲数组将存储一系列帧，这些帧将存储其中加载的页面。 此数组将如下所示：
```c++
#define DEFBUFSIZE 1024  
bFrame buf[DEFBUFSIZE]; // or the size that the user defined by the input parameter
```
这将是为缓冲区分配的空间，缓冲区管理器以及文档和访问方法将访问该缓冲区以引用所需页面。
## 页面格式
在这个项目中，我们不需要参考页面的详细结构。唯一重要的信息是`page_id`和`page_size`。所以你不需要设计页面格式

## 文档格式
我们建议使用基于目录的结构来组织数据库文档，如类中所述。每个文档都有一个基页，其中包含指向文档中每个页的指针。基页中的每个指针都按页面的顺序排列。此类文档中的数据页没有指针，只有记录。必须查阅基本页或目录才能获取文档中的下一页。 选择基于目录的文档格式是因为它似乎适合查找所请求记录的特定页面。文档的目录库将允许快速访问正确的页面，而无需搜索一长串页面来访问正确的页面。

## 缓冲技术
我们选择 LRU 作为我们实验室中唯一的替换策略。LRU 始终从用于组织缓冲区页的 LRU 队列中逐出最近最少使用的页面，这些页面按上次引用时间排序。它始终选择在 LRU 位置找到的页面作为受害者。LRU 最重要的优点是其恒定的运行时复杂性。 此外，LRU以其在具有高时间局部性的参考模式中的良好性能而闻名，即当前引用的页面在不久的将来具有很高的重新引用概率。


## 哈希技术
对于缓冲区中的每个帧，必须保留缓冲区控制块。每个缓冲区控制块 （BCB） 都包含一个`page_id`、一个`frame_id`、`page_latch`、`fix_count`和`dirty_bit`
`page_ids`用作哈希函数的键，该哈希函数将`page_id`映射到 `BCB`。
必须保留两个哈希表：一个用于将`page_ids`映射到`frame_ids`和 `BCB`，另一个用于将`frame_ids`映射到`page_ids`。
我们建议使用简单的静态哈希技术。在静态哈希中，存储桶的数量是固定的。如果存储桶已满，则会为额外的数据条目连接溢出链。哈希函数使用键值将其映射到存储桶。为了在单个存储桶内搜索，将使用顺序搜索。存储桶的数量不会随着时间的推移而改变。

哈希表的静态哈希技术。散列函数如下所示：
$$
H(k) = (page_id)%buffer_size
$$
缓冲区控制块将包含`page_id`、`frame_id`、`latch`、`count`、`dirty_bit`
`page_id` 到 `BCB` 的哈希表如下所示：`BCB hTable[BufferSize]`
`frame_id `到 `page_id` 的表看起来像：`int hTable[BufferSize]`
```c++
struct BCB  
{  
BCB();  
int page_id;  
int frame_id;  
int latch;  
int count;  
int dirty;  
BCB * next;  
};
```

## 文档存储
在我们的项目中，我们只需要磁盘上的一个物理文档。数据库中的所有数据将保留在此单个文档中。此文档将保存在工作目录中，并命名为 `data.dbf`。应始终找到此文档，即使它是空的，系统首次运行时也是如此。

# 类设计

## 数据存储管理器

```c++
class DSMgr  
{  
	public:  
	DSMgr();  
	int OpenFile(string filename);  
	int CloseFile();  
	bFrame ReadPage(int page_id);  
	int WritePage(int frame_id, bFrame frm);  
	int Seek(int offset, int pos);  
	FILE * GetFile();  
	void IncNumPages();  
	int GetNumPages();  
	void SetUse(int index, int use_bit);  
	int GetUse(int index);  
	private:  
	FILE *currFile;  
	int numPages;  
	int pages[MAXPAGES];  
};
```

## 缓冲区管理器

```c++
class BMgr  
{  
	public:  
	BMgr();  
	// Interface functions  
	int FixPage(int page_id, int prot);  
	void NewPage FixNewPage();  
	int UnfixPage(int page_id);  
	int NumFreeFrames();  
	// Internal Functions  
	int SelectVictim();  
	int Hash(int page_id);  
	void RemoveBCB(BCB * ptr, int page_id);  
	void RemoveLRUEle(int frid);  
	void SetDirty(int frame_id);  
	void UnsetDirty(int frame_id);  
	void WriteDirtys();  
	PrintFrame(int frame_id);  
	private:  
	// Hash Table  
	int ftop[DEFBUFSIZE];  
	BCB* ptof[DEFBUFSIZE];  
};
```

# 缓冲接口函数
这些接口函数将为文档及其上方的访问管理器提供接口。 所需的功能包括：

## FixPage(int page_id, int prot)
此函数的原型是 `FixPage(Page_id, protection)`，它返回一个`frame_id`。 文档和访问管理器将使用记`record_id`中的`page_id`调用此页面。该函数查看页面是否已在缓冲区中，如果是，则返回相应的`frame_id`。如果页面尚未驻留在缓冲区中，它将根据需要选择受害页面，并在请求的页面中加载。

## FixNewPage()
此函数的原型是 `FixNewPage()`，它返回一个`page_id`和一个`frame_id`。 当插入、索引拆分或对象创建时需要新页面时，使用此函数。返回`page_id`以分配给`record_id`和元数据。此函数将找到一个空页面，文档和访问管理器可以使用该页面来存储一些数据。

## UnfixPage(int page_id)
此函数的原型是 `UnfixPage(page_id)`，它返回一个`frame_id`。此函数是对 `FixPage` 或 `FixNewPage` 调用的补充。此函数递减帧上的定位计数。如果计数减少到零，则会移除页面上的锁，并且可以移除框架（如果选定）。`page_id`将转换为`frame_id`，并且可以取消锁定，以便在页面上的计数减少到零时可以选择它作为受害页面。

## NumFreeFrames()
`NumFreeFrames` 函数查看缓冲区并返回可用且可以使用的缓冲区页数。这对于查询处理器的 N 路排序特别有用。该函数的原型看起来像 `NumFreeFrames()`，并返回一个从 0 到 BUFFERSIZE-1（1023） 的整数。

## SelectVictim()
选择受害者函数选择要替换的帧。如果设置了所选帧的 [[100-大数据基础/016-高级数据库系统/概念#dirty bit|dirty_bit]]，则需要将页面写入磁盘。

## Hash(int page_id)
哈希函数将page_id作为参数并返回帧 ID。

## RemoveBCB(BCB* ptr, int page_id)
`RemoveBCB` 函数从数组中删除`page_id`的缓冲区控制块。 仅当 `SelectVictim()` 函数需要替换帧时，才会调用此函数。

## RemoveLRUEle(int frid)
`RemoveLRUEle`函数从列表中删除LRU元素。

## SetDirty(int frame_id)
`SetDirty` 函数设置`frame_id`的dirty bit。这个dirty bit用于知道是否写出帧。如果内容已以任何方式修改，则必须写入框架。这包括任何目录页和数据页。如果位为 1，则写入。如果此位为零，则不会写入。

## UnsetDirty(int frame_id)
`UnsetDirty` 函数将相应`frame_id`的`dirty_bit`赋值为零。调用此函数的主要原因是当 `setDirty()` 函数被调用但页面实际上是临时关系的一部分时。在这种情况下，实际上不需要写入页面，因为它不想保存。

## WriteDirtys()
`WriteDirtys` 函数必须在系统关闭时调用。该函数的目的是写出仍在缓冲区中可能需要写入的任何页面。如果`dirty_bit`为 1，则它只会将页面写出到文档中。

## PrintFrame(int frame_id)
`PrintFrame` 函数打印出frame_id描述的框架的内容

# 数据存储接口函数
当前数据文档将保存在 `DSManager` 类中
此文档将命名为 `data.dbf`
## OpenFile(string filename)
每当需要打开文档进行读取或写入时，都会调用 `OpenFile` 函数。 此函数的原型是 `OpenFile(String filename)`并返回错误代码。 该函数打开文档名指定的文档。

## CloseFile()
当需要关闭数据文档时，将调用关闭文档函数。原型是 `CloseFile()` 并返回一个错误代码。
此函数关闭当前使用的文档。 此函数应仅在数据库更改或进程关闭时调用。

## ReadPage(int page_id)
`ReadPage` 函数由缓冲区管理器中的 `FixPage` 函数调用。这个原型是 `ReadPage(page_id, bytes)`，并返回它已读取的内容。此函数调用 `fseek()` 和 `frea()` 从文档中获取数据。

## WritePage(int frame_id, bFrame frm)
每当从缓冲区中取出页面时，都会调用 `WritePage` 函数。原型是 `WritePage(frame_id, frm)`，并返回写入的字节数。此函数调用 `fseek()` 和 `fwrite()` 将数据保存到文档中。

## Seek(int offset, int pos)
查找函数移动文档指针。

## GetFile()
获取文档函数返回当前文档。

## IncNumPages()
`IncNumPages` 函数递增页面计数器。

## GetNumPages()
`GetNumPages` 函数返回页面计数器。

## SetUse(int page_id, int use_bit)
`SetUse` 函数` look` 设置页面数组中的位。此数组跟踪正在使用的页面。如果删除了页面中的所有记录，则该页面不再实际使用，并且可以在数据库中再次重用。为了知道页面是否可重用，将检查数组中是否有任何设置为零`use_bits`。`fixNewPage` 函数首先检查此数组的use_bit是否为零。如果找到一个，则重复使用该页面。 否则，将分配一个新页面。

## GetUse(int page_id)
`GetUse` 函数返回相应`page_id`的当前`use_bit`。

# Experiment Setup
在我们的项目中，您需要执行跟踪驱动的实验来演示您的实现结果。追踪是根据 `Zipf` 分布生成的。跟踪中总共有 `500,000` 个页面引用，这些引用仅限于编号范围从 1 到 `50,000` 的一组页面。
每条跟踪记录的格式为`“x, ###”`
其中 `x` 为 0（读取）或 1（写入），`###` 是引用的页码。
您需要扫描跟踪文档并打印出内存和磁盘之间的总` I/O`。
缓冲区在实验开始时应该是空的。跟踪中涉及的所有 `50,000` 页都需要首先在磁盘中具体化，这对应于基于目录的文档数据`.dbf`

## Implementation Plan
![](https://cdn.jsdelivr.net/gh/RongkangXiong/pic-bed/blog/img/202212041705891.png)
