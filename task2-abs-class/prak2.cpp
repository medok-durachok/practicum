#include <iostream>
#include<bits/stdc++.h>
#include <vector>

using namespace std;


bool Numerical(string s);
class Transport {
	static int total;
public:
	virtual void setType() = 0;
	virtual void setTicketCost() = 0;
	virtual const Transport& operator() (int n) = 0;
	string sort, type, passCapacity, isRailVehicle;
	int ticketCost;

	Transport() { total++; }

	Transport(const Transport& obj) {
		sort = obj.sort;
		type = obj.type;
		ticketCost = obj.ticketCost;
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
		while(Numerical(passCapacity) == false) {
			cout << "Wrong format. Try again: ";
			cin >> passCapacity;
		}
	}

	void setIsRail() {
		cout << "Enter yes if this sort of transport is rail: ";
		cin >> isRailVehicle;
		transform(isRailVehicle.begin(), isRailVehicle.end(), isRailVehicle.begin(), ::tolower);
		while(isRailVehicle != "yes" && isRailVehicle != "true" && isRailVehicle != "no" && isRailVehicle != "false" && isRailVehicle != "0" && isRailVehicle != "1") {
			cout << "Wrong format. Try again: ";
			cin >> isRailVehicle;
			transform(isRailVehicle.begin(), isRailVehicle.end(), isRailVehicle.begin(), ::tolower);
		}
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

	string getPassCapacity() const { return passCapacity; }

	string getIsRail() const { return isRailVehicle; }

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
	vector<Surface> sf;
public:
	Surface() : Transport() { 
		surfTotal++; 
	}
	Surface(const Surface& obj) : Transport(obj) { }
	void setType() { type = "surface"; }
	void setTicketCost() { ticketCost = 300; }

	const Surface& operator() (int n) {
		sf.erase(sf.begin() + n - 1);
		return *this;
	}

	void newItem() {
		surfTotal++;
		Surface n;
		n.setAll();
		this->sf.push_back(n);
	}

	friend ostream& operator<< (ostream& s, const Surface& obj) {
		for(int i = 0; i != obj.sf.size(); ++i) {
			s << i + 1 << endl;
			s << obj.sf[i].getSort() << endl;
			s << obj.sf[i].getType() << endl;
			s << obj.sf[i].getTicketCost() << endl;
			s << obj.sf[i].getPassCapacity() << endl;
			s << obj.sf[i].getIsRail() << endl;
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
	vector<Underground> ug;
public:
	Underground() : Transport() { underTotal++; }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
	void setTicketCost() { ticketCost = 470; }

	const Underground& operator() (int n) {
		ug.erase(ug.begin() + n - 1);
		return *this;
	}

	void newItem() {
		Underground n;
		n.setAll();
		this->ug.push_back(n);
	}

	friend ostream& operator<< (ostream& s, const Underground& obj) {
		for(int i = 0; i != obj.ug.size(); ++i) {
			s << obj.ug[i].getSort() << endl;
			s << obj.ug[i].getType() << endl;
			s << obj.ug[i].getTicketCost() << endl;
			s << obj.ug[i].getPassCapacity() << endl;
			s << obj.ug[i].getIsRail() << endl;
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

bool Numerical(string s) {
	int i = 0;
	if(s[0] == '+') i = 1;
    for(; i != s.size(); ++i) {
        if((s[i] >= '0' && s[i] <= '9') == false) {
            return false;
        }
    }
    return true;
}

void Menu() {
	cout << "----------Info----------" << endl;
	cout << "Press necessary key to choose menu item" << endl;
	cout << "1: add new item" << endl;
	cout << "2: show number of objects" << endl;
	cout << "3: delete item" << endl;
	cout << "4: show the entire list" << endl;
	cout << "0: exit" << endl;
	cout << "------------------------" << endl;
}

int main() {
	Surface s;
	Underground u;
	Menu();
	int menu_item; string str; bool cont = true; char c;
	while(cont) {
		cout << "Menu item: ";
		cin >> menu_item;
		switch(menu_item) {
		case 1:
			cout << "Add new item to the list of surface transport? y/n ";
			cin >> c;
    		if (c == 'y' || c == 'Y') s.newItem();
			else {
				cout << "Add new item to the list of underground transport? y/n ";
				cin >> c;
    			if (c == 'y' || c == 'Y') u.newItem();
			}
			break;
		case 2:
			cout << "Show number of surface transport? y/n ";
			cin >> c;
    		if (c == 'y' || c == 'Y') Surface::show();
			else {
				cout << "Show number of underground transport? y/n ";
				cin >> c;
    			if (c == 'y' || c == 'Y') Underground::show();
				else Transport::show();
			}
			break;
		case 3:
			cout << "Delete item from surface transport? y/n ";
			cin >> c;
    		if (c == 'y' || c == 'Y') {
				cout << "Enter index of item to delete: ";
				cin >> str;
				while(Numerical(str) == false) {
					cout << "Wrong index. Try again: ";
					cin >> str;
				}
				s(stoi(str));
			}
			else {
				cout << "Delete item from underground transport? y/n ";
				cin >> c;
    			if (c == 'y' || c == 'Y')  {
					cout << "Enter index of item to delete: ";
					cin >> str;
					while(Numerical(str) == false) {
							cout << "Wrong index. Try again: ";
					cin >> str;
					}
					s(stoi(str));
				}
			}
			break;
		case 4:
			cout << s;
			cout << u;
			break;
		case 0:
			cont = false;
			break;
		default:
			cout << "Wrong key" << endl;
		}
	}
	return 0;
}

/*Реализовать некоторый абстрактный класс, удовлетворяющий следующим обязательным требованиям:
	не хочется скоропостижнуться от одного лишь взгляда
	нормально вывод оформить
	методом пристального взгляда оценить косяки и места, над которыми можно подушнить
*/
