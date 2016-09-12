/****************************************************************************
Copyright (c) 2012 Zynga Inc.
Copyright (c) 2013-2015 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __JSB_LOCALSTORAGE_H
#define __JSB_LOCALSTORAGE_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <string>
#include "CCPlatformMacros.h"

class GLLocalStorage
{
public:
	/**
	* @addtogroup storage
	* @{
	*/

	/** Local Storage support for the JS Bindings.*/

	/** Initializes the database. If path is null, it will create an in-memory DB. */
	static void glLocalStorageInit(const std::string& fullpath/* = "" */);
	/** Frees the allocated resources. */
	static void glLocalStorageFree();

	/** Sets an item in the JS. */
	static void glLocalStorageSetItem(const std::string& key, const std::string& value);

	/** Gets an item from the JS. */
	static bool glLocalStorageGetItem(const std::string& key, std::string *outItem);

	/** Removes an item from the JS. */
	static void glLocalStorageRemoveItem(const std::string& key);

	/** Removes all items from the JS. */
	static void localStorageClear();

	static int getIsInitialized();

private:
	static void glLocalStorageCreateTable();
	// end group
	/// @}

#endif // __JSB_LOCALSTORAGE_H
};

