#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <memory>

class Event {
public:
	virtual ~Event() {}
	virtual void get_data() const = 0;
};

class TypedEvent : public Event 
{
public:
	template <typename ...Ts> TypedEvent (Ts&&... data) : m_data( std::forward<Ts>(data)... ) {}
	template <typename ...Ts> virtual void get_data() const override () {
		return std::shared_ptr<TypedEvent<Ts...>>(new TypedEvent<Ts...>(m_data));
	}
private:
	std::tuple<T...> m_data;
};

int main(int argc, const char *argv[])
{
	std::cout << "Hello WOrld" << std::endl;
	return 0;
}
