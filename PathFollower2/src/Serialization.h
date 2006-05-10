#ifndef SERIALIZATION_H
#define SERIALIZATION_H 1

#include <string>

#include <fstream>

class Archive
{

public:
	Archive(std::string filename, Mode m_mode) : m_valid(true), m_size(0), m_loaded(0) { };

	virtual void read(const char* data, unsigned int size) = 0;
	virtual void write(const char* data, unsigned int size) = 0;

	bool isValid() { return m_valid; };

	unsigned int getSize() { return m_size; };
	unsigned int getLoaded() { return m_loaded; };

	typedef enum {READ, WRITE} Mode;

protected:
	bool m_valid;

	unsigned int m_size;
	unsigned int m_loaded;

	Mode m_mode;
	std::string m_filename;
	std::fstream m_file;

};

template <typename Ty>
class WriteObjectsMap: public std::map<osg::ref_ptr<Ty>, unsigned int>
{

public:
	unsigned int getOrCreateId(Ty* ptr)
	{

		unsigned int result;

		RefMap::iterator iter = m_data.find(ptr);
		if(iter != m_data.end())
		{

			result = iter->second;

		} else {

			result = m_data.size();
			m_data[ptr] = result;

		};

        return result;
		
	};

private:
	typedef std::map<osg::ref_ptr<Ty>, unsigned int> RefMap;
	RefMap m_data;

};

template <typename Ty>
class ReadObjectsMap: public std::map<unsigned int, osg::ref_ptr<Ty>>
{

public:
	ReadObjectsMap(Archive& ar)
	{

		unsigned int size;
		ar >> size;

		unsigned int count;

		for(count = 0; count < size; count++)
		{



		while(size--)
		{



	Ty* getOrCreateObject(unsigned int id)
	{

		Ty* result = NULL;

		RefMap::iterator iter = m_data.find(id);
		if(iter != m_data.end())
		{

			result = iter->second;

		} else {

			result = new Ty;
			m_data[id] = result;

		};

		return result;

	};

private:
	typedef std::map<unsigned int, osg::ref_ptr<Ty>> RefMap;
	RefMap m_data;

};


class Serializable
{

public:
	Serializable() : m_loaded(false);

	virtual Archive& operator>>(Archive& ar) { m_loaded = false; };
	virtual Archive& operator<<(Archive& ar) { };

	bool getLoaded() { return m_loaded; };

private:
	bool m_loaded;

};

#include "SerializationBasicTypes.h"

#endif