==========================================================================
BxFactories - Utilities for creating object factories in C++ projects 
==========================================================================

:author: F.Mauger <francois.mauger@unicaen.fr>
:date: 2020-03-18


Introduction
============

This  header-only C++  library provides  somes classes  and macros  to
easily implement factories  of objects.

The  classes  are  registered  in some  global  (singleton)  or  local
register objects using an unique registration identifier.  Client code
can request such a register to instantiate default constructed objects
of any  registered class on-the-fly.   Configuration/initialization of
such objects is under the responsibility  of the client code.  A class
addressed  by  the  factory   registration  mechanism  implemented  in
BxFactories must :

- inherit  some unique  base class,  possibly abstract  (interface and
  polymorphism),
- have a  default constructor which preferably  instantiates objects of
  this  class in  some reasonable  state,  meaning the  object is  not
  usable but at least is not corrupted (uninitialized pointers, memory
  leaks...), which should be the case in any circonstances anyway,
- be  associated by  the  user  or developper  to  some unique  string
  identifier  (here unique  means the  user/developper must  carefully
  choose this  identifier to  avoid name collision,  particularly when
  classes are  registered at the  same place from  various compilation
  units.  I recommend  to use  a *stringification*  of the  full class
  identifier like ``"foo::bar::my_class"``).

BxFactories is inspired by some work from the Bayeux library. I do not
expect its approach  works in all contexts where  object factories are
needed. It  works for me. I  would be happy  if it can also  for other
people's projects.

BxFactories use  the C++11 standard  and depends on the  Boost library
(header only).   I guess this  dependency could be removed  soon using
C++11 standard library.

Albeit BxFactories  may use  global singleton factory  registers, each
associated to a base class  of user choice and automatically populated
thanks  to preprocessing  macros,  it  is not  mandatory  to use  this
technique. You may  create your own factory register and  fill it with
some specific set of object factories  of your choice. In such a case,
you are responsible of managing the register object by yourself.


Examples
========

Example  1 in  the  ``examples`` directory  illustrates some  possible
usage of BxFactories.
