AC_DEFUN([AC_PROG_APXS], [
    AC_CHECK_PROG([HAVE_APXS], [apxs], [yes], [no])
    if test "$HAVE_APXS" = "yes"; then
        APXS="apxs"
    else
        AC_CHECK_PROG([HAVE_APXS2], [apxs2], [yes], [no])
        if test "$HAVE_APXS2" = "yes"; then
            APXS="apxs2"
        else
            AC_MSG_ERROR([apxs is NOT found.])
        fi
    fi
    AC_SUBST([APXS])
])

AC_DEFUN([AC_PROG_APACHCTL], [
    AC_CHECK_PROG([HAVE_APACHECTL], [apachectl], [yes], [no])
    if test "$HAVE_APACHECTL" = "yes"; then
        APACHECTL="apachectl"
    else
        AC_MSG_ERROR([apachectl is NOT found.])
    fi
    AC_SUBST([APACHECTL])
])