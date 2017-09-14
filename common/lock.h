/*
 * lock.h
 *
 *  Created on: 2017Äê8ÔÂ29ÈÕ
 *      Author: lucius
 */

#ifndef LOCK_H_
#define LOCK_H_
#include <stdint.h>
typedef uint32_t  spin_lock_t;

void spin_lock(spin_lock_t *lock);
void spin_unlock(spin_lock_t *lock);
#endif /* LOCK_H_ */
