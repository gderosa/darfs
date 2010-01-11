/*
 * DARFS: browse files inside a DAR archive (http://dar.linux.free.fr/)
 * Copyright (C) 2010 Guido De Rosa <guido.derosa@vemarsas.it>
 * 
 * based on: FUSE: Filesystem in Userspace  
 * Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
 * 
 * This program can be distributed under the terms of the GNU GPL.
 */


#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <fuse.h>
#include <fuse_opt.h>

const char * cplusplus_hello_str();
int darfs_exists(const char * path);
mode_t darfs_mode(const char * path);
off_t darfs_size(const char * path);

static char * archive = NULL;

static int darfs_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs);

#define hello_str cplusplus_hello_str()

static const char *hello_path = "/hello";

/** options for fuse_opt.h */
/* struct options {
   int readonly;
}options;
*/

/** macro to define options */
/* #define HELLOFS_OPT_KEY(t, p, v) { t, offsetof(struct options, p), v } */


static int hello_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else if (darfs_exists(path)) {
		stbuf->st_mode = darfs_mode(path);
		stbuf->st_nlink = 1; /* TODO: darfs_nlink()? */
		stbuf->st_size = darfs_size(path); 
	} else
		res = -ENOENT;

	return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, hello_path + 1, NULL, 0);

	return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, hello_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, hello_path) != 0)
		return -ENOENT;

	len = strlen(hello_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, hello_str + offset, size);
	} else
		size = 0;

	return size;
}

static struct fuse_operations hello_oper = {
	.getattr	= hello_getattr,
	.readdir	= hello_readdir,
	.open		= hello_open,
	.read		= hello_read,
};

static int darfs_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs)
{
     if (key == FUSE_OPT_KEY_NONOPT && archive == NULL) {
             archive = strdup(arg);
             return 0;
     }
     return 1;
}

int main(int argc, char *argv[])
{
	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	
	fuse_opt_parse(&args, NULL, NULL, darfs_opt_proc);

	ret = fuse_main(args.argc, args.argv, &hello_oper, NULL);
	
	/** free arguments */
  fuse_opt_free_args(&args);
  
	return ret;
}
