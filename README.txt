clomy.h
==========

A header-only universal C library that provide basic data structure.

Features:
  1. Arena
  2. Dynamic array
  3. Hash table
  4. String & String builder
  5. Tiny Assertions

To learn how to use this library I would recommend checking examples/ codes
which is sorted in increasing complexity and detail explanation of the
feature.

Usage
==========

To use this library:
	#define CLOMY_IMPLEMENTATION
	#include "clomy.h"

To add "clomy_" namespace prefix:
	#define CLOMY_NO_SHORT_NAMES

By default, CLOMY uses platform native API. To prefer using LIBC
	#define CLOMY_PREFER_LIBC 
