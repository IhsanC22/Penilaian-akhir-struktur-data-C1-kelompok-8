#include <iostream>
#include <string>
#include <vector>// untuk vector
#include <algorithm> 
#include <limits> // untuk numeric_limits
using namespace std;

//menyimpan data user dan admin
struct User {
    string username;
    string password;
    string role; // Menambahkan peran (user atau admin)
};

//mengautentikasi akun
bool authenticateUser(User users[], int totalUsers, string username, string password, User& currentUser) {
    for (int i = 0; i < totalUsers; i++) {
        if (username == users[i].username && password == users[i].password) {
            currentUser = users[i];
            return true;
        }
    }
    return false;
}

//menyimpan data disk dalam program gudang
struct disc {
    string nama;
    int harga;
    int rating;
    int stok;
    string keterangan;
};

//double linked list
struct node {
    disc data;
    node* next;
    node* prev;
};

const int NO_OF_CHARS = 256;
node* HEAD = NULL;
node* TAIL = NULL;
node* mainListHead = NULL; //menyimpan data dari disk yang di pesan
node* pendingStockHead = NULL; //menyimpan data dari disk yang di pesan


//untuk fungsi search booyer more
void badCharHeuristic(const string& pat, int size, int badChar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++) {
        badChar[i] = -1;
    }
    for (int i = 0; i < size; i++) {
        badChar[pat[i]] = i;
    }
}

//fungsi search booyer more
void search(node* head, const string& caridata) {
    int m = caridata.size();
    int badChar[NO_OF_CHARS];
    badCharHeuristic(caridata, m, badChar);

    node* temp = head;
    bool found = false;
    while (temp != NULL) {
        int n = temp->data.nama.size();
        int s = 0;
        while (s <= (n - m)) {
            int j = m - 1;
            while (j >= 0 && caridata[j] == temp->data.nama[s + j]) {
                j--;
            }
            if (j < 0) {
                cout << "Nama: " << temp->data.nama << endl;
                cout << "Harga: " << temp->data.harga << endl;
                cout << "Stok: " << temp->data.stok << endl;
                cout << "Rating: " << temp->data.rating << endl;
                cout << "Keterangan: " << temp->data.keterangan << endl;
                cout << endl;
                found = true;
                break;
            } else {
                s += max(1, j - badChar[temp->data.nama[s + j]]);
            }
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "Data dengan nama '" << caridata << "' tidak ditemukan." << endl;
    }
}

// search booyer more melalui keterangan item
void searchByKeterangan(node* head, const string& cariKeterangan) {
    int m = cariKeterangan.size();
    int badChar[NO_OF_CHARS];
    badCharHeuristic(cariKeterangan, m, badChar);

    node* temp = head;
    bool found = false;
    while (temp != NULL) {
        int n = temp->data.keterangan.size();
        int s = 0;
        while (s <= (n - m)) {
            int j = m - 1;
            while (j >= 0 && cariKeterangan[j] == temp->data.keterangan[s + j]) {
                j--;
            }
            if (j < 0) {
                cout << "Nama: " << temp->data.nama << endl;
                cout << "Harga: " << temp->data.harga << endl;
                cout << "Stok: " << temp->data.stok << endl;
                cout << "Rating: " << temp->data.rating << endl;
                cout << "Keterangan: " << temp->data.keterangan << endl;
                cout << endl;
                found = true;
                break;
            } else {
                s += max(1, j - badChar[temp->data.keterangan[s + j]]);
            }
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "Data dengan keterangan '" << cariKeterangan << "' tidak ditemukan." << endl;
    }
}

//untuk swap data
void swap(node* a, node* b) {
    disc temp = a->data;
    a->data = b->data;
    b->data = temp;
}

//untuk memisahkan data
node* partition(node* low, node* high) {
    int pivot = low->data.harga;
    node* i = low;
    for (node* j = low->next; j != high; j = j->next) {
        if (j->data.harga < pivot) {
            i = i->next;
            swap(i, j);
        }
    }
    swap(i, low);
    return i;
}

//fungsi sortingan data
void quickSort(node* low, node* high) {
    if (low != high && low->next != high) {
        node* pivot = partition(low, high);
        quickSort(low, pivot);
        quickSort(pivot->next, high);
    }
}

//fungsi merging data
node* merge(node* left, node* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }
    
     node* result = NULL;
    if (left->data.nama <= right->data.nama) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    return result;
}

//untuk split data
void split(node* source, node** left, node** right) {
    node* fast;
    node* slow;
    slow = source;
    fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *left = source;
    *right = slow->next;
    slow->next = NULL;
}

//fungsi sortingan data lainnya
void mergeSort(node** head) {
    node* current = *head;
    node* left;
    node* right;

    if (current == NULL || current->next == NULL) {
        return;
    }
    split(current, &left, &right);
    mergeSort(&left);
    mergeSort(&right);
    *head = merge(left, right);
}


//untuk menambahkan data di baris pertama linked list
void addfirst(node** head, disc databaru) {
    system("cls");
    node* nodebaru = new node;
    nodebaru->data = databaru;
    nodebaru->next = *head;
    nodebaru->prev = NULL;
    if (*head != NULL) {
        (*head)->prev = nodebaru;
    }
    *head = nodebaru;
}

//untuk menambahkan data di baris terakhir linked list
void addlast(node** head, node** tail, disc databaru) {
    system("cls");
    node* nodebaru = new node;
    nodebaru->data = databaru;
    nodebaru->next = NULL;
    if (*head == NULL) {
        nodebaru->prev = NULL;
        *head = nodebaru;
        *tail = nodebaru;
        return;
    }
    node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nodebaru;
    nodebaru->prev = temp;
    *tail = nodebaru; 
}

//untuk menambahkan data di baris spesifik linked list
void addMiddle(node** head, string nama, int stok, int harga, int rating, string keterangan) {
    system("cls");
    if (*head == NULL) {
        cout << "List masih kosong. Tambahkan sebagai data pertama." << endl;
        return;
    }

    int posisi;
    cout << "Masukkan posisi data baru: ";
    cin >> posisi;

    if (posisi <= 1) {
        // Menambahkan sebagai data pertama
        addfirst(head, {nama, harga, rating, stok, keterangan});
        cout << "Data berhasil ditambahkan sebagai data pertama." << endl;
    } else {
        node* temp = *head;
        int count = 1;
        while (temp->next != NULL && count < posisi - 1) {
            temp = temp->next;
            count++;
        }

        if (temp->next == NULL) {
            // Menambahkan sebagai data terakhir
            addlast(head, &TAIL, {nama, harga, rating, stok, keterangan});
            cout << "Data berhasil ditambahkan sebagai data terakhir." << endl;
        } else {
            // Menambahkan di tengah-tengah
            node* newNode = new node;
            newNode->data.nama = nama;
            newNode->data.harga = harga;
            newNode->data.rating = rating;
            newNode->data.stok = stok;
            newNode->data.keterangan = keterangan;
            newNode->next = temp->next;
            newNode->prev = temp;
            temp->next->prev = newNode;
            temp->next = newNode;
            cout << "Data berhasil ditambahkan di posisi ke-" << posisi << "." << endl;
        }
    }
    cout << "Tekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}

//untuk menampilkan data linked list
void tampil(node* head) {
	system("cls");
    if (head == NULL) {
        cout << "List masih kosong" << endl;
        return;
    }
    while (head != NULL) {
        cout << "Nama: " << head->data.nama << ", Harga: " << head->data.harga << ".RP" << ", Rating: " << head->data.rating << ", Stok: " << head->data.stok << ", Keterangan: " << head->data.keterangan << endl;
        head = head->next;
    }
    cout << endl;
}

//untuk menghapus baris pertama linked list
void hapusawal(node** head) {
    system("cls");
    if (*head == NULL) {
        cout << "List masih kosong" << endl;
        return;
    }
    if ((*head)->next != NULL) {
        (*head)->next->prev = NULL;
    }
    *head = (*head)->next;
    cout << "DATA PERTAMA BERHASIL TERHAPUS " << endl;
}

//untuk menghapus baris akhir linked list
void hapusakhir(node** head, node** tail) {
    system("cls");
    if (*head == NULL) {
        cout << "List kosong" << endl;
        return;
    }
    if ((*head)->next == NULL) {
        delete *head;
        *head = NULL;
        *tail = NULL;
    } else {
        node* temp = *tail;
        *tail = temp->prev;
        (*tail)->next = NULL;
        delete temp;
    }
    cout << "DATA DISK BERHASIL TERHAPUS " << endl;
}

//fungsi mengedit data dalam linked list
void rubah(node* head) {
	system("cls");
    if (head == NULL) {
        cout << "List kosong" << endl;
        return;
    }
    string cari;
    tampil(HEAD); // untuk menampilkan data yang mau di ubah
    cout << "Masukan nama data disk yang akan diubah: ";
    cin >> cari;
    while (head != NULL) {
        if (head->data.nama == cari) {
            cout << "Masukan data baru:" << endl;
            cout << "Nama: ";
            cin >> head->data.nama;
            cout << "Harga: ";
            cin >> head->data.harga;
            cout << "Rating: ";
            cin >> head->data.rating;
            cout << "Stok: ";
            cin >> head->data.stok;
            cout << "Keterangan: ";
            cin >> head->data.keterangan;
            cout << "Data berhasil diubah" << endl;
            return;
        }
        head = head->next;
    }
    cout << "Data Disk tidak ditemukan" << endl;
}

// untuk menjalankan fungsi queue yg memesan disk dan dimasukan ke dalam data sendiri untuk sementara
void addToPendingStock(node** head, string nama, int stok, int harga, int rating, string keterangan) {
    node* newNode = new node;
    newNode->data.nama = nama;
    newNode->data.harga = harga;
    newNode->data.rating = rating;
    newNode->data.stok = stok;
    newNode->data.keterangan = keterangan;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
}

//untuk memproses data yg ditambahkan dari addtopendingstock yg kemudian di masukan ke linked list utama
void processPendingStock(node** head, node** pendingStockHead) {
    if (*pendingStockHead == NULL) {
        cout << "List Pending pesanan Kosong" << endl;
        cout << "Tekan Enter untuk kembali...";
		system("pause >nul");
        return;
    }

    node* firstPendingItem = *pendingStockHead;
    node* mainListTemp = *head;
    bool found = false;
    
        cout << "Processing pending item:" << endl;
    cout << "Nama: " << firstPendingItem->data.nama << ", Stok: " << firstPendingItem->data.stok << ", Harga: " << firstPendingItem->data.harga << ", Rating: " << firstPendingItem->data.rating << ", Keterangan: " << firstPendingItem->data.keterangan << endl;

    while (mainListTemp != NULL) {
        if (mainListTemp->data.nama == firstPendingItem->data.nama) {
            mainListTemp->data.stok += firstPendingItem->data.stok;
            found = true;
            break;
        }
        if (mainListTemp->next == NULL) {
            break;
        }
        mainListTemp = mainListTemp->next;
    }

    if (!found) {
        node* newNode = new node;
        newNode->data.nama = firstPendingItem->data.nama;
        newNode->data.harga = firstPendingItem->data.harga;
        newNode->data.rating = firstPendingItem->data.rating;
        newNode->data.stok = firstPendingItem->data.stok;
        newNode->data.keterangan = firstPendingItem->data.keterangan;
        newNode->next = NULL;
        newNode->prev = mainListTemp;
        
        if (*head == NULL) {
            *head = newNode;
        } else {
            mainListTemp->next = newNode;
        }
    }

    *pendingStockHead = (*pendingStockHead)->next;
    delete firstPendingItem;

    cout << "Pesanan di proses" << endl;
    cout << "Tekan Enter untuk kembali...";
	system("pause >nul");
}

bool compareByName(const disc& a, const disc& b) {
    return a.nama < b.nama;
}

bool compareByStock(const disc& a, const disc& b) {
    return a.stok > b.stok; // Mengurutkan dari besar ke kecil
}

//untuk menampilkan data terurut melalui nama
void displaySortedDataByName(node* head) {
    vector<disc> data;
    if (head == NULL) {
        cout << "List masih kosong" << endl;
        return;
    }
    while (head != NULL) {
        data.push_back(head->data);
        head = head->next;
    }
    sort(data.begin(), data.end(), compareByName);

    for (size_t i = 0; i < data.size(); ++i) {
        cout << "Nama: " << data[i].nama << ", Harga: " << data[i].harga << ", Stok: " << data[i].stok
             << ", Rating: " << data[i].rating << ", Keterangan: " << data[i].keterangan << endl;
    }
}

//untuk menampilkan data terurut melalui jumlah stok
void displaySortedDataByStock(node* head) {
    vector<disc> data;
        if (head == NULL) {
        cout << "List masih kosong" << endl;
        return;
    }
    while (head != NULL) {
        data.push_back(head->data);
        head = head->next;
    }
    sort(data.begin(), data.end(), compareByStock);

    for (size_t i = 0; i < data.size(); ++i) {
        cout << "Nama: " << data[i].nama << ", Harga: " << data[i].harga << ", Stok: " << data[i].stok
             << ", Rating: " << data[i].rating << ", Keterangan: " << data[i].keterangan << endl;
    }
}

//untuk error handling pada jenis inputan yang salah
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//sama seperti di atas
int getMenuChoice() {
    int pilih;
    while (true) {
        cout << "Masukan pilihanmu: ";
        if (cin >> pilih) {
            break;
        } else {
            cout << "input invalid" << endl;
            clearInputBuffer();
        }
    }
    clearInputBuffer();
    return pilih;
}

void menuadmin(User currentUser){
	    // Menu hanya untuk admin
    	bool loggedIn = true;
    	int pilihan = 0;
    	string nama;
    	int stok;
    	int harga;
    	int rating;
    	string keterangan;
    	disc data1, data2;
        while (loggedIn) {
            system("cls");
            cout << "======================================" << endl;
            cout << "=== Program Gudang penyimpanan DVD ===" << endl;
            cout << "======================================" << endl;
            cout << "1. Tambahkan data disk" << endl;
            cout << "2. Hapus data disk" << endl;
            cout << "3. Ubah data disk" << endl;
            cout << "4. Tampilkan data disk" << endl;
            cout << "5. sorting data disk" << endl;
            cout << "6. searching data disk" << endl;
            cout << "7. memesan disk" << endl;
            cout << "8. memproses list pending" << endl;
            cout << "9. logout" << endl;
            cout << "10. keluar" << endl;
            cout << "=======================================" << endl;
            int pilihan = getMenuChoice();

            switch (pilihan) {
		case 1:
			{
				system("cls");
			    cout << "1. Tambahkan data disk di awal" << endl;
			    cout << "2. Tambahkan data disk di tengah" << endl;
			    cout << "3. Tambahkan data disk di akhir" << endl;
			    int pilihanadd = getMenuChoice();
			
			    switch (pilihanadd)
			    {
			        case 1:
			            cout << "Nama: ";
			            cin >> data1.nama;
			            cout << "Harga: ";
			            cin >> data1.harga;
			            cout << "Rating: ";
			            cin >> data1.rating;
			            cout << "Stok: ";
			            cin >> data1.stok;
			            cout << "Keterangan: ";
			            cin >> data1.keterangan;
			            addfirst(&HEAD, data1);
			            cout << "DATA PERTAMA BERHASIL DITAMBAHKAN" << endl;
			            cout << "Tekan Enter untuk kembali...";
			            cin.ignore();
			            cin.get();
			            break;
					case 2: {
							string nama;
							int stok, harga, rating;
						    string keterangan;
							
						    cout << "Nama: ";
						    cin >> nama;
						    cout << "Harga: ";
						    cin >> harga;
						    cout << "Rating: ";
						    cin >> rating;
						    cout << "Stok: ";
						    cin >> stok;
						    cout << "Keterangan: ";
						    cin >> keterangan;
							
						    addMiddle(&HEAD, nama, stok, harga, rating, keterangan);
							cout << "DATA BERHASIL DITAMBAHKAN" << endl;
							cout << "Tekan Enter untuk kembali...";
							cin.ignore();
							cin.get();
							break;
					}
			        case 3:
			            cout << "Nama: ";
			            cin >> data2.nama;
			            cout << "Harga: ";
			            cin >> data2.harga;
			            cout << "Rating: ";
			            cin >> data2.rating;
			            cout << "Stok: ";
			            cin >> data2.stok;
			            cout << "Keterangan: ";
			            cin >> data2.keterangan;
			            addlast(&HEAD, &TAIL, data2);
			            cout << "DATA BERHASIL DITAMBAHKAN" << endl;
			            cout << "Tekan Enter untuk kembali...";
			            cin.ignore();
			            cin.get();
			            break;
			        default:
			            cout << "Pilihan tidak valid" << endl;
			            break;
			    }
			    break;
			}
		case 2:
		{
			system("cls");
		    cout << "1. Hapus data disk pertama" << endl;
		    cout << "2. Hapus data disk terakhir" << endl;
		    int deleteOption = getMenuChoice();
		
		    switch (deleteOption)
		    {
		        case 1:
		            hapusawal(&HEAD);
		            cout << "Data pertama berhasil dihapus." << endl;
		            break;
		        case 2:
		            hapusakhir(&HEAD, &TAIL);
		            cout << "Data terakhir berhasil dihapus." << endl;
		            break;
		        default:
		            cout << "Pilihan tidak valid." << endl;
		            break;
		    }
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
		}
        case 3:
            rubah(HEAD);
            cout << "Tekan Enter untuk kembali...";
			cin.ignore();
			cin.get();
            break;
		case 4:
    		tampil(HEAD);
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul"); // pause program dan menunggu input user
    		break;
		case 5:
		{
			system("cls");
		    cout << "1. Sorting data disk berdasarkan nama" << endl;
		    cout << "2. Sorting data disk berdasarkan harga" << endl;
		    int pilihansort = getMenuChoice();
		
		    switch (pilihansort)
		    {
		        case 1:
		            mergeSort(&HEAD);
		            tampil(HEAD);
		            cout << "Data telah diurutkan menurut nama." << endl;
		            break;
		        case 2:
		            quickSort(HEAD, NULL);
		            tampil(HEAD);
		            cout << "Data telah diurutkan menurut harga." << endl;
		            break;
		        default:
		            cout << "Pilihan tidak valid." << endl;
		            break;
		    }
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
		}
		case 6:
		{
			system("cls");
		    cout << "1. Searching data disk berdasarkan nama" << endl;
		    cout << "2. Searching data disk berdasarkan keterangan" << endl;
		    int pilihansearch = getMenuChoice();
		
		    switch (pilihansearch)
		    {
		        case 1:
		        {
		            string cari_nama;
		            cout << "Masukkan Nama Disk yang ingin dicari : ";
		            cin.ignore();
		            getline(cin, cari_nama);
		            search(HEAD, cari_nama);
		            break;
		        }
		        case 2:
		        {
		            string cari_keterangan;
		            cout << "Masukkan Keterangan Disk yang ingin dicari : ";
		            cin.ignore();
		            getline(cin, cari_keterangan);
		            searchByKeterangan(HEAD, cari_keterangan);
		            break;
		        }
		        default:
		            cout << "Pilihan tidak valid." << endl;
		            break;
		    }
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
		}
   		case 7:
   			cout << "Nama: ";
            cin >> nama;
            cout << "Harga: ";
            cin >> harga;
            cout << "Rating: ";
            cin >> rating;
            cout << "Stok: ";
            cin >> stok;
            cout << "Keterangan: ";
            cin >> keterangan;
            addToPendingStock(&pendingStockHead, nama, stok, harga, rating, keterangan);
            cout << "Pending stock item ditambahkan." << endl;
            break;
    	case 8:
    		system("cls");
       		processPendingStock(&HEAD, &pendingStockHead);
        	break;
        case 9:
            loggedIn = false; // fungsi Logout 
            cout << "Anda berhasil logout." << endl;
            break;
        case 10:
        	exit(0);
        default:
            cout << "Pilihan tidak valid" << endl;
            }
        }
}

void menupegawai(User currentUser){
	//menu hanya untuk pegawai
	bool loggedIn = true;
    int pilihan = 0;
    string nama;
    int stok;
    int harga;
    int rating;
    string keterangan;
    disc data1, data2;
	while (loggedIn) {
	system("cls");
	cout << "==============================" << endl;
	cout << "=== Gudang penyimpanan DVD ===" << endl;
    cout << "======= Menu Pegawai =========" << endl;
    cout << "==============================" << endl;
    cout << "1. Tampilkan data disk" << endl;
    cout << "2. Tampilkan stok disk" << endl;
    cout << "3. Searching data disk" << endl;
    cout << "4. proses list pending" << endl;
    cout << "5. logout" << endl;
    cout << "6. keluar" << endl;
    cout << "==============================" << endl;
    int pilihan = getMenuChoice();

    switch (pilihan) {
        case 1:
        	// menampilkan data yg di urutkan melalui nama
        	system("cls");
            displaySortedDataByName(HEAD);
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
        case 2: {
            // menampilkan data yg di urutkan melalui jumlah stok
            system("cls");
            displaySortedDataByStock(HEAD);
            cout << "Data telah diurutkan menurut stok." << endl;
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
        }
		case 3:
		{
			system("cls");
		    cout << "1. Searching data disk berdasarkan nama" << endl;
		    cout << "2. Searching data disk berdasarkan keterangan" << endl;
		    int pilihansearch = getMenuChoice();
		
		    switch (pilihansearch)
		    {
		        case 1:
		        {
		            string cari_nama;
		            cout << "Masukkan Nama Disk yang ingin dicari : ";
		            cin.ignore();
		            getline(cin, cari_nama);
		            search(HEAD, cari_nama);
		            break;
		        }
		        case 2:
		        {
		            string cari_keterangan;
		            cout << "Masukkan Keterangan Disk yang ingin dicari : ";
		            cin.ignore();
		            getline(cin, cari_keterangan);
		            searchByKeterangan(HEAD, cari_keterangan);
		            break;
		        }
		        default:
		            cout << "Pilihan tidak valid." << endl;
		            break;
		    }
    		cout << "Tekan Enter untuk kembali...";
    		system("pause >nul");
    		break;
		}
        case 4:
        	system("cls");
       		processPendingStock(&HEAD, &pendingStockHead);
        	break;
        case 5:
            loggedIn = false; // Logout dan kembali ke menu login
            cout << "Anda berhasil logout." << endl;
            break;
        case 6:
        	exit(0);
        default:
            cout << "Pilihan tidak valid." << endl;
    }
}
}

//menu untuk login
void menuLogin(User users[], int& totalUsers) {
    while (true) {
    	system("cls");
        cout << "=== Selamat Datang! ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Registrasi user baru" << endl;
        cout << "3. Keluar" << endl;
        cout << "=======================" << endl;
        int pilih = getMenuChoice();

        switch (pilih) {
            case 1: {
                string enteredUsername, enteredPassword;
                User currentUser;

                cout << "Username: ";
                cin >> enteredUsername;
                cout << "Password: ";
                cin >> enteredPassword;

                if (authenticateUser(users, totalUsers, enteredUsername, enteredPassword, currentUser)) {
                    cout << "Login berhasil. Selamat datang, " << currentUser.username << "!" << endl;
                    if (currentUser.role == "user") {
                        // Panggil menupegawai(currentUser) untuk user
                        menupegawai(currentUser);
                    } else if (currentUser.role == "admin") {
                        // Panggil menuadmin(currentUser) untuk admin
                        menuadmin(currentUser);
                    }
                } else {
                    cout << "Login gagal. Periksa username dan password." << endl;
                }
                break;
            }
            case 2: {
                string newUsername, newPassword;
                cout << "Masukkan username baru: ";
                cin >> newUsername;
                cout << "Masukkan password baru: ";
                cin >> newPassword;

                // Tambahkan akun baru ke dalam array users
                users[totalUsers] = {newUsername, newPassword, "user"};
                totalUsers++;

                cout << "Registrasi berhasil. Akun " << newUsername << " telah dibuat." << endl;
                break;
            }
            case 3:
                exit(0); // Keluar dari program
            default:
                cout << "Pilihan tidak valid." << endl;
        }
    }
}

int main() {
    User users[100] = {
        {"guna", "123", "user"},     // Hak akses user
        {"adman", "456", "admin"}    // Hak akses admin
    };
    int totalUsers = 2;

    menuLogin(users, totalUsers);

    return 0;
}

