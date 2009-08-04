#ifndef SINGLETON_H
#define SINGLETON_H

#include "stdafx.h"
namespace Sajiki
{
	template <class Template>

	class Singleton
	{
	public:
		static Template* Instance()
		{
			if (_instance == NULL)
				_instance = new Template;
			return _instance;
		}
		static void Destroy()
		{
			delete _instance;
			_instance = 0;
		}
	protected:
		Singleton() {};
	private:
		static Template* _instance;
	};

	template <class Template> Template* Singleton<Template>::_instance = NULL;
}
#endif