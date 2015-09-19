// ============================================================================
//
// This file is part of the uetli compiler.
//
// Copyright (C) 2014-2015 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================

#include <cstring>

template <typename K, typename V, typename H>
uetli::util::HashMap<K, V, H>::Entry::Entry(void)
{
	next = 0;
}


template <typename K, typename V, typename H>
uetli::util::HashMap<K, V, H>::HashMap(void)
{
	size = standardSize;
	nEntries = 0;
	entryTable = new Entry*[size];
	memset(entryTable, 0, standardSize * sizeof(Entry*));
	updateHashMask();
}


template <typename K, typename V, typename H>
uetli::util::HashMap<K, V, H>::HashMap(const HashMap<K, V, H>& hashMap)
{
	this->size = hashMap.size;
	updateHashMask();
	entryTable = new Entry*[size];
    for (size_t i = 0; i < size; i++) {
		const Entry* mapEntry = hashMap->entryTable[i];

		while (mapEntry != 0) {
			this->put(mapEntry->key, mapEntry->value);
			mapEntry = mapEntry->next;
		}
	}
}


template <typename K, typename V, typename H>
uetli::util::HashMap<K, V, H>::~HashMap(void)
{
	deleteContent();
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::resize(size_t newSize)
{
    size_t oldSize = size;
	{
        size_t i = 0;
        for (i = 0; (1U << i) < newSize && i <= sizeof(size_t) * 8; i++);
		size = 1 << i;
	}
	Entry** oldEntryTable = entryTable;
	entryTable = new Entry*[size];
	memset(entryTable, 0, size * sizeof(Entry*));
	updateHashMask();

	nEntries = 0;

	for (unsigned int i = 0; i < oldSize; i++) {
		Entry* entry = oldEntryTable[i];
		while (entry != 0) {
			Entry* next = entry->next;
			putEntry(entry);
			entry = next;
		}
	}
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::put(const K& key, const V& value)
{
	if (double(nEntries) > size * 1.5) {
		resize(size * 2);
	}

	Entry* entry = new Entry();
	entry->key = key;
	entry->value = value;
    putEntry(entry);
}


template <typename K, typename V, typename H>
bool uetli::util::HashMap<K, V, H>::contains(const K& key) const
{
    size_t index = getEntryIndex(key);
	const Entry* entry = entryTable[index];
	while (entry != 0) {

		if (entry->key == key)
			return true;

		entry = entry->next;
	}
	return false;
}


template <typename K, typename V, typename H>
const V* uetli::util::HashMap<K, V, H>::getReference(const K& key) const throw()
{
    size_t index = getEntryIndex(key);
    const Entry* entry = entryTable[index];
    while (entry != 0) {

        if (entry->key == key)
            return &entry->value;

        entry = entry->next;
    }
    return 0;
}


template <typename K, typename V, typename H>
V* uetli::util::HashMap<K, V, H>::getReference(const K& key) throw()
{
    return const_cast<V*> (
        const_cast<const HashMap<K, V, H>*> (this)->getReference(key)
    );
}


template <typename K, typename V, typename H>
const V& uetli::util::HashMap<K, V, H>::get(const K& key) const
{
    const V* ret = getReference(key);
    if (ret != 0)
        return *ret;
    else
        throw NoEntryException("Invalid key");
}


template <typename K, typename V, typename H>
V& uetli::util::HashMap<K, V, H>::get(const K& key)
{
    V* ret = getReference(key);
    if (ret != 0)
        return *ret;
    else
        throw NoEntryException("Invalid key");
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::clear(void)
{
	deleteContent();

	size = standardSize;
	entryTable = new Entry*[size];
	memset(entryTable, 0, standardSize * sizeof(Entry*));
	updateHashMask();
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::deleteContent(void)
{
    for (size_t i = 0; i < size; i++) {
		while (entryTable[i] != 0) {
			Entry* e = entryTable[i];
			entryTable[i] = e->next;
			delete e;
			e = 0;
		}
	}
	delete[] entryTable;
	entryTable = 0;
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::updateHashMask(void)
{
	hashMask = size - 1;
}


template <typename K, typename V, typename H>
size_t uetli::util::HashMap<K, V, H>::getEntryIndex(const K& key) const
{
    return hashMask & H::hash(key);
}


template <typename K, typename V, typename H>
void uetli::util::HashMap<K, V, H>::putEntry(void* e)
{
	Entry* toAdd = (Entry*) e;
    size_t index = getEntryIndex(toAdd->key);
	Entry* entry = entryTable[index];
	Entry* lastEntry = 0;

	while (entry != 0) {
		lastEntry = entry;
		entry = entry->next;
	}
	
	entry = toAdd;
	if (lastEntry != 0)
		lastEntry->next = entry;
	else
		entryTable[index] = entry;

	entry->next = 0;

	nEntries++;
}


template <typename K>
size_t uetli::util::DefaultHash<K>::hash(const K& key)
{
    const void* pointer = &key;
    if (sizeof(K) == sizeof(char)) {
        return (size_t) *reinterpret_cast<const unsigned char*> (pointer);
    }
    else if (sizeof(K) == sizeof(short)) {
        return (size_t) *reinterpret_cast<const unsigned short*> (pointer);
    }
    else if (sizeof(K) == sizeof(int)) {
        return (size_t) *reinterpret_cast<const unsigned int*> (pointer);
    }
    else if (sizeof(K) == sizeof(long)) {
        return (size_t) *reinterpret_cast<const unsigned long*> (pointer);
    }
    else if (sizeof(K) == sizeof(long long)) {
        return (size_t) *reinterpret_cast<const unsigned long long*> (pointer);
    }
    else if (sizeof(K) == sizeof(size_t)) {
        return (size_t) *reinterpret_cast<const size_t*> (pointer);
    }
    else {
        const size_t* valPtr = reinterpret_cast<const size_t*> (pointer);
        size_t hash = 172786267;
        const size_t prime1 = 45280901;
        const size_t prime2 = 94494509;
        for (size_t i = 0; i < sizeof(K) / sizeof(size_t); i++) {
            hash = hash * prime1 ^ valPtr[i] * prime2;
        }
        return hash;
    }
}

