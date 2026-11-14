#ifndef _DB_LK_LIST_H_
#define _DB_LK_LIST_H_

#include "LinkListBase.h"
// 带头结点的非循环双向链表，位置编号从 0 开始。
template <class ElemType>
class DbLkList : public LinkListBase<ElemType, DbNode<ElemType> >
{
public:
    DbLkList();
    DbLkList(const DbLkList<ElemType>& source);
    virtual ~DbLkList();
    DbLkList<ElemType>& operator=(const DbLkList<ElemType>& source);

    // 原地翻转，保持头结点和结点数量不变。
    // 翻转后游标回到头结点；空表和单结点表同样返回 true。
    bool Reverse();

protected:
    virtual bool Link(DbNode<ElemType>* pPreNode,DbNode<ElemType>* pNextNode);
};
template <class ElemType>
DbLkList<ElemType>::DbLkList()
{
}
template <class ElemType>
DbLkList<ElemType>::DbLkList(const DbLkList<ElemType>& source)
    : LinkListBase<ElemType, DbNode<ElemType> >()
{
    // 先构造空表，再调用基类赋值，避免直接复制拥有的结点指针。
    *this = source;
}
template <class ElemType>
DbLkList<ElemType>::~DbLkList()
{
    // 由基类析构函数释放结点。
}
template <class ElemType>
DbLkList<ElemType>& DbLkList<ElemType>::operator=(
    const DbLkList<ElemType>& source)
{
    if (this != &source)
        LinkListBase<ElemType, DbNode<ElemType> >::operator=(source);
    return *this;
}

template <class ElemType>
bool DbLkList<ElemType>::Link(DbNode<ElemType>* pPreNode,
                            DbNode<ElemType>* pNextNode)
{
    if (!pPreNode)
        return false;
    pPreNode->m_pNext = pNextNode;
    if (pNextNode)
        pNextNode->m_pPre = pPreNode;
    return true;
}
template <class ElemType>
bool DbLkList<ElemType>::Reverse()
{
    DbNode<ElemType>* pNode = this->m_pNodeHead->m_pNext;
    DbNode<ElemType>* pPrevious = NULL;
    // 原首结点将成为新尾结点；空表的尾结点仍为头结点。
    this->m_pNodeTail = pNode ? pNode : this->m_pNodeHead;
    while (pNode)
    {
        // 先保存原后继，防止改链后无法继续访问。
        DbNode<ElemType>* pNext = pNode->m_pNext;
        Link(pNode, pPrevious);
        pPrevious = pNode;
        pNode = pNext;
    }
    // 接回头结点，同时设置新首结点的前驱。
    Link(this->m_pNodeHead, pPrevious);
    this->m_pNodeHead->m_pPre = NULL;
    this->m_pNodeCur = this->m_pNodeHead;
    return true;
}

#endif
