#pragma once

#include <memory.h>  
#include <stdio.h>
#define BOOLEAN int
#define TRUE 1
#define FALSE 0
#define VOID void
#define DEFAULT_CAP 16


#define ASSERT_INDEX(__pos) ASSERT((__pos<m_size)&&(__pos>=0))
template <class T>
class List {
private:
	BOOLEAN m_isStack;
	T* m_array;
	int m_capSize;
	int m_size, m_typeSize;
	VOID GrowCheck(int szChk);
public:
	List(VOID* array, int caplen, BOOLEAN init) {
		m_array = (T*)array;
		m_capSize = caplen;
		m_isStack = TRUE;
		memset(array, 0, m_capSize * sizeof(T));
	}
	List(VOID* array, int caplen) {
		m_array = (T*)array;
		m_capSize = caplen;
		m_isStack = TRUE;
	}
	~List() {
		if (!m_isStack) {
			if(m_array!=NULL)
			delete m_array;
		}
	}
	List() :List(DEFAULT_CAP) {}
	List(int cap) :m_size(0), m_typeSize(sizeof(T)), m_array(NULL), m_capSize(cap), m_isStack(FALSE){}
	VOID Add(const T&  t);
	VOID Clear();
	VOID Set(int index,const T& t);
	VOID Remove(int index);
	VOID operator=(List<T>& srclist);
	VOID CopyList(List<T>& src);
	VOID  AddArray(T arr[], int len);
	VOID AddList(List<T>& srclist, int srcsize); 
	VOID  SetSize(int ss);
	int Size() { return m_size; }
	T& operator[](int pos);
};

#define CREATE_STACKLIST(type,len) (type*)alloca(len*sizeof(type)),len

template<class T>
VOID List<T>::Remove(int index) {
	ASSERT_INDEX(index);
	//m_array[index] = NULL;
	if (m_size != 1) {
		memcpy(m_array + index, m_array + index + 1, sizeof(T) * (m_size - index - 1)); 
	}
	m_array[m_size - 1] = { 0 };
	--m_size;
}
template<class T>
VOID List<T>::GrowCheck(int szChk) {
	if (!m_isStack) {
		int inclen = 0;

		if ((szChk <= m_capSize) && m_array != NULL) {
			return;
		}
		inclen = m_capSize;
		while (szChk > inclen) {
			inclen = inclen << 1;
		}
		m_capSize = inclen;
		if (m_array == NULL) {
			m_array = new T[m_capSize];
			memset(m_array, 0, sizeof(T) * m_capSize);
			return;
		}
		T* narr = new T[m_capSize];
		memcpy(narr, m_array, sizeof(T) * m_size);
		memset(narr + m_size, 0, sizeof(T) * (m_capSize - m_size));
		delete m_array;
		m_array = narr;

	}
	else {
		ASSERT((szChk) <= this->m_capSize);
	}
}
template<class T>
VOID List<T>::Add(const T&  t) {
	GrowCheck(m_size+1);
	m_array[m_size++] = t;
}



template<class T>
VOID List<T>::CopyList(List<T>& src) {
	if (srclist.m_size == 0) {
		this->Clear();
		return;
	}
	GrowCheck(src.m_size);
	if (m_size != 0) {
		this->Clear();
	}
	T* tmparr = m_array;
	memcpy(tmparr, src.m_array, src.m_size * sizeof(T));
	m_size = src.m_size;
}


template<class T>
VOID  List<T>::Clear() {
	delete m_array;
	m_array = NULL;
	//memset(m_array, 0, sizeof(T) * m_size);
	m_size = 0;
}

template<class T>
VOID  List<T>::Set(int index, const T& t) {
	ASSERT_INDEX(index);
	m_array[index] = t;
}

template<class T>
VOID List<T>::operator=(List<T>& srclist) {
	ASSERT(srclist.m_size < this->m_capSize);

	 
		this->CopyList(srclist);
	 

}
template<class T>
T& List<T>::operator[](int pos) {
	ASSERT_INDEX(pos);
	return m_array[pos];
}
template<class T>
VOID List<T>::AddArray(T arr[], int len) {
	GrowCheck(m_size + len);
	T* tmparr = m_array + m_size;
	memcpy(tmparr, t, len * sizeof(T));
	m_size += len;
}

template<class T>
VOID List<T>::AddList(List<T>& srclist, int srcsize) {
	ASSERT(srclist.m_size >= srcsize);
	this->AddArray(srclist.m_array, srcsize);

}

template<class T>
VOID  List<T>::SetSize(int ss) {
	if (ss == this->m_size) {
		return;
	}
	ASSERT(ss < this->m_size);
	memset(this->m_array + ss, 0, (m_capSize - ss) * sizeof(T));
	this->m_size = ss;
}

