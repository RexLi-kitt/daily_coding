#include"listbase.h"
#include<cstring>
#include<cstdio>
template<class ElemType>

class Sqlist: public ListBase<ElemType>{
    public:
        Sqlist(int nBufferSize = 16);
        Sqlist(const Sqlist& other);
        ~Sqlist();
        int Length() const;
        bool IsEmpty() const;
        void Clear();
        bool GetElem(int position, ElemType& e) const;
        bool SetElem(int position, const ElemType e);
        bool Delete(int position, ElemType& e);
        bool Insert(int position, const ElemType e);
        void Traverse(bool (*visit)(const ElemType&)) const;
        int AddTail(const ElemType e);
        virtual Sqlist & operator = (const Sqlist<ElemType> &source);

    protected:
        int SaveData(ElemType * pTData,int nDataLen);
        int SetBufferSize(int nSize);
        ElemType* m_pElemData;
        int m_nDataLen;
        int m_nBufferLen;

};
template<class ElemType>
bool Sqlist<ElemType>::Insert(int position, const ElemType eData){
    if(position < 0) return false;
    if(m_nDataLen >= m_nBufferLen){
        if(!SetBufferSize(m_nDataLen * 2)) return false;
    }
    if(position <= m_nDataLen - 1){
        memmove(m_pElemData + position + 1, m_pElemData + position, sizeof(ElemType)*(m_nDataLen - position));
        m_pElemData[position] = eData;
    }
    else{
        position = m_nDataLen;
        m_pElemData[position] = eData;
    }
    m_nDataLen++;
    return true;
}
template<class ElemType>
bool Sqlist<ElemType>::Delete(int position,ElemType& eData){
    if(position < 0 || position >= m_nDataLen){
        return false;
    }
    if(position < m_nDataLen - 1){
        memmove(m_pElemData + position,m_pElemData + position + 1
            ,sizeof(ElemType) * (m_nDataLen - position - 1));
    }
    m_nDataLen--;
    return true;
}
template<class ElemType>
void Sqlist<ElemType>::Traverse(bool (*visit)(const ElemType&))const{
    if(!visit){
        return;
    }
    int position;
    for(position = 0;position < m_nDataLen;position ++){
        if(!(*visit)(m_pElemData[position])){
            break;
        }
    }
}

