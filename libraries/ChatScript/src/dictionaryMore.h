



#define MAX_DICTIONARY	 0x000fffff  //   1M word vocabulary limit (doubling this FAILS on amazon server)

#define MAX_HASH_BUCKETS 0x10000 
#define MAX_ENTRIES      0x000fffff 

#define NODEBITS 0x00ffffff
#define GETNEXTNODE(D) (D->nextNode & NODEBITS)		// top byte is the length of joined phrases of which this is header
#define MULTIHEADERBITS 0xFF000000
#define GETMULTIWORDHEADER(D)  (D->nextNode >> 24)
#define SETMULTIWORDHEADER(D,n) (  D->nextNode &= NODEBITS, D->nextNode |= n << 24 )

#define HASH_EXTRA		2			// +1 for being 1-based and +1 for having uppercase bump

///   DEFINITION OF A MEANING 
typedef unsigned int MEANING; //   a flagged indexed dict ptr
#define TYPE_RESTRICTION	BASIC_POS
//		TYPE_RESTRICTION	0xf0000000
#define SYNSET_MARKER		0x08000000  // this meaning is a synset head
#define INDEX_BITS          0x03F00000  //   7 bits of ontology indexing ability  63 possible meanings allowed
#define MEANING_BASE		0x000fffff	//   the index of the dictionary item
#define INDEX_OFFSET        20          //   shift for ontoindex  (rang 0..63)  
#define MAX_MEANING			63			// limit

#define STDMEANING ( INDEX_BITS | MEANING_BASE | TYPE_RESTRICTION ) // no synset marker
#define SIMPLEMEANING ( INDEX_BITS | MEANING_BASE ) // simple meaning, no type


//   values of TokenMarkers (seen processing input) BAD some unused now
#define QUESTIONMARK 1         
#define EXCLAMATIONMARK 2     
#define SUPPOSEQUESTIONBIT 4   
#define PAST 8
#define FUTURE 16
#define INSULTBIT 32
#define NOTFLAG 64
#define USERINPUT 256
#define PRESENT 512

//   codes for BurstWord argument
#define SIMPLE 0
#define STDBURST 0		// normal burst behavior
#define POSSESSIVES 1
#define CONTRACTIONS 2
#define HYPHENS 4
#define COMPILEDBURST 8  // prepare argument as though it were output script		
#define NOBURST 16		// dont burst (like for a quoted text string)

//   values for FindWord lookup
#define PRIMARY_CASE_ALLOWED 1
#define SECONDARY_CASE_ALLOWED 2
#define STANDARD_LOOKUP (PRIMARY_CASE_ALLOWED |  SECONDARY_CASE_ALLOWED )
#define LOWERCASE_LOOKUP 4
#define UPPERCASE_LOOKUP 8



// control over topics
#define TOPIC_NOERASE 1     //   do not erase as you go
#define TOPIC_REPEAT 2      //   allow repeat output

