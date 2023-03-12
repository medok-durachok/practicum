#include <iostream>

using namespace std;

class Transport {
	static int total;
	virtual void setType() = 0;
	virtual void setTicketCost() = 0;
public:
	string sort, type;
	int pass_capacity, ticket_cost;
	bool is_rail_vehicle;

	Transport() { total++; }

	Transport(const Transport& obj) {
		sort = obj.sort;
		type = obj.type;
		pass_capacity = obj.pass_capacity;
		is_rail_vehicle = obj.is_rail_vehicle;
	}

	void setSort() {
		cout << "Enter sort of transport: ";
		cin >> sort;
	}

	void setPassCapacity() {
		cout << "Enter passengers capacity: ";
		cin >> pass_capacity;
	}

	void setIsRail() {
		cout << "Enter yes if this sort of transport is rail: ";
		cin >> is_rail_vehicle;
	}

	void setAll() {
		setSort();
		setType();
		setTicketCost();
		setPassCapacity();
		setIsRail();
	}

	string getSort() const { return sort; }

	string getType() const { return type; }

	int getTicketCost() const { return ticket_cost; }

	int getPassCapacity() const { return pass_capacity; }

	bool getIsRail() const { return is_rail_vehicle; }

	friend ostream& operator<< (ostream& s, const Transport& obj) {
		s << obj.getSort() << endl;
		s << obj.getType() << endl;
		s << obj.getTicketCost() << endl;
		s << obj.getPassCapacity() << endl;
		s << obj.getIsRail() << endl;
		return s;
	}

	Transport& operator= (const Transport& obj) {
		this->sort = obj.getSort();
		this->type = obj.getType();
		this->pass_capacity = obj.getPassCapacity();
		this->is_rail_vehicle = obj.getIsRail();
		return *this;
	}

	static void show() {
		cout << "All transport: " << total << endl;
	}

	virtual ~Transport() { total--; }
};

int Transport::total = 0;
class Surface : public Transport {
	static int surf_total;
public:
	Surface() : Transport() { surf_total++; }
	Surface(const Surface& obj) : Transport(obj) { }
	void setType() { type = "surface"; }
	void setTicketCost() { ticket_cost = 300; }
	static void show() {
		cout << "All surface transport: " << surf_total << endl;
	}
	virtual ~Surface() { surf_total--; }
};
int Surface::surf_total = 0;
class Underground : public Transport {
	static int under_total;
public:
	Underground() : Transport() { under_total++; }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
	void setTicketCost() { ticket_cost = 470; }
	static void show() {
		cout << "All underground transport: " << under_total << endl;
	}
	virtual ~Underground() { under_total--; }
};
int Underground::under_total = 0;

int main() {
	Surface f, s;
	Underground u;
	f.setAll();
	u.setAll();
	cout << f;
	return 0;
}

/*Реализовать некоторый абстрактный класс, удовлетворяющий следующим обязательным требованиям:
	- класс неплоский,
*/
