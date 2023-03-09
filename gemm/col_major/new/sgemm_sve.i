# 0 "sgemm_sve.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "sgemm_sve.c"
# 1 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stdint.h" 1 3
# 9 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stdint.h" 3
# 1 "/usr/local/gcc-11.1.0/lib/gcc/aarch64-unknown-linux-gnu/11.1.0/include/stdint.h" 1 3 4
# 9 "/usr/local/gcc-11.1.0/lib/gcc/aarch64-unknown-linux-gnu/11.1.0/include/stdint.h" 3 4
# 1 "/usr/include/stdint.h" 1 3 4
# 26 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 33 "/usr/include/aarch64-linux-gnu/bits/libc-header-start.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 446 "/usr/include/features.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/sys/cdefs.h" 1 3 4
# 452 "/usr/include/aarch64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/wordsize.h" 1 3 4
# 453 "/usr/include/aarch64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/long-double.h" 1 3 4
# 454 "/usr/include/aarch64-linux-gnu/sys/cdefs.h" 2 3 4
# 447 "/usr/include/features.h" 2 3 4
# 470 "/usr/include/features.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/gnu/stubs.h" 1 3 4




# 1 "/usr/include/aarch64-linux-gnu/bits/wordsize.h" 1 3 4
# 6 "/usr/include/aarch64-linux-gnu/gnu/stubs.h" 2 3 4


# 1 "/usr/include/aarch64-linux-gnu/gnu/stubs-lp64.h" 1 3 4
# 9 "/usr/include/aarch64-linux-gnu/gnu/stubs.h" 2 3 4
# 471 "/usr/include/features.h" 2 3 4
# 34 "/usr/include/aarch64-linux-gnu/bits/libc-header-start.h" 2 3 4
# 27 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/aarch64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/aarch64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/aarch64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/aarch64-linux-gnu/bits/timesize.h" 2 3 4
# 29 "/usr/include/aarch64-linux-gnu/bits/types.h" 2 3 4



# 31 "/usr/include/aarch64-linux-gnu/bits/types.h" 3 4
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/include/aarch64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/typesizes.h" 1 3 4
# 142 "/usr/include/aarch64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/time64.h" 1 3 4
# 143 "/usr/include/aarch64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 28 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/wchar.h" 1 3 4
# 29 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/wordsize.h" 1 3 4
# 30 "/usr/include/stdint.h" 2 3 4




# 1 "/usr/include/aarch64-linux-gnu/bits/stdint-intn.h" 1 3 4
# 24 "/usr/include/aarch64-linux-gnu/bits/stdint-intn.h" 3 4
typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
# 35 "/usr/include/stdint.h" 2 3 4


# 1 "/usr/include/aarch64-linux-gnu/bits/stdint-uintn.h" 1 3 4
# 24 "/usr/include/aarch64-linux-gnu/bits/stdint-uintn.h" 3 4
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
# 38 "/usr/include/stdint.h" 2 3 4





typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;


typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;





typedef signed char int_fast8_t;

typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
# 71 "/usr/include/stdint.h" 3 4
typedef unsigned char uint_fast8_t;

typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
# 87 "/usr/include/stdint.h" 3 4
typedef long int intptr_t;


typedef unsigned long int uintptr_t;
# 101 "/usr/include/stdint.h" 3 4
typedef __intmax_t intmax_t;
typedef __uintmax_t uintmax_t;
# 10 "/usr/local/gcc-11.1.0/lib/gcc/aarch64-unknown-linux-gnu/11.1.0/include/stdint.h" 2 3 4
# 10 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stdint.h" 2 3
# 2 "sgemm_sve.c" 2
# 1 "/usr/include/stdio.h" 1 3 4
# 27 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 28 "/usr/include/stdio.h" 2 3 4





# 1 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stddef.h" 1 3 4
# 209 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 34 "/usr/include/stdio.h" 2 3 4


# 1 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stdarg.h" 1 3 4
# 40 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 37 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/include/aarch64-linux-gnu/bits/types/__fpos_t.h" 1 3 4




# 1 "/usr/include/aarch64-linux-gnu/bits/types/__mbstate_t.h" 1 3 4
# 13 "/usr/include/aarch64-linux-gnu/bits/types/__mbstate_t.h" 3 4
typedef struct
{
  int __count;
  union
  {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
# 6 "/usr/include/aarch64-linux-gnu/bits/types/__fpos_t.h" 2 3 4




typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;
# 40 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/types/__fpos64_t.h" 1 3 4
# 10 "/usr/include/aarch64-linux-gnu/bits/types/__fpos64_t.h" 3 4
typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;
# 41 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/types/__FILE.h" 1 3 4



struct _IO_FILE;
typedef struct _IO_FILE __FILE;
# 42 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/types/FILE.h" 1 3 4



struct _IO_FILE;


typedef struct _IO_FILE FILE;
# 43 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/types/struct_FILE.h" 1 3 4
# 35 "/usr/include/aarch64-linux-gnu/bits/types/struct_FILE.h" 3 4
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;




typedef void _IO_lock_t;





struct _IO_FILE
{
  int _flags;


  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;


  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset;


  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;







  __off64_t _offset;

  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};
# 44 "/usr/include/stdio.h" 2 3 4
# 52 "/usr/include/stdio.h" 3 4
typedef __gnuc_va_list va_list;
# 63 "/usr/include/stdio.h" 3 4
typedef __off_t off_t;
# 77 "/usr/include/stdio.h" 3 4
typedef __ssize_t ssize_t;






typedef __fpos_t fpos_t;
# 133 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/aarch64-linux-gnu/bits/stdio_lim.h" 1 3 4
# 134 "/usr/include/stdio.h" 2 3 4



extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;






extern int remove (const char *__filename) __attribute__ ((__nothrow__ , __leaf__));

extern int rename (const char *__old, const char *__new) __attribute__ ((__nothrow__ , __leaf__));



extern int renameat (int __oldfd, const char *__old, int __newfd,
       const char *__new) __attribute__ ((__nothrow__ , __leaf__));
# 173 "/usr/include/stdio.h" 3 4
extern FILE *tmpfile (void) ;
# 187 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;




extern char *tmpnam_r (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;
# 204 "/usr/include/stdio.h" 3 4
extern char *tempnam (const char *__dir, const char *__pfx)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;







extern int fclose (FILE *__stream);




extern int fflush (FILE *__stream);
# 227 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 246 "/usr/include/stdio.h" 3 4
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes) ;




extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) ;
# 279 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, const char *__modes) __attribute__ ((__nothrow__ , __leaf__)) ;
# 292 "/usr/include/stdio.h" 3 4
extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __attribute__ ((__nothrow__ , __leaf__)) ;




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__ , __leaf__)) ;





extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__ , __leaf__));




extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__ , __leaf__));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));







extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...);




extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg);




extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));



extern int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));
# 379 "/usr/include/stdio.h" 3 4
extern int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));







extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...) ;




extern int scanf (const char *__restrict __format, ...) ;

extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...) __attribute__ ((__nothrow__ , __leaf__));






extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf")

                               ;
extern int scanf (const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf")
                              ;
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__ , __leaf__))

                      ;
# 432 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) ;





extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) ;


extern int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__format__ (__scanf__, 2, 0)));




extern int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) ;
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) ;
extern int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__ , __leaf__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 485 "/usr/include/stdio.h" 3 4
extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);





extern int getchar (void);






extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 510 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream);
# 521 "/usr/include/stdio.h" 3 4
extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);





extern int putchar (int __c);
# 537 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream);







extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream);


extern int putw (int __w, FILE *__stream);







extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     ;
# 603 "/usr/include/stdio.h" 3 4
extern __ssize_t __getdelim (char **__restrict __lineptr,
                             size_t *__restrict __n, int __delimiter,
                             FILE *__restrict __stream) ;
extern __ssize_t getdelim (char **__restrict __lineptr,
                           size_t *__restrict __n, int __delimiter,
                           FILE *__restrict __stream) ;







extern __ssize_t getline (char **__restrict __lineptr,
                          size_t *__restrict __n,
                          FILE *__restrict __stream) ;







extern int fputs (const char *__restrict __s, FILE *__restrict __stream);





extern int puts (const char *__s);






extern int ungetc (int __c, FILE *__stream);






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream) ;




extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s);
# 673 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream);







extern int fseek (FILE *__stream, long int __off, int __whence);




extern long int ftell (FILE *__stream) ;




extern void rewind (FILE *__stream);
# 707 "/usr/include/stdio.h" 3 4
extern int fseeko (FILE *__stream, __off_t __off, int __whence);




extern __off_t ftello (FILE *__stream) ;
# 731 "/usr/include/stdio.h" 3 4
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);




extern int fsetpos (FILE *__stream, const fpos_t *__pos);
# 757 "/usr/include/stdio.h" 3 4
extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;



extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;







extern void perror (const char *__s);





# 1 "/usr/include/aarch64-linux-gnu/bits/sys_errlist.h" 1 3 4
# 26 "/usr/include/aarch64-linux-gnu/bits/sys_errlist.h" 3 4
extern int sys_nerr;
extern const char *const sys_errlist[];
# 782 "/usr/include/stdio.h" 2 3 4




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
# 800 "/usr/include/stdio.h" 3 4
extern FILE *popen (const char *__command, const char *__modes) ;





extern int pclose (FILE *__stream);





extern char *ctermid (char *__s) __attribute__ ((__nothrow__ , __leaf__));
# 840 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
# 858 "/usr/include/stdio.h" 3 4
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);
# 873 "/usr/include/stdio.h" 3 4

# 3 "sgemm_sve.c" 2
# 1 "sve_finc_s.h" 1
# 4 "sgemm_sve.c" 2


# 1 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/arm_sve.h" 1 3
# 31 "/home/dev/bin/gcc_sve/lib/gcc/aarch64-linux-gnu/9.0.1/include/arm_sve.h" 3
typedef __fp16 float16_t;
typedef float float32_t;
typedef double float64_t;

#pragma GCC aarch64 "arm_sve.h"
# 7 "sgemm_sve.c" 2



# 9 "sgemm_sve.c"
void sgemm_armv8a_sve_asm_16x8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                               float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                               void* b_next) {






  if (svcntb() != 64) {
    fprintf(
# 19 "sgemm_sve.c" 3 4
           stderr
# 19 "sgemm_sve.c"
                 , "Wrong SVE vector length! You compiled for vector length of 256bits, but your length is %ld.\n",
            svcntb() * 8);
    return;
  }







  uint64_t k_iter = k0 / 4;
  uint64_t k_left = k0 % 4;
  uint64_t rs_c = rs_c0;
  uint64_t cs_c = cs_c0;

  __asm__ volatile(
      "                                            \n\t"
      " ldr x0,%[aaddr]                            \n\t"
      " ldr x1,%[baddr]                            \n\t"
      " ldr x2,%[caddr]                            \n\t"
      "                                            \n\t"
      " ldr w3,%[a_next]                           \n\t"
      " ldr x4,%[b_next]                           \n\t"
      "                                            \n\t"
      " ldr x5,%[k_iter]                           \n\t"
      " ldr x6,%[k_left]                           \n\t"
      "                                            \n\t"



      "                                            \n\t"
      " ldr x10,%[cs_c]                             \n\t"
      " lsl x10,x10,#2                              \n\t"
      "                                            \n\t"
      " ldr w13,%[rs_c]                            \n\t"

      "                                            \n\t"
      " mov w11, #8                                \n\t"
      "                                            \n\t"
      " add x20,x2,x10                             \n\t"
      " add x21,x20,x10                            \n\t"
      " add x22,x21,x10                            \n\t"
      " add x23,x22,x10                            \n\t"
      " add x24,x23,x10                            \n\t"
      " add x25,x24,x10                            \n\t"
      " add x26,x25,x10                            \n\t"
      "                                            \n\t"
      " prfm pldl1keep,[x2]                        \n\t"
      " prfm pldl1keep,[x20]                       \n\t"
      " prfm pldl1keep,[x21]                       \n\t"
      " prfm pldl1keep,[x22]                       \n\t"
      " prfm pldl1keep,[x23]                       \n\t"
      " prfm pldl1keep,[x24]                       \n\t"
      " prfm pldl1keep,[x25]                       \n\t"
      " prfm pldl1keep,[x26]                       \n\t"
      "                                            \n\t"
      " ptrue p0.s                                 \n\t"
      "                                            \n\t"
      " ld1w  z0.s, p0/z, [x0]                     \n\t"
      " ld1w  z1.s, p0/z, [x0, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " dup z12.s, #0                              \n\t"
      " dup z13.s, #0                              \n\t"
      " dup z14.s, #0                              \n\t"
      " dup z15.s, #0                              \n\t"
      " prfm PLDL1KEEP, [x1, #128]                 \n\t"
      " dup z16.s, #0                              \n\t"
      " prfm PLDL1KEEP, [x1, #192]                 \n\t"
      "                                            \n\t"
      " dup z17.s, #0                              \n\t"
      " dup z18.s, #0                              \n\t"
      " dup z19.s, #0                              \n\t"
      "                                            \n\t"
      " prfm PLDL1KEEP, [x0, #256]                 \n\t"
      "                                            \n\t"
      " dup z20.s, #0                              \n\t"
      " prfm PLDL1KEEP, [x0, #320]                 \n\t"
      " dup z21.s, #0                              \n\t"
      " prfm PLDL1KEEP, [x0, #384]                \n\t"
      " dup z22.s, #0                              \n\t"
      "                                            \n\t"
      " dup z23.s, #0                              \n\t"
      " prfm PLDL1KEEP, [x0, #448]                \n\t"
      " dup z24.s, #0                              \n\t"
      " dup z25.s, #0                              \n\t"
      "                                            \n\t"
      " dup z26.s, #0                              \n\t"
      " dup z27.s, #0                              \n\t"
      "                                            \n\t"
      " cmp x5,#0                                  \n\t"
      " beq .S256CONSIDERKLEFT                     \n\t"
      "                                            \n\t"
      " add x0, x0, #64                            \n\t"
      " add x1, x1, #32                            \n\t"
      "                                            \n\t"
      " cmp x5,1                                   \n\t"
      " beq .S256LASTITER                          \n\t"
      "                                            \n\t"
      " S256LOOP:                                  \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " prfm PLDL1KEEP, [x1, #224]                 \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " prfm PLDL1KEEP, [x1, #288]                 \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " prfm PLDL1KEEP, [x0, #448]                \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " prfm PLDL1KEEP, [x0, #512]                \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " prfm PLDL1KEEP, [x0, #576]                \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " prfm PLDL1KEEP, [x0, #640]                \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0]                    \n\t"
      " ld1w   z1.s, p0/z, [x0, #1, MUL VL]        \n\t"

      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #32]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #36]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #40]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #44]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #48]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #52]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #56]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #60]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #2, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #3, MUL VL]        \n\t"


      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #64]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #68]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #72]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #76]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #80]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #84]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #88]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #92]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #4, MUL VL]        \n\t"
      " ld1w   z0.s, p0/z, [x0, #5, MUL VL]        \n\t"


      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #96]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #100]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #104]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #108]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #112]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #116]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #120]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #124]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #6, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #7, MUL VL]        \n\t"


      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " add x0, x0, #256                           \n\t"

      " add x1, x1, #128                           \n\t"

      "                                            \n\t"
      " sub x5,x5,1                                \n\t"
      " cmp x5,1                                   \n\t"
      " bne S256LOOP                               \n\t"
      "                                            \n\t"
      " .S256LASTITER:                             \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0]                    \n\t"
      " ld1w   z1.s, p0/z, [x0, #1, MUL VL]        \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #32]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #36]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #40]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #44]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #48]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #52]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #56]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #60]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #2, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #3, MUL VL]        \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #64]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #68]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #72]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #76]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #80]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #84]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #88]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #92]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #4, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #5, MUL VL]        \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " add x0, x0, #192                           \n\t"

      " add x1, x1, #96                           \n\t"
      "                                            \n\t"
      " .S256CONSIDERKLEFT:                        \n\t"
      " cmp x6,0                                   \n\t"
      " beq .S256POSTACCUM                         \n\t"
      "                                            \n\t"
      ".S256LOOPKLEFT:                             \n\t"
      "                                            \n\t"
      " ld1w  z0.s, p0/z, [x0]                     \n\t"
      " ld1w  z1.s, p0/z, [x0, #1, MUL VL]         \n\t"
      " add x0, x0, #64                            \n\t"
      "                                            \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      " add x1, x1, #32                            \n\t"
      "                                            \n\t"
      " sub x6,x6,1                                \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " cmp x6,0                                   \n\t"
      " bne .S256LOOPKLEFT                         \n\t"
      "                                            \n\t"
      " .S256POSTACCUM:                            \n\t"
      "                                            \n\t"
      " ldr x0,%[alpha]                            \n\t"
      " ldr x1,%[beta]                             \n\t"
      " ld1rw  z8.s, p0/z, [x0]                    \n\t"
      " ld1rw  z9.s, p0/z, [x1]                    \n\t"
      "                                            \n\t"
      " cmp w13,#1                                 \n\t"
      " bne .S256GENSTORED                         \n\t"
      "                                            \n\t"
      " .S256COLSTORED:                            \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROCOLSTOREDS1               \n\t"
      "                                            \n\t"
      " ld1w  {z0.s}, p0/z, [x2]                   \n\t"
      " ld1w  {z1.s}, p0/z, [x2, #1, MUL VL]       \n\t"
      "                                            \n\t"
      " ld1w  {z2.s}, p0/z, [x20]                  \n\t"
      " ld1w  {z3.s}, p0/z, [x20, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z4.s}, p0/z, [x21]                  \n\t"
      " ld1w  {z5.s}, p0/z, [x21, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z6.s}, p0/z, [x22]                  \n\t"
      " ld1w  {z7.s}, p0/z, [x22, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"
      "                                            \n\t"
      " .S256BETAZEROCOLSTOREDS1:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z12.s, z8.s               \n\t"
      " fmla z1.s, p0/m, z13.s, z8.s               \n\t"
      " fmla z2.s, p0/m, z14.s, z8.s               \n\t"
      " fmla z3.s, p0/m, z15.s, z8.s               \n\t"
      " fmla z4.s, p0/m, z16.s, z8.s               \n\t"
      " fmla z5.s, p0/m, z17.s, z8.s               \n\t"
      " fmla z6.s, p0/m, z18.s, z8.s               \n\t"
      " fmla z7.s, p0/m, z19.s, z8.s               \n\t"
      "                                            \n\t"
      " st1w  {z0.s}, p0, [x2]                     \n\t"
      " st1w  {z1.s}, p0, [x2, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " st1w  {z2.s}, p0, [x20]                    \n\t"
      " st1w  {z3.s}, p0, [x20, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z4.s}, p0, [x21]                    \n\t"
      " st1w  {z5.s}, p0, [x21, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z6.s}, p0, [x22]                    \n\t"
      " st1w  {z7.s}, p0, [x22, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROCOLSTOREDS2               \n\t"
      "                                            \n\t"
      " ld1w  {z0.s}, p0/z, [x23]                   \n\t"
      " ld1w  {z1.s}, p0/z, [x23, #1, MUL VL]       \n\t"
      "                                            \n\t"
      " ld1w  {z2.s}, p0/z, [x24]                  \n\t"
      " ld1w  {z3.s}, p0/z, [x24, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z4.s}, p0/z, [x25]                  \n\t"
      " ld1w  {z5.s}, p0/z, [x25, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z6.s}, p0/z, [x26]                  \n\t"
      " ld1w  {z7.s}, p0/z, [x26, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"
      "                                            \n\t"
      " .S256BETAZEROCOLSTOREDS2:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z20.s, z8.s               \n\t"
      " fmla z1.s, p0/m, z21.s, z8.s               \n\t"
      " fmla z2.s, p0/m, z22.s, z8.s               \n\t"
      " fmla z3.s, p0/m, z23.s, z8.s               \n\t"
      " fmla z4.s, p0/m, z24.s, z8.s               \n\t"
      " fmla z5.s, p0/m, z25.s, z8.s               \n\t"
      " fmla z6.s, p0/m, z26.s, z8.s               \n\t"
      " fmla z7.s, p0/m, z27.s, z8.s               \n\t"
      "                                            \n\t"
      " st1w  {z0.s}, p0, [x23]                     \n\t"
      " st1w  {z1.s}, p0, [x23, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " st1w  {z2.s}, p0, [x24]                    \n\t"
      " st1w  {z3.s}, p0, [x24, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z4.s}, p0, [x25]                    \n\t"
      " st1w  {z5.s}, p0, [x25, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z6.s}, p0, [x26]                    \n\t"
      " st1w  {z7.s}, p0, [x26, #1, MUL VL]        \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " b .S256END                                 \n\t"
      "                                            \n\t"
      " .S256GENSTORED:                            \n\t"
      "                                            \n\t"
      " index z10.s, wzr, w13                       \n\t"
      " mul w3, w13, w11                          \n\t"
      " index z11.s, w3, w13                       \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROGENSTOREDS1               \n\t"
      "                                            \n\t"
      " ld1w {z0.s}, p0/z, [x2, z10.s, UXTW #2]     \n\t"
      " ld1w {z1.s}, p0/z, [x2, z11.s, UXTW #2]     \n\t"
      "                                            \n\t"
      " ld1w {z2.s}, p0/z, [x20, z10.s, UXTW #2]    \n\t"
      " ld1w {z3.s}, p0/z, [x20, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z4.s}, p0/z, [x21, z10.s, UXTW #2]    \n\t"
      " ld1w {z5.s}, p0/z, [x21, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z6.s}, p0/z, [x22, z10.s, UXTW #2]    \n\t"
      " ld1w {z7.s}, p0/z, [x22, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"
      "                                            \n\t"
      " .S256BETAZEROGENSTOREDS1:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z12.s, z8.s               \n\t"
      " fmla z1.s, p0/m, z13.s, z8.s               \n\t"
      " fmla z2.s, p0/m, z14.s, z8.s               \n\t"
      " fmla z3.s, p0/m, z15.s, z8.s               \n\t"
      " fmla z4.s, p0/m, z16.s, z8.s               \n\t"
      " fmla z5.s, p0/m, z17.s, z8.s               \n\t"
      " fmla z6.s, p0/m, z18.s, z8.s               \n\t"
      " fmla z7.s, p0/m, z19.s, z8.s               \n\t"
      "                                            \n\t"
      " st1w {z0.s}, p0, [x2, z10.s, UXTW #2]        \n\t"
      " st1w {z1.s}, p0, [x2, z11.s, UXTW #2]        \n\t"
      "                                            \n\t"
      " st1w {z2.s}, p0, [x20, z10.s, UXTW #2]       \n\t"
      " st1w {z3.s}, p0, [x20, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " st1w {z4.s}, p0, [x21, z10.s, UXTW #2]        \n\t"
      " st1w {z5.s}, p0, [x21, z11.s, UXTW #2]        \n\t"
      "                                            \n\t"
      " st1w {z6.s}, p0, [x22, z10.s, UXTW #2]       \n\t"
      " st1w {z7.s}, p0, [x22, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROGENSTOREDS2               \n\t"
      "                                            \n\t"
      " ld1w {z0.s}, p0/z, [x23, z10.s, UXTW #2]     \n\t"
      " ld1w {z1.s}, p0/z, [x23, z11.s, UXTW #2]     \n\t"
      "                                            \n\t"
      " ld1w {z2.s}, p0/z, [x24, z10.s, UXTW #2]    \n\t"
      " ld1w {z3.s}, p0/z, [x24, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z4.s}, p0/z, [x25, z10.s, UXTW #2]    \n\t"
      " ld1w {z5.s}, p0/z, [x25, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z6.s}, p0/z, [x26, z10.s, UXTW #2]    \n\t"
      " ld1w {z7.s}, p0/z, [x26, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"
      "                                            \n\t"
      " .S256BETAZEROGENSTOREDS2:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z20.s, z8.s               \n\t"
      " fmla z1.s, p0/m, z21.s, z8.s               \n\t"
      " fmla z2.s, p0/m, z22.s, z8.s               \n\t"
      " fmla z3.s, p0/m, z23.s, z8.s               \n\t"
      " fmla z4.s, p0/m, z24.s, z8.s               \n\t"
      " fmla z5.s, p0/m, z25.s, z8.s               \n\t"
      " fmla z6.s, p0/m, z26.s, z8.s               \n\t"
      " fmla z7.s, p0/m, z27.s, z8.s               \n\t"
      "                                            \n\t"
      " st1w {z0.s}, p0, [x23, z10.s, UXTW #2]       \n\t"
      " st1w {z1.s}, p0, [x23, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " st1w {z2.s}, p0, [x24, z10.s, UXTW #2]      \n\t"
      " st1w {z3.s}, p0, [x24, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " st1w {z4.s}, p0, [x25, z10.s, UXTW #2]      \n\t"
      " st1w {z5.s}, p0, [x25, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " st1w {z6.s}, p0, [x26, z10.s, UXTW #2]      \n\t"
      " st1w {z7.s}, p0, [x26, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " .S256END:                                  \n\t"
      "                                            \n\t"
      :
      :
      [aaddr] "m"(a),
      [baddr] "m"(b),
      [caddr] "m"(c),
      [k_iter] "m"(k_iter),
      [k_left] "m"(k_left),
      [alpha] "m"(alpha),
      [beta] "m"(beta),
      [rs_c] "m"(rs_c),
      [cs_c] "m"(cs_c),
      [a_next] "m"(a_next),
      [b_next] "m"(b_next)
      :
      "x0", "x1", "x2", "w3", "x4", "x5", "x6", "x10", "w11", "w13", "x20", "x21", "x22", "x23", "x24", "x25", "x26",
      "x27", "x28", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14",
      "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30",
      "z31", "p0");
}



void sgemm_armv8a_sve_asm_2vx8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                                   float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                                   void* b_next) {


  uint64_t k_iter = k0 / 4;

  uint64_t k_left = k0 % 4;




  uint64_t rs_c = rs_c0;
  uint64_t cs_c = cs_c0;

  __asm__ volatile
(
"                                            \n\t"
" ldr x0,%[aaddr]                            \n\t"
" ldr x1,%[baddr]                            \n\t"
" ldr x2,%[caddr]                            \n\t"
"                                            \n\t"
" ldr x3,%[a_next]                           \n\t"
" ldr x4,%[b_next]                           \n\t"
"                                            \n\t"
" ldr x5,%[k_iter]                           \n\t"
" ldr x6,%[k_left]                           \n\t"
"                                            \n\t"
" ldr x7,%[alpha]                            \n\t"
" ldr x8,%[beta]                             \n\t"
"                                            \n\t"
" ldr x9,%[cs_c]                             \n\t"
" lsl x10,x9,#2                              \n\t"
"                                            \n\t"
" ldr x13,%[rs_c]                            \n\t"

"                                            \n\t"
" mov x11,#0                                 \n\t"
" incw x11                                   \n\t"
"                                            \n\t"
" add x20,x2,x10                             \n\t"
" add x21,x20,x10                            \n\t"
" add x22,x21,x10                            \n\t"
" add x23,x22,x10                            \n\t"
" add x24,x23,x10                            \n\t"
" add x25,x24,x10                            \n\t"
" add x26,x25,x10                            \n\t"
"                                            \n\t"
" prfm pldl1keep,[x2]                        \n\t"
" prfm pldl1keep,[x20]                       \n\t"
" prfm pldl1keep,[x21]                       \n\t"
" prfm pldl1keep,[x22]                       \n\t"
" prfm pldl1keep,[x23]                       \n\t"
" prfm pldl1keep,[x24]                       \n\t"
" prfm pldl1keep,[x25]                       \n\t"
" prfm pldl1keep,[x26]                       \n\t"
"                                            \n\t"
" ptrue p0.s                                 \n\t"
"                                            \n\t"
" ld1w  " "z0" ".s, " "p0" "/z, [" "x0" "]           \n\t" " ld1w  " "z1" ".s, " "p0" "/z, [" "x0" ",#1, MUL VL]\n\t"
"                                            \n\t"
" ld1rw  " "z2" ".s, " "p0" "/z"",[" "x1" ", #" "0""]""\n\t" " ld1rw  " "z3" ".s, " "p0" "/z"",[" "x1" ", #" "4""]""\n\t" " ld1rw  " "z4" ".s, " "p0" "/z"",[" "x1" ", #" "8""]""\n\t" " ld1rw  " "z5" ".s, " "p0" "/z"",[" "x1" ", #" "12""]""\n\t" " ld1rw  " "z6" ".s, " "p0" "/z"",[" "x1" ", #" "16""]""\n\t" " ld1rw  " "z7" ".s, " "p0" "/z"",[" "x1" ", #" "20""]""\n\t" " ld1rw  " "z8" ".s, " "p0" "/z"",[" "x1" ", #" "24""]""\n\t" " ld1rw  " "z9" ".s, " "p0" "/z"",[" "x1" ", #" "28""]""\n\t"
"                                            \n\t"
"                                            \n\t"
" dup " "z10" ".s, #0\n\t" " dup " "z11" ".s, #0\n\t" " dup " "z12" ".s, #0\n\t" " dup " "z13" ".s, #0\n\t"
" prfm PLDL1KEEP, [x1, #64]                  \n\t"
" dup " "z14" ".s, #0\n\t" " dup " "z15" ".s, #0\n\t" " dup " "z16" ".s, #0\n\t" " dup " "z17" ".s, #0\n\t"
" dup " "z18" ".s, #0\n\t" " dup " "z19" ".s, #0\n\t" " dup " "z20" ".s, #0\n\t" " dup " "z21" ".s, #0\n\t"
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t"
" dup " "z22" ".s, #0\n\t" " dup " "z23" ".s, #0\n\t" " dup " "z24" ".s, #0\n\t" " dup " "z25" ".s, #0\n\t"
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t"
"                                            \n\t"
"                                            \n\t"
" cmp x5,#0                                  \n\t"
" beq .S2VX8CONSIDERKLEFT                    \n\t"
"                                            \n\t"
" incb x0, ALL, MUL #2                       \n\t"
" add x1, x1, #32                            \n\t"
"                                            \n\t"
" cmp x5,1                                   \n\t"
" beq .S2VX8LASTITER                         \n\t"
"                                            \n\t"
"                                            \n\t"
" .S2VX8LOOP:                                \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z0" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z1" ".s, " "z2" ".s\n\t" " ld1w   " "z26" ".s, " "p0" "/z, [" "x0" ", #" "0"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "0" "]\n\t"
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z0" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z1" ".s, " "z3" ".s\n\t" " ld1w   " "z27" ".s, " "p0" "/z, [" "x0" ", #" "1"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "4" "]\n\t"
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z0" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z1" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "8" "]\n\t"
" prfd pldl1keep,p0, [x0, #4, MUL VL]        \n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z0" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z1" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "12" "]\n\t"
" prfd pldl1keep,p0, [x0, #5, MUL VL]        \n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z0" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z1" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "16" "]\n\t"
" prfd pldl1keep,p0, [x0, #6, MUL VL]        \n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z0" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z1" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "20" "]\n\t"
" prfd pldl1keep,p0, [x0, #7, MUL VL]        \n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z0" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z1" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "24" "]\n\t"
" prfd pldl1keep,p0, [x0, #8, MUL VL]        \n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z0" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z1" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "28" "]\n\t"
" prfd pldl1keep,p0, [x0, #9, MUL VL]        \n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z26" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z27" ".s, " "z2" ".s\n\t" " ld1w   " "z0" ".s, " "p0" "/z, [" "x0" ", #" "2"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "32" "]\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z26" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z27" ".s, " "z3" ".s\n\t" " ld1w   " "z1" ".s, " "p0" "/z, [" "x0" ", #" "3"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "36" "]\n\t"
" prfm PLDL1KEEP, [x1, #96]                 \n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z26" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z27" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "40" "]\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z26" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z27" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "44" "]\n\t"
" prfm PLDL1KEEP, [x1, #160]                 \n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z26" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z27" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "48" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z26" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z27" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "52" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z26" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z27" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "56" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z26" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z27" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "60" "]\n\t"
" incb x0, ALL, MUL #4                       \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z0" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z1" ".s, " "z2" ".s\n\t" " ld1w   " "z26" ".s, " "p0" "/z, [" "x0" ", #" "0"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "64" "]\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z0" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z1" ".s, " "z3" ".s\n\t" " ld1w   " "z27" ".s, " "p0" "/z, [" "x0" ", #" "1"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "68" "]\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z0" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z1" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "72" "]\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z0" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z1" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "76" "]\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z0" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z1" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "80" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z0" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z1" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "84" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z0" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z1" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "88" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z0" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z1" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "92" "]\n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z26" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z27" ".s, " "z2" ".s\n\t" " ld1w   " "z0" ".s, " "p0" "/z, [" "x0" ", #" "2"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "96" "]\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z26" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z27" ".s, " "z3" ".s\n\t" " ld1w   " "z1" ".s, " "p0" "/z, [" "x0" ", #" "3"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "100" "]\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z26" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z27" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "104" "]\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z26" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z27" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "108" "]\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z26" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z27" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "112" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z26" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z27" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "116" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z26" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z27" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "120" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z26" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z27" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "124" "]\n\t"
" incb x0, ALL, MUL #4                       \n\t"
" add x1, x1, #128                           \n\t"
"                                            \n\t"
" sub x5,x5,1                                \n\t"
" cmp x5,1                                   \n\t"
" bne .S2VX8LOOP                             \n\t"
" .S2VX8LASTITER:                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z0" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z1" ".s, " "z2" ".s\n\t" " ld1w   " "z26" ".s, " "p0" "/z, [" "x0" ", #" "0"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "0" "]\n\t"
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z0" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z1" ".s, " "z3" ".s\n\t" " ld1w   " "z27" ".s, " "p0" "/z, [" "x0" ", #" "1"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "4" "]\n\t"
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z0" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z1" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "8" "]\n\t"
" prfd pldl1keep,p0, [x0, #4, MUL VL]        \n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z0" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z1" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "12" "]\n\t"
" prfd pldl1keep,p0, [x0, #5, MUL VL]        \n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z0" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z1" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "16" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z0" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z1" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "20" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z0" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z1" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "24" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z0" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z1" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "28" "]\n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z26" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z27" ".s, " "z2" ".s\n\t" " ld1w   " "z0" ".s, " "p0" "/z, [" "x0" ", #" "2"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "32" "]\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z26" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z27" ".s, " "z3" ".s\n\t" " ld1w   " "z1" ".s, " "p0" "/z, [" "x0" ", #" "3"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "36" "]\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z26" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z27" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "40" "]\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z26" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z27" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "44" "]\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z26" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z27" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "48" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z26" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z27" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "52" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z26" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z27" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "56" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z26" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z27" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "60" "]\n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z0" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z1" ".s, " "z2" ".s\n\t" " ld1w   " "z26" ".s, " "p0" "/z, [" "x0" ", #" "4"", MUL VL]\n\t" " ld1rw  " "z2" ".s, ""p0""/z, [" "x1"",#" "64" "]\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z0" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z1" ".s, " "z3" ".s\n\t" " ld1w   " "z27" ".s, " "p0" "/z, [" "x0" ", #" "5"", MUL VL]\n\t" " ld1rw  " "z3" ".s, ""p0""/z, [" "x1"",#" "68" "]\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z0" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z1" ".s, " "z4" ".s\n\t" " ld1rw  " "z4" ".s, ""p0""/z, [" "x1"",#" "72" "]\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z0" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z1" ".s, " "z5" ".s\n\t" " ld1rw  " "z5" ".s, ""p0""/z, [" "x1"",#" "76" "]\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z0" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z1" ".s, " "z6" ".s\n\t" " ld1rw  " "z6" ".s, ""p0""/z, [" "x1"",#" "80" "]\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z0" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z1" ".s, " "z7" ".s\n\t" " ld1rw  " "z7" ".s, ""p0""/z, [" "x1"",#" "84" "]\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z0" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z1" ".s, " "z8" ".s\n\t" " ld1rw  " "z8" ".s, ""p0""/z, [" "x1"",#" "88" "]\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z0" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z1" ".s, " "z9" ".s\n\t" " ld1rw  " "z9" ".s, ""p0""/z, [" "x1"",#" "92" "]\n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z26" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z27" ".s, " "z2" ".s\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z26" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z27" ".s, " "z3" ".s\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z26" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z27" ".s, " "z4" ".s\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z26" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z27" ".s, " "z5" ".s\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z26" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z27" ".s, " "z6" ".s\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z26" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z27" ".s, " "z7" ".s\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z26" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z27" ".s, " "z8" ".s\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z26" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z27" ".s, " "z9" ".s\n\t"
" incb x0, ALL, MUL #6                       \n\t"
" add x1, x1, #96                            \n\t"
"                                            \n\t"
" .S2VX8CONSIDERKLEFT:                       \n\t"
" cmp x6,0                                   \n\t"
" beq .S2VX8POSTACCUM                        \n\t"
"                                            \n\t"
".S2VX8LOOPKLEFT:                            \n\t"
"                                            \n\t"
" ld1w  " "z0" ".s, " "p0" "/z, [" "x0" "]           \n\t" " ld1w  " "z1" ".s, " "p0" "/z, [" "x0" ",#1, MUL VL]\n\t"
" incb x0, ALL, MUL #2                       \n\t"
"                                            \n\t"
" ld1rw  " "z2" ".s, " "p0" "/z"",[" "x1" ", #" "0""]""\n\t" " ld1rw  " "z3" ".s, " "p0" "/z"",[" "x1" ", #" "4""]""\n\t" " ld1rw  " "z4" ".s, " "p0" "/z"",[" "x1" ", #" "8""]""\n\t" " ld1rw  " "z5" ".s, " "p0" "/z"",[" "x1" ", #" "12""]""\n\t" " ld1rw  " "z6" ".s, " "p0" "/z"",[" "x1" ", #" "16""]""\n\t" " ld1rw  " "z7" ".s, " "p0" "/z"",[" "x1" ", #" "20""]""\n\t" " ld1rw  " "z8" ".s, " "p0" "/z"",[" "x1" ", #" "24""]""\n\t" " ld1rw  " "z9" ".s, " "p0" "/z"",[" "x1" ", #" "28""]""\n\t"
" add x1, x1, #32                            \n\t"
"                                            \n\t"
" sub x6,x6,1                                \n\t"
"                                            \n\t"
" fmla " "z10" ".s, " "p0" "/m, " "z0" ".s, " "z2" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z1" ".s, " "z2" ".s\n\t"
" fmla " "z12" ".s, " "p0" "/m, " "z0" ".s, " "z3" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z1" ".s, " "z3" ".s\n\t"
" fmla " "z14" ".s, " "p0" "/m, " "z0" ".s, " "z4" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z1" ".s, " "z4" ".s\n\t"
" fmla " "z16" ".s, " "p0" "/m, " "z0" ".s, " "z5" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z1" ".s, " "z5" ".s\n\t"
" fmla " "z18" ".s, " "p0" "/m, " "z0" ".s, " "z6" ".s\n\t" " fmla " "z19" ".s, " "p0" "/m, " "z1" ".s, " "z6" ".s\n\t"
" fmla " "z20" ".s, " "p0" "/m, " "z0" ".s, " "z7" ".s\n\t" " fmla " "z21" ".s, " "p0" "/m, " "z1" ".s, " "z7" ".s\n\t"
" fmla " "z22" ".s, " "p0" "/m, " "z0" ".s, " "z8" ".s\n\t" " fmla " "z23" ".s, " "p0" "/m, " "z1" ".s, " "z8" ".s\n\t"
" fmla " "z24" ".s, " "p0" "/m, " "z0" ".s, " "z9" ".s\n\t" " fmla " "z25" ".s, " "p0" "/m, " "z1" ".s, " "z9" ".s\n\t"
"                                            \n\t"
" cmp x6,0                                   \n\t"
" bne .S2VX8LOOPKLEFT                        \n\t"
"                                            \n\t"
" .S2VX8POSTACCUM:                           \n\t"
" prfm PLDL2KEEP, [x3]                       \n\t"
" prfm PLDL2KEEP, [x4]                       \n\t"
"                                            \n\t"
" ld1rw  z29.s, p0/z, [x7]                   \n\t"
" ld1rw  z30.s, p0/z, [x8]                   \n\t"
"                                            \n\t"
" cmp x13,#1                                 \n\t"
" bne .S2VX8GENSTORED                        \n\t"
"                                            \n\t"
" .S2VX8COLSTORED:                           \n\t"
"                                            \n\t"




" dup " "z0" ".s, #0\n\t" " dup " "z1" ".s, #0\n\t" " dup " "z2" ".s, #0\n\t" " dup " "z3" ".s, #0\n\t" " dup " "z4" ".s, #0\n\t" " dup " "z5" ".s, #0\n\t" " dup " "z6" ".s, #0\n\t" " dup " "z7" ".s, #0\n\t""                                            \n\t"" fcmp s" "30" ",#0.0                       \n\t"" beq .S" "2VX8" "BETAZERO" "CONT_S" "COLSTOREDS" "1" "       \n\t""                                            \n\t"" ld1w  " "z0" ".s, " "p0" "/z, [" "x2" "]           \n\t" " ld1w  " "z1" ".s, " "p0" "/z, [" "x2" ",#1, MUL VL]\n\t"" ld1w  " "z2" ".s, " "p0" "/z, [" "x20" "]           \n\t" " ld1w  " "z3" ".s, " "p0" "/z, [" "x20" ",#1, MUL VL]\n\t"" ld1w  " "z4" ".s, " "p0" "/z, [" "x21" "]           \n\t" " ld1w  " "z5" ".s, " "p0" "/z, [" "x21" ",#1, MUL VL]\n\t"" ld1w  " "z6" ".s, " "p0" "/z, [" "x22" "]           \n\t" " ld1w  " "z7" ".s, " "p0" "/z, [" "x22" ",#1, MUL VL]\n\t""                                            \n\t"" fmul " "z0" ".s, " "p0" "/m, " "z0" ".s, " "z30" ".s\n\t" " fmul " "z1" ".s, " "p0" "/m, " "z1" ".s, " "z30" ".s\n\t" " fmul " "z2" ".s, " "p0" "/m, " "z2" ".s, " "z30" ".s\n\t" " fmul " "z3" ".s, " "p0" "/m, " "z3" ".s, " "z30" ".s\n\t"" fmul " "z4" ".s, " "p0" "/m, " "z4" ".s, " "z30" ".s\n\t" " fmul " "z5" ".s, " "p0" "/m, " "z5" ".s, " "z30" ".s\n\t" " fmul " "z6" ".s, " "p0" "/m, " "z6" ".s, " "z30" ".s\n\t" " fmul " "z7" ".s, " "p0" "/m, " "z7" ".s, " "z30" ".s\n\t""                                            \n\t"" .S" "2VX8" "BETAZERO" "CONT_S" "COLSTOREDS" "1" ":          \n\t""                                            \n\t"" fmla " "z0" ".s, " "p0" "/m, " "z10" ".s, " "z29" ".s\n\t" " fmla " "z1" ".s, " "p0" "/m, " "z11" ".s, " "z29" ".s\n\t" " fmla " "z2" ".s, " "p0" "/m, " "z12" ".s, " "z29" ".s\n\t" " fmla " "z3" ".s, " "p0" "/m, " "z13" ".s, " "z29" ".s\n\t"" fmla " "z4" ".s, " "p0" "/m, " "z14" ".s, " "z29" ".s\n\t" " fmla " "z5" ".s, " "p0" "/m, " "z15" ".s, " "z29" ".s\n\t" " fmla " "z6" ".s, " "p0" "/m, " "z16" ".s, " "z29" ".s\n\t" " fmla " "z7" ".s, " "p0" "/m, " "z17" ".s, " "z29" ".s\n\t""                                            \n\t"" st1w  {" "z0" ".s}, " "p0" ", [" "x2" "]           \n\t" " st1w  {" "z1" ".s}, " "p0" ", [" "x2" ",#1, MUL VL]\n\t"" st1w  {" "z2" ".s}, " "p0" ", [" "x20" "]           \n\t" " st1w  {" "z3" ".s}, " "p0" ", [" "x20" ",#1, MUL VL]\n\t"" st1w  {" "z4" ".s}, " "p0" ", [" "x21" "]           \n\t" " st1w  {" "z5" ".s}, " "p0" ", [" "x21" ",#1, MUL VL]\n\t"" st1w  {" "z6" ".s}, " "p0" ", [" "x22" "]           \n\t" " st1w  {" "z7" ".s}, " "p0" ", [" "x22" ",#1, MUL VL]\n\t"
" dup " "z8" ".s, #0\n\t" " dup " "z9" ".s, #0\n\t" " dup " "z10" ".s, #0\n\t" " dup " "z11" ".s, #0\n\t" " dup " "z12" ".s, #0\n\t" " dup " "z13" ".s, #0\n\t" " dup " "z14" ".s, #0\n\t" " dup " "z15" ".s, #0\n\t""                                            \n\t"" fcmp s" "30" ",#0.0                       \n\t"" beq .S" "2VX8" "BETAZERO" "CONT_S" "COLSTOREDS" "2" "       \n\t""                                            \n\t"" ld1w  " "z8" ".s, " "p0" "/z, [" "x23" "]           \n\t" " ld1w  " "z9" ".s, " "p0" "/z, [" "x23" ",#1, MUL VL]\n\t"" ld1w  " "z10" ".s, " "p0" "/z, [" "x24" "]           \n\t" " ld1w  " "z11" ".s, " "p0" "/z, [" "x24" ",#1, MUL VL]\n\t"" ld1w  " "z12" ".s, " "p0" "/z, [" "x25" "]           \n\t" " ld1w  " "z13" ".s, " "p0" "/z, [" "x25" ",#1, MUL VL]\n\t"" ld1w  " "z14" ".s, " "p0" "/z, [" "x26" "]           \n\t" " ld1w  " "z15" ".s, " "p0" "/z, [" "x26" ",#1, MUL VL]\n\t""                                            \n\t"" fmul " "z8" ".s, " "p0" "/m, " "z8" ".s, " "z30" ".s\n\t" " fmul " "z9" ".s, " "p0" "/m, " "z9" ".s, " "z30" ".s\n\t" " fmul " "z10" ".s, " "p0" "/m, " "z10" ".s, " "z30" ".s\n\t" " fmul " "z11" ".s, " "p0" "/m, " "z11" ".s, " "z30" ".s\n\t"" fmul " "z12" ".s, " "p0" "/m, " "z12" ".s, " "z30" ".s\n\t" " fmul " "z13" ".s, " "p0" "/m, " "z13" ".s, " "z30" ".s\n\t" " fmul " "z14" ".s, " "p0" "/m, " "z14" ".s, " "z30" ".s\n\t" " fmul " "z15" ".s, " "p0" "/m, " "z15" ".s, " "z30" ".s\n\t""                                            \n\t"" .S" "2VX8" "BETAZERO" "CONT_S" "COLSTOREDS" "2" ":          \n\t""                                            \n\t"" fmla " "z8" ".s, " "p0" "/m, " "z18" ".s, " "z29" ".s\n\t" " fmla " "z9" ".s, " "p0" "/m, " "z19" ".s, " "z29" ".s\n\t" " fmla " "z10" ".s, " "p0" "/m, " "z20" ".s, " "z29" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z21" ".s, " "z29" ".s\n\t"" fmla " "z12" ".s, " "p0" "/m, " "z22" ".s, " "z29" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z23" ".s, " "z29" ".s\n\t" " fmla " "z14" ".s, " "p0" "/m, " "z24" ".s, " "z29" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z25" ".s, " "z29" ".s\n\t""                                            \n\t"" st1w  {" "z8" ".s}, " "p0" ", [" "x23" "]           \n\t" " st1w  {" "z9" ".s}, " "p0" ", [" "x23" ",#1, MUL VL]\n\t"" st1w  {" "z10" ".s}, " "p0" ", [" "x24" "]           \n\t" " st1w  {" "z11" ".s}, " "p0" ", [" "x24" ",#1, MUL VL]\n\t"" st1w  {" "z12" ".s}, " "p0" ", [" "x25" "]           \n\t" " st1w  {" "z13" ".s}, " "p0" ", [" "x25" ",#1, MUL VL]\n\t"" st1w  {" "z14" ".s}, " "p0" ", [" "x26" "]           \n\t" " st1w  {" "z15" ".s}, " "p0" ", [" "x26" ",#1, MUL VL]\n\t"
"                                            \n\t"
" b .S2VX8END                                \n\t"
"                                            \n\t"
" .S2VX8GENSTORED:                           \n\t"
"                                            \n\t"
" index z8.s, wzr, w13                       \n\t"
" mul x3, x13, x11                           \n\t"
" index z9.s, w3, w13                        \n\t"
"                                            \n\t"




" dup " "z0" ".s, #0\n\t" " dup " "z1" ".s, #0\n\t" " dup " "z2" ".s, #0\n\t" " dup " "z3" ".s, #0\n\t" " dup " "z4" ".s, #0\n\t" " dup " "z5" ".s, #0\n\t" " dup " "z6" ".s, #0\n\t" " dup " "z7" ".s, #0\n\t""                                            \n\t"" fcmp s" "30" ",#0.0                       \n\t"" beq .S" "2VX8" "BETAZERO" "GENI_S" "COLSTOREDS" "1" "       \n\t""                                            \n\t"" ld1w  " "z0" ".s, " "p0" "/z, [" "x2" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z1" ".s, " "p0" "/z, [" "x2" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z2" ".s, " "p0" "/z, [" "x20" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z3" ".s, " "p0" "/z, [" "x20" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z4" ".s, " "p0" "/z, [" "x21" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z5" ".s, " "p0" "/z, [" "x21" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z6" ".s, " "p0" "/z, [" "x22" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z7" ".s, " "p0" "/z, [" "x22" "," "z9" ".s, UXTW #2]\n\t""                                            \n\t"" fmul " "z0" ".s, " "p0" "/m, " "z0" ".s, " "z30" ".s\n\t" " fmul " "z1" ".s, " "p0" "/m, " "z1" ".s, " "z30" ".s\n\t" " fmul " "z2" ".s, " "p0" "/m, " "z2" ".s, " "z30" ".s\n\t" " fmul " "z3" ".s, " "p0" "/m, " "z3" ".s, " "z30" ".s\n\t"" fmul " "z4" ".s, " "p0" "/m, " "z4" ".s, " "z30" ".s\n\t" " fmul " "z5" ".s, " "p0" "/m, " "z5" ".s, " "z30" ".s\n\t" " fmul " "z6" ".s, " "p0" "/m, " "z6" ".s, " "z30" ".s\n\t" " fmul " "z7" ".s, " "p0" "/m, " "z7" ".s, " "z30" ".s\n\t""                                            \n\t"" .S" "2VX8" "BETAZERO" "GENI_S" "COLSTOREDS" "1" ":          \n\t""                                            \n\t"" fmla " "z0" ".s, " "p0" "/m, " "z10" ".s, " "z29" ".s\n\t" " fmla " "z1" ".s, " "p0" "/m, " "z11" ".s, " "z29" ".s\n\t" " fmla " "z2" ".s, " "p0" "/m, " "z12" ".s, " "z29" ".s\n\t" " fmla " "z3" ".s, " "p0" "/m, " "z13" ".s, " "z29" ".s\n\t"" fmla " "z4" ".s, " "p0" "/m, " "z14" ".s, " "z29" ".s\n\t" " fmla " "z5" ".s, " "p0" "/m, " "z15" ".s, " "z29" ".s\n\t" " fmla " "z6" ".s, " "p0" "/m, " "z16" ".s, " "z29" ".s\n\t" " fmla " "z7" ".s, " "p0" "/m, " "z17" ".s, " "z29" ".s\n\t""                                            \n\t"" st1w  {" "z0" ".s}, " "p0" ", [" "x2" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z1" ".s}, " "p0" ", [" "x2" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z2" ".s}, " "p0" ", [" "x20" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z3" ".s}, " "p0" ", [" "x20" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z4" ".s}, " "p0" ", [" "x21" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z5" ".s}, " "p0" ", [" "x21" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z6" ".s}, " "p0" ", [" "x22" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z7" ".s}, " "p0" ", [" "x22" "," "z9" ".s, UXTW #2]\n\t"
" dup " "z10" ".s, #0\n\t" " dup " "z11" ".s, #0\n\t" " dup " "z12" ".s, #0\n\t" " dup " "z13" ".s, #0\n\t" " dup " "z14" ".s, #0\n\t" " dup " "z15" ".s, #0\n\t" " dup " "z16" ".s, #0\n\t" " dup " "z17" ".s, #0\n\t""                                            \n\t"" fcmp s" "30" ",#0.0                       \n\t"" beq .S" "2VX8" "BETAZERO" "GENI_S" "COLSTOREDS" "2" "       \n\t""                                            \n\t"" ld1w  " "z10" ".s, " "p0" "/z, [" "x23" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z11" ".s, " "p0" "/z, [" "x23" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z12" ".s, " "p0" "/z, [" "x24" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z13" ".s, " "p0" "/z, [" "x24" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z14" ".s, " "p0" "/z, [" "x25" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z15" ".s, " "p0" "/z, [" "x25" "," "z9" ".s, UXTW #2]\n\t"" ld1w  " "z16" ".s, " "p0" "/z, [" "x26" "," "z8" ".s, UXTW #2]\n\t" " ld1w  " "z17" ".s, " "p0" "/z, [" "x26" "," "z9" ".s, UXTW #2]\n\t""                                            \n\t"" fmul " "z10" ".s, " "p0" "/m, " "z10" ".s, " "z30" ".s\n\t" " fmul " "z11" ".s, " "p0" "/m, " "z11" ".s, " "z30" ".s\n\t" " fmul " "z12" ".s, " "p0" "/m, " "z12" ".s, " "z30" ".s\n\t" " fmul " "z13" ".s, " "p0" "/m, " "z13" ".s, " "z30" ".s\n\t"" fmul " "z14" ".s, " "p0" "/m, " "z14" ".s, " "z30" ".s\n\t" " fmul " "z15" ".s, " "p0" "/m, " "z15" ".s, " "z30" ".s\n\t" " fmul " "z16" ".s, " "p0" "/m, " "z16" ".s, " "z30" ".s\n\t" " fmul " "z17" ".s, " "p0" "/m, " "z17" ".s, " "z30" ".s\n\t""                                            \n\t"" .S" "2VX8" "BETAZERO" "GENI_S" "COLSTOREDS" "2" ":          \n\t""                                            \n\t"" fmla " "z10" ".s, " "p0" "/m, " "z18" ".s, " "z29" ".s\n\t" " fmla " "z11" ".s, " "p0" "/m, " "z19" ".s, " "z29" ".s\n\t" " fmla " "z12" ".s, " "p0" "/m, " "z20" ".s, " "z29" ".s\n\t" " fmla " "z13" ".s, " "p0" "/m, " "z21" ".s, " "z29" ".s\n\t"" fmla " "z14" ".s, " "p0" "/m, " "z22" ".s, " "z29" ".s\n\t" " fmla " "z15" ".s, " "p0" "/m, " "z23" ".s, " "z29" ".s\n\t" " fmla " "z16" ".s, " "p0" "/m, " "z24" ".s, " "z29" ".s\n\t" " fmla " "z17" ".s, " "p0" "/m, " "z25" ".s, " "z29" ".s\n\t""                                            \n\t"" st1w  {" "z10" ".s}, " "p0" ", [" "x23" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z11" ".s}, " "p0" ", [" "x23" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z12" ".s}, " "p0" ", [" "x24" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z13" ".s}, " "p0" ", [" "x24" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z14" ".s}, " "p0" ", [" "x25" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z15" ".s}, " "p0" ", [" "x25" "," "z9" ".s, UXTW #2]\n\t"" st1w  {" "z16" ".s}, " "p0" ", [" "x26" "," "z8" ".s, UXTW #2]\n\t" " st1w  {" "z17" ".s}, " "p0" ", [" "x26" "," "z9" ".s, UXTW #2]\n\t"
"                                            \n\t"
" .S2VX8END:                                 \n\t"
"                                            \n\t"
:
:
 [aaddr] "m" (a),
 [baddr] "m" (b),
 [caddr] "m" (c),
 [k_iter] "m" (k_iter),
 [k_left] "m" (k_left),
 [alpha] "m" (alpha),
 [beta] "m" (beta),
 [rs_c] "m" (rs_c),
 [cs_c] "m" (cs_c),
 [a_next] "m" (a_next),
 [b_next] "m" (b_next)
:
 "x0","x1","x2","x3",
 "x4","x5","x6",
 "x7","x8","x9",
 "x10","x11","x13",
 "x20","x21","x22","x23","x24","x25","x26",
 "z0","z1","z2",
 "z3","z4","z5",
 "z6","z7","z8",
 "z9","z10","z11",
 "z12","z13","z14",
 "z15","z16","z17","z18","z19",
 "z20","z21","z22","z23",
 "z24","z25","z26","z27",
 "z28","z29","z30","z31",
 "p0"
);
}
