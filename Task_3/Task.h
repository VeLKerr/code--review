#ifndef TASK_H
#define TASK_H

#include "FixedSet.h"

/**
 *	Inputting vector of numbers for storing.
 *  <para>vec</para> - vector for storing numbers before 
 *  writing to <code>FixedSet</code>.
 */
extern void inputData(std::vector<int> &vec);

/**
 *  Inputting numbers, checking its contains and outputting results.
 *	<para>set</para> - <code>FixedSet</code> with numbers.
 */
extern void checking(FixedSet &set);

#endif