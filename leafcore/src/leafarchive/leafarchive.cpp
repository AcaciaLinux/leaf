/**
 * @file		leafarchive/leafarchive.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafArchive
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafarchive.h"

#include <filesystem>

#include <archive.h>
#include <archive_entry.h>

int LeafArchive::copy_data(struct archive *ar, struct archive *aw){
	FUN();

	int r;
	const void *buff;
	size_t size;
	la_int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r < ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r < ARCHIVE_OK) {
			_error = "Copy data error: " + std::string(archive_error_string(aw));
			LOGE(_error);
			return (r);
		}
	}
}

bool LeafArchive::load(std::string path){
	FUN();
	int r;
	LOGI("Loading archive " + path);

	if (_archive != nullptr || _ext != nullptr){
		_error = "An other archive seems to be open";
		LOGE("Failed to load archive: " + _error);
		return false;
	}

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

	if ((r = archive_read_open_filename(arch, path.c_str(), 10240))){
		_error = "Failed to open archive " + path;
		return false;
	}
		
	return true;
}

bool LeafArchive::extract(std::string destination){
	FUN();
	_error = "";

	if (!std::filesystem::exists(destination)){
		_error = destination + " does not exist";
		LOGE(_error);
		return false;
	}

	if (!std::filesystem::is_directory(destination)){
		_error = destination + " is not a directory";
		LOGE(_error);
		return false;
	}

	std::string prevDir = std::filesystem::current_path();

	std::filesystem::current_path(destination);

	if (_archive == nullptr || _ext == nullptr){
		_error = "The archive does not seem to be opened, load the archive first using LeafArchive::load()";
		LOGE("Failed to load archive: " + _error);
		return false;
	}

	bool res = true;
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
				_error = std::string(archive_error_string(arch));
				LOGE("Exctraction error: " + _error);
			}if (r < ARCHIVE_WARN){
				std::filesystem::current_path(prevDir);
				return false;
			}

			LOGI("Processing " + std::string(archive_entry_pathname(entry)));

			r = archive_write_header(ext, entry);
			if (r < ARCHIVE_OK){
				if (!_error.empty())
					_error += "\n";
				_error = std::string(archive_error_string(ext));
				LOGE("Exctraction error: " + _error);
			}else if (archive_entry_size(entry) > 0) {
				r = copy_data(arch, ext);
				if (r < ARCHIVE_OK){
					if (!_error.empty())
						_error += "\n";
					_error = std::string(archive_error_string(ext));
					LOGE("Exctraction error: " + _error);
				}if (r < ARCHIVE_WARN){
					std::filesystem::current_path(prevDir);
					return false;
				}
			}
			r = archive_write_finish_entry(ext);
			if (r < ARCHIVE_OK){
				if (!_error.empty())
					_error += "\n";
				_error = std::string(archive_error_string(ext));
				LOGE("Exctraction error: " + _error);
			}if (r < ARCHIVE_WARN){
				std::filesystem::current_path(prevDir);
				return false;
			}
		}
	} catch (...){
		_error = "libcurl encountered a fatal error";
		LOGE("Failed to extract archive: " + _error);
		res = false;
	}

	archive_read_close(arch);
	archive_read_free(arch);
	archive_write_close(ext);
	archive_write_free(ext);

	std::filesystem::current_path(prevDir);

	return res;
}
