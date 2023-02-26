#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

bool DateFormatCheck(string);
bool Numeric(string);
void TableHeader() {
	cout << "\nNAME" << setw(20) << "SURNAME" << setw(20) << "PHONE NUMBER" << setw(15) << "DATE OF BIRTH" << endl;
}

class Note {
	string name, surname, phone_num, birthday;
public:
	Note() {
		name = this->name;
		surname = this->surname;
		phone_num = this->phone_num;
		birthday = this->birthday;
	}

	void SetName() {
		cout << "Enter name: ";
		cin >> name;
	}
	string GetName() const { return name; }

	void SetSurname() {
		cout << "Enter surname: ";
		cin >> surname;
	}
	string GetSurname() const { return surname; }

	void SetPhone() {
		cout << "Enter phone number: ";
		cin >> phone_num;
		while(Numeric(phone_num) == false) {
			cout << "Wrong phone format. Try again: ";
			cin >> phone_num;
		}
	}
	string GetPhone() const { return phone_num; }

	void SetBD() {
		cout << "Enter date of birth (dd/mm/yyyy format): ";
		cin >> birthday;
		while(DateFormatCheck(birthday) == false) {
			cout << "Wrong date format. Try again: ";
			cin >> birthday;
		}
	}
	string GetBD() const { return birthday; }

	void SetAll() {
		SetName();
		SetSurname();
		SetPhone();
		SetBD();
	}
	void GetAll() const {
		cout << GetName() << setw(20);
		cout << GetSurname() << setw(20);
		cout << GetPhone() <<setw(15);
		cout << GetBD() << endl;
	}
};

class Notebook : public Note {
	struct Node {
		Note el;
		Node *next;

		Node() {
			el.SetAll();
			this->next = nullptr;
		}
	};
	Node *GetLast(Node *list) {
		if (list == nullptr) return nullptr;
	    while (list->next) {
	        list = list->next;
	    }
	    return list;
	}

	Node *list;
public:
	Notebook() : list(nullptr) {}

	void Show() {
		if (list == nullptr) {
			cout << "Empty list" << endl;
			return;
		}

		TableHeader();
		Node *ptr = list;
		while (ptr != nullptr) {
			ptr->el.GetAll();
			ptr = ptr->next;
		}
	}

	void Show(Note item) {
		item.GetAll();
	}

	void NewItem() {
		Node *ptr = new Node(), *last = GetLast(list);
		if(last == nullptr) list = ptr;
		else last->next = ptr;
	}

	void SearchByName(string s) {
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

		short cnt = 0;
		Node *curr = list;
		while(curr != nullptr) {
			if(curr->el.GetName() == s) {
				if(cnt == 0) TableHeader();
				Show(curr->el);
				cnt++;
			}
			curr = curr->next;
		}
		if(cnt == 0) cout << "No records" << endl;
		delete curr;
	}

	void SearchBySurname(string s) {
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

		short cnt = 0;
		Node *curr = list;
		while(curr != nullptr) {
			if(curr->el.GetSurname() == s) {
				if(cnt == 0) TableHeader();
				Show(curr->el);
				cnt++;
			}
			curr = curr->next;
		}
		if(cnt == 0) cout << "No records" << endl;
		delete curr;
	}

	void SearchByPhone(string s) {
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

		short cnt = 0;
		Node *curr = list;
		while(curr != nullptr) {
			if(curr->el.GetPhone() == s) {
				if(cnt == 0) TableHeader();
				Show(curr->el);
				cnt++;
			}
			curr = curr->next;
		}
		if(cnt == 0) cout << "No records" << endl;
		delete curr;;
	}

	void SearchByBD(string s) {
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

		short cnt = 0;
		Node *curr = list;
		while(curr != nullptr) {
			if(curr->el.GetBD() == s) {
				if(cnt == 0) TableHeader();
				Show(curr->el);
				cnt++;
			}
			curr = curr->next;
		}
		if(cnt == 0) cout << "No records" << endl;
		delete curr;
	}

	void SortBySurname() {                       
	    if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

	    Node *curr = list;
	    Node *sorted = nullptr;

	    while (curr != nullptr)
	    {
	        Node *next_el = curr->next;

	        if(sorted == nullptr || (sorted->el.GetSurname().compare(curr->el.GetSurname()) > 0)) {           //вставка первым или перед первым 
	        	curr->next = sorted;
	        	sorted = curr;
	    	} else {
	        	Node *p = sorted;
	        	while((p->next != nullptr) && (p->next->el.GetSurname().compare(curr->el.GetSurname()) < 0)) {      //проход по элементам, меньшим нужного
	            	p = p->next;      
	        	}
	        	curr->next = p->next;                                                    //вставка между элементами
	        	p->next = curr;
	    	}

	        curr = next_el;
	    }
	    list = sorted;
	}

	void SortByName() { 
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

	    Node *curr = list;
	    Node *sorted = nullptr;

	    while (curr != nullptr)
	    {
	        Node *next_el = curr->next;

	        if(sorted == nullptr || (sorted->el.GetName().compare(curr->el.GetName()) > 0)) {           
	        	curr->next = sorted;
	        	sorted = curr;
	    	} else {
	        	Node *p = sorted;
	        	while((p->next != nullptr) && (p->next->el.GetName().compare(curr->el.GetName()) < 0)) {      
	            	p = p->next;      
	        	}
	        	curr->next = p->next;                                                    
	        	p->next = curr;
	    	}

	        curr = next_el;
	    }
	    list = sorted;
	}

	void Delete() {
		if(list == nullptr) {
	    	cout << "Empty list" << endl;
	    	return;
	    }

		Node *prev = nullptr, *curr = list;
		char c;

		while (curr != nullptr) {
			cout << "Record about " << curr->el.GetName() << " " << curr->el.GetSurname() << ". Delete this? y/n" << endl;
			cin >> c;
    		if (c == 'y' || c == 'Y') {
      			if(curr == list) {
          			list = list->next;
          			delete curr;
          			curr = list;
        		} else {
        			prev->next = curr->next;
        			delete curr;
        			curr = prev->next;
      			}
    		} else { 
      			prev = curr;
      			curr = curr->next;
     		}		
  		}
	}

	void ClearList() {
		Node *ptr;
		while(list != nullptr) {
			ptr = list;
			list = list->next;
			delete ptr;
		}
	}

	~Notebook() { ClearList(); }
};

bool DateFormatCheck(string s) {
	if(s.size() != 10) return false;
	if(s[2] != '/' || s[5] != '/') return false;

	short y, m, d; bool is_leap;
	try { 
		y = stoi(s.substr(6, 4)); 
		m = stoi(s.substr(3, 2));
		d = stoi(s.substr(0, 2));
	}
	catch(...) { return false; }

	if(m > 12 || d > 31) return false;

	is_leap = ((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)));
	if(is_leap == false && m == 2 && d > 28) return false; 						//невисокосный февраль
	if(is_leap == true && m == 2 && d > 29) return false;						//високосный февраль

	if(d > 30 && ((m == 4) || (m == 6) || (m == 9) || (m == 11))) return false;

	return true;
}

bool Numeric(string s) {
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
	cout << "2: search by name" << endl;
	cout << "3: search by surname" << endl;
	cout << "4: search by phone number" << endl;
	cout << "5: serch by birthday" << endl;
	cout << "6: sort by surname" << endl;
	cout << "7: sort by name" << endl;
	cout << "8: delete item" << endl;
	cout << "9: show the entire list" << endl;
	cout << "0: exit" << endl;
}

int main() {
	Notebook i;
	Menu();
	int menu_item; string search, s_menu; bool cont = true;
	while(cont) {

		cout << "------------------------" << endl;
		cout << "Menu item: ";
		cin >> s_menu;

		try { menu_item = stoi(s_menu); }
		catch(...) { 
			cout << "Wrong key" << endl;
			continue; 
		}

		switch(menu_item) {
		case 1:
			i.NewItem();
			break;
		case 2:
			cout << "Enter name: ";
			cin >> search;
			i.SearchByName(search);
			break;
		case 3:
			cout << "Enter surname: ";
			cin >> search;
			i.SearchBySurname(search);
			break;
		case 4:
			cout << "Enter phone number: ";
			cin >> search;
			while(Numeric(search) == false) {
				cout << "Wrong phone format. Try again: ";
				cin >> search;
			}
			i.SearchByPhone(search);
			break;
		case 5:
			cout << "Enter date of birth (dd/mm/yyyy format): ";
			cin >> search;
			while(DateFormatCheck(search) == false) {
				cout << "Wrong date format. Try again: ";
				cin >> search;
			}
			i.SearchByBD(search);
			break;
		case 6:
			i.SortBySurname();
			break;
		case 7:
			i.SortByName();
			break;
		case 8:
			i.Delete();
			break;
		case 9:
			i.Show();
			break;
		case 0:
			cont = false;
			i.ClearList();
			break;
		default:
			cout << "Wrong key" << endl;
		}
	}

	return 0;	
}