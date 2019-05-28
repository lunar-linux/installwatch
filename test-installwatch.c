/* $Id: test-installwatch.c,v 0.6.3.2 2001/12/14 00:06:05 izto Exp $ */
/*
 * Copyright (C) 1998-99 Pancrazio `Ezio' de Mauro <p@demauro.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * July-15-2018 modifications by Stefan Wold <ratler@lunar-linux.org>
 *   - fixed refcount in test_chmod
 *   - added a few tests for GLIBC_MINOR >= 4
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>

#include <config.h>

#ifndef	LIBDIR 
	#define LIBDIR "/usr/local/lib"
#endif

#define TESTFILE "/tmp/installwatch-test"
#define TESTFILE2 TESTFILE "2"

int *refcount;
int *timecount;
int passed, failed;
void* libc_handle=NULL;

void check_installwatch(void) {
	char *error;

	time(NULL);

	libc_handle=dlopen(LIBDIR"/installwatch.so",RTLD_LAZY);
	if(!libc_handle) {
		puts("Unable to open "LIBDIR"/installwatch.so");
		exit(255);
	}

	time(NULL);

	timecount=(int*)dlsym(libc_handle,"__installwatch_timecount");	
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		exit(255);
	}

	if((*timecount)<2) {
		puts("This program must be run with installwatch");
		dlclose(libc_handle);
		exit(255);
	}

	refcount=(int*)dlsym(libc_handle,"__installwatch_refcount");	
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		exit(255);
	}
}

void test_chmod(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	close(fd);
	chmod(TESTFILE, 0600);
	unlink(TESTFILE);
}

void test_chown(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	close(fd);
	chown(TESTFILE, geteuid(), getegid());
	unlink(TESTFILE);
}

void test_chroot(void) {
	chroot("/");
}

void test_creat(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	close(fd);
	unlink(TESTFILE);
}

void test_fchmod(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	fchmod(fd, 0600);
	close(fd);
	unlink(TESTFILE);
}

void test_fchown(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	fchown(fd, geteuid(), getegid());
	close(fd);
	unlink(TESTFILE);
}

void test_fopen(void) {
        FILE *fd;

        fd = fopen(TESTFILE,"w");
        fclose(fd);
        unlink(TESTFILE);
}

void test_ftruncate(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	ftruncate(fd, 0);
	close(fd);
	unlink(TESTFILE);
}

void test_lchown(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	close(fd);
	lchown(TESTFILE, geteuid(), getegid());
	unlink(TESTFILE);
}

void test_link(void) {
	int fd;

	fd = creat(TESTFILE, 0600);
	close(fd);
	link(TESTFILE, TESTFILE2);
	unlink(TESTFILE);
	unlink(TESTFILE2);
}

void test_mkdir(void) {
	mkdir(TESTFILE, 0700);
	rmdir(TESTFILE);
}

void test_open(void) {
	int fd;

	fd = open(TESTFILE, O_CREAT, O_RDWR, 0700);
	close(fd);
	unlink(TESTFILE);
}

void test_rename(void) {
	int fd;

	fd = creat(TESTFILE, 0700);
	close(fd);
	rename(TESTFILE, TESTFILE2);
	unlink(TESTFILE2);
}

void test_symlink(void) {
	int fd;

	fd = creat(TESTFILE, 0700);
	close(fd);
	symlink(TESTFILE, TESTFILE2);
	unlink(TESTFILE);
	unlink(TESTFILE2);
}

void test_truncate(void) {
	int fd;

	fd = creat(TESTFILE, 0700);
	close(fd);
	truncate(TESTFILE, 0);
	unlink(TESTFILE);
}

void test_unlink(void) {
	int fd;

	fd = creat(TESTFILE, 0700);
	close(fd);
	unlink(TESTFILE);
}

#if(GLIBC_MINOR >= 1)

void test_creat64(void) {
	int fd;

	fd = creat64(TESTFILE, 0600);
	close(fd);
	unlink(TESTFILE);
}

void test_fopen64(void) {
        FILE *fd;

        fd = fopen64(TESTFILE,"w");
        fclose(fd);
        unlink(TESTFILE);
}

void test_ftruncate64(void) {
	int fd;

	fd = creat64(TESTFILE, 0600);
	ftruncate64(fd, 0);
	close(fd);
	unlink(TESTFILE);
}

void test_open64(void) {
	int fd;

	fd = open64(TESTFILE, O_CREAT, O_RDWR, 0700);
	close(fd);
	unlink(TESTFILE);
}

void test_truncate64(void) {
	int fd;

	fd = creat64(TESTFILE, 0700);
	close(fd);
	truncate64(TESTFILE, 0);
	unlink(TESTFILE);
}

#endif

#if(GLIBC_MINOR >= 4)
void test_renameat(void) {
  int fd;
  FILE *fd_read;
	fd = creat(TESTFILE, 0700);
	close(fd);
	renameat(AT_FDCWD, TESTFILE, AT_FDCWD, TESTFILE2);
	fd_read = fopen(TESTFILE2, "r");
  close(fd_read);
  if (fd_read != NULL)
    unlinkat(AT_FDCWD, TESTFILE2, 0);
}

void test_linkat(void) {
  int fd;
  fd = creat(TESTFILE, 0700);
  close(fd);

  if (linkat(AT_FDCWD, TESTFILE, AT_FDCWD, TESTFILE2, 0) == 0) {
    unlink(TESTFILE);
    unlink(TESTFILE2);
  }
}

void test_symlinkat(void) {
  int fd;
  fd = creat(TESTFILE, 0700);
  close(fd);

  if(symlinkat(TESTFILE, AT_FDCWD, TESTFILE2) == 0) {
    unlink(TESTFILE);
    unlink(TESTFILE2);
  }
}

void test_mkdirat(void) {
  mkdirat(AT_FDCWD, TESTFILE, 0700);
  rmdir(TESTFILE);
}

void test_unlinkat(void) {
  int fd;
  fd = creat(TESTFILE, 0700);
  close(fd);
  unlinkat(AT_FDCWD, TESTFILE, 0);
}
#endif

#ifdef HAVE_UTIMENSAT
void test_utimensat(void) {
  int fd;
  clockid_t clk_id = CLOCK_REALTIME;
  struct timespec *ts;
  fd = creat(TESTFILE, 0700);
  close(fd);
  clock_gettime(clk_id, &ts);
  utimensat(0, TESTFILE, ts, AT_SYMLINK_NOFOLLOW);
  unlink(TESTFILE);
}
#endif

#ifdef HAVE_RENAMEAT2
void test_renameat2(void) {
	int fd;
	FILE *fd_read;
	fd = creat(TESTFILE, 0700);
	close(fd);
	renameat2(AT_FDCWD, TESTFILE, AT_FDCWD, TESTFILE2, 0);
	fd_read = fopen(TESTFILE2, "r");
	close(fd_read);
	if (fd_read != NULL)
		unlinkat(AT_FDCWD, TESTFILE2, 0);
}
#endif

int do_test(const char *name, void (*function)(void), int increment) {
	int old_refcount;
	
	printf("Testing %s... ", name);
	old_refcount = *refcount;
	function();
	if(*refcount == old_refcount + increment) {
		printf("wanted refcount=%d returned refcount=%d",
			(old_refcount+increment),*refcount);
		puts("passed");
		passed++;
		return 0;
	} else {
		printf("wanted refcount=%d returned refcount=%d",
			(old_refcount+increment),*refcount);
	        puts("failed");
		failed++;
		return 1;
	}
}

int main(int argc, char **argv) {
	struct stat statbuf;

	check_installwatch();

	if(stat(TESTFILE, &statbuf) != -1) {
		printf(TESTFILE " already exists. Please remove it and run %s again\n", argv[0]);
		exit(254);
	}
	if(stat(TESTFILE2, &statbuf) != -1) {
		printf(TESTFILE2 " already exists. Please remove it and run %s again\n", argv[0]);
		exit(254);
	}
	puts("Testing installwatch " VERSION);
	puts("Using " TESTFILE " and " TESTFILE2 " as a test files\n");
	passed = failed = 0;
	do_test("chmod", test_chmod, 3);
	do_test("chown", test_chown, 3);
	do_test("chroot", test_chroot, 1);
	do_test("creat", test_creat, 2);
#if(GLIBC_MINOR >= 1)
	do_test("creat64", test_creat64, 2);
#endif
	do_test("fchmod", test_fchmod, 3);
	do_test("fchown", test_fchown, 3);
	do_test("fopen",test_fopen,2);
#if(GLIBC_MINOR >= 1)
	do_test("fopen64",test_fopen64,2);
#endif	
	do_test("ftruncate", test_ftruncate, 3);
#if(GLIBC_MINOR >= 1)
	do_test("ftruncate64", test_ftruncate64, 3);
#endif
	do_test("lchown", test_lchown, 3);
	do_test("link", test_link, 4);
	do_test("mkdir", test_mkdir, 2);
	/* do_test("mknod", test_mknod, 2); */
	do_test("open", test_open, 2);
#if(GLIBC_MINOR >= 1)
	do_test("open64", test_open64, 2);
#endif
	do_test("rename", test_rename, 3);
	do_test("rmdir", test_mkdir, 2);
	do_test("symlink", test_symlink, 4);
	do_test("truncate", test_truncate, 3);
#if(GLIBC_MINOR >= 1)
	do_test("truncate64", test_truncate64, 3);
#endif
	do_test("unlink", test_unlink, 2);
#if(GLIBC_MINOR >= 4)
  do_test("renameat", test_renameat, 4);
  do_test("linkat", test_linkat, 4);
  do_test("symlinkat", test_symlinkat, 4);
  do_test("mkdirat", test_mkdirat, 2);
  do_test("unlinkat", test_unlinkat, 2);
#endif
#ifdef HAVE_UTIMENSAT
	do_test("utimensat", test_utimensat, 2);
#endif
#ifdef HAVE_RENAMEAT2
	do_test("renameat2", test_renameat2, 4);
#endif

	putchar('\n');
	if(failed != 0) {
		printf("%d tests were not successful!\n", failed);
		printf("Please email this log to the maintainer with the output of\n");
		printf("\tnm %s\n", argv[0]);
	} else
		printf("All tests successful!\n");

	if(libc_handle!=NULL)
		dlclose(libc_handle);

	return failed;
}

