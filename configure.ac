AC_INIT(starcommanderconfig, version-0.1)

# check for c++ compiler
AC_PROG_CXX
AC_LANG(C++)

# check for headers
PKG_CHECK_MODULES(SDL, sdl2)
PKG_CHECK_MODULES(SDL_Mixer, SDL2_mixer)
PKG_CHECK_MODULES(Freetype, freetype2)
# TODO: pkg-check for glew

# TODO: just bundle this lib
AC_MSG_CHECKING([for libSOIL])
if find /usr/include -iname 'SOIL' | egrep '.*'
then
	AC_SUBST(SOIL_CFLAGS, -I`find /usr/include -iname 'SOIL'`)
elif find /opt -iname 'SOIL' | egrep '.*'
then
	AC_SUBST(SOIL_CFLAGS, -I`find /opt -iname 'SOIL'`)
else
	AC_MSG_RESULT([no])
fi

# can probably make this better
AC_ARG_WITH([mac],
  [use a mac],
  AC_SUBST(GL_LINKER_FLAG, '-framework OpenGL'),
  AC_SUBST(GL_LINKER_FLAG, -lGL)
  )


AC_OUTPUT(Makefile Tupfile) # src/Makefile)
