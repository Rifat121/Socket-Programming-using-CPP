//
// Created by Lenovo on 12/5/2023.
//
#include <regex>

bool isValidEmail(const std::string& email) {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    return std::regex_match(email, emailRegex);
}

bool searchTest(const std::string& email){
    const std::string key = "test";
    for(int i=0, j=0; i<email.size(); i++)
    {
        if(email[i]==key[j])
        {
            while(j<5 && email[i]==key[j])
            {
                i++,j++;
            }
            if(j>=4)
                return true;
            else
                j=0;
        }
        else
            j=0;
    }
    return false;
}
bool validateUserEmail(std::string email) {

    bool haveTest = searchTest(email);
    if(haveTest)
        return isValidEmail(email);
    return false;
}