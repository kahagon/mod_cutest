mod_cutest
==============
CuTest integration for Apache module development.  
CuTest is an Unit Testing Framework for C.
[CuTest WEB site](http://cutest.sourceforge.net/)  
The purpose of mod_cutest is to be able to treat actual Apache data structures 
(e.g. request_rec) in CuTest unit test.

Rough program for running test is below.

1. write test code for your module.  
2. compile your module, and install it 
   to your module directory(apxs -q libexecdir).  
3. access http://localhost/cutest?dl_path=/path/to/mod_foo.so for running test, 
   and you can see result of test on your browser.

Install
--------------
    $ git clone https://github.com/oasynnoum/mod_cutest.git
    $ cd mod_cutest
    $ ./configure
    $ make
    $ sudo make install

Files which will be installed is below.

    libexecdir=`apxs -q libexecdir`
    includedir=`apxs -q includedir`
    bindir=`apxs -q bindir`
    # ${libexecdir}/mod_cutest.so
    # ${libexecdir}/libmodcutest.so
    # ${includedir}/ModCuTest.h
    # ${bindir}/modcutest-config

Example
--------------
Here is a more detailed example.  
We will work through a simple test first exercise.  

### write test code
The goal is to create an Apache module 
which including a function that converts a null-terminated string 
to all upper case.
First, lets create apache module skelton named modcutest_example.

    $ apxs -g -n modcutest_example
    $ cd modcutest_example

Add include directive to `mod_modcutest_example.c`.

    #include "ModCuTest.h"

Next, add functions below to your source code.
contents:

    char* StrToUpper(char* str) {
        return str;
    }

    /* the second argument is an instance of request_rec. */
    void TestStrToUpper(CuTest *tc, request_rec *r) {
        char* input = strdup("hello world");
        char* actual = StrToUpper(input);
        char* expected = "HELLO WORLD";
        CuAssertStrEquals(tc, expected, actual);
    }

    CuSuite* CuGetSuite(void) {
        CuSuite* suite = CuSuiteNew();
        SUITE_ADD_TEST(suite, TestStrToUpper);
        return suite;
    }

### compile
Compile with mod_cutest options.

    $ apxs2 -c mod_modcutest_example.c \
    >       `modcutest-config --cflags` \
    >       `modcutest-config --libs`
    
Install your module.

    $ sudo apxs2 -i -a -n 'modcutest_example' mod_cutest_example.la

Add next code to your httpd.conf.

    <IfModule mod_modcutest_example.c>
        <Location /modcutest_example>
            SetHandler modcutest_example
        </Location>
    </IfModule>

Restart Apache.

    $ sudo apachectl restart

### run test
Access next URL.

    http://localhost/cutest?dl_path=/path/to/mod_modcutest_example.so

`dl_path` is the key to specify your module installed path.  
Actually, you should replace `/path/to` to `apxs -q libexecdir`.

You will an error because we haven't implemented the `StrToUpper()` correctly.
We are just returning the string without changing it to upper case.

    char* StrToUpper(char* str) {
        return str;
    }

Rewrite this as follows:

    char* StrToUpper(char* str) {
        char* p;
        for (p = str ; *p ; ++p) *p = toupper(*p);
        return str;
    }

Recompile and run the tests again. The test should pass this time.

WHAT TO DO NEXT
--------------
At this point you might want to write more tests for the
`StrToUpper()` function. Here are some ideas:

`TestStrToUpper_EmptyString` : pass in ""  
`TestStrToUpper_UpperCase`   : pass in "HELLO WORLD"  
`TestStrToUpper_MixedCase`   : pass in "HELLO world"  
`TestStrToUpper_Numbers`     : pass in "1234 hello"

As you write each one of these tests add it to `CuGetSuite()`.  
If you don't the tests won't be run. Later as you write
other functions and write tests for them be sure to include those
in `CuGetSuite()` also. The `CuGetSuite()` function should
include all the tests in mod_modcutest_example.c

CuTest offers a rich set of CuAssert functions. Here is a list:

    void CuAssert(CuTest* tc, char* message, int condition);
    void CuAssertTrue(CuTest* tc, int condition);
    void CuAssertStrEquals(CuTest* tc, char* expected, char* actual);
    void CuAssertIntEquals(CuTest* tc, int expected, int actual);
    void CuAssertPtrEquals(CuTest* tc, void* expected, void* actual);
    void CuAssertPtrNotNull(CuTest* tc, void* pointer);

CREDITS
--------------
Please look out AUTHORS file.