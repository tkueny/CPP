/**********************************************************************
 * James Stahl - Team Coyote
 **********************************************************************

 * File: Directory.h
 * Defines a directory system
 *
 * Implementation: files stored in unsorted_list, as the order does not matter.
 * Hash function implemented for constant-time access to any file.
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <unordered_set>
#include "File.h"

using namespace std;

/* begin hash functionality methods */

// hash File objects based on name
struct file_hasher
{
    // overload operator (), so this can be called as a function
    size_t operator()(const File &f) const
    {
        // returns an unsigned int using stl-provided hash function based on
        // name member in File object
        return hash<string>()(f.get_name());
    }
};

// compare File objects based on name
struct file_comparator
{
    bool operator()(const File &left, const File &right) const
    {
        // returns true if Files have same name
        return left.get_name() == right.get_name();
    }
};

/* end hash functionality methods */

class Directory
{
public:
    // constructors
    Directory();

    // destructor
    ~Directory();

    // accessors/display functions
    void display_list() const;

    unordered_set<File, file_hasher, file_comparator> get_file_list() const;

    string * get_file_name_list();

    File get_File(string);

    File get_File(File);

    File get_File(int);

    // mutators
    bool add_file(File &);

    void remove_file(string);

    void remove_file(File);

    // calculator
    int calculate_total_blocks();
    bool exists(string n);

private:
    unordered_set<File, file_hasher, file_comparator> files;
};

#endif
