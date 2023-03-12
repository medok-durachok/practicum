#include <iostream>

using namespace std;

class Transport {
	virtual void setType() = 0;
	static int total;
public:
	string sort, type;
	int pass_capacity;
	bool is_smth_bad;

	Transport() { total++; }

	Transport(const Transport& obj) {
		sort = obj.sort;
		type = obj.type;
		pass_capacity = obj.pass_capacity;
		is_smth_bad = obj.is_smth_bad;
	}

	void setSort() {
		cout << "Enter sort of transport: ";
		cin >> sort;
	}

	void setPassCapacity() {
		cout << "Enter passengers capacity: ";
		cin >> pass_capacity;
	}

	void set() {
		cout << "Enter: ";
		cin >> is_smth_bad;
	}

	void setAll() {
		setSort();
		setType();
		setPassCapacity();
		set();
	}

	string getSort() const { return sort; }

	string getType() const { return type; }

	int getPassCapacity() const { return pass_capacity; }

	bool getSmth() const { return is_smth_bad; }

	friend ostream& operator<< (ostream& s, const Transport& obj) {
		s << obj.getSort() << endl;
		s << obj.getType() << endl;
		s << obj.getPassCapacity() << endl;
		s << obj.getSmth() << endl;
		return s;
	}

	Transport& operator= (const Transport& obj) {
		this->sort = obj.getSort();
		this->type = obj.getType();
		this->pass_capacity = obj.getPassCapacity();
		this->is_smth_bad = obj.getSmth();
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
	static void show() {
		cout << "All surface transport: " << surf_total << endl;
	}
	virtual ~Surface() { surf_total--; }
};
int Surface::surf_total = 0;
class Underground : public Transport {
	static int under_total;
public:
	Underground() : Transport() {under_total++; }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
	static void show() {
		cout << "All underground transport: " << under_total << endl;
	}
	virtual ~Underground() { under_total--; }
};
int Underground::under_total = 0;

int main() {
	Surface f, s;
	f.setAll();
	s.setAll();
	f = s;
	cout << f;
	return 0;
}

/*Реализовать некоторый абстрактный класс, удовлетворяющий следующим обязательным требованиям:
	- класс неплоский,
	- класс содержит явно описанные 
	статические члены класса (по существу),
	константные методы (по существу),
	перегруженную операцию [] или () (если их использование не противоречит структуре выбранного класса).
*/
