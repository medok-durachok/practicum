#include <iostream>

using namespace std;

class Transport {
	virtual void setType() = 0;
public:
	string sort, type;
	int pass_capacity;
	bool is_smth_bad;

	Transport() { }

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
};

class Surface : public Transport {
public:
	Surface() : Transport() {  }
	Surface(const Surface& obj) : Transport(obj) { }
	void setType() { type = "surface"; }
};

class Underground : public Transport {
public:
	Underground() : Transport() { }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
};

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
	деструктор,
	статические члены класса (по существу),
	константные методы (по существу),
	перегруженную операцию [] или () (если их использование не противоречит структуре выбранного класса).
*/
