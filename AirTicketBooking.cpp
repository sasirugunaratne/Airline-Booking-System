#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

// ------------------------------
// Passenger Structure
// ------------------------------
struct Passenger {
    string name, passport, address;
    int age;
    int seat;
};

// ------------------------------
struct Flight {
    string flightNo;
    string dep;
    string arr;
    string origin;
    string dest;
};

// ------------------------------
struct Booking {
    bool exists = false;

    string destination;
    string travelClass;
    string aircraft;

    int passengers = 0;
    vector<Passenger> pax;

    Flight leg1;
    Flight leg2;

    double basePrice = 0;
    double finalPerPerson = 0;
    double totalPrice = 0;
};

// Global booking memory
Booking booking;

// ------------------------------
// Pricing
// ------------------------------
double getBasePrice(string dest, string cls) {
    if (dest == "New York") {
        if (cls == "First") return 9500;
        if (cls == "Business") return 5000;
        return 4500;
    }
    if (dest == "UK") {
        if (cls == "First") return 8500;
        if (cls == "Business") return 4500;
        return 3500;
    }
    if (dest == "Singapore") {
        if (cls == "First") return 7500;
        if (cls == "Business") return 4000;
        return 3000;
    }
    if (dest == "China") {
        if (cls == "First") return 8000;
        if (cls == "Business") return 3500;
        return 2000;
    }
    if (dest == "Sydney") {
        if (cls == "First") return 9000;
        if (cls == "Business") return 5000;
        return 4800;
    }
    return 0;
}

// ------------------------------
// Flights
// ------------------------------
Flight NY[5][2] = {
    {{"FA1001","08:00","14:00","KUL","DXB"}, {"FA2001","18:00","06:00+1","DXB","JFK"}},
    {{"FA1002","10:30","16:30","KUL","DXB"}, {"FA2002","20:00","08:00+1","DXB","JFK"}},
    {{"FA1003","13:00","19:00","KUL","DXB"}, {"FA2003","22:00","10:00+1","DXB","JFK"}},
    {{"FA1004","15:45","21:45","KUL","DXB"}, {"FA2004","23:30","11:30+1","DXB","JFK"}},
    {{"FA1005","18:20","00:20+1","KUL","DXB"}, {"FA2005","02:00","14:00+1","DXB","JFK"}}
};

Flight UK[5][2] = {
    {{"FA1101","07:00","13:00","KUL","DXB"}, {"FA2101","17:00","22:00","DXB","LHR"}},
    {{"FA1102","09:30","15:30","KUL","DXB"}, {"FA2102","19:00","00:30+1","DXB","LHR"}},
    {{"FA1103","12:00","18:00","KUL","DXB"}, {"FA2103","21:00","02:30+1","DXB","LHR"}},
    {{"FA1104","14:40","20:40","KUL","DXB"}, {"FA2104","23:45","05:00+1","DXB","LHR"}},
    {{"FA1105","17:10","23:10","KUL","DXB"}, {"FA2105","01:30","07:30+1","DXB","LHR"}}
};

Flight SIN[5][2] = {
    {{"FA1201","06:00","07:00","KCH","KUL"}, {"FA2201","09:30","10:45","KUL","SIN"}},
    {{"FA1202","08:15","09:15","KCH","KUL"}, {"FA2202","11:00","12:15","KUL","SIN"}},
    {{"FA1203","10:30","11:30","KCH","KUL"}, {"FA2203","13:00","14:15","KUL","SIN"}},
    {{"FA1204","12:45","13:45","KCH","KUL"}, {"FA2204","15:30","16:45","KUL","SIN"}},
    {{"FA1205","15:00","16:00","KCH","KUL"}, {"FA2205","17:45","19:00","KUL","SIN"}}
};

// CHINA direct
Flight CHINA[5][2] = {
    {{"FA1301","07:30","15:30","KUL","PEK"}, {""}},
    {{"FA1302","09:45","17:45","KUL","PVG"}, {""}},
    {{"FA1303","12:00","20:00","KUL","CAN"}, {""}},
    {{"FA1304","14:15","22:15","KUL","SHE"}, {""}},
    {{"FA1305","18:00","02:00+1","KUL","HKG"}, {""}}
};

Flight SYD[5][2] = {
    {{"FA1401","06:00","08:00","KCH","KUL"}, {"FA2401","10:30","21:30","KUL","SYD"}},
    {{"FA1402","08:30","10:30","KCH","KUL"}, {"FA2402","13:00","00:00+1","KUL","SYD"}},
    {{"FA1403","11:00","13:00","KCH","KUL"}, {"FA2403","15:30","02:30+1","KUL","SYD"}},
    {{"FA1404","13:45","15:45","KCH","KUL"}, {"FA2404","18:00","05:00+1","KUL","SYD"}},
    {{"FA1405","16:20","18:20","KCH","KUL"}, {"FA2405","20:30","07:30+1","KUL","SYD"}}
};

// ------------------------------
// Show flights
// ------------------------------
void showFlights(Flight f[][2]) {
    for (int i = 0; i < 5; i++) {
        cout << i+1 << ". " << f[i][0].flightNo << " "
             << f[i][0].origin << " -> " << f[i][0].dest
             << " (" << f[i][0].dep << " - " << f[i][0].arr << ")";

        if (f[i][1].flightNo != "") {
            cout << "\n   + " << f[i][1].flightNo << " "
                 << f[i][1].origin << " -> " << f[i][1].dest
                 << " (" << f[i][1].dep << " - " << f[i][1].arr << ")";
        }
        cout << "\n";
    }
}

void showTransit(string dest) {
    if (dest == "New York") cout << "Transit in Dubai: 4 hours\n";
    if (dest == "UK") cout << "Transit in Dubai: 3 hours\n";
    if (dest == "Sydney") cout << "Transit in KL: 2.5 hours\n";
    if (dest == "Singapore") cout << "Transit in KL: 1 hour\n";
    if (dest == "China") cout << "Direct Flight\n";
}

// ------------------------------
// SEAT SELECTION
// ------------------------------
int maxSeats(string cls) {
    if (cls == "First") return 20;
    if (cls == "Business") return 30;
    return 50;
}

bool seatTaken(vector<Passenger> &pax, int seat) {
    for (auto &p : pax)
        if (p.seat == seat) return true;
    return false;
}

// ------------------------------
// BOOK TICKET
// ------------------------------
void bookTicket() {
    cin.ignore();
    Booking b;
    b.exists = true;

    cout << "\nDestinations:\n1. New York\n2. UK\n3. Singapore\n4. China\n5. Sydney\nChoose: ";
    int d; cin >> d;

    if (d == 1) b.destination = "New York";
    if (d == 2) b.destination = "UK";
    if (d == 3) b.destination = "Singapore";
    if (d == 4) b.destination = "China";
    if (d == 5) b.destination = "Sydney";

    cout << "\nClass:\n1. First\n2. Business\n3. Economy\nChoose: ";
    int c; cin >> c;

    if (c == 1) b.travelClass = "First";
    if (c == 2) b.travelClass = "Business";
    if (c == 3) b.travelClass = "Economy";

    cout << "\nNumber of Passengers: ";
    cin >> b.passengers;

    cin.ignore();

    // Get details for each passenger
    for (int i = 0; i < b.passengers; i++) {
        Passenger p;

        cout << "\n----- Passenger " << i+1 << " -----\n";

        cout << "Enter Name: ";
        getline(cin, p.name);

        cout << "Enter Age: ";
        cin >> p.age;

        cin.ignore();
        cout << "Enter Passport: ";
        getline(cin, p.passport);

        cout << "Enter Address: ";
        getline(cin, p.address);

        // SEAT SELECTION
        int maxS = maxSeats(b.travelClass);

        while (true) {
            cout << "Select Seat (1-" << maxS << "): ";
            cin >> p.seat;

            if (p.seat < 1 || p.seat > maxS) {
                cout << "Invalid seat. Try again.\n";
                continue;
            }
            if (seatTaken(b.pax, p.seat)) {
                cout << "Seat already taken. Choose again.\n";
                continue;
            }
            break;
        }

        cin.ignore();
        b.pax.push_back(p);
    }

    // Show flights
    cout << "\nAvailable Flights:\n";
    int flightChoice;

    if (b.destination == "New York") showFlights(NY);
    if (b.destination == "UK") showFlights(UK);
    if (b.destination == "Singapore") showFlights(SIN);
    if (b.destination == "China") showFlights(CHINA);
    if (b.destination == "Sydney") showFlights(SYD);

    cout << "\nChoose flight (1-5): ";
    cin >> flightChoice;
    flightChoice--;

    // Assign flights
    if (b.destination == "New York") {
        b.leg1 = NY[flightChoice][0];
        b.leg2 = NY[flightChoice][1];
        b.aircraft = "Airbus A380";
    }
    if (b.destination == "UK") {
        b.leg1 = UK[flightChoice][0];
        b.leg2 = UK[flightChoice][1];
        b.aircraft = "Airbus A380";
    }
    if (b.destination == "Singapore") {
        b.leg1 = SIN[flightChoice][0];
        b.leg2 = SIN[flightChoice][1];
        b.aircraft = "Boeing 787-9";
    }
    if (b.destination == "China") {
        b.leg1 = CHINA[flightChoice][0];
        b.leg2 = CHINA[flightChoice][1];
        b.aircraft = "Boeing 787-9";
    }
    if (b.destination == "Sydney") {
        b.leg1 = SYD[flightChoice][0];
        b.leg2 = SYD[flightChoice][1];
        b.aircraft = "Boeing 787-9";
    }

    // Pricing
    double base = getBasePrice(b.destination, b.travelClass);
    b.basePrice = base;

    // Per-person final price (age-based handled individually)
    double tax = base * 0.10;
    double fee = base * 0.06;

    b.finalPerPerson = base + tax + fee;
    b.totalPrice = b.finalPerPerson * b.passengers;

    booking = b;
    cout << "\nBooking Successful!\n";
}

// ------------------------------
// SHOW TICKET
// ------------------------------
void showTicket() {
    if (!booking.exists) {
        cout << "No booking found.\n";
        return;
    }

    cout << "\n====== FUTURE AIR TICKET ======\n";
    cout << "Destination: " << booking.destination << "\n";
    cout << "Class: " << booking.travelClass << "\n";
    cout << "Aircraft: " << booking.aircraft << "\n\n";

    showTransit(booking.destination);

    cout << "\n----- FLIGHT DETAILS -----\n";
    cout << "Leg 1: " << booking.leg1.flightNo << " "
         << booking.leg1.origin << " -> " << booking.leg1.dest
         << " (" << booking.leg1.dep << " - " << booking.leg1.arr << ")\n";

    if (booking.leg2.flightNo != "") {
        cout << "Leg 2: " << booking.leg2.flightNo << " "
             << booking.leg2.origin << " -> " << booking.leg2.dest
             << " (" << booking.leg2.dep << " - " << booking.leg2.arr << ")\n";
    }

    cout << "\n----- PASSENGERS -----\n";
    for (int i = 0; i < booking.pax.size(); i++) {
        auto &p = booking.pax[i];

        cout << "\nPassenger " << i+1 << ":\n";
        cout << "Name: " << p.name << "\n";
        cout << "Age: " << p.age << "\n";
        cout << "Passport: " << p.passport << "\n";
        cout << "Address: " << p.address << "\n";
        cout << "Seat: " << p.seat << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nPrice Per Passenger: RM " << booking.finalPerPerson << "\n";
    cout << "Total Price: RM " << booking.totalPrice << "\n";
}

// ------------------------------
// CANCEL
// ------------------------------
void cancelBooking() {
    if (!booking.exists) {
        cout << "No booking exists.\n";
        return;
    }

    booking = Booking();
    cout << "Booking cancelled.\n";
}

// ------------------------------
// MENU
// ------------------------------
int main() {
    while (true) {
        int c;
        cout << "\n========= FUTURE AIR MENU =========\n";
        cout << "1. Book Air Tickets\n";
        cout << "2. Check Ticket Details\n";
        cout << "3. Cancel Booking\n";
        cout << "4. Exit\nChoose: ";
        cin >> c;

        if (c == 1) bookTicket();
        else if (c == 2) showTicket();
        else if (c == 3) cancelBooking();
        else if (c == 4){
            cout << "Thank you for using Future Air Booking System.\n";
            break;
        }
        else cout << "Invalid choice. Try again.\n";
    }
}
