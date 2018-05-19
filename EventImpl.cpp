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

#include <sys/time.h>

#include "EventImpl.hpp"

CEventImpl::CEventImpl(void) :
	mCnt(0)
{
	pthread_mutex_init(&mMtx, NULL);
	pthread_cond_init(&mVar, NULL);
}

CEventImpl::~CEventImpl(void)
{
	pthread_cond_destroy(&mVar);
	pthread_mutex_destroy(&mMtx);
}

void CEventImpl::Wait(void)
{
	pthread_mutex_lock(&mMtx);

	++mCnt;

	if (mCnt > 0)
		pthread_cond_wait(&mVar, &mMtx);

	pthread_mutex_unlock(&mMtx);
}

bool CEventImpl::WaitTimeout(int msec)
{
	struct timeval now;
	struct timespec to;
	int ret = 0;

	pthread_mutex_lock(&mMtx);

	++mCnt;

	if (mCnt > 0) {
		gettimeofday(&now, NULL);
		to.tv_sec = now.tv_sec + msec / 1000;
		to.tv_nsec = now.tv_usec * 1000 + ((msec % 1000) * 1000000);
		ret = pthread_cond_timedwait(&mVar, &mMtx, &to);
	}

	pthread_mutex_unlock(&mMtx);

	return 0 == ret;
}

void CEventImpl::Wakeup(void)
{
	pthread_mutex_lock(&mMtx);

	if (mCnt > 0) {
		pthread_cond_signal(&mVar);
		mCnt = 0;
	} else {
		--mCnt;
	}

	pthread_mutex_unlock(&mMtx);
}

