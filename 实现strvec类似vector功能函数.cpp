#include<iostream>
#include<string>
#include<memory>
#include<utility>

using namespace std;

class strVec{
	public:
		strVec():elements(NULL),first_free(NULL),cap(NULL){}
		strVec(const strVec&);
		strVec & operator=(const strVec&);
		~strVec();
		size_t size() const {
			return first_free - elements;
		}
		size_t capacity()const{
			return cap - elements;
		}
		void push_back(const string &);
		string *begin(){return elements;}
		string *end(){return first_free;}
private:
	static allocator<string>alloc;
	void chk_n_alloc(){if(size() == capacity()) reallocate();}
	void reallocate();
	void free();
	pair<string*,string*>alloc_n_copy(string*,string*);
	string *elements;
	string *first_free;
	string* cap;
};

//拷贝构造函数

strVec::strVec(const strVec&rhs)
{
	auto newData = alloc_n_copy(rhs.elements,rhs.first_free);
	elements = newData.first;
	cap = first_free = newData.second;
}

//operator=函数
strVec& strVec::operator=(const strVec&rhs)
{
	pair<string*,string*>newData = alloc_n_copy(rhs.elements,rhs.first_free);
	free();
	elements = newData.first;
	first_free = cap = newData.second;
	return *this;
}
 
//push_back函数
void strVec::push_back(const string &str)
{
	chk_n_alloc();
	alloc.construct(first_free++,str);
}
// 在push_back函数时，如果检查后控件不够用 ， 就重新分配空间
void strVec::reallocate()
{
	size_t newCapacity = size() ? 2 *size() : 1;
	string * ele = alloc.allocate(newCapacity);
	string *dest = ele;
	for(size_t i = 0;i != size();++i)
		alloc.construct(dest++,move(*elements++));
	free();
	elements = ele;
	first_free = dest;
	cap = elements + newCapacity;
}
//free()函数调用，释放占用空间

void strVec::free()
{
	if(elements){
		for(auto beg = first_free;beg != elements;)
			alloc.destroy(--beg);
		alloc.deallocate(elements,cap - elements);
	}
}
pair<string*,string*>strVec::alloc_n_copy(string*lhs,string*rhs)
{
	auto data = alloc.allocate(rhs - lhs);
	return pair<string*,string*>(data,uninitialized_copy(lhs,rhs,data));
}



