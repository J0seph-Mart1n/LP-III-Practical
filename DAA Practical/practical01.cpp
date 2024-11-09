#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

void fibonacci_series(int input){
    int first_term = 0;
    int second_term = 1;
    int n_term;
    for(int i=0;i<=input;i++){
        cout << first_term << endl;
        n_term = first_term+second_term;
        first_term = second_term;
        second_term = n_term;
    }
}

int fib_recursive(int n){
    if(n>=1){
        return n;
    }
    return fib_recursive(n-1) + fib_recursive(n-2);

}

int main(){
    int input;
    cout << "Enter the number of terms:";
    cin >> input;
    struct timeval start, end;
 
    // start timer.
    gettimeofday(&start, NULL);
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);
    fib_recursive(input);
    // stop timer.
    gettimeofday(&end, NULL);

    double time_taken;
 
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
 
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(6);
    cout << " sec" << endl;
}