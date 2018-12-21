#include <QCoreApplication>
#include <QDebug>

void Swap(int *a, int i, int j)
{
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

bool Less(int *a, int i, int j)
{
    return a[i] < a[j];
}

/*选择排序：首先找到数组中最小的元素，将它和数组中第一个元素交换位置，其次在剩下的元素中找到最小的元素，将它和数组的第二个位置交换位置
 如此往复，不断的选择剩余元素中的最小者
 对于长度为N的数组，选择排序需要大约N²/2次比较和N次交换 (N-1)+(N-2)+...+2+1=N(N-1)/2~N²/2
 缺点：与初始状态无关，一个已经有序的数组或是主键完全相同的数组和一个随机排列的数组所用的排序时间竟然一样长
 优点：数据移动是最少的，交换次数和数组大小是线性关系
*/
void SelectSort(int *a, int len)
{
    for(int i=0;i<len;i++)
    {
        int min = i;
        for(int j=i;j<len;j++)
        {
            if(a[j] < a[min])
                min = j;
        }
        Swap(a, i, min);
    }
}

/*插入排序：与选择排序一样，当前索引左边的元素都是有序的，但是他们的最终位置还不确定，为了给更小的元素腾出空间，他们可能会被移动
 * 当索引达到数组右端时，排序完成
 * 对于随机排列的长度为N且主键不重复的数组，平均情况下需要~N²/4比较和~N²/4交换，最坏情况下需要~N²/2比较和~N²/2交换
 * 最好情况下需要N-1次比较和0次交换
*/
void InsertSort(int *a, int len)
{
    for(int i=0;i<len;i++)
    {
        for(int j=i;j>0 && Less(a,j,j-1);j--)
            Swap(a, j, j-1);
    }
}

/*希尔排序*/
void ShellSort(int *a, int len)
{
    int h = 1;
    while(h < len/3)
        h = 3*h + 1;
    while(h >= 1)
    {
       for(int i=h;i<len;i++)
       {
           for(int j=i;j>=h&&Less(a,j,j-h);j-=h)
               Swap(a, j, j-h);
       }
       h = h/3;
    }
}

/*归并排序
 * 自顶向下
*/
void merge(int *a, int lo, int mid, int hi, int* aux)
{
    int i = lo;
    int j = mid + 1;
    for(int k=lo;k<=hi;k++)
        aux[k] = a[k];
    for(int k=lo;k<=hi;k++)
    {
        if(i > mid)
            a[k] = aux[j++];
        else if(j > hi)
            a[k] = aux[i++];
        else if(Less(aux, i, j))
            a[k] = aux[i++];
        else
            a[k] = aux[j++];
    }
}

void MSort(int *a, int lo, int hi, int* aux)
{
    if(hi <= lo)
        return;
    int mid = (hi + lo)/2;
    //顺序为01-23-03-45-67-47-07
    MSort(a, lo, mid, aux);
    MSort(a, mid+1, hi, aux);
    merge(a, lo, mid, hi, aux);
}

void MergeSort(int *a, int len)
{
    int aux[len] = {0};//避免在递归中重复申请
    for(int i=0;i<len;i++)
        aux[i] = a[i];
    MSort(a, 0, len-1, aux);
}

/*
 * 自底向上
 * 当数组长度为2的次幂时，自顶向下和自底向上的归并排序所用的比较次数和数组访问次数相同
*/
void MergeSort2(int* a, int len)
{
    int aux[len] = {0};
    for(int i=0;i<len;i++)
        aux[i] = a[i];
    for(int sz=1;sz<len;sz=sz+sz)//子数组大小
    {
        for(int lo=0;lo<len-sz;lo+=sz+sz)
        {
            int hi = (lo+sz+sz-1 < len-1) ? lo+sz+sz-1 : len-1;
            merge(a, lo, lo+sz-1, hi, aux);
        }
    }
}

/*快速排序
 * 归并排序将数组分成两个子数组分别排序，并将有序的子数组归并以将整个数组排序
 * 快速排序是当两个子数组都有序时整个数组也就自然有序了
*归并排序每次从数组中间划分，等分数组；快速排序的切分位置取决于数组内容
*/
int partition(int *a, int lo, int hi)
{
    int i = lo;
    int j = hi + 1;
    while(true)
    {
        while(Less(a, ++i, lo))
        {
            if(i == hi)
                break;
        }
        while(Less(a, lo, --j))
        {
            if(j == lo)
                break;
        }
        if(i >= j)
            break;
        Swap(a, i, j);
    }
    Swap(a, lo, j);
    return j;
}

void QSort(int *a, int lo, int hi)
{
    if(hi <= lo)
        return;
    int j = partition(a, lo, hi);
    QSort(a, lo, j-1);
    QSort(a, j+1, hi);
}

void QuickSort(int *a, int len)
{
    QSort(a, 0, len-1);
}

/*
 * 改进：对于小数组，插入排序比快排更快
*/
void QSort2(int *a, int lo, int hi)
{
    int M = 5;
    if(hi <= lo+M)
    {
        InsertSort(&a[lo], hi-lo+1);
        return;
    }
    int j = partition(a, lo, hi);
    QSort(a, lo, j);
    QSort(a, j+1, hi);
}

void QuickSort2(int *a, int len)
{
    QSort2(a, 0, len-1);
}


/*堆有序 ：当一个二叉树的每个节点都大于等于它的两个子节点时，称为堆有序。根节点是堆有序二叉树中最大节点。
 * 二叉堆：是一组能用堆有序的完全二叉树排序的元素，并在数组中按照层级存储（不使用数组的第一个位置）。
 * 位置k的节点的父节点的位置为k/2，子节点的位置为2k和2k+1
 * 一棵大小为N的完全二叉树的高度为lgN
 *
*/

void swim(int *a, int k)
{
    while(k>1 && Less(a,k/2,k))
    {
        Swap(a, k/2, k);
        k = k/2;
    }
}

void sink(int *a, int len, int k)
{
    while(2*k <= len)
    {
        int j = 2*k;
        if((j+1 != len)&& Less(a,j,j+1))
            j++;
        if(Less(a,k,j))
        {
            Swap(a, k, j);
            k = j;
        }
        else
            break;
    }
}

int delMax(int *a, int len)
{
   int max = a[1];
   Swap(a, 1, --len);
   a[len] = 0;
   sink(a, len, 1);
   return max;
}

void insertPQ(int *a, int len, int v)
{
    a[len] = v;
    swim(a, len);
}
/*
 * 堆排序：1）构造堆，从右至左使用sink，只需从N/2处开始即可，即从树的倒数第二层开始
 * 2）堆排序，类似delMax
*/
void StackSort(int *a, int len)
{
    //构造堆
    for(int k=len/2;k>=1;k--)
    {
        sink(a, len, k);
    }
    for(int i=0;i<len;i++)
        printf("%d ", a[i]);
    //下沉排序
    while(len>1)
    {
        Swap(a, 1, --len);
        sink(a, len, 1);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("SelectSort:\n");
    int a0[] = {0,3,6,5,2,1,8,9};
    int a1[] = {0,3,5,6,2,1,8,9};
    memcpy(a1, a0, sizeof(a0));
    SelectSort(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nInsertSort:\n");
    InsertSort(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nShellSort:\n");
    ShellSort(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nMergeSort:\n");
    MergeSort(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nMergeSort2:\n");
    MergeSort2(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nQuickSort:\n");
    QuickSort(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);

    memcpy(a1, a0, sizeof(a0));
    printf("\nQuickSort2:\n");
    QuickSort2(a1, 8);
    for(int i=0;i<8;i++)
        printf("%d", a1[i]);


    int a2[9] = {0};
    memcpy(a2+1, a0, sizeof(a0));
    printf("\nStackSort:\n");
    for(int i=0;i<9;i++)
        printf("%d ", a2[i]);
    printf("\n");
    StackSort(a2, 9);
    for(int i=1;i<9;i++)
        printf("%d ", a2[i]);

    printf("\n");
    memcpy(a1, a0, sizeof(a0));
    memcpy(a2, 0, 9);
    for(int i=0;i<9;i++)
    {
        insertPQ(a2, i, a1[i]);
    }
    for(int i=0;i<9;i++)
    {
        int max = delMax(a2, 9-i);
        printf("%d : ", max);
        for(int i=0;i<9;i++)
            printf("%d", a2[i]);
        printf("\n");
    }



    return a.exec();
}
