/*
 * This version of Singleton combines the versions of multiple
 * authors to construct the most stable Singleton that I find
 * on the web.
 *
 * The common version is to use the Curiously Recurring 
 * Template Pattern (CRTP). Source: 
 * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 *
 * Scott Meyers version of Singleton.
 * Scott Meyers in his Effective C++ book adds a slightly improved 
 * version and in the getInstance() method returns a reference 
 * instead of a pointer. So the pointer final deleting problem disappears.
 * One advantage of this solution is that the function-static object is 
 * initialized when the control flow is first passing its definition.
 *
 * Andrei Alexandrescu's version of Singleton.
 * Phoenix version of Singleton which revive if needed. One such
 * situation would happen when a singleton::instance is called
 * after the singleton was already destroyed before. This could
 * happen for multiple reasons, some are strange class management or
 * (and worse) compiler destruction order. 
 *
 * This singleton implementation was not thread-safe until the C++ 11 
 * standard. In C++11 the thread-safety initialization and destruction 
 * is enforced in the standard. 
 * 
 * Thomas Badie version take advantage of std::forward to forward
 * arguments to the singleton. In this way the Singleton class
 * is as generic as possible. 
 */

#ifndef SINGLETON_H
#define SINGLETON_H

#include <utility>
#include <cstdlib>

template <typename T>
class Singleton
{
private:
	static T* _instance;
	static bool _isDestroyed;

public:
	template <typename... Args> 
	static T* Instance (Args... args)
	{
		if (_instance == nullptr) {
			if (_isDestroyed) {
				OnDeadReference ();
			} else {
				CreateSingleton (std::forward<Args>(args)...);
			}
		}

		return _instance;
	}

	template <typename... Args>
	static void CreateSingleton (Args... args)
	{
		_instance = new T (std::forward<Args>(args)...);

		atexit (KillPhoenixSingleton);
	}

	static void KillPhoenixSingleton ()
	{
		_instance->~T ();

		_isDestroyed = true;
	}

	static void OnDeadReference ()
	{
		new (_instance) T;

		_isDestroyed = false;

		atexit (KillPhoenixSingleton);
	}
};

template<typename T> 
T* Singleton<T>::_instance (nullptr);

template<typename T>
bool Singleton<T>::_isDestroyed (false);

#endif