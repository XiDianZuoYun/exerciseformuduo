#include<iostream>
#include <algorithm>
#include<set>
using namespace std;
#include<vector>
int coinChange(vector<int>& coins, int amount) {
    if(coins.size()<=1&&amount!=0)
        return -1;
    else if(amount==0)
        return 0;
    vector<int> tmp(amount+1,65534);
    tmp[0]=0;
    for(int i=0;i<coins.size()&&coins[i]<=amount;++i)
        tmp[coins[i]]=1;
    for(int j=0;j<=amount;++j)
    {
        for(int i=0;i<coins.size();++i)
        {
            if(j-coins[i]>=0)
                tmp[j]=min(tmp[j],tmp[j-coins[i]]+1);
        }
        if(tmp[j]==65534)
            tmp[j]=-1;
    }
    return tmp[amount];
}
int main()
{
  std::vector<int> v{2};
  std::cout << coinChange(v,3) << '\n';
  return 0;
}
