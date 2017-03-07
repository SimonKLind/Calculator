#ifndef HASHES_H
#define HASHES_H

#include <string>

namespace Hashes{
	int hash(std::string s){
		int val = 7;
		for(int i=0; i<s.length(); ++i) val = val*31+s[i];
		return val;
	}
}

#endif