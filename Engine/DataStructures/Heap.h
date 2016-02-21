#ifndef HEAP_H
#define HEAP_H

#define father(nod) (nod >> 1)
#define left_son(nod) (nod << 1)
#define right_son(nod) ((nod << 1) + 1)

#include <vector>
template <class HeapElement, class Comparator>
class Heap
{
public:
	Heap ();
	Heap (std::vector<HeapElement> list);
	~Heap ();
	void Insert (HeapElement element);
	void Remove (int position);
	int Size();
	HeapElement ElementAt (int position);
	void ReplaceAt (int position, HeapElement element);
	HeapElement Top ();
	void Clear ();

private:
	void Percolate (int position);
	void Sift (int position);
	void MakeHeap (void);
	void Swap (int positionOne, int positionTwo);

private:
	std::vector<int> _heap;
	std::vector<HeapElement> _elements;
	std::vector<int> _elementPositions;
};

// WTF, in C++ templates cannot be separated in header
// and source files (the code need to be visible
// for the compiler at precompile phase, so they need
// to be in the SAME file... fk)
template <class HeapElement, class Comparator>
Heap<HeapElement, Comparator>::Heap ()
{
	_heap.push_back (0);
	_elementPositions.push_back (0);
    HeapElement emptyElement;
	_elements.push_back (emptyElement);

}

template <class HeapElement, class Comparator>
Heap<HeapElement, Comparator>::Heap (std::vector<HeapElement> list)
{
	_heap.push_back (0);
	_elementPositions.push_back (0);
	HeapElement emptyElement;
	_elements.push_back (emptyElement);

	for (int i=0;i<list.size();i++) {
		_heap.push_back (i+1);
		_elementPositions.push_back (i+1);
		_elements.push_back (list [i]);
	}
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Insert (HeapElement element)
{
	_elements.push_back (element);

	int position = _elements.size () - 1;

	_heap.push_back (position);
	_elementPositions.push_back (Size ());

	Percolate (Size ());
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Remove (int position)
{
    int currentPosition = _elementPositions[position];

	Swap (currentPosition, Size ());

	_heap.pop_back ();

	Comparator comparator;
	if (comparator (_elements[_heap[currentPosition]], _elements[_heap[father(currentPosition)]])) {
		Percolate (currentPosition);
		return ;
	}

	Sift (currentPosition);
}

template <class HeapElement, class Comparator>
int Heap<HeapElement, Comparator>::Size (void)
{
	return _heap.size() - 1;
}

template <class HeapElement, class Comparator>
HeapElement Heap<HeapElement, Comparator>::Top (void)
{
	return _elements[_heap [1]];
}

template <class HeapElement, class Comparator>
HeapElement Heap<HeapElement, Comparator>::ElementAt (int position)
{
	return _elements [_heap [_elementPositions [position]]];
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::ReplaceAt (int position, HeapElement element)
{
	int heapPosition = _elementPositions [position];

	_elements [_heap [heapPosition]] = element;

	Comparator comparator;
	if (heapPosition > 1 && comparator (_elements[_heap[heapPosition]], _elements[_heap[father(heapPosition)]])) {
		Percolate (heapPosition);
		return ;
	}

	Sift (heapPosition);
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Swap (int positionOne, int positionTwo)
{
	_elementPositions[_heap[positionOne]] = positionTwo;
	_elementPositions[_heap[positionTwo]] = positionOne;
	std::swap (_heap[positionOne], _heap[positionTwo]);
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::MakeHeap (void)
{
	for (int i=_heap.size ();i>0;i--) {
		Sift (i);
	}
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Percolate (int position)
{
	Comparator comparator;
	while (position > 1) {
		int fatherPosition = father(position);

		if (comparator (_elements[_heap[fatherPosition]], _elements[_heap[position]])) {
			return ;
		}

		Swap (position, fatherPosition);

		position = fatherPosition;
	}
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Sift (int position)
{
	Comparator comparator;

	while (true) {
        if (left_son (position) > Size ()) {
            return ;
        }

		if (right_son(position) > Size()) {
			if (comparator (_elements[_heap[left_son(position)]], _elements[_heap[position]])) {
				Swap (position, left_son(position));
			}

			return ;
		}

		int bigestSon = comparator (_elements[_heap[left_son(position)]], _elements[_heap[right_son(position)]]) ? 
							left_son(position) : right_son(position);

		if (comparator(_elements[_heap[position]], _elements[_heap[bigestSon]])) {
			return;
		}
	
		Swap (position, bigestSon);
		position = bigestSon;
	}
}

template <class HeapElement, class Comparator>
void Heap<HeapElement, Comparator>::Clear ()
{
	// while (_heap.size () > 1) {
	// 	_heap.pop_back ();
	// 	_elements.pop_back ();
	// 	_elementPositions.pop_back ();
	// }

	// _heap.shrink_to_fit ();
	// _elements.shrink_to_fit ();
	// _elementPositions.shrink_to_fit ();

	_heap.clear();
	_elements.clear();
	_elementPositions.clear();

	std::vector<int> ().swap (_heap);
	std::vector<int> ().swap (_elementPositions);
	std::vector<HeapElement> ().swap (_elements);

	_heap.push_back (0);
	_elementPositions.push_back (0);
    HeapElement emptyElement;
	_elements.push_back (emptyElement);
}

template <class HeapElement, class Comparator>
Heap<HeapElement, Comparator>::~Heap ()
{
	
}

#endif