#ifndef __LEAFARCHIVE_H__
#define __LEAFARCHIVE_H__

#include <string>

class LeafArchive{

public:

	/**
	 * @brief	Loads the archive at the supplied path
	 * @param	path	The path to the archive
	 */
	void				load(std::string path);

	/**
	 * @brief	Extracts the archive into the specified destination
	 * 			Changes the current working path temporarily to the supplied path
	 * @param	dest	The destination directory to put the resulting files in
	 */
	void				extract(std::string dest);

private:
	int					_flags;

	void*				_archive = nullptr;
	void*				_ext = nullptr;

	int					copy_data(struct archive *ar, struct archive *aw);
};

#endif