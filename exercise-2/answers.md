# Answers or adapting a C program to CHERI C

If you want to do the exercise yourself, please don't read the rest of this file.

## Step 4 answer

Change the implementation of `write_off` to be:
```c
const void *new_buf = buf + off;
return (write(fildes, new_buf, nbyte));
```

## Step 6 answer

The problem is that just an integer is passed and not a valid capability.
The solution is to pass a `FILE *` that exists in `cat.c` into the functions `do_cat` and `verbose_cat` in `methods.c`.

Here's one example of how to do it:
```diff
diff --git a/exercise-2/cat.c b/exercise-2/cat.c
index 344e505..99ab2c1 100644
--- a/exercise-2/cat.c
+++ b/exercise-2/cat.c
@@ -165,13 +165,7 @@ scanfiles(char *argv[], int verbose)
                        warn("%s", path);
                        rval = 1;
                } else if (verbose) {
-                       if (fd == STDIN_FILENO)
-                               do_cat((long)stdin, verbose);
-                       else {
-                               fp = fdopen(fd, "r");
-                               do_cat((long)fp, verbose);
-                               fclose(fp);
-                       }
+                       do_cat(fd, verbose);
                } else {
                        do_cat(fd, verbose);
                        if (fd != STDIN_FILENO)
diff --git a/exercise-2/methods.c b/exercise-2/methods.c
index bb78a75..a78e61f 100644
--- a/exercise-2/methods.c
+++ b/exercise-2/methods.c
@@ -66,19 +66,16 @@ extern const char *filename;
 static ssize_t
 write_off(int fildes, const void *buf, ptroff_t off, size_t nbyte)
 {
-
-       return (write(fildes, (const void *)(off + (uintptr_t)buf), nbyte));
+       const void *new_buf = buf + off;
+       return (write(fildes, new_buf, nbyte));
 }
 
 static void
-verbose_cat(long file)
+verbose_cat(FILE *fp)
 {
-       FILE *fp;
        int ch, gobble, line, prev;
        wint_t wch;
 
-       fp = (FILE *)file;
-
        /* Reset EOF condition on stdin. */
        if (fp == stdin && feof(stdin))
                clearerr(stdin);
@@ -209,9 +206,15 @@ raw_cat(long file)
 void
 do_cat(long file, int verbose)
 {
-
+       FILE *fp;
        if (verbose) {
-               verbose_cat(file);
+               if (file == STDIN_FILENO)
+                       verbose_cat(stdin);
+               else {
+                       fp = fdopen(file, "r");
+                       verbose_cat(fp);
+                       fclose(fp);
+               }
        } else {
                raw_cat(file);
        }
```
