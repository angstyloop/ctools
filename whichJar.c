#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

/* Given a class name and a root directory, go through all subdirectories to find
 * the .jar file where the class was defined. For now, just print the name.
 */

const char* fext(const char*restrict fname) {
    const char* ptr = strrchr(fname, '.');
    return ptr && (ptr != fname) ? ptr+1 : NULL;
}

#ifdef TEST_FEXT
int main() {
    char str1[] = "./foo.bar";
    char str2[] = "./baz";
    assert(!strcmp("bar", fext(str1)));
    assert(!fext(str2));
    printf("%s\n", fext(str2));
    return 0;
}
#endif

int isjar(const char*restrict fname) {
    const char* ext = fext(fname);
    return ext ? !strcmp(ext, "jar") : 0;
}

#ifdef TEST_ISJAR
int main() {
    char str1[] = "./foo.jar";
    char str2[] = "./jazz";
    assert(isjar(str1));
    assert(!isjar(str2));
    return 0;
}
#endif

int isdir(const char*restrict path) {
    struct stat statbuf;
    if (stat(path, &statbuf)) return 0;
    return S_ISDIR(statbuf.st_mode);
}

/* assumes test.d is an existing directory */
#ifdef TEST_ISDIR
int main() {
    assert(isdir("test.d"));
    return 0;
}
#endif

/* print every .jar file name, and recurse on every directory inside the given one */
void whichJarHelper(char*restrict class, char*restrict dname) {
    DIR *d = opendir(dname);
    if (!d) return;
    struct dirent* de;
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.') {
            continue;
        };
        if (isjar(de->d_name)) {
            printf("%s\n", de->d_name);
        }
        if (isdir(de->d_name)) {
            chdir(de->d_name);
            whichJarHelper(class, ".");
            chdir("..");
        }
    }
    closedir(d);
}


#ifdef TEST_WHICH_JAR_HELPER
int main() {
    whichJarHelper("foo", ".");
}
#endif

// if no directory is provided, use the current one 
void whichJar(char*restrict className, char*restrict dname) {
    whichJarHelper(className, dname && strlen(dname) ? dname : ".");
}

#ifdef TEST_WHICH_JAR
int main() {
    whichJar("foo", NULL);
    //whichJarHelper("foo", 0);
    //whichJarHelper("foo", ".");
}
#endif
