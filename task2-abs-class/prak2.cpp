#include <iostream>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

bool Numerical(string s);

class Transport {
	static int total;
public:
	virtual void setType() = 0;							//наземный или метро, заполняется автоматически
	virtual void setTicketCost() = 0;					//тоже автоматически фиксированная цена
	virtual const Transport& operator() (int n) = 0;	//через () удаляем элемент :)
	string sort, type, passCapacity, isRailVehicle;
	int ticketCost;

	Transport() { total++; }

	Transport(const Transport& obj) {
		sort = obj.sort;					//вообще можно как угодно заполнить, на это ограничений не ввела (пока)
		type = obj.type;
		ticketCost = obj.ticketCost;
		passCapacity = obj.passCapacity;	//пассажировместимость, только число
		isRailVehicle = obj.isRailVehicle;  //является ли рельсоходным, трамвай, например, в наземном мб таким. проверки честности нет, есть проверка на введенное значение для трансляции в тру/фолс
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
		if(isRailVehicle == "yes" || isRailVehicle == "1") isRailVehicle = "true";
		else isRailVehicle = "false";
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
		cout << "Overloading of =" << endl;			
		this->sort = obj.getSort();
		this->type = obj.getType();
		this->passCapacity = obj.getPassCapacity();
		this->isRailVehicle = obj.getIsRail();
		return *this;
	}

	static void show() {												//это просто для статик поля и все
		cout << "All transport: " << total << endl;
	}

	virtual ~Transport() { total--; }
};
int Transport::total = 0;

class Surface : public Transport {										//класс наземки
	static int surfTotal;
	vector<Surface> sf;
public:
	Surface() : Transport() { surfTotal++; }
	Surface(const Surface& obj) : Transport(obj) { }
	void setType() { type = "surface"; }
	void setTicketCost() { ticketCost = 300; }

	const Surface& operator() (int i) {								//собственно перегрузка (), ну, в задании было нужно..
		if(i > sf.size() || i <= 0) {
			cout << "No available item to delete" << endl;
			return *this;
		}
		sf.erase(sf.begin() + i - 1);
		return *this;
	}

	void newItem() {												//добавляем в вектор
		surfTotal++;
		Surface n;
		n.setAll();
		this->sf.push_back(n);
	}

	void Change(int i) {											//заменяем элемент, вообще, как оказалось, лишний метод..
		if(i > sf.size() || i <= 0) {
			cout << "No available item to change" << endl;
			return;
		}
		Surface obj;
		obj.setAll();
		sf[i - 1] = obj;
	}

	friend ostream& operator<< (ostream& s, const Surface& obj) {		//выводим всю инфу вектора вообще 
		s << "---------------------" << endl;
		cout << "--SURFACE TRANSPORT--" << endl;
		for(int i = 0; i != obj.sf.size(); ++i) {
			s << "---------------------" << endl;
			s << "#" << i + 1 << endl;
			s << "Sort: " << obj.sf[i].getSort() << endl;
			s << "Type: " << obj.sf[i].getType() << endl;
			s << "TicketCost: " << obj.sf[i].getTicketCost() << endl;
			s << "Passangers Capacity: " << obj.sf[i].getPassCapacity() << endl;
			s << "Rail or not: " << obj.sf[i].getIsRail() << endl;
		}
		s << "---------------------" << endl;
		return s;
	}

	static void show() {												//вовзращаем значение статик — колво членов класса 
		cout << "All surface transport: " << surfTotal << endl;
	}
	virtual ~Surface() { 
		this->sf.clear();
		surfTotal--; 
	}
};
int Surface::surfTotal = 0;

class Underground : public Transport {									//в целом аналогичные штуки
	static int underTotal;
	vector<Underground> ug;
public:
	Underground() : Transport() { underTotal++; }
	Underground(const Surface& obj) : Transport(obj) { }
	void setType() { type = "underground"; }
	void setTicketCost() { ticketCost = 470; }

	const Underground& operator() (int i) {
		if(i > ug.size() || i <= 0) {
			cout << "No available item to delete" << endl;
			return *this;
		}
		ug.erase(ug.begin() + i - 1);
		return *this;
	}

	void newItem() {
		Underground n;
		n.setAll();
		this->ug.push_back(n);
	}
	
	void Change(int i) {
		if(i > ug.size() || i <= 0) {
			cout << "No available item to change" << endl;
			return;
		}
		Underground obj;
		obj.setAll();
		ug[i - 1] = obj;
	}

	friend ostream& operator<< (ostream& s, const Underground& obj) {
		s << "UNDERGROUND TRANSPORT" << endl;
		for(int i = 0; i != obj.ug.size(); ++i) {
			s << "---------------------" << endl;
			s << "#" << i + 1 << endl;
			s << "Sort: " << obj.ug[i].getSort() << endl;
			s << "Type: " << obj.ug[i].getType() << endl;
			s << "TicketCost: " << obj.ug[i].getTicketCost() << endl;
			s << "Passangers Capacity: " << obj.ug[i].getPassCapacity() << endl;
			s << "Rail or not: " << obj.ug[i].getIsRail() << endl;
		}
		s << "---------------------" << endl;
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

bool Numerical(string s) {									//проверяем некоторые поля на правильность заполнения
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
	cout << "4: change item" << endl;
	cout << "5: show the entire list" << endl;
	cout << "6: = overloading" << endl;
	cout << "0: exit" << endl;
	cout << "------------------------" << endl;
}

int main() {
	Surface s, f;
	Underground u, g;
	Menu();
	int menu_item, i; string str; bool cont = true; char c;
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
			cout << "Show number of surface transport objects? y/n ";
			cin >> c;
    		if (c == 'y' || c == 'Y') Surface::show();
			else {
				cout << "Show number of underground transport objects? y/n ";
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
			cout << "Change item from surface transport? y/n ";
			cin >> c;
    		if (c == 'y' || c == 'Y') {
				cout << "Enter index of item to change: ";
				cin >> str;
				while(Numerical(str) == false) {
					cout << "Wrong index. Try again: ";
					cin >> str;
				}
				s.Change(stoi(str));
			}
			else {
				cout << "Change item from underground transport? y/n ";
				cin >> c;
    			if (c == 'y' || c == 'Y')  {
					cout << "Enter index of item to change: ";
					cin >> str;
					while(Numerical(str) == false) {
						cout << "Wrong index. Try again: ";
						cin >> str;
					}
					u.Change(stoi(str));
				}
			}
			break;
		case 5:
			cout << s;
			cout << u;
			break;
		case 6:
			f = s;
			cout << "Overloaded output" << endl;				//я так и не придумала, как в main можно адекватно это впихнуть, извините :(
			cout << f;
			g = u;
			cout << g;
			break;
		case 0:
			cont = false;
			break;
		default:
			cout << "Wrong key" << endl;
		}
	}
	Surface r = s;									//тоже не знаю, как внутри условно программы можно это реализовать, можно вот так втупую это обойти.....?
	cout << r;
	return 0;
}
