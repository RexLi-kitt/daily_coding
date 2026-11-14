template<class ElemType> class ListBase{
    public:
        virtual int Length()const = 0;
        virtual bool IsEmpty() const = 0;//const承诺不修改对象的普通成员变量
        virtual void Clear() = 0;
        virtual bool GetElem(int position, ElemType& e) const = 0;
        virtual bool SetElem(int position, const ElemType e) = 0;
        virtual bool Delete(int position,ElemType& e) = 0;
        virtual bool Insert(int position, const ElemType e) = 0;
        virtual void Traverse(bool (*visit)(const ElemType&)) const = 0;
        virtual int AddTail(const ElemType e) = 0;
};