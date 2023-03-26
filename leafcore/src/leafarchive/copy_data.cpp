/**
 * @file		leafarchive/copy_data.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafArchive::copy_data()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"
#include "globals.h"

#include <archive.h>
#include "leafarchive.h"

int LeafArchive::copy_data(struct archive *ar, struct archive *aw){
	FUN();

	LEAF_DEBUG_EX("LeafArchive::copy_data()");

	int r;
	const void *buff;
	size_t size;
	la_int64_t offset;

	for (;;) {
		if (!proceed)
			throw new LeafError(Error::ABORT);

		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r < ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r < ARCHIVE_OK) {
			throw new LeafError(Error::ARCH_COPY);
		}
	}
}
