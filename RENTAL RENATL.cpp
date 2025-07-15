#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
using namespace std;

class Car {
protected:
    int id;
    string brand;
    string model;
    double pricePerDay;
    bool isRented;
    int rentedDays;
    string rentedBy;

public:
    Car() : id(0), brand(""), model(""), pricePerDay(0), isRented(false), rentedDays(0), rentedBy("") {}

    virtual void setCar(int _id, string _brand, string _model, double _price) {
        id = _id;
        brand = _brand;
        model = _model;
        pricePerDay = _price;
        isRented = false;
        rentedDays = 0;
        rentedBy = "";
    }

    virtual void display() const = 0;
    virtual void rent(int days, const string& user) = 0;
    virtual void returnCar() = 0;

    int getId() const { return id; }
    bool isAvailable() const { return !isRented; }

    virtual ~Car() {}
};

class LuxuryCar : public Car {
public:
    void display() const override {
        cout << "ID: " << id << ", Luxury - " << brand << " " << model
             << ", Rs" << pricePerDay << "/day, "
             << (isRented ? ("Rented by: " + rentedBy) : "Available") << endl;
    }

    void rent(int days, const string& user) override {
        isRented = true;
        rentedDays = days;
        rentedBy = user;
        cout << "Note: Luxury tax will be included in final billing.\n";
    }

    void returnCar() override {
        isRented = false;
        double luxuryTax = 0.2 * rentedDays * pricePerDay;
        double total = rentedDays * pricePerDay + luxuryTax;

        cout << "CAR RETURNED SUCCESSFULLY.\n";
        cout << "Rented by: " << rentedBy << endl;
        cout << "Total rental days: " << rentedDays << endl;
        cout << "Total amount (including luxury tax): Rs" << total << endl;

        ofstream fout("rental_log.txt", ios::app);
        if (!fout) {
            cerr << "Error: Could not write to rental_log.txt\n";
        } else {
            fout << "Returned: Luxury " << brand << " " << model
                 << ", By: " << rentedBy
                 << ", Days: " << rentedDays << ", Total: Rs" << total << "\n";
            fout.close();
        }

        rentedBy = "";
        rentedDays = 0;
    }
};

class EconomyCar : public Car {
public:
    void display() const override {
        cout << "ID: " << id << ", Economy - " << brand << " " << model
             << ", Rs" << pricePerDay << "/day, "
             << (isRented ? ("Rented by: " + rentedBy) : "Available") << endl;
    }

    void rent(int days, const string& user) override {
        isRented = true;
        rentedDays = days;
        rentedBy = user;
    }

    void returnCar() override {
        isRented = false;
        double total = rentedDays * pricePerDay;

        cout << "CAR RETURNED SUCCESSFULLY.\n";
        cout << "Rented by: " << rentedBy << endl;
        cout << "Total rental days: " << rentedDays << endl;
        cout << "Total amount: Rs" << total << endl;

        ofstream fout("rental_log.txt", ios::app);
        if (!fout) {
            cerr << "Error: Could not write to rental_log.txt\n";
        } else {
            fout << "Returned: Economy " << brand << " " << model
                 << ", By: " << rentedBy
                 << ", Days: " << rentedDays << ", Total: Rs" << total << "\n";
            fout.close();
        }

        rentedBy = "";
        rentedDays = 0;
    }
};

class CarRentalSystem {
private:
    Car* cars[100];
    int count;

public:
    CarRentalSystem() : count(0) {}

    ~CarRentalSystem() {
        for (int i = 0; i < count; ++i)
            delete cars[i];
    }

    void addCar(Car* car, int id, string brand, string model, double price) {
        if (count < 100) {
            car->setCar(id, brand, model, price);
            cars[count++] = car;
        } else {
            cout << "Car list full!\n";
            delete car;
        }
    }

    void showAvailableCars() const {
        if (count == 0) {
            cout << "\nNo cars available in the system.\n";
            return;
        }

        cout << "\nAVAILABLE CARS:\n";
        bool availableFound = false;
        for (int i = 0; i < count; i++) {
            if (cars[i]->isAvailable()) {
                cars[i]->display();
                availableFound = true;
            }
        }
        if (!availableFound) {
            cout << "No cars currently available for rent.\n";
        }
    }

    bool carExists(int id) const {
        for (int i = 0; i < count; i++) {
            if (cars[i]->getId() == id) {
                return true;
            }
        }
        return false;
    }

    void rentCar(int id) {
        if (!carExists(id)) {
            cout << "Error: Car ID not found.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            if (cars[i]->getId() == id) {
                if (cars[i]->isAvailable()) {
                    string user;
                    do {
                        cout << "Enter your name: ";
                        cin.ignore();
                        getline(cin, user);
                        if (user.empty()) {
                            cout << "Name cannot be empty. Please try again.\n";
                        }
                    } while (user.empty());

                    int days;
                    cout << "Enter number of days to rent: ";
                    while (!(cin >> days) || days <= 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a positive number: ";
                    }

                    cars[i]->rent(days, user);
                    cout << "CAR RENTED SUCCESSFULLY.\n";

                    ofstream fout("rental_log.txt", ios::app);
                    if (!fout) {
                        cerr << "Error: Could not write to rental_log.txt\n";
                    } else {
                        fout << "Rented: Car ID " << id << ", By: " << user << ", Days: " << days << "\n";
                        fout.close();
                    }
                } else {
                    cout << "Car is already rented.\n";
                }
                return;
            }
        }
    }

    void returnCar(int id) {
        if (!carExists(id)) {
            cout << "Error: Car ID not found.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            if (cars[i]->getId() == id) {
                if (!cars[i]->isAvailable()) {
                    cars[i]->returnCar();
                } else {
                    cout << "Car was not rented.\n";
                }
                return;
            }
        }
    }
};

int main() {
    cout << "===== CAR RENTAL SYSTEM =====\n";

    CarRentalSystem rental;

    rental.addCar(new EconomyCar, 1, "Suzuki", "Swift", 6500);
    rental.addCar(new EconomyCar, 2, "Hyundai", "Elantra", 8500);
    rental.addCar(new LuxuryCar, 3, "Toyota", "Camry", 15000);
    rental.addCar(new LuxuryCar, 4, "BMW", "X5", 25000);

    int choice, id;
    while (true) {
        cout << "\n--- MENU ---\n";
        cout << "1. Show available cars\n2. Rent a car\n3. Return a car\n4. Exit\n";
        cout << "Enter choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number between 1 and 4: ";
        }

        switch (choice) {
            case 1:
                rental.showAvailableCars();
                break;
            case 2: {
                cout << "Enter Car ID to rent: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number: ";
                }
                rental.rentCar(id);
                break;
            }
            case 3: {
                cout << "Enter Car ID to return: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number: ";
                }
                rental.returnCar(id);
                break;
            }
            case 4:
                cout << "Thank you for using the system!\n";
                return 0;
        }
    }
}