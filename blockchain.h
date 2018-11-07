typedef struct {
	char prevHash[257];
	char currentHash[257];
	char nextHash[257];
	char blockTitle[129];
	long long int blockNumber;
	char merkleTree[257];
	time_t creationTime;
} Block;

typedef struct {
	char prevHash[257];
	char currentHash[257];
	char ownerHash[257];
	char license[129];
	time_t creationTime;
} Transaction;

typedef struct {
	struct MerkleNode *parent;
	struct MerkleNode *leftChild;
	struct MerkleNode *rightChild;
	char hash[257];
} MerkleNode;

typedef struct {
	MerkleNode *root;
} MerkleTree;

Block createBlock();
void saveBlock(Block);
Block loadBlock(char*);
bool broadcastBlock(Block, char*, char**, int);
bool validateBlock(Block block);
bool recieveBlock(Block*, int fd);
void reaper(int);