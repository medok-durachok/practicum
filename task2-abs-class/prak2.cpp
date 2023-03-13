#include <iostream>
#include <vector>

using namespace std;

class Transport {
	static int total;
	virtual void setType() = 0;
	virtual void setTicketCost() = 0;
public:
	string sort, type;
	int passCapacity, ticketCost;
	bool isRailVehicle;

	Transport() { 
		total++; }

	Transport(const Transport& obj) {
		sort = obj.sort;
		type = obj.type;
		passCapacity = obj.passCapacity;
		isRailVehicle = obj.isRailVehicle;
	}

	void setSort() {
		cout << "Enter sort of transport: ";
		cin >> sort;
	}

	void setPassCapacity() {
		cout << "Enter passengers capacity: ";
		cin >> passCapacity;
	}

	void setIsRail() {
		cout << "Enter yes if this sort of transport is rail: ";
		cin >> isRailVehicle;
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

	int getTicketCost() const { return ticketCost; }

	int getPassCapacity() const { return passCapacity; }

	bool getIsRail() const { return isRailVehicle; }

	Transport& operator= (const Transport& obj) {
		this->sort = obj.getSort();
		this->type = obj.getType();
		this->passCapacity = obj.getPassCapacity();
		this->isRailVehicle = obj.getIsRail();
		return *this;
	}

	static void show() {
		cout << "All transport: " << total << endl;
	}

	virtual ~Transport() { total--; }
};

int Transport::total = 0;
class Surface : public Transport {
	static int surfTotal;
	int routeNum;
	vector<Surface> sf;
public:
	Surface() : Transport() { 
		surfTotal++; 
	}
	Surface(const Surface& obj) : Transport(obj) { }
	void setType() { type = "surface"; }
	void setTicketCost() { ticketCost = 300; }
	void setRouteNum() { 
		cout << "Route number: ";
		cin >> routeNum;
	}

	int getRouteNum() const { return routeNum; }

	void newItem() {
		Surface n;
		n.setAll();
		n.setRouteNum();
		this->sf.push_back(n);
	}

	friend ostream& operator<< (ostream& s, const Surface& obj) {
		for(int i = 0; i != obj.sf.size(); ++i) {
			s << obj.sf[i].getSort() << endl;
			s << obj.sf[i].getType() << endl;
			s << obj.sf[i].getTicketCost() << endl;
			s << obj.sf[i].getPassCapacity() << endl;
			s << obj.sf[i].getIsRail() << endl;
			s << obj.sf[i].getRouteNum() << endl;
		}
		return s;
	}

	static void show() {
		cout << "All surface transport: " << surfTotal << endl;
	}
	virtual ~Surface() { 
		this->sf.clear();
		surfTotal--; 
	}
};
int Surface::surfTotal = 0;
class Underground : public Transport {
	static int underTotal;
	string metrolineName;
	vector<Underground> ug;
public:
	Underground() : Transport() { underTotal++; }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
	void setTicketCost() { ticketCost = 470; }
	void setLineName() {
		cout << "Name of metro line: ";
		cin >> metrolineName;
	}
	string getLineName() const {

	}	
	void newItem() {
		Underground n;
		n.setAll();
		n.setLineName();
		this->ug.push_back(n);
	}

	friend ostream& operator<< (ostream& s, const Underground& obj) {
		for(int i = 0; i != obj.ug.size(); ++i) {
			s << obj.ug[i].getSort() << endl;
			s << obj.ug[i].getType() << endl;
			s << obj.ug[i].getTicketCost() << endl;
			s << obj.ug[i].getPassCapacity() << endl;
			s << obj.ug[i].getIsRail() << endl;
			s << obj.ug[i].getLineName() << endl;
		}
		return s;
	}

	static void show() {
		cout << "All underground transport: " << underTotal << endl;
	}
	virtual ~Underground() { 
		this->ug.clear();
		underTotal--; 
	}
};
int Underground::underTotal = 0;

int main() {
	Surface f, s;
	Underground u;
	f.newItem();
	cout << f;
	return 0;
}

/*Реализовать некоторый абстрактный класс, удовлетворяющий следующим обязательным требованиям:
	не хочется скоропостижнуться от одного лишь взгляда
	нужно добавить личных полей в каждый класс
	перегрузки тоже немного растащить
	кидаем эксепшены
	ну хоть чет подобное интерфейсу сделать
	методом пристального взгляда оценить косяки и места, над которыми можно подушнить
*/
