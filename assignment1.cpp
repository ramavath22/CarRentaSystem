#include <iostream>
#include <vector>
#include <ctime>
#include <bsoncxx/builder/stream/document.hpp>
#include <optional>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

class Car;
class Consumer;

class User {
    public:
        User(std::string username, int ID,std::string pwd)
            : username(username), ID(ID), pwd(pwd) {}

        virtual void rentCar(const Car& car) =0;
        virtual void returnCar(Car& car) =0;
        virtual void showInfo() const =0;

        virtual int getID() const = 0;

    protected:
        std::string username;
        int ID;
        std::string pwd;
};

class Car{
    public:
        Car(int ID, std::string model, std::string condition, std::string otherDetails)
            :ID(ID), model(model), condition(condition), otherDetails(otherDetails) {}

        void rentCar(User& user) ;
        void returnCar(User& user);


        std::string getModel() const {
            return model;
        }

        std::string getCondition() const {
            return condition;
        }

        std::string getOtherDetails() const {
            return otherDetails;
        }

        time_t getDueDate() const{
            std::cout << "Due Date: " << dueDate << std::endl;
            return dueDate;
        }

        void showInfo() const {
            std::cout << "ID: "<< ID << "Model: " << model << ", Condition: " << condition << ", Other Details: " << otherDetails << std::endl;
        }

        bool isRented() const {
            return rented;
        }

        void setID(const int newID) {
             ID = newID;
        }

        void setModel(const std::string& newModel) {
             model = newModel;
        }

        void setCondition(const std::string& newCondition) {
             condition = newCondition;
        }

        void setOtherDetails(const std::string& newOtherDetails) {
             otherDetails = newOtherDetails;
        }


        int getID() const ;

    private:
        int ID;
        std::string model;
        std::string condition;
        std::string otherDetails;
        bool rented;
        time_t dueDate;

};

void Car::rentCar(User& user) {
    rented = true;
    dueDate =time(0) + 7*24*60*60;
    user.rentCar(*this);
    std::cout<<"car rented succesfully , thank you "<<std::endl;
}

void Car::returnCar(User& user){
   // only the same consumer should return this car 
   // it shoud be rented 
   if(rented==true){
    rented = false;
    user.returnCar(*this);
   }else{
    std::cout << "No cars to return"<< std::endl;
   }
}

class Consumer : public User{
    public:
        Consumer(std::string username, int ID, std::string pwd, int rentedCars = 0, int fineDue = 0, int consumerRecord =50)
            :User(username, ID, pwd), rentedCars(rentedCars), fineDue(fineDue), consumerRecord(consumerRecord) {}

        void rentCar(const Car& car) override;
        void returnCar(Car& car) override;
        void showInfo() const override;

        void clearDue();
        int getID() const override;

        std::string getUsername() const {
            return username;
        }

        std::string getPwd() const {
            return pwd;
        }

        int getFineDue() const {
            return fineDue;
        }

        int getConsumerRecord() const {
            std::cout << "Consumer Record: " << consumerRecord << std::endl;
            return consumerRecord;
        }

        int getRentedCars() const {
            std::cout << "Rented Cars: " << rentedCars << std::endl;
            return rentedCars;
        }

        void setID(const int newID) {
             ID = newID;
        }

        void setUsername(const std::string newUsername) {
             username = newUsername;
        }

        void setPwd(const std::string newPwd) {
             pwd = newPwd;
        }

        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }

        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }

        void setConsumerRecord(const int newConsumerRecord) {
             consumerRecord = newConsumerRecord;
        }

    private:
        int rentedCars;
        int fineDue;
        int consumerRecord;
};

int Consumer::getID() const {
    return ID;
}

void Consumer::rentCar(const Car& car)  {
    if(car.isRented()){
        if (rentedCars <3 && consumerRecord >= 30){
            rentedCars++;
            std::cout << "Car rented successfully" << rentedCars << std::endl;
        }
        else{
            std::cout << "Cannot rent more cars or insufficient consumerRecord" << std::endl;
        }
    }else{
        std::cout << "Car not available" << std::endl;
    }
    
}

void Consumer::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  //fine of 10 ruppes per day
    }
}

void Consumer::showInfo() const {
    std::cout << "Consumer Username: " << username << ", ID: " << ID << ", Consumer Record: " << consumerRecord << std::endl;
}

void Consumer::clearDue() {
    fineDue = 0;
}

class Employee : public User {
public:
    Employee(std::string username, int ID, std::string pwd,int rentedCars = 0, int fineDue = 0, int employeeRecord = 70)
        : User(username, ID, pwd),rentedCars(rentedCars), fineDue(fineDue), employeeRecord(employeeRecord) {}

    void rentCar(const Car& car) override;
    void returnCar(Car& car) override;
    void showInfo() const override;

    void clearDue();
    int getID() const override;

    std::string getUsername() const {
        return username;
    }

    std::string getPwd() const {
        return pwd;
    }

    int getFineDue() const {
        return fineDue;
    }

    int getEmployeeRecord() const {
        std::cout << "Employee Record: " << employeeRecord << std::endl;
        return employeeRecord;
    }

    int getRentedCars() const {
        std::cout << "Rented Cars: " << rentedCars << std::endl;
        return rentedCars;
    }

        void setID(const int newID) {
             ID = newID;
        }

        void setUsername(const std::string newUsername) {
             username = newUsername;
        }

        void setPwd(const std::string newPwd) {
             pwd = newPwd;
        }

        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }

        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }

        void setEmployeeRecord(const int newEmployeeRecord) {
             employeeRecord = newEmployeeRecord;
        }

private:
    int rentedCars;
    int fineDue;
    int employeeRecord;
};

void Employee::rentCar(const Car& car) {
    if(car.isRented()){
        if (rentedCars < 2 && employeeRecord >= 40) {
        rentedCars++;
        std::cout << "Car rented successfully with a 15% discount!" << std::endl;
    } else {
        std::cout << "Cannot rent more cars or insufficient employee record." << std::endl;
    }
    }else{
        std::cout << "Car not Available" << std::endl;
    }
    
}

void Employee::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  //fine of 10 ruppes per day
    }
}

void Employee::showInfo() const {
    std::cout << "Employee Username: " << username << ", ID: " << ID << ", Employee Record: " << employeeRecord << std::endl;
}

void Employee::clearDue() {
    fineDue = 0;
}

class Manager : public User {
public:
    Manager(std::string username, int ID, std::string pwd)
        : User(username, ID, pwd) {}

    void showInfo() const override;
    int getID() const override;

};



void Manager::showInfo() const {
    std::cout << "Manager Username: " << username << ", ID: " << ID << std::endl;
}



int Employee::getID() const {
    return ID;
}

int Manager::getID() const {
    return ID;
}

int Car::getID() const {
    return ID;
}

class Database {
public:
    Database(const std::string& connectionString);

    void addCar(const Car& car);
    void updateCar(const Car& car);
    void removeCar(const Car& car);
    bool searchCar(const Car& car);

    void addConsumer(const Consumer& consumer);
    void updateConsumer(const Consumer& consumer);
    void removeConsumer(const Consumer& consumer);
    bool searchConsumer(const Consumer& consumer);

    void addEmployee(const Employee& employee);
    void updateEmployee(const Employee& employee);
    void removeEmployee(const Employee& employee);
    bool searchEmployee(const Employee& employee);

    void showAllCars() const;
    void showAllConsumers() const;
    void showAllEmployees() const;

    bool verifyConsumer(int consumerId, const std::string& pwd) const;
    bool verifyEmployee(int employeeId, const std::string& pwd) const;
    bool verifyManager(int managerId, const std::string& pwd) const;

    // std::vector<Consumer> getAllConsumers();

    void clearCollection(const std::string& collectionUsername);


private:
    mongocxx::instance instance;  // MongoDB C++ driver instance
    mongocxx::client client;      // MongoDB client
    mongocxx::database db;        // MongoDB database


    // Helper functions
    bsoncxx::document::value createCarDocument(const Car& car) const;
    bsoncxx::document::value createConsumerDocument(const Consumer& consumer) const;
    bsoncxx::document::value createEmployeeDocument(const Employee& employee) const;
};

Database::Database(const std::string& connectionString)
    : client(mongocxx::uri(connectionString)), db(client["carRentalDB"]) {
    // Initialize MongoDB
    if (!client) {
        std::cerr << "Error: Failed to connect to the database." << std::endl;
    }
    else{
        std::cout << "Connected to the database successfully!" << std::endl;
    }
}


// Helper function to convert Car object to MongoDB document
bsoncxx::document::value Database::createCarDocument(const Car& car) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "model" << car.getModel()
        << "id" << car.getID()
        << "condition" << car.getCondition()
        << "otherDetails" << car.getOtherDetails()
        << "rented" << car.isRented()
        << "dueDate" << bsoncxx::types::b_date{std::chrono::system_clock::from_time_t(car.getDueDate())}
        << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Consumer object to MongoDB document
bsoncxx::document::value Database::createConsumerDocument(const Consumer& consumer) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "username" << consumer.getUsername()
        << "id" << consumer.getID()
        << "pwd" << consumer.getPwd()
        << "rentedCars" << consumer.getRentedCars()
        << "fineDue" << consumer.getFineDue()
        << "consumerRecord" << consumer.getConsumerRecord()
        << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Employee object to MongoDB document
bsoncxx::document::value Database::createEmployeeDocument(const Employee& employee) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "username" << employee.getUsername()
        << "id" << employee.getID()
        << "pwd" << employee.getPwd()
        << "rentedCars" << employee.getRentedCars()
        << "fineDue" << employee.getFineDue()
        << "employeeRecord" << employee.getEmployeeRecord()
        << bsoncxx::builder::stream::finalize;
}

// Add a car to the database
void Database::addCar(const Car& car) {
    auto collection = db["cars"];
    auto document = createCarDocument(car);
    collection.insert_one(document.view());
}

// Update a car in the database
void Database::updateCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    auto update = createCarDocument(car);
    collection.update_one(filter.view(), update.view());
}

// Remove a car from the database
void Database::removeCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    collection.remove_one(filter.view());
}

// Search a car from the database
bool Database::searchCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "couldn't find this car in the database" << std::endl;
        return false;
    }
}

// Add a consumer to the database
void Database::addConsumer(const Consumer& consumer) {
    auto collection = db["consumers"];
    auto document = createConsumerDocument(consumer);
    collection.insert_one(document.view());
}

// Update a consumer in the database
void Database::updateConsumer(const Consumer& consumer) {
    auto collection = db["consumers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << consumer.getID() << bsoncxx::builder::stream::finalize;
    auto update = createConsumerDocument(consumer);
    collection.update_one(filter.view(), update.view());
}

// Remove a consumer from the database
void Database::removeConsumer(const Consumer& consumer) {
    auto collection = db["consumers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << consumer.getID() << bsoncxx::builder::stream::finalize;
    collection.remove_one(filter.view());
}

// Search a consumer in the database
bool Database::searchConsumer(const Consumer& consumer) {
    auto collection = db["consumers"];
    auto filter = bsoncxx::builder::stream::document{} << "ID" << consumer.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "Couldn't find the Consumer in the database " << std::endl;
        return false;
    }
}

// Add an employee to the database
void Database::addEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto document = createEmployeeDocument(employee);
    collection.insert_one(document.view());
}

// Update an employee in the database
void Database::updateEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    auto update = createEmployeeDocument(employee);
    collection.update_one(filter.view(), update.view());
}

// Remove an employee from the database
void Database::removeEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    collection.remove_one(filter.view());
}

// search for a employee in the database
bool Database::searchEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "Couldn't find the employee in the database " << std::endl;
        return false;
    }
}

// Show all cars in the database
void Database::showAllCars() const {
    auto collection = db["cars"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all consumers in the database
void Database::showAllConsumers() const {
    auto collection = db["consumers"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all employees in the database
void Database::showAllEmployees() const {
    auto collection = db["employees"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

bool Database::verifyConsumer(int consumerId, const std::string& pwd) const {
    auto collection = db["consumers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << consumerId << "pwd" << pwd << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyEmployee(int employeeId, const std::string& pwd) const {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employeeId << "pwd" << pwd << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyManager(int managerId, const std::string& pwd) const {
    const int knownManagerId = 1;
    const std::string knownManagerPwd = "manager123";

    return (managerId == knownManagerId) && (pwd == knownManagerPwd);
}

int main() {
    // Sample usage of the car rental system
    

    std::string atlasConnectionString = "mongodb+srv://dineshnaik9867:@Dineshnaik9@cluster0.opwg8hy.mongodb.net/";
    Database database(atlasConnectionString);

    Car car1(1,"Audi", "Good", "White");
    Car car2(2,"Benz", "Excellent", "Blue");
    Car car3(3,"Hyundai", "Average", "Yellow");
    Car car4(4,"Toyota", "Fair", "Black");
    Car car5(5,"Rangerover", "Excellent", "Red");


    Consumer consumer1("Andy Dufresne", 1, "pass923",0, 10, 20);
    Consumer consumer2("Bob Dylan", 2, "pass786", 0, 5, 40);
    Consumer consumer3("Catherine", 3, "pass201", 3, 15, 80);
    Consumer consumer4("Dwayne Johnson", 4, "pass202", 2, 8, 55);
    Consumer consumer5("Euler", 5, "pass303", 1, 5, 65);

    Employee employee1("Aryan", 101, "empPass007", 2, 20, 70);
    Employee employee2("Bharat", 102, "empPass123", 1, 15, 80);
    Employee employee3("Chundru", 103, "empPass596", 3, 25, 90);
    Employee employee4("Daksh", 104, "pass432", 3, 20, 30);
    Employee employee5("Bhargav", 105, "empPass859", 0, 0, 60);

    int newID;
    
    std::vector<Consumer> consumers;
    
    int consumerId;
    std::string consumerPwd;

    int updateConsumerID;
    Consumer consumerToUpdate(" ",0," ",0,0,0);

    std::string newUsername,newPwd;
    int newRentedCars, newFineDue, newConsumerRecord;


    std::vector<Employee> employees;

    std::vector<Car> cars;

    int carID;

    std::string carModel;
    int updateCarID;

    std::string newModel, newCondition, newOtherDetails;
    Car carToUpdate(0, " "," "," ");

    int employeeId;
    std::string employeePwd;

    int updateEmployeeID;
    Employee employeeToUpdate(" ",0," ",0,0,0);

    
    int newEmployeeRecord;

    consumers.push_back(consumer1);
    consumers.push_back(consumer2);
    consumers.push_back(consumer3);
    consumers.push_back(consumer4);
    consumers.push_back(consumer5);

    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);

    employees.push_back(employee1);
    employees.push_back(employee2);
    employees.push_back(employee3);
    employees.push_back(employee4);
    employees.push_back(employee5);

    
    int choice;
    do {
        //choose who is visiting 
        std::cout << "\n******** Car Rental System profile**\n";
        std::cout << "1. Consumer\n";
        std::cout << "2. Employee\n";
        std::cout << "3. Manager\n";
        std::cout << "0. Exit\n";
        std::cout << "Please enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                  // Verify Consumer ID and Pwd
                  std::cout << "Please enter Consumer ID: ";
                  std::cin >> consumerId;
                  std::cout << "Please enter Consumer Pwd: ";
                  std::cin >> consumerPwd;

                  // Consumer verification logic
                  if (database.verifyConsumer(consumerId, consumerPwd)) {
                    std::cout << "Consumer Login Successful!\n";
                    int consumerChoice;
                    std::cout << "\n******** Car Rental System Consumer Menu**\n";
                    std::cout << "1. Show profile\n";
                    std::cout << "2. Rent Car\n";
                    std::cout << "3. Return Car\n";
                    std::cout << "4. Rented Cars\n";
                    std::cout << "5. Consumer Record\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Please enter your choice: ";
                    std::cin >> consumerChoice;

                    switch(consumerChoice) {
                        case 1:
                            for (const auto& consumer : consumers) {
                                if (consumer.getID() == consumerId) {
                                    consumer.showInfo();
                                    break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                            }
                            break;

                        case 2:
                            database.showAllCars() ;
                            
                            std::cout << "select your car id: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& consumer : consumers) {
                                    if(consumer.getID() == consumerId)
                                       car.rentCar(consumer);
                                       std::cout << "car rented successfully";
                                   break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;
                           

                        case 3:
                            std::cout << "select your car id that you wanted to return: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& consumer : consumers) {
                                    if(consumer.getID() == consumerId){
                                       car.returnCar(consumer);
                                    }
                                       
                                   break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                             }
                            }
                          
                            break;

                        case 4:
                            for (const auto& consumer : consumers) {
                                if (consumer.getID() == consumerId) {
                                    consumer.getRentedCars();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }
                            break;
                            
                            break;

                        case 5:
                            for (const auto& consumer : consumers) {
                                if (consumer.getID() == consumerId) {
                                    consumer.getConsumerRecord();
                                    break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                            }
                            break;
                    
                        case 0:
                            // Exit the program
                            break;

                        default:
                           std::cout << "Invalid choice. Please enter a valid option.\n"; 
                           break;
                    }
                    
                  } else {
                    std::cout << "Invalid Consumer ID or Pwd. Login failed.\n";
                  }

                 break;
                  
                  
            
            case 2:
                  

                  // Verify Employee ID and Pwd
                  std::cout << "Please enter Employee ID: ";
                  std::cin >> employeeId;
                  std::cout << "Please enter Employee Pwd: ";
                  std::cin >> employeePwd;

                  // Employee verification logic
                  if (database.verifyEmployee(employeeId, employeePwd)) {
                    std::cout << "Employee Login Successful!\n";
                    int employeeChoice;
                    std::cout << "\n******** Car Rental System Employee Menu**\n";
                    std::cout << "1. Show profile\n";
                    std::cout << "2. Rent Car\n";
                    std::cout << "3. Return Car\n";
                    std::cout << "4. Rented Cars\n";
                    std::cout << "5. Employee Record\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Please enter your choice: ";
                    std::cin >> employeeChoice;

                    switch(employeeChoice) {
                        case 1:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.showInfo();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }break;

                        case 2:
                            database.showAllCars() ;
                            
                            std::cout << "select your car id: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& employee : employees) {
                                    if(employee.getID() == employeeId)
                                       car.rentCar(employee);
                                       std::cout << "car rented successfully";
                                   break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;
                           

                        case 3:
                            std::cout << "select your car id that you wanna to return: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& employee : employees) {
                                    if(employee.getID() == employeeId){
                                       car.returnCar(employee);
                                    }
                                       
                                   break;  // Assuming consumer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;

                        case 4:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.getRentedCars();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }
                            break;

                        case 5:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.getEmployeeRecord();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }break;

                        case 0:
                          // Exit the program
                        break;

                        default:
                           std::cout << "Invalid choice. Please enter a valid option.\n";
                           break;
                    }  
                    
                  } else {
                    std::cout << "Invalid Employee ID or Pwd. Login failed.\n";
                  }

                  break;
                    

            case 3:
                int managerId;
                std::string managerPwd;

                // Verify Manager ID and Pwd
                std::cout << "Please enter Manager ID: ";
                std::cin >> managerId;
                std::cout << "Please enter Manager Pwd: ";
                std::cin >> managerPwd;

                // Manager verification logic
                if (database.verifyManager(managerId, managerPwd)) {
                  std::cout << "Manager Login Successful!\n";

                    // Manager operations
                  int managerChoice;
                  std::cout << "\n******** Car Rental System manager Menu**\n";
                  std::cout << "1. show all cars\n";
                  std::cout << "2. show all consumers\n";
                  std::cout << "3. show all employees\n";
                  std::cout << "4. database operations for cars\n";
                  std::cout << "5. database operations for consumers\n";
                  std::cout << "6. database operations for employee\n";
                  std::cout << "0. Exit\n";
                  std::cout << "Please enter your choice: ";
                  std::cin >> managerChoice;

                  switch(managerChoice) {
                    case 1:
                        database.showAllCars();
                        break;

                    case 2:
                        database.showAllConsumers();
                        break;

                    case 3:
                        database.showAllEmployees();
                        break;

                    case 4:
                        int carOperation;
                           std::cout << "\n******** Car Rental System manager operations**\n";
                           std::cout << "1. update car\n";
                           std::cout << "2. add a car\n";
                           std::cout << "3. remove a car\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Please enter your choice: ";
                           std::cin >> carOperation;

                           switch(carOperation){
                            case 1:
                                  
                                  std::cout << "Please enter the ID of the car to update: ";
                                  std::cin >> updateCarID ;

                                  

                                  carToUpdate.setID(updateCarID);

                                  if (database.searchCar(carToUpdate)) {
                                   // Car found, proceed with update
                                     

                                     std::cout << "Please enter the new model: ";
                                     std::cin >> newModel;

                                     std::cout << "Please enter the new condition: ";
                                     std::cin >> newCondition;

                                     std::cout << "Please enter the new other details: ";
                                     std::cin >> newOtherDetails;

                                     carToUpdate.setModel(newModel);
                                     carToUpdate.setCondition(newCondition);
                                     carToUpdate.setOtherDetails(newOtherDetails);

                                     database.updateCar(carToUpdate);
                                     std::cout << "Car details updated successfully!\n";
                                  } else {
                                     std::cout << "Car not found. Update failed.\n";
                                  }
                                  break;
                               
                                
                            case 2:
                                 {
                                     std::cout << "Please enter the ID of the new car: ";
                                     std::cin >> newID;

                                     std::cout << "Please enter the model of the new car: ";
                                     std::cin >> newModel;

                                     std::cout << "Please enter the condition of the new car: ";
                                     std::cin >> newCondition;

                                     std::cout << "Please enter the otherDetails of the new car: ";
                                     std::cin >> newOtherDetails;

                                     carToUpdate.setID(newID);
                                     carToUpdate.setModel(newModel);
                                     carToUpdate.setCondition(newCondition);
                                     carToUpdate.setOtherDetails(newOtherDetails);

                                     database.addCar(carToUpdate);
                                     std::cout << "New car added to the database!\n";
                                 }
                                 break;

                            case 3:
                                 {
                                     std::cout << "Please enter the ID of the car to remove: ";
                                     std::cin >> updateCarID;


                                     for (const auto& car : cars) {
                                          if (car.getID() == updateCarID) {
                                            //  newPwd=consumer.getPwd();
                                            database.removeCar(car);
                                            std::cout << "Car removed from the database!\n";

                                          }
                                          
                                     }
                                 }
                                 break;

                            case 0:
                                 // Exit the program
                                break;

                            default:
                                std::cout << "Invalid choice. Please enter a valid option.\n";
                                break;
                           }
                        break;

                     case 5:
                        int consumerOperation;
                           std::cout << "\n******** Car Rental System manager operations**\n";
                           std::cout << "1. update consumer\n";
                           std::cout << "2. add a consumer\n";
                           std::cout << "3. remove a consumer\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Please enter your choice: ";
                           std::cin >> consumerOperation;

                           switch(consumerOperation){
                            case 1:
                                  
                                  std::cout << "Please enter the ID of the consumer to update: ";
                                  std::cin >> updateConsumerID;

                                  consumerToUpdate.setID(updateConsumerID);

                                  if (database.searchConsumer(consumerToUpdate)) {
                                       // Consumer found, proceed with update
                                      std::string newUsername,newPwd;
                                      int newRentedCars, newConsumerFine, updateConsumerRecord;

                                      std::cout << "Please enter the new username: ";
                                      std::cin >> newUsername;
                                      for (const auto& consumer : consumers) {
                                          if (consumer.getID() == updateConsumerID) {
                                             newPwd=consumer.getPwd();
                                          }
                                      }
                                      std::cout << "Please enter the new number of rented cars: ";
                                      std::cin >> newRentedCars;
                                      std::cout << "Please enter the new value of fine_due: ";
                                      std::cin >> newFineDue;
                                      std::cout << "Please enter the new value of consumer record value: ";
                                      std::cin >> updateConsumerRecord;

                                      consumerToUpdate.setUsername(newUsername);
                                      consumerToUpdate.setPwd(newPwd);
                                      consumerToUpdate.setRentedCars(newRentedCars);
                                      consumerToUpdate.setFineDue(newFineDue);
                                      consumerToUpdate.setConsumerRecord(newConsumerRecord);

                                      database.updateConsumer(consumerToUpdate);
                                      std::cout << "Consumer details updated successfully!\n";
                                  } else {
                                      std::cout << "Consumer not found. Update failed.\n";
                                  }
                                  break;
                                
                            case 2:
                                  {

                                     std::cout << "Please enter the username of the new consumer: ";
                                     std::cin.ignore(); // Ignore the newline character left in the buffer
                                     std::getline(std::cin, newUsername);


                                     std::cout << "Please enter the id of the new consumer: ";
                                     std::cin >> newID;

                                     std::cout << "Please enter the pwd of the new consumer: ";
                                     std::cin >> newPwd;

                                     std::cout << "Please enter the number of rented cars for the new consumer: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Please enter the fine due for the new consumer: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Please enter the consumer record for the new consumer: ";
                                     std::cin >> newConsumerRecord;

                                      consumerToUpdate.setID(newID);
                                      consumerToUpdate.setUsername(newUsername);
                                      consumerToUpdate.setPwd(newPwd);
                                      consumerToUpdate.setRentedCars(newRentedCars);
                                      consumerToUpdate.setFineDue(newFineDue);
                                      consumerToUpdate.setConsumerRecord(newConsumerRecord);

                                     database.addConsumer(consumerToUpdate);
                                     std::cout << "New consumer added to the database!\n";
                                  }
                                  break;

                            case 3:
                                  {
                                     std::cout << "Please enter the ID of the consumer to remove: ";
                                     std::cin >> updateConsumerID;

                                     for (const auto& consumer : consumers) {
                                          if (consumer.getID() == updateConsumerID) {
                                            //  newPwd=consumer.getPwd();
                                            database.removeConsumer(consumer);
                                            std::cout << "Consumer removed from the database!\n";
                                          }
                                          
                                     }

                                  }
                                  break;

                            case 0:
                                 // Exit the program
                                break;

                            default:
                                std::cout << "Invalid choice. Please enter a valid option.\n";
                                break;
                           }
                        break;
                     case 6:
                              int employeeOperation;
                           std::cout << "\n******** Car Rental System manager operations**\n";
                           std::cout << "1. update employee\n";
                           std::cout << "2. add a employee\n";
                           std::cout << "3. remove a employee\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Please enter your choice: ";
                           std::cin >> employeeOperation;

                              switch(employeeOperation){
                                case 1:
                                
                                std::cout << "Please enter the ID of the employee to update: ";
                                std::cin >> updateEmployeeID;

                                employeeToUpdate.setID(updateEmployeeID);

                                if (database.searchEmployee(employeeToUpdate)) {
                                     
                                     std::cout << "Please enter the new username: ";
                                     std::cin >> newUsername;
                                      for (const auto& employee : employees) {
                                          if (employeeToUpdate.getID() == updateEmployeeID) {
                                             newPwd=employeeToUpdate.getPwd();
                                          }
                                      }

                                     std::cout << "Please enter the new number of rented cars: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Please enter the new fine due: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Please enter the new employee record: ";
                                     std::cin >> newEmployeeRecord;

                                      employeeToUpdate.setUsername(newUsername);
                                      employeeToUpdate.setPwd(newPwd);
                                      employeeToUpdate.setRentedCars(newRentedCars);
                                      employeeToUpdate.setFineDue(newFineDue);
                                      employeeToUpdate.setEmployeeRecord(newEmployeeRecord);
 
                                     database.updateEmployee(employeeToUpdate);
                                     std::cout << "Employee details updated successfully!\n";
                                } else {
                                     std::cout << "Employee not found. Update failed.\n";
                                }
                                
                                break;

                            case 2:
                                {

                                     std::cout << "Please enter the username of the new employee: ";
                                     std::cin.ignore(); // Ignore the newline character left in the buffer
                                     std::getline(std::cin, newUsername);

                                     std::cout << "Please enter the id of the new employee: ";
                                     std::cin >> newID;

                                     std::cout << "Please enter the pwd of the new employee: ";
                                     std::cin >> newPwd;

                                     std::cout << "Please enter the number of rented cars for the new employee: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Please enter the fine due for the new employee: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Please enter the employee record for the new employee: ";
                                     std::cin >> newEmployeeRecord;

                                      employeeToUpdate.setUsername(newUsername);
                                      employeeToUpdate.setID(newID);
                                      employeeToUpdate.setPwd(newPwd);
                                      employeeToUpdate.setRentedCars(newRentedCars);
                                      employeeToUpdate.setFineDue(newFineDue);
                                      employeeToUpdate.setEmployeeRecord(newEmployeeRecord);

                                     database.addEmployee(employeeToUpdate);
                                     std::cout << "New employee added to the database!\n";
                                }
                                break;

                            case 3:
                                {
                                     std::cout << "Please enter the ID of the employee to remove: ";
                                     std::cin >> updateEmployeeID;

                                     for (const auto& employee : employees) {
                                          if (employee.getID() == updateEmployeeID) {
                                            //  newPwd=consumer.getPwd();
                                            database.removeEmployee(employee);
                                            std::cout << "Employee removed from the database!\n";
                                          }
                                     }
                                }
                                break;

                            case 0: 
                               break;

                            default:
                               std::cout << "Invalid choice for Database Operations.\n";
                               break;
                           }

                        break;

                    case 0:
                        //get back to main menu
                        break;

                    default:
                        std::cout << "Invalid choice for Database Operations.\n";
                        break;

                  }
                } else {
                    std::cout << "Invalid Manager ID or Pwd. Login failed.\n";
                }

            
        }
       

    } while (choice != 0);

    return 0;
}