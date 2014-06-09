#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <memory>

class Event {
public:
	Event(const std::string& name)
		: m_name(name) {}
	virtual ~Event() {}
private:
	std::string m_name;
};

template< typename... T >
class TypedEvent : public Event 
{
public:
	TypedEvent (const std::string& name, const T&... data) 
		: Event(name), m_data(data...) {}
	std::tuple<T...> get_data(){return m_data;};	
private:
	std::tuple<T...> m_data;
};

int main(int argc, const char *argv[])
{
	std::shared_ptr<Event> e = std::make_shared<Event>("hello");
	std::shared_ptr<Event> d = std::make_shared<TypedEvent<int>>("hello", 5);

	std::shared_ptr<TypedEvent<int>> ed;
	ed = std::dynamic_pointer_cast<TypedEvent<int>>(d);

	std::tuple<int> data = ed->get_data();

	std::cout << std::get<0>(data) << std::endl;
	
	return 0;
}
