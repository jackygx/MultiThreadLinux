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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

#include <Thread.hpp>

namespace Platform {

static void *sThreadProc(void *arg)
{
	try {
		RunnableFn task((RunnableFn::Token *)arg);

		CHECK_PARAM(task, "Task is null");

		task();

	} catch (const IException &e) {
		e.Show();
	}

	return NULL;
}

void CreateThread(const RunnableFn &task)
{
	pthread_t mThread;
	auto token = RunnableFn(task).ToToken();

	if (0 != pthread_create(&mThread, NULL, sThreadProc, token)) {
		token->Release();
		throw E("Fail to create thread: ", DEC(errno));
	}
}

}

