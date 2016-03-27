#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

template <class T>
class Buffer
{
protected:
	std::vector<T> _buffer;
	std::size_t _chunkSize;

public:
	Buffer ();
	~Buffer ();

	void Add (T next);
	void Add (Buffer<T> nextBuffer);

	void SetChunkSize (std::size_t size);

	std::size_t GetChunkSize ();
	std::size_t GetChunkByteSize ();
	std::size_t GetChunksCount ();

	T& operator[] (std::size_t pos);

	const T* GetPointer () const;
	std::size_t GetSize () const;
	std::size_t GetBytesCount () const;

	void Clear ();
};

template <class T>
Buffer<T>::Buffer () :
	_chunkSize (1)
{

}

template <class T>
Buffer<T>::~Buffer ()
{
	Clear ();
}

template <class T>
void Buffer<T>::Add (T next)
{
	_buffer.push_back (next);
}

template <class T>
void Buffer<T>::Add (Buffer<T> nextBuffer)
{
	for (float next : nextBuffer._buffer) {
		_buffer.push_back (next);
	}
}

template <class T>
void Buffer<T>::SetChunkSize (std::size_t size)
{
	_chunkSize = size;
}

template <class T>
std::size_t Buffer<T>::GetChunkSize ()
{
	return _chunkSize;
}

template <class T>
std::size_t Buffer<T>::GetChunkByteSize ()
{
	return sizeof (T) * _chunkSize;
}

template <class T>
std::size_t Buffer<T>::GetChunksCount ()
{
	return _buffer.size () / GetChunkByteSize ();
}

template <class T>
T& Buffer<T>::operator[] (std::size_t pos)
{
	return _buffer [pos];
}

template <class T>
const T* Buffer<T>::GetPointer () const
{
	return &_buffer [0];
}

template <class T>
std::size_t Buffer<T>::GetSize () const
{
	return _buffer.size ();
}

template <class T>
std::size_t Buffer<T>::GetBytesCount () const
{
	return sizeof (T) * _buffer.size ();
}

template<class T>
void Buffer<T>::Clear ()
{
	_buffer.clear ();
	_buffer.shrink_to_fit ();
}

#endif