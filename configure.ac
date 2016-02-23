# Autoconf and automake init
AC_INIT([omap-usb-boot], [0.1], [contact@paulk.fr])
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects foreign])

# Checks
AC_CONFIG_SRCDIR([src/omap-usb-boot.c])
AC_PROG_CC
AC_PROG_INSTALL

# Language
AC_LANG_C

# libusb
PKG_CHECK_MODULES(USB, libusb-1.0, [], [])

LIBS="$LIBS $USB_LIBS"
CFLAGS="$CFLAGS $USB_CFLAGS"

# Files to generate
AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
