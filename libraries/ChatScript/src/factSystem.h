#ifndef _FACTSYSTEMH_
#define _FACTSYSTEMH_
#ifdef INFORMATION
Copyright (C) 2011 by Bruce Wilcox

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endif

#ifdef SMALL
#define MAX_FACT_NODES 450000 
#elif WIN32
#define MAX_FACT_NODES 800000 
#else
#define MAX_FACT_NODES 800000 
#endif

typedef struct FACT //   a statment in MEANING 
{  
	//   xref lists of facts in appropriate field position - FIELDS 
	unsigned int fproperties;		

 	FACTOID subjectHead;	
	FACTOID verbHead;		
	FACTOID objectHead;		

    FACTOID subjectNext;	
 	FACTOID verbNext;		
    FACTOID objectNext;  	

	MEANING subject;		
	MEANING verb;			
    MEANING object;	
} FACT;

// we only use some of the bottom 32 bits of the properties.... users can use the top bits and the contiguous bottom bits

extern FACT* factBase;		//   start of fact space
extern FACT* factEnd;	//   end of fact space
extern FACT* currentFact;		//   most recent fact found or created
extern FACT* dictionaryFacts;	//   end of wordnet facts (before topic facts)
extern FACT* topicFacts;		//   end of topic facts (start of chatbot facts)
extern FACT* build0Facts;		//   end of build0 facts (start of build1 facts)
extern FACT* chatbotFacts;	//   end of chatbot facts (before USER)
extern FACT* factFree;	//   end of facts - most recent fact allocated (ready for next allocation)



extern MEANING Mmember;
extern MEANING Mis;
extern unsigned int maxFacts;

unsigned int Fact2Index(FACT* F);
inline FACT* Index2Fact(unsigned int e) { return (e) ? ( e + factBase) : 0;}
inline int currentFactIndex() { return (currentFact) ? (int)((currentFact - factBase)) : 0;}
bool ExportFacts(char* name, int set);
bool ImportFacts(char* name, char* store, char* erase, char* transient);
void InitFacts();
void CloseFacts();
void InitFactWords();
void ResetFactSystem();

FACT* FindFact(MEANING subject, MEANING verb, MEANING object, unsigned int properties = 0);
FACT* CreateFact(MEANING subject,MEANING verb, MEANING object,unsigned int properties = 0);
void FreeFact(FACT* F);
char* ReadField(char* ptr,char* field);

void SetSubjectHead(MEANING, FACT* value);
void SetObjectHead(MEANING, FACT* value);
unsigned int GetSetID(char* x);
void ReadFacts(const char* name,uint64 zone,bool user = false);
FACT* ReadFact(char* &ptr);
char* WriteFact(FACT* F,bool comments,char* buffer,bool ignoreDead = false);
void TraceFact(FACT* F,bool ignoreDead = false);
void WriteFacts(FILE* out,FACT* from);
char* EatFact(char* ptr,unsigned int flags = 0);
void VerifyDictionaryFact(WORDP D,void* junk);

//   Facts have threaded lists (both head and next)
//   Words have just the start of fact threaded lists (no next)

inline FACT* GetSubjectHead(FACT* F) {return Index2Fact(F->subjectHead);}
inline FACT* GetVerbHead(FACT* F) {return Index2Fact(F->verbHead);}
inline FACT* GetObjectHead(FACT* F) {return Index2Fact(F->objectHead);}
bool IsMember(WORDP who,WORDP parent);

FACT* GetFactRefDecode(char* word);
void KillFact(FACT* F);
char* GetSetType(char* x);
inline FACT* GetSubjectNext(FACT* F) { return Index2Fact(F->subjectNext);}
inline FACT* GetVerbNext(FACT* F) {return Index2Fact(F->verbNext);}
inline FACT* GetObjectNext(FACT* F) {return Index2Fact(F->objectNext);}
inline void SetSubjectNext(FACT* F, FACT* value){ F->subjectNext = Fact2Index(value);}
inline void SetVerbNext(FACT* F, FACT* value) {F->verbNext = Fact2Index(value);}
inline void SetObjectNext(FACT* F, FACT* value){ F->objectNext = Fact2Index(value);}

inline void SetSubjectHead(FACT* F, FACT* value){ F->subjectHead = Fact2Index(value);}
inline void SetVerbHead(FACT* F, FACT* value) {F->verbHead = Fact2Index(value);}
inline void SetObjectHead(FACT* F, FACT* value){ F->objectHead = Fact2Index(value);}

inline FACT* GetSubjectHead(WORDP D) {return Index2Fact(D->subjectHead);}
inline FACT* GetVerbHead(WORDP D) {return Index2Fact(D->verbHead);}
inline FACT* GetObjectHead(WORDP D)  {return Index2Fact(D->objectHead);}
inline void SetSubjectHead(WORDP D, FACT* value) {D->subjectHead = Fact2Index(value);}
inline void SetVerbHead(WORDP D, FACT* value) {D->verbHead = Fact2Index(value);}
inline void SetObjectHead(WORDP D, FACT* value) {D->objectHead = Fact2Index(value);}

inline FACT* GetSubjectHead(MEANING M) {return GetSubjectHead(Meaning2Word(M));}
inline FACT* GetVerbHead(MEANING M) {return GetVerbHead(Meaning2Word(M));}
inline FACT* GetObjectHead(MEANING M)  {return GetObjectHead(Meaning2Word(M));}
inline void SetSubjectHead(MEANING M, FACT* value) {SetSubjectHead(Meaning2Word(M),value);}
inline void SetVerbHead(MEANING M, FACT* value) {SetVerbHead(Meaning2Word(M),value);}
inline void SetObjectHead(MEANING M, FACT* value) {SetObjectHead(Meaning2Word(M),value);}

#endif
