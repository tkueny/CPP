/**********************************************************************
 * Lead Author: Tom Kueny - Team Coyote
 * Additional author: James Stahl
 **********************************************************************
*
* Taking inspiration from "diskprocess.cpp" with some modifications to the original methods of
* File: diskprocess.cpp
 This source file defines  a disk emulation process that
 will serve as the lowest level of a file system.
 In the include file, there is one class DiskBlockType
 which defines what a block is, and  one class DiskProcess that defines
 the disk and operations on the disk. Constructors and destructors are
 defined in the include file. The remaining function implementations
 are in this file.


 Author: D.M. Lyons dlyons@fordham.edu
 Most recent revision: Spring 2016

 If you use this program or any part of it you need to incude this entire
 comment. No warranty or support implied.

 c dmlyons 2012- 2017 Fordham CIS.
*
*
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "Directory.h"

#pragma
#define SENTINEL 999

using namespace std;

#ifndef BLOCKTYPE
#define BLOCKTYPE
class BlockType
{
public:
    int blockSize;
    char *data;

    BlockType(int blcksize = 10)
    {
        blockSize = 10;
        data = new char[blcksize];
        for (int i = 0; i < blcksize; i++)
            data[i] = '\0';
    }

    ~BlockType()
    { delete data; };
};

#endif

#ifndef DISK
#define DISK
class Disk
{
//    private:
public:
    int blockSize;
    int numBlocks;
    int numCreated;
    int numReads;
    int numWrites;
    int currentBlock;
    bool logging;
    Directory directory;
    // std::list<int> freeSpaceList;
    ofstream logfile;
    vector<BlockType *> disk;

    bool createBlock(int blockNumber)
    {
        if (blockNumber < numBlocks)
        {
            // push_back() used to add BlockTypes to disk
            disk.push_back(new BlockType(blockSize));
            numCreated++;
            return true;
        } else
        {
            cerr << "DISK: Tried to create a block beyond disk size.\n";
            return false;
        }
    }

public:

    Disk(int blcksize = 10, int blocknum = 10)
    {
        enableLogging("logfile.txt");
        blockSize = blcksize;
        numBlocks = blocknum;
        BlockType *block = new BlockType();
        numCreated = numReads = numWrites = currentBlock = 0;
        for (int i = 0; i < numBlocks; i++)
            createBlock(i);
        disk.push_back(block);
    }

    ~Disk()
    {
        for (int i = 0; i < numBlocks; i++)
            if (disk[i] != NULL)
                delete disk[i];
        if (logging)
        {
            logfile << "DISK: Terminating.\n";
            logfile << "DISK: Blocks created were " << numCreated << " of total "
                    << numBlocks << " which is "
                    << 100 * float(numCreated) / float(numBlocks) << " %\n";
            logfile.close();
        }
    }

    int getBlockSize()
    { return blockSize; }

    int getNumBlocks()
    { return numBlocks; }

    // bool CreateDisk(int blockSize, int numBlocks); Constructor for Disk class?
    bool enableLogging(string logfileName)
    {
        logfile.open(logfileName);
        if (logfile.is_open())
        { // did the file open?
            logging = true;
            logfile << "DISK: Logging enabled to " << logfileName << "\n";
            logfile << "DISK: block size " << blockSize
                    << " number of blocks " << numBlocks << "\n";
        } else cerr << "DISK: Could not open " << logfileName << "\n";
        return logging;
    }
    //Prof Lyons
    bool WriteDisk(int offset, BlockType *buffer)
    {


        if (offset < 0 || offset >= numBlocks)
        {
            cerr << "DISK: Block number is outside bounds\n";
            return -1;
        }
        if (buffer->blockSize != blockSize)
        {
            cerr << "DISK: bad block size sent to write.\n";
            return -1;
        }
        if (disk[offset] == NULL) // not ceated yet
            if (!createBlock(offset)) // is it possible to create?
                return -1; // bad block number create request

        // do the data write
        if (buffer->data != NULL && disk[offset]->data != NULL)
        {
            for (int i = 0; i < blockSize; i++)

                disk[offset]->data[i] = buffer->data[i];
            numWrites++;
        } else cerr << "DISK: Bad buffer pointers sent to write\n";

        // do logging if enabled
        if (logging)
            logfile << "DISK: Write to block " << offset << "\n";
        return 0;

    }
    //Tom kueny
    bool ReadDisk(int offset, BlockType *buffer)
    {
        if (offset < 0 || offset >= numBlocks)
        {
            cerr << "DISK: Block number is outside bounds\n";
            return -1;
        }
        if (buffer->blockSize != blockSize)
        {
            cerr << "DISK: Bad block size sent to read.\n";
            return -1;
        }
        if (disk[offset] == NULL)
            if (!createBlock(offset))
                return -1;
        if (buffer->data != NULL && disk[offset]->data != NULL)
        {
            for (int i = 0; i < blockSize; i++)
                buffer->data[i] = disk[offset]->data[i];
            numReads++;
        } else cerr << "DISK: Bad buffer pointers sent to read\n";
        if (logging)
            logfile << "DISK: Read to block " << offset << "\n";
        return 0;
    }

    void writeStats()
    {
        logfile << "DISK: block size " << blockSize
                << " number of blocks " << numBlocks << "\n";
        logfile << "DISK: Num reads " << numReads
                << " Num writes " << numWrites << "\n";
        logfile << "DISK: Blocks created were " << numCreated << " of total "
                << numBlocks << " which is "
                << 100 * float(numCreated) / float(numBlocks) << " %\n";

        return;
    }

    int availableContiguousBlocksStartBlock(int blockSize) {
        int startingBlock = 0;
        for (int i = 0; i < numBlocks; i++) {

            if (disk[i]->data[0] == '\0') {
                startingBlock = i;

                for (int j = i; j < startingBlock + blockSize; j++) {
                    if (disk[j]->data[0] != '\0') {
                        startingBlock = -1;
                        break;
                    }
                    return startingBlock;
                }
                if (startingBlock != -1)
                    return startingBlock;
            }
        }
        return startingBlock;
    }

    bool compactionNeeded()
    {
        bool needed = false;
        bool emptyBlockFound = false;
        int emptyStartBlock;
        for (int i = 0; i < numBlocks; i++)
        {
            if (emptyBlockFound && disk[i]->data[0] != '\0')
            {
                needed = true;
                break;
            }
            if (disk[i]->data[0] == '\0')
            {
                emptyBlockFound = true;
            }
        }
        return needed;
    }

    int Write(File &obj, char *newBuffer, int numchards = 0)
    {
        //no idea why numchards is passed but hey lets roll with it
        numchards = 0;

        //get amount of chars
        while (newBuffer[numchards] != '\0')
        {
            numchards++;

        }

        //calculate number of blocks
        int numberofBlocks = (int) ceil((float) numchards / (float) blockSize /*10*/);

        //set file obj to have correct block size based on buffer
        obj.set_size(numberofBlocks);

        //check to see if there's enough room on the disk

        //if enough space, set a starting block
        int startingBlock;// = SENTINEL;    // initialized to SENTINEL in case no blocks available

        startingBlock = availableContiguousBlocksStartBlock(numberofBlocks);

        if(startingBlock == -1)
        {
            // if not enough blocks
            return -1;
        }

        obj.set_starting_block(startingBlock);

        //set file data buffer
        obj.set_data(newBuffer);

        //copy data buffer
        char *fileBuffer = obj.get_data();

        //counter for buffer
        static int fileBufferIndex = 0;

        int starter = obj.get_starting_block();
        fileBufferIndex = 0;
        for (int x = 0; x < obj.get_size(); x++)
        {
            BlockType *newBlock = new BlockType();
            for (int y = 0; y < blockSize && fileBufferIndex < numchards; y++)
            {
                newBlock->data[y] = fileBuffer[fileBufferIndex];
                fileBufferIndex++;
            }

            WriteDisk(starter + x, newBlock);
        }

        /* add file to the directory */

        // remove the file with the given filename
        directory.remove_file(obj);

        // add file back with new info
        directory.add_file(obj);

        return numchards;
    }
    //Tom Kueny
    void compact()
    {
        //base case
        if (!compactionNeeded())
            return;
        else if (compactionNeeded())
        {
            //get empty block index
            bool empty = false;
            int n = 0;
            int firstEmptyIndex;
            while (!empty)
            {
                if (disk[n]->data[0] == '\0')
                {
                    empty = true;
                    firstEmptyIndex = n;
                }
                n++;
            }
            //get next file's index
            bool nextFileFound = false;
            int nextFileIndex;
            int ctr = firstEmptyIndex;
            while (!nextFileFound)
            {
                if (disk[ctr]->data[0] != '\0')
                {
                    nextFileFound = true;
                    nextFileIndex = ctr;
                }
                ctr++;
            }
            //copy file object to an object NOT ON THE DISK
            File newFile = directory.get_File(nextFileIndex);
            //delete object with same name ON THE DISK
            Delete(newFile.get_name());
            Create(newFile);
            newFile.set_starting_block(firstEmptyIndex);
            //delete object with same name ON THE DISK

            //write copied file on disk at appropriate index
            Write(newFile, newFile.get_data());
            //check for more files that need to be compacted
            if (compactionNeeded())
                compact();
        }
    }
    //Tom Kueny
    bool Create(std::string const &aFileName)
    {
        File newFile(aFileName);
        if(directory.add_file(newFile));
        return true; //add file to set, no size = no writing, yet
    }
    //Tom Kueny
    bool Create(File& obj){
        File newFile = obj;
        directory.add_file(newFile);
        return true;
    }
    //Tom Kueny
    bool Delete(std::string const &aFileName)
    {

        //NOTE: COMPACT MUST BE CALLED IN TANDEM WITH DELETE

        // obtain info for deletion
        File target = directory.get_File(aFileName);
        int fileBlocks = target.get_size();
        int startingBlock = target.get_starting_block();

        // clear appropriate data on disk and replace with dummy data
        for (int i = startingBlock; i < startingBlock + fileBlocks; i++)
        {
            delete disk[i]->data;
            disk[i]->data = new char[blockSize];
            for(int j=0; j < blockSize; j++)
                disk[i]->data[j] = '\0';

        }

        // remove file from directory
        directory.remove_file(aFileName);

        // automatically compact disk after deletion
        this->compact();
        return true;
    }

    /* When Delete() is used, compact() must be called after to avoid fragmentation.
     * This method calls the two sequentially. compact() cannot be called within
     * Delete() because compact() uses Delete(), creating an infinite loop
     */
    //James Stahl
    void Delete_and_compact(const string & file_name)
    {
        if(Delete(file_name))
            this->compact();
    }
    //Tom Kueny
    File Open(std::string const &aFileName)
    {
        File obj = directory.get_File(aFileName);
        return obj;

    }
    //Tom Kueny
    bool Close(string fileName, char *buffer = NULL)
    {
        //buffer is NULL if no write is done
        if (buffer != NULL)
        {
            File obj = directory.get_File(fileName);
            Write(obj, buffer);
        }
        return true;
    }
    //Tom Kueny
    string Read(string fileName, int numchards = 0, char *buffer = NULL)
    {

        File fileObj = directory.get_File(fileName);
        int starter = fileObj.get_starting_block();
        static int ctr = 0;
        char* newBuffer = new char;

        for(int i = 0; i < fileObj.get_size(); i++){
            BlockType* tempBuffer = new BlockType;
            ReadDisk(starter, tempBuffer);
            starter++;
            int j = 0;
            while(tempBuffer->data[j] != '\0'){
                j++;

            }

            for(int n = 0; n < j; n++){
                newBuffer[ctr] = tempBuffer->data[n];
                ctr++;
            }

        }
        ctr = 0;
        string buff = "";
        buff = newBuffer;
        if(buff == "")
            buff = fileObj.get_data();

        return buff;
    }

    //Tom Kueny
    int Stats(std::string const & aFileName)
    {
        File f = directory.get_File(aFileName);
        int s = f.get_block_size();
        return s;
    }

    vector<string> List_strings()
    {
        // vector to be returned
        vector<string> ret;

        // obtain list of files from the directory
        unordered_set<File, file_hasher, file_comparator> list
                = this->directory.get_file_list();

        // store file_name, size tuples as one tabbed string in ret
        string tuple;
        for(const auto & file: list)
        {
            tuple = file.get_name() + "\t\t\t" + to_string(file.get_size());
            ret.push_back(tuple);
        }

        return ret;
    }

    unordered_set<File, file_hasher, file_comparator> List_Files()
    {
        return directory.get_file_list();
    }

};

#endif
