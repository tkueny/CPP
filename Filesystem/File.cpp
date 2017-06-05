/*
Lead Author: James Stahl
Additional Authors: Tom Kueny
*/
#include "File.h"

#define DEFAULT_BLOCK_SIZE 10

// set size of 1 char in bytes (i.e. if CHAR_SIZE is 1, then 1 char = 1 byte)
#define CHAR_SIZE 1

int File::block_size = DEFAULT_BLOCK_SIZE;
//const string File::default_name=DEFAULT_NAME;
//
//const string File::get_default(){
//    return default_name;
//}
File::File(std::string fileName)
{
    data = NULL;
    block_size = DEFAULT_BLOCK_SIZE;
    name = fileName;
    size = 0;
    // calculate_size();
    starting_block = 0;
    // calculate_size();
}

File::File(std::string fileName, int blockSize, char *buffer)
{
    data = buffer;
    block_size = blockSize;
    name = fileName;
    size = NULL;
    calculate_size();
    starting_block = NULL;
}

File::File(const File &f)
{
    starting_block = f.starting_block;
    data = f.get_data();    // accessor used to simplify obtaining pointer
    name = f.name;
    size = f.size;
}

File& File::operator=(const File& right) {
    starting_block = right.starting_block;
    data = right.get_data();    // accessor used to simplify obtaining pointer
    name = right.name;
    size = right.size;
}

// operator overload
bool File::operator==(const File &right) const
{
    // File objects are considered equivalent if they share a name
    return right.name == name;
}

// destructor
File::~File()
{
    //delete [] data;
}

// accessors
string File::get_name() const
{
    return name;
}

int File::get_size() const
{
    return size;
}

int File::get_block_size() const
{
    return block_size;
}

char *File::get_data() const
{
    return data;
}

int File::get_starting_block() const
{
    return starting_block;
}

// mutators
void File::set_data(char *d)
{
    data = d;
}

void File::set_size(int s)
{
    size = s;
}

void File::set_block_size(int s)
{
    block_size = s;
}

void File::set_name(string n)
{
    name = n;
}

void File::set_starting_block(int b)
{
    starting_block = b;
}

// calculators

/* calculates size in blocks and sets size accordingly */
void File::calculate_size()
{
    // determine size of data in bytes
    string d = data;
    int bytes =(int)(CHAR_SIZE * d.length());

    // determine size of data in blocks
    int blocks = bytes / block_size;

    // set size accordingly
    size = blocks;
}
