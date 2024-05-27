
#include <vector>
#include <string>
#include <iostream>

class Test {
    private:

    public:
        std::string _name;
        Test(std::string name): _name(name) {};
        ~Test() {};

};


int main()
{
    std::vector<Test *> tests;
    Test t("teste1");
    Test tt("teste2");

    tests.push_back(&t);
    tests.push_back(&tt);

    t._name = "teste3";


    for (auto& test : tests)
    {
        std::cout << "name " << test->_name << std::endl;
    }

    return (0);
}