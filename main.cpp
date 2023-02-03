#include <ctime>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

// Transaction Data

struct TransactionData
{
    double amount;
    string senderKey;
    string receiverKey;
    time_t timestamp;
};

// Block Class
class Block
{
private:
    int index;
    size_t blockHash;
    size_t previousHash;
    size_t generateHash();

public:
    Block(int idx, TransactionData, size_t prevHash);

    // get Original Hash
    size_t getHash();

    // get previous Hash
    size_t getPreviousHash();

    // Transaction Data
    TransactionData data;

    bool isHashValid();
};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash)
{
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();
}

// private functions
size_t Block::generateHash()
{
    hash<string> hash1;
    hash<size_t> hash2;
    string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timestamp);

    return (hash1(toHash) + hash2(previousHash));
}

// public functions
size_t Block::getHash()
{
    return blockHash;
}
size_t Block::getPreviousHash()
{
    return previousHash;
}
bool Block::isHashValid()
{
    return generateHash() == blockHash; // No one will be able to alter the hash we can match it witn the generated hash
}
// Blockchain Class
class BlockChain
{
private:
    Block createGenesisBlock();

public:
    // Public Chain
    vector<Block> chain;

    // constructor
    BlockChain();

    // Public functions
    void addBlock(TransactionData data);
    bool isChainValid();

    Block *getLatestBlock(); // so that we can change data in that block
};

BlockChain::BlockChain()
{

    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
}
// first block in the blockchain
Block BlockChain::createGenesisBlock()
{
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.receiverKey = "None";
    d.senderKey = "None";
    d.timestamp = time(&current);

    hash<int> hash1;
    Block genesis(0, d, hash1(0));
    return genesis;
}

Block *BlockChain::getLatestBlock()
{
    return &chain.back();
}

void BlockChain::addBlock(TransactionData d)
{

    int index = (int)chain.size() - 1;
    Block newBlock(index, d, getLatestBlock()->getHash());
}

bool BlockChain::isChainValid()
{
    vector<Block>::iterator it;
    int chainLen = (int)chain.size();

    for (it = chain.begin(); it != chain.end(); ++it)
    {

        Block currentBlock = *it;
        if (!currentBlock.isHashValid())
            return false;

        if (chainLen > 1)
        {
            Block previousBlock = *(it - 1);
            if (currentBlock.getPreviousHash() != previousBlock.getHash())
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    BlockChain AwesomeCoin;

    // Data for first added block
    TransactionData data1;
    time_t data1Time;
    data1.amount = 1.5;
    data1.receiverKey = "Joe";
    data1.senderKey = "Sally";
    data1.timestamp = time(&data1Time);

    AwesomeCoin.addBlock(data1);
    cout << "Is Chain Valid" << endl
         << AwesomeCoin.isChainValid() << endl;

    TransactionData data2;
    time_t data2Time;
    data1.amount = 0.873;
    data1.receiverKey = "Ujjwal";
    data1.senderKey = "red";
    data1.timestamp = time(&data2Time);

    cout << "Is Chain Valid" << endl
         << AwesomeCoin.isChainValid() << endl;

    Block *hack = AwesomeCoin.getLatestBlock();
    hack->data.amount = 10000;

    cout << "Is Chain Valid" << endl
         << AwesomeCoin.isChainValid() << endl;
}