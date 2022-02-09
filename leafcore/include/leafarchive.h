#ifndef __LEAFARCHIVE_H__
#define __LEAFARCHIVE_H__

#include <string>

class LeafArchive{

public:
	bool				load(std::string path);
	bool				extract(std::string destination);

	std::string			getError();

private:
	int					_flags;
	std::string			_error;

	void*				_archive = nullptr;
	void*				_ext = nullptr;

	int					copy_data(struct archive *ar, struct archive *aw);
};

#endif