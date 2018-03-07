// =============================================================================
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
// =============================================================================

#ifndef UETLI_UTIL_HASHMAP_H_
#define UETLI_UTIL_HASHMAP_H_

#include <exception>
#include <vector>
#include <string>

namespace uetli
{
    namespace util
    {
        template <typename K, typename V, typename H>
        class HashMap;

        template <typename K>
        class DefaultHash;

        class NoEntryException;
    }
}


/// \brief simple hash map
///
/// This class represents a dictionary data structure implemented as a hash
/// table.
///
///
/// \tparam K the key type
///
/// \tparam V the value type
///
/// \tparam H a hash-class which should implement the static function:
///           <code>static size_t H::hash(const K&);</code>
///
/// \author Nicolas Winkler
///
template <typename K, typename V, typename H = uetli::util::DefaultHash<K> >
class uetli::util::HashMap
{
private:

    /// initial length of the hash table
    static const size_t standardSize = 16;

    ///
    /// \brief structure to store entries
    ///
	struct Entry
	{
		K key;
		V value;
		Entry* next;

		Entry(void);
	};

    /// list of pointers to Entry*, each representing a linked list
    /// containing every entry at the specific position.
    Entry** entryTable;

    /// size of the allocated table (in number of buckets in the table)
    size_t size;

    /// bitmask applied to the hash-value mapping it to the table space
    size_t hashMask;

    /// absolute number of entries
    size_t nEntries;

public:

    ///
    /// \brief default constructor
    ///
    /// The hash map is initialized to the default length and is empty.
    ///
    HashMap(void);

    ///
    /// \brief copy constructor
    /// \param map the hash map to copy
    ///
    HashMap(const HashMap<K, V, H>& map);

    ~HashMap(void);

    ///
    /// Returns the number of elements that are currently held in this
    /// HashMap. This has nothing to do with the allocated table where
    /// entries are stored.
    ///
    /// \return the number of currently held entries
    ///
    size_t getElementCount(void) const;

    ///
    /// Resizes the table of the hashmap. The table may be longer than
    /// the specified size.
    ///
    /// This function does not affect the content of the dictionary. It
    /// can be used if it is previously known that a large number of entries
    /// will be added in the near future.
    ///
    /// \param newSize the desired size.
    ///
    void resize(size_t newSize);

    ///
    /// Add a new entry to the dictionary
    /// \param key
    /// \param value
    ///
    void put(const K& key, const V& value);

    ///
    /// Checks, if the dictionary contains an entry to a specific key.
    ///
    /// \param key the key to test
    ///
    /// \return <code>true</code>, if the key was found in the dictionary,
    ///         <code>false</code> otherwise.
    ///
    bool contains(const K& key) const;

    ///
    /// This method searches the dictionary for the given key
    /// and returns a constant pointer to the corresponding
    /// value when found. This function will not throw any exception
    /// in any case. Therefore it can also be used in cases where one is
    /// not absolutely certain that the dictionary contains the value,
    /// without the necessity for a try-catch-block.
    ///
    /// <code>getReference(x)</code> will return 0 if and only if
    /// <code>contains(x)</code> returns <code>false</code>
    ///
    /// \param key the key to search for
    ///
    /// \return a constant pointer to the value belonging to the given key.
    ///         If the dictionary does not contain this key, 0 is returned.
    ///
    const V* getReference(const K& key) const throw();

    ///
    /// This method searches the dictionary for the given key
    /// and returns a constant pointer to the corresponding
    /// value when found. This function will not throw any exception
    /// in any case. Therefore it can also be used in cases where one is
    /// not absolutely certain that the dictionnary contains the value,
    /// without the necessity for a try-catch-block.
    ///
    /// <code>getReference(x)</code> will return 0 if and only if
    /// <code>contains(x)</code> returns <code>false</code>
    ///
    /// \param key the key to search for
    ///
    /// \return a constant pointer to the value belonging to the given key.
    ///         If the dictionnary does not contain this key, 0 is returned.
    ///
    V* getReference(const K& key) throw();


    ///
    /// This method searches the dictionnary for the given key
    /// and returns the constant version of the corresponding
    /// value when found. Otherwise this function will
    /// throw an exception.
    ///
    /// \param key the key to search for
    ///
    /// \return the value belonging to the given key
    ///
    /// \throws uetli::util::NoEntryException, when no key is found
    ///
    const V& get(const K& key) const;

    ///
    /// This method searches the dictionnary for the given key and returns
    /// the corresponding value when found. Otherwise this function
    /// will throw an exception.
    ///
    /// \param key the key to search for
    ///
    /// \return the value belonging to the given key
    ///
    /// \throws uetli::util::NoEntryException, when no key is found
    ///
    V& get(const K& key);

    ///
    /// \brief clears the hash map
    ///
    /// A call to this function will remove all entries
    /// previously stored in the dictionary and will
    /// shrink (or grow) the hash table to it's standard size.
    ///
    void clear(void);

private:
    void deleteContent(void);
    void updateHashMask(void);

private:
    size_t getEntryIndex(const K& key) const;
    void putEntry(void*);
};


///void processMethod(Method* method, MethodDeclaration* declaration);
/// simple Hash-function for general purpose
///
/// \tparam K type of the key value
///
/// \author Nicolas Winkler
///
template <typename K>
class uetli::util::DefaultHash
{
    DefaultHash(void) {}
public:
    ///
    /// \brief calculates a hash function
    /// \param key the key value
    /// \return the hash value of the key
    ///
    static size_t hash(const K& key);
};


namespace uetli
{
namespace util
{

///
/// hash function for class <code>std::string</code>
///
/// \author Nicolas Winkler
///
template <>
class DefaultHash<std::string>
{
    DefaultHash(void);
public:
    ///
    /// \brief calculates a hash function
    /// \param key the key value
    /// \return the hash value of the key
    ///
    static size_t hash(const std::string& key);
};

} // namespace uetli::util
} // namespace uetli


///
/// This exception class is thrown by the uetli::util::HashMap class
/// to indicate that a desired entry does not exist.
///
/// \author Nicolas Winkler
///
class uetli::util::NoEntryException : public std::exception
{
    /// error messace
    std::string message;
public:
    NoEntryException(const std::string& message);
    ~NoEntryException(void) throw();

    ///
    /// \return a short description of the error
    ///
    const char* what(void) const throw();
};

#include "HashMap.inl"

#endif // UETLI_UTIL_HASHMAP_H_
