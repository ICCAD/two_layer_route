#include <stdio.h>
#include <iostream>
#include <set>

using namespace std;

int main ()
{
  set<int> myset;
  set<int>::iterator itlow,itup;

  for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

  itlow=myset.lower_bound (30);                //       ^
  itup=myset.upper_bound (40);                 //                  ^
  cout << "itlow " << (*itlow) << " itup " << (*itup) << endl;

/*
  myset.erase(itlow,itup);                     // 10 20 70 80 90

  cout << "myset contains:";
  for (set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';
*/
  return 0;
}
