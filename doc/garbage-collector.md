This document describes the garbage collectors implemented in the Newton ZVM.
You may use this document, along with the source code, to learn the concepts of
garbage collectors practially.

Finally, learn which garbage collectors are available in the Newton ZVM
written at OneCube Software Solutions.

## Introduction

The heap is where your objects are stored. It is created when the virtual
machine starts. Its size may increase or decrease during execution. This area is
managed by a garbage collector. When the heap is exhausted, garbage is collected.
Thus, space is reclaimed and available for creating other objects.

Garbage collection is the process which involves scanning the heap, identifying
live objects, and disposing dead objects.

It should be noted that the virtual machine uses memory outside the heap for
native handles, internal data structures, etc. This memory is manually managed
by the implementation.

A live object, or a referenced object, is an object to which the program maintains
a reference.

A dead object, or an unreferenced object, is an object to which no references are
maintained by the program. Which means, the memory used by a dead object can be
reclaimed.

In programming languages like C and C++, allocation and deallocation of memory
is manually done. In Zen, deallocation of objects is automatically taken care by
the garbage collection system.

The garbage collection is done in three phases.
    * Identification
    * Reclamation
    * Compaction

### Identification

In this phase, the live objects are identified and marked as live.

### Reclamation

In this phase, the dead objects are reclaimed.

The memory allocator holds a list of references to free spaces. It searches for
free spaces whenever an allocation is required.

### Compaction

In this phase, the live objects are grouped together. It improves the
performance of memory allocator. It is an optional phase.

Many researches have shown that most objects live for a short period.
Which means, garbage collection can be improved if the objects live in
generational spaces.

The heap is divided into spaces known as generations.
    * New Generation
    * Old Generation
    * Immortal Generation

#### New Generation

The **new generation** is the space where new objects are generally allocated.
When its space is exhausted, it causes a *minor garbage collection*. Objects here
are collected quickly and the surviving objects are moved to **old generation**.

Minor garbage collection is a *stop-the-world* event. All the mutator threads
are paused until the collection completes.

This space is further divided into three regions.
    * Left Region
    * Middle Region
    * Right Region

A sequential allocator is used to allocate objects.

Each thread receives a *thread local allocation buffer* (TLAB). They are
allocated from the middle region. When the thread terminates, the objects in
the TLAB are merged with the middle region.

A copying garbage collector is used to trace and collect dead objects.

The life cycle of objects in the new generation is done in four folds.
Allocation occurs in the first fold. Deallocation occurs in both second
and third folds, with migrations. Promotions to old generation is done in the
fourth fold.

##### Fold 1 - Allocation

An object is allocated in the middle region. Both the left and right regions
are initially empty.

##### Fold 2 - Migration to Left Region

When the middle region is exhausted, a minor garbage collection is done. All
the live objects are moved to the left region. All the dead objects are disposed,
reclaiming memory.

##### Fold 3 - Migration to Right Region

When the middle region is exhausted again, a minor garbage collection is done.
Dead objects are disposed. Unlike *Fold 2*, live objects are moved to the right
region from both middle and left regions.

The age of the objects from the left region are incremented. After moving, both
left and middle regions are cleared.

Finally, the left and right regions are swapped. The left region becomes the
right region, and vice versa.

##### Fold 4 - Promotion

An object is promoted to the old generation if eligible. The objects age determines
the eligiblity.

*Fold 4* always follows *Fold 2* and *Fold 3*.

#### Old Generation

The **old generation** is the space where old objects are stored. When its space
is exhausted, it causes a *major garbage collection*.

Major garbage collection is a *stop-the-world* event. All the mutator threads
are paused until the collection completes.

An object is considered old if it survives a tenure in the new generation.
Objects with long life span and large objects are directly allocated in this
generation.

The collection of old generation space is slower than new generation because
all the live objects are involved. It is necessary to minimize major garbage
collection events.

The old generation is divided into two regions.
    * White Region
    * Black Region

The white region stores the tenured objects. The garbage collector copies the
tenured objects into the it during promotion.

Large objects are directly allocated in the old generation, in particular the
black region. A *segregated fits allocator* is used here.

#### Immortal Generation

The **immortal generation** is the space where immortal objects are stored.
An object is considered immortal if it is likely to survive until the virtual
machine is disposed. Classes, functions, fields, and other such components
belong in this category.

The immortal generation is included whenever the entire heap is subjected to
garbage collection. Classes and functions are loaded multiple times, if
necessary.