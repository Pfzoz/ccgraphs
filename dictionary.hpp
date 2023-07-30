#ifndef DICTIONARY
#define DICTIONARY

template <typename T>
class Dictionary
{

public:
    std::vector<T> values;
    std::vector<std::string> keys;
    Dictionary(){};
    ~Dictionary(){};

    void set(std::string key, T value)
    {
        int exists = 0;
        for (int i = 0; i < this->keys.size(); i++)
        {
            if (this->keys[i] == key)
            {
                exists = 1;
                this->values[i] = value;
            }
        }
        if (!exists)
        {
            this->values.push_back(value);
            this->keys.push_back(key);
        }
    };

    T get(std::string key)
    {
        for (int i = 0; i < this->keys.size(); i++)
        {
            if (this->keys[i] == key)
            {
                return this->values[i];
            }
        }
        std::cout << "KeyError\n";
        exit(1);
    };

    void clear()
    {
        this->keys.clear();
        this->values.clear();
    };

    void erase(std::string key)
    {
        for (int i = 0; i < this->keys.size(); i++)
        {
            if (this->keys[i] == key)
            {
                this->keys.erase(this->keys.begin() + i);
                this->values.erase(this->values.begin() + i);
            }
        }
    };

    int size() { return this->keys.size(); };
};

#endif