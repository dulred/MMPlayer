#include <iostream>
#include <string>
using namespace std;

// word1 = "ab", word2 = "pqrs"
class Solution {
public:
    string mergeAlternately(string word1, string word2) {
       
       string mergeString = "";

       int len  = word1.length() < word2.length() ? word1.length() : word2.length();

       int j = 0;
       int k = 0;

       for (int i = 0; i < (word1.length() + word2.length()); ++i)
       {
            if (i < len*2)
            {
                if (j <=k)
                    {
                        mergeString+=word1[j];
                        ++j;
                    }else{
                        mergeString+=word2[k];
                        ++k;
                    }     
            }else{

                if (word1.length() > word2.length())
                {
                        mergeString+=word1.substr(len);
                }else{
                        mergeString+=word2.substr(len);
                }

                break;
            }
       }

       return mergeString;
    }
};

int main (){
    Solution sol;
    string word1 = "ab";
    string word2 = "pqrs";
    string res = sol.mergeAlternately(word1, word2);
    printf("%s\n", res.c_str());
    return 0;
}