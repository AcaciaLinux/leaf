/**
 * @file		leafarchive/load.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafArchive::load()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafarchive.h"

#include <archive.h>

void LeafArchive::load(std::string path){
	FUN();

	LEAF_DEBUG_EX("LeafArchive::load()");

	int r;
	LOGI("Loading archive " + path);

	if (_archive != nullptr || _ext != nullptr)
		throw new LeafError(Error::ARCH_ALREADYOPEN);

	_flags = ARCHIVE_EXTRACT_TIME;
	_flags |= ARCHIVE_EXTRACT_PERM;
	_flags |= ARCHIVE_EXTRACT_ACL;
	_flags |= ARCHIVE_EXTRACT_FFLAGS;

	_archive = archive_read_new();
	struct archive* arch = (struct archive*)_archive;
	
	archive_read_support_format_all(arch);
	archive_read_support_filter_all(arch);

	_ext = archive_write_disk_new();
	struct archive* ext = (struct archive*)_ext;

	archive_write_disk_set_options(ext, _flags);
	archive_write_disk_set_standard_lookup(ext);

	if ((r = archive_read_open_filename(arch, path.c_str(), 10240)))
		throw new LeafError(Error::OPENFILER, path);
}
