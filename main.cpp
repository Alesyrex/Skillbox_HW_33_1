#include <iostream>
#include <exception>
#include <map>

class InvalidVendorCodeException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "The vendor code is missing.";
    }
};

class InvalidAmountException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Quantity exceeds available.";
    }
};

void findInvalidArgument(std::string _vendor, int _amount)
{
    if(_vendor.empty())
        throw std::invalid_argument("vendor code");
    if(_amount <= 0)
        throw std::invalid_argument("amount");
}

void addBase(std::string _vendor, int _amount, std::map<std::string,int>* base)
{
    findInvalidArgument(_vendor, _amount);
    bool find = false;
    for(auto it = base->begin();it != base->end();++it)
    {
        if(it->first == _vendor)
        {
            it->second += _amount;
            find = true;
        }
    }
    if (!find)
        base->insert(std::pair<std::string,int>(_vendor,_amount));
}

void addOrRemove (std::string _vendor, int _amount, std::map<std::string,int>* input, std::map<std::string,int>* output)
{
    findInvalidArgument(_vendor, _amount);
    auto it = output->begin();
    for(;it != output->end();++it)
    {
        if(it->first == _vendor)
        {
            if(it->second >= _amount)
            {
                auto itShop = input->begin();
                for (;itShop != input->end();++itShop)
                {
                    if(itShop->first == _vendor)
                    {
                        itShop->second += _amount;
                        it->second -= _amount;
                        break;
                    }
                }
                if(itShop == input->end())
                {
                    input->insert(std::pair<std::string,int>(it->first,_amount));
                    it->second -= _amount;
                }
                break;
            }
            else
            {
                throw InvalidAmountException();
            }
        }
    }
    if(it == output->end())
    {
        throw InvalidVendorCodeException();
    }
    if (it->second == 0) output->erase(it);
}

int main() {

    std::string vendor;
    int amount = 0;
    std::string operation;
    auto* base = new std::map<std::string,int>();
    auto* shopCart = new std::map<std::string,int>();

    std::cout << "Populate the store database (vendor code and amount, enter 'exit' to start shopping):" << std::endl;
    while (vendor != "exit")
    {

        std::cin >> vendor;
        if(vendor == "exit") break;
        std::cin >> amount;
        std::cin.clear();
        std::cin.ignore();

        try {
            addBase(vendor, amount, base);
        }
        catch (std::invalid_argument &x) {
            std::cerr << "Invalid argument supplied: " << x.what() << std::endl;
        }
    }

    while(operation != "exit") {
        do {
            std::cout << "What operation do you want to perform?" << std::endl;
            std::cout << "-- add (to add an item to the shopping cart)\n-- remove (to remove an item from the shopping cart)\n";
            std::cout << "-- exit (to exit)\n>";
            std::cin >> operation;
            if (operation != "add" && operation != "remove" && operation != "exit")
                std::cout << "Incorrect operation! Repeat input." << std::endl;
        } while (operation != "add" && operation != "remove" && operation != "exit");

        if (operation == "add") {
            bool input = true;
            while (input) {
                std::cout << "Input vendor code and amount:" << std::endl;
                std::cin >> vendor >> amount;
                try {
                    addOrRemove(vendor, amount, shopCart, base);
                    input = false;
                }
                catch (std::invalid_argument &x) {
                    std::cerr << "Invalid argument supplied: " << x.what() << std::endl;
                    input = false;
                }
                catch (const InvalidVendorCodeException &x) {
                    std::cerr << "False: " << x.what() << std::endl;
                    input = false;
                }
                catch (const InvalidAmountException &x) {
                    std::cerr << "False: " << x.what() << std::endl;
                    input = false;
                }
            }
        }

        if (operation == "remove") {
            bool input = true;
            while (input) {
                std::cout << "Input vendor code and amount:" << std::endl;
                std::cin >> vendor >> amount;
                try {
                    addOrRemove(vendor, amount, base, shopCart);
                    input = false;
                }
                catch (std::invalid_argument &x) {
                    std::cerr << "Invalid argument supplied: " << x.what() << std::endl;
                    input = false;
                }
                catch (const InvalidVendorCodeException &x) {
                    std::cerr << "False: " << x.what() << std::endl;
                    input = false;
                }
                catch (const InvalidAmountException &x) {
                    std::cerr << "False: " << x.what() << std::endl;
                    input = false;
                }
            }
        }
    }

    delete base;
    delete shopCart;

    return 0;
}
