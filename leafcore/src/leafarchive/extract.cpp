/**
 * @file		leafarchive/extract.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafArchive::extract()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafarchive.h"

#include <archive.h>
#include <archive_entry.h>
#include <filesystem>

void LeafArchive::extract(std::string destination){
	FUN();

	LEAF_DEBUG_EX("LeafArchive::extract()");

	//Check if the archive has been loaded
	if (_archive == nullptr || _ext == nullptr)
		throw new LeafError(Error::ARCH_NOTLOADED);

	{	//Check if the destination directory exists
		std::error_code ec;
		bool exists = std::filesystem::exists(destination, ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, "In checking destination dir " + destination, ec);

		if (!exists)
			throw new LeafError(Error::OPENDIR, "Destination dir " + destination + " does not exist");
	}

	if (!std::filesystem::is_directory(destination))
		throw new LeafError(Error::NOTDIR, "Destination dir " + destination);

	std::string prevDir = std::filesystem::current_path();

	{
		std::error_code ec;
		std::filesystem::current_path(destination, ec);

		if (ec)
			throw new LeafError(Error::CHDIR, "Destination path " + destination, ec);
	}

	int r;
	struct archive_entry *entry;
	struct archive* ext = (struct archive*)_ext;
	struct archive* arch = (struct archive*)_archive;

	try {
		for (;;) {
			r = archive_read_next_header(arch, &entry);
			if (r == ARCHIVE_EOF)
				break;
			if (r < ARCHIVE_OK){
				throw new LeafError(Error::ARCH_ERR, archive_error_string(arch));
			}if (r < ARCHIVE_WARN){
				std::filesystem::current_path(prevDir);
				throw new LeafError(Error::ARCH_WARN, archive_error_string(arch));
			}

			LOGF("Processing " + std::string(archive_entry_pathname(entry)));

			r = archive_write_header(ext, entry);
			if (r < ARCHIVE_OK){
				throw new LeafError(Error::ARCH_ERR, archive_error_string(ext));
			}else if (archive_entry_size(entry) > 0) {
				r = copy_data(arch, ext);
				if (r < ARCHIVE_OK){
					throw new LeafError(Error::ARCH_ERR, archive_error_string(ext));
				}if (r < ARCHIVE_WARN){
					std::filesystem::current_path(prevDir);
					throw new LeafError(Error::ARCH_WARN, archive_error_string(ext));
				}
			}
			r = archive_write_finish_entry(ext);
			if (r < ARCHIVE_OK){
				throw new LeafError(Error::ARCH_ERR, archive_error_string(ext));
			}if (r < ARCHIVE_WARN){
				std::filesystem::current_path(prevDir);
				throw new LeafError(Error::ARCH_WARN, archive_error_string(ext));
			}
		}
	} catch (LeafError* e) {
		throw e;
	} catch (...){
		throw new LeafError(Error::ARCH_FATAL);
	}

	archive_read_close(arch);
	archive_read_free(arch);
	archive_write_close(ext);
	archive_write_free(ext);

	std::filesystem::current_path(prevDir);
}
