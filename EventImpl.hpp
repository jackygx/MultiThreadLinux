/*
 * Copyright (c) 2018 Guo Xiang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <pthread.h>

class CEventImpl
{
public:
	CEventImpl(void);
	virtual ~CEventImpl(void);

	void Wait(void);
	bool WaitTimeout(int msec);
	void Wakeup(void);

private:
	pthread_cond_t mVar;
	pthread_mutex_t mMtx;
	int mCnt;
};

#endif /* __EVENT_HPP__ */

