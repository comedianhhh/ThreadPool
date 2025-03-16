#include "Threadpool.h"

mutex _m;
int main()
{

    ThreadPool pool(8);
    int n = 20;
    for (int i = 1; i <= n; i++) {
        pool.submit([](int id) {
            if (id % 2 == 1) {
                this_thread::sleep_for(0.2s);
            }
            unique_lock<mutex> lc(_m);
            cout << "id : " << id << endl;
            }, i);
    }

}