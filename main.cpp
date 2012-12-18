#include "stdafx.h"
#include <time.h>
#include <sys/time.h>
#include "SchemeRuntime.h"
#include <mcheck.h>

int time_subtract(struct timeval *result, struct timeval *x,
		struct timeval *y) {
	if (x->tv_sec > y->tv_sec)
		return -1;
	if ((x->tv_sec == y->tv_sec) && (x->tv_usec > y->tv_usec))
		return -1;
	result->tv_sec = (y->tv_sec - x->tv_sec);
	result->tv_usec = (y->tv_usec - x->tv_usec);
	if (result->tv_usec < 0) {
		result->tv_sec--;
		result->tv_usec += 1000000;
	}
	return 0;
}

int main(int argc, char **argv) {
	setenv("MALLOC_TRACE", "malloc_check_file.txt", 1);
	mtrace();
	struct timeval start, stop, diff;
	gettimeofday(&start, 0);
	if (argc > 1) {
		SchemeRuntime runtime;
		for (int i = 1; i < argc; i++) {
			ifstream fin(argv[i], ios::in);
			if (fin) {
				string text;
				string *tmp = new string();
				while (getline(fin, *tmp)) {
					text.append(*tmp + "\n");
				}
				fin.close();
				delete tmp;

				runtime.loadStr(text);
				try {
					runtime.lexAnalise();
					runtime.run();
				} catch (Exception* e) {
					cout << e->what() << endl;
					;
					delete e;
				}
			} else {
				cout << "File '" << argv[i] << " doesn't exist" << endl;
			}
		}
	} else {
		cout << "please pass in scheme file paths as files to be excuted"
				<< endl;
	}

	gettimeofday(&stop, 0);
	time_subtract(&diff, &start, &stop);
	cout << endl << "time: " << diff.tv_usec << "us" << endl;

	return 0;
}
