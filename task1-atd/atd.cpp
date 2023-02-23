#include <iostream>
#include <iomanip>

using namespace std;

class Note {
protected:
	string name, surname, phone_num, birthday;
	int id = 1;
public:
	Note() {
		name = this->name;
		surname = this->surname;
		phone_num = this->phone_num;
		birthday = this->birthday;
	}
	//конструктор с параметром
	void setName() {
		cout << "Enter name: " << endl;
		cin >> name;
	}
	string getName() const { return name; }

	void setSurname() {
		cout << "Enter surname: " << endl;
		cin >> surname;
	}
	string getSurname() const { return surname; }

	void setPhone() {
		cout << "Enter phone number: " << endl;
		cin >> phone_num;
	}
	string getPhone() const { return phone_num; }

	void setBD() {
		cout << "Enter date of birth: " << endl;
		cin >> birthday;
	}
	string getBD() const { return birthday; }

	void SetAll() {
		setName();
		setSurname();
		setPhone();
		setBD();
	}
	void GetAll() const {
		cout << getName() << setw(20);
		cout << getSurname() << setw(20);
		cout << getPhone() <<setw(15);
		cout << getBD() << endl;
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
	Node *getLast(Node *list) {
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
			cout << "Empty list";
		}

		cout << "NAME" << setw(20) << "SURNAME" << setw(20) << "PHONE NUMBER" << setw(15) << "DATE OF BIRTH" << endl;

		Node *ptr = list;
		while (ptr != nullptr) {
			ptr->el.GetAll();
			ptr = ptr->next;
		}
	}

	void Show(Note item) {
		item.GetAll();
	}

	void newItem() {
		Node *ptr = new Node(), *last = getLast(list);
		if(last == nullptr) list = ptr;
		else last->next = ptr;
	}

	void SearchByName(string s) {
		while(list != nullptr) {
			if(list->el.getName() == s) {
				Show(list->el);
			}
			list = list->next;
		}
	}

	void SearchBySurname(string s) {
		while(list != nullptr) {
			if(list->el.getSurname() == s) {
				Show(list->el);
			}
			list = list->next;
		}
	}

	void SearchByPhone(string s) {
		while(list != nullptr) {
			if(list->el.getPhone() == s) {
				Show(list->el);
			}
			list = list->next;
		}
	}

	void SearchByBD(string s) {
		while(list != nullptr) {
			if(list->el.getBD() == s) {
				Show(list->el);
			}
			list = list->next;
		}
	}

	void SortBySurname() {                        // проход по неотсортированной части
	    Node *curr = list;
	    Node *sorted = nullptr;

	    while (curr != nullptr)
	    {
	        Node *next_el = curr->next;

	        if(sorted == nullptr || (sorted->el.getSurname().compare(curr->el.getSurname()) > 0)) {           //вставка первым или перед первым 
	        	curr->next = sorted;
	        	sorted = curr;
	    	} else {
	        	Node *p = sorted;
	        	while((p->next != nullptr) && (p->next->el.getSurname().compare(curr->el.getSurname()) < 0)) {      //проход по элементам, меньшим нужного
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
	    Node *curr = list;
	    Node *sorted = nullptr;

	    while (curr != nullptr)
	    {
	        Node *next_el = curr->next;

	        if(sorted == nullptr || (sorted->el.getName().compare(curr->el.getName()) > 0)) {           
	        	curr->next = sorted;
	        	sorted = curr;
	    	} else {
	        	Node *p = sorted;
	        	while((p->next != nullptr) && (p->next->el.getName().compare(curr->el.getName()) < 0)) {      
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
		Node *prev = nullptr, *curr = list;
		char c;

		while (curr != nullptr) {
			cout << "Record about " << curr->el.getName() << " " << curr->el.getSurname() << ". Delete this? y/n" << endl;
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
	cout << "------------------------" << endl;
}

int main() {
	Notebook i;
	//Menu();
	int menu_item; string s; bool cont = true;
	while(cont) {
		cout << "Menu item: ";
		cin >> menu_item;
		switch(menu_item) {
		case 1:
			i.newItem();
			break;
		case 2:
			cout << "Enter name: ";
			cin >> s;
			i.SearchByName(s);
			break;
		case 3:
			cout << "Enter surname: ";
			cin >> s;
			i.SearchBySurname(s);
			break;
		case 4:
			cout << "Enter phone number: ";
			cin >> s;
			i.SearchByPhone(s);
			break;
		case 5:
			cout << "Enter date of birth (dd.mm.yy format): ";
			cin >> s;
			i.SearchByBD(s);
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