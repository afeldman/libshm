#!/usr/bin/env python
# encoding: utf-8
# waf script for builduing project
# author: Anton Feldmann
# Copyright 2016 anton.feldmann@gmail.com
# license: MIT

import os, sys
from waflib import Build, TaskGen

name = 'libshm'

major  = 0
minor  = 1
bugfix = 0

name_version = '%s-%d.%d.%d' % (name, major, minor, bugfix)

application = name
version     = '%d.%d.%d' % (major, minor, bugfix)

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx compiler_c')

    #Add configuration options
    shmopt = opt.add_option_group ("%s Options" % name.upper())

    shmopt.add_option('--shared',
                      action='store_true',
                      default=False,
                      help='build all libs as shared libs')
    shmopt.add_option('--clang',
                      action='store_true',
                      default=True,
                      help='usec lang')

def configure(conf):

    from waflib import Options

    if Options.options.clang:
        conf.load('clang clang++')
    else:
        conf.load('compiler_cxx compiler_c')


def build(bld):

    # libshm headerfile install
    bld.install_files('${PREFIX}/include/libshm/', bld.path.ant_glob(['include/libshm/*.hpp'], remove=False))
    bld.install_files('${PREFIX}/include/libshm/', bld.path.ant_glob(['include/libshm/*.tpp'], remove=False))

    from waflib import Options
# process libshm.pc.in -> libshm.pc - by default it use the task "env" attribute
    pcf = bld(
        features = 'subst',
        source = '%s.pc.in' % name,
        target = '%s.pc' % name,
        install_path = '${PREFIX}/lib/pkgconfig/'
        )

    pcf.env.table.update(
        {'LIBS':'',
         'VERSION': version,
         'NAME': name,
         'PREFIX': '%s' % Options.options.prefix,
         'INCLUDEDIR': 'include/%s' % name}
        )
