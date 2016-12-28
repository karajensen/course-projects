#ifndef PATH_H
#define PATH_H

#include "ShroomBot.h"

class PATH;
enum {OUTERSPIRAL=0,ZIGZAG,CLEANUP};
typedef void(PATH::*pmf)(const PERCEPT &p);
struct PMF { pmf function; };

class PATH
{
	public:
		PMF path;
		void OuterSpiral(const PERCEPT &p);
		void ZigZag(const PERCEPT &p);
		void CleanUp(const PERCEPT &p);
		void Path(const PERCEPT &p){(this->*path.function)(p);}
		void ChangePath(int newPath = CLEANUP)
		{
			switch(newPath)
			{
				case 0:{path.function = &PATH::OuterSpiral; break;}
				case 1:{path.function = &PATH::ZigZag; break;}
				case 2:{path.function = &PATH::CleanUp; break;}
				default:{path.function = &PATH::CleanUp; break;}
			}	
		}
};

#endif