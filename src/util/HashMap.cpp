#include "HashMap.h"


template<>
size_t DefaultHash<std::string>::hash(const std::string& key)
{
    const size_t prime1 = 14399107;
    const size_t prime2 = 10382231;

    size_t hash = 35809439;

    typedef std::string::const_iterator StringIterator;
    for (StringIterator i = key.begin(); i != key.end(); i++) {
        hash = hash * prime1 ^ (*i) * prime2;
    }
    return hash;
}


uetli::util::NoEntryException::NoEntryException(const std::string &message) :
    message(message)
{
}


uetli::util::NoEntryException::~NoEntryException(void) throw()
{
}


const char* uetli::util::NoEntryException::what(void) const throw()
{
    return message.c_str();
}



