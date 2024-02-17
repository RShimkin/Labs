using namespace std;

typedef int Tinfo;

struct NODE{
	Tinfo info;
	NODE* next;
};

typedef NODE* Tlist;

void Init(Tlist&);

bool Empty(Tlist);

void AddToHead(Tlist&, int);

int Count(Tlist list);

void AddToTail(Tlist&, int);

void View(Tlist);

void DeleteAll(Tlist&);

int LongestIncSequence(Tlist);