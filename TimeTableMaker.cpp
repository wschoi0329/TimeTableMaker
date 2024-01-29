#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include <queue>

using namespace std;

class Subject {
private:
    string name; // ���Ǹ�
    int credit; // ����
    int* days; // �������Ͽ� ����°�
    float* start; // ���� �ð�
    float* end; // �� �ð�
    int count; // �����Ͽ� ��� ����°�

public:
    Subject(string name, int count, int credit) : name(name), count(count), credit(credit) {}
    Subject(string name, int day1, float start1, float end1, int count, int credit) : Subject(name, count, credit) {
        days = new int[count];
        start = new float[count];
        end = new float[count];
        days[0] = day1;
        start[0] = start1;
        end[0] = end1;
    }
    Subject(string name, int day1, float start1, float end1, int day2, float start2, float end2, int count, int credit) : Subject(name, count, credit) {
        days = new int[count];
        start = new float[count];
        end = new float[count];
        days[0] = day1;
        days[1] = day2;
        start[0] = start1;
        start[1] = start2;
        end[0] = end1;
        end[1] = end2;
    }
    Subject(string name, int* days, float* start, float* end, int count, int credit)
        : name(name), days(days), start(start), end(end), count(count), credit(credit) {
    }
    string getName() { return name; }
    int getCount() { return count; }
    int* getDays() { return days; }
    float* getStart() { return start; }
    int getCredit() { return credit; }
    float* getEnd() { return end; }




};

class Manager {
private:
    vector<Subject*> subs;//����� ����
    vector<string> subsnames;//�����̸��� ����, �ߺ� üũ, ���� ��ȣ �ο���
    vector<int>* groups;
    vector<Subject*>* vecarr;

    int groupnum;
    vector<vector<Subject*>> sub_tables;
    vector<int**>check_tables;
    vector<Subject*> esubs;

    bool option_check[3] = { false,false,false };//�ɼ� ���� ������� ���� ����, �ִ� ���� �ð�, �ִ� ���� ��
    vector<int> emptydays;//�ɼ�1 ��������
    float maxempty;//�ɼ�2 �ִ� ���� �ð�
    int maxnum;//�ɼ�3 �ִ� ���� ��
    vector<Subject*> present_table;
    int present_check[22][5];

public:

    void play() {
        while (true) {
            cout << "1) ���� ����   2) �ð�ǥ ����   3) ����\n\n";
            cout << "==> ";

            int menu;
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }

            }

            if (1 <= menu && menu <= 3) {
                switch (menu) {
                case 1:
                    subject_manage();
                    break;
                case 2:
                    table_manage();
                    break;
                case 3:
                default:
                    cout << "���α׷� �����մϴ�." << endl;
                    return;
                }
            }
            else {
                cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
            }

        }
    }

    void group_make() {
        vector<int> vec;
        while (true) {
            int count = 1;
            string input;
            cout << "\n============���� ��ϵ� ���� ��Ȳ============" << endl << endl;
            for (int i = 0; i < subsnames.size(); i++) {
                cout << "���� �̸�: " << subsnames.at(i) << " / ���� ��ȣ: " << (i + 1) << endl;

            }
            cout << "\n=============================================" << endl << endl;
            cout << "������ ���Ŀ� �°� ������� �׷��� ���� �Է��� �ּ��� " << endl;
            cout << "ex) 1,2/3,4/5,6,7/8,9,10,11" << endl;
            getline(cin, input);
            int i;
            for (i = 0; i < input.size(); i++) {
                if (input.at(i) == ',') {
                    input.replace(i, 1, " ");
                }
                else if (input.at(i) == '/') {
                    input.replace(i, 1, " -1 ");
                    count++;
                }
            }

            groupnum = count; // group����
            stringstream ss;
            ss.str(input);
            int num;
            bool check = true;
            bool* checkarr = new bool[subsnames.size()];
            for (int i = 0; i < subsnames.size(); i++)
                checkarr[i] = true;
            while (ss >> num) {
                if (1 <= num && num <= subsnames.size()) {
                    vec.push_back((num - 1));
                    if (checkarr[num - 1] == false) {
                        check = false;
                        break;
                    }
                    checkarr[num - 1] = false;
                }
                else if (num == -1)
                    vec.push_back(num);
                else {
                    check = false;
                    break;
                }
            }
            /*for (int i = 0; i < subsnames.size(); i++) {
               if (checkarr[i]) {
                check = false;
               }
            }*/
            if (check == false) {
                cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                vec.clear();
                delete[] checkarr;
                continue;
            }
            if (check)
                break;
        }
        vecarr = new vector<Subject*>[groupnum];
        int index = 0;
        for (int i = 0; i < vec.size(); i++) {
            if (vec.at(i) == -1) {
                index++;
                continue;
            }
            for (int k = 0; k < subs.size(); k++) {
                if (subs.at(k)->getName() == subsnames.at(vec.at(i))) {
                    vecarr[index].push_back(subs.at(k));
                }
            }
        }
    }

    void subject_manage() {
        while (true) {
            int menu;
            cout << "\n[�������]\n\n1) ���� �߰�\t2) ���� ����\t3) ���� ��Ȳ\t4) ���� ���� ����" << endl;
            cout << "\n==> ";
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
            }
            else {
                getchar();
            }

            if (1 <= menu && menu <= 4) {
                switch (menu) {
                case 1:
                    cout << endl;
                    add_subject();
                    break;
                case 2:
                    delete_subject();
                    break;
                case 3:
                    list_subject();
                    break;
                case 4:
                default:
                    cout << '\n';
                    return;

                }
            }
            else {
                cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
            }

        }

    }

    void add_subject() {
        while (true) {
            string subname;

            int credit;
            bool check = false;
            int count;
            int* days;
            float* start;
            float* end;

            cout << "���� �̸��� �Է��ϼ���.(�Է� �����Ϸ��� q�� �Է��ϼ���.)==> ";
            getline(cin, subname);


            if (subname == "q") {
                cout << "\n���� �Է��� �����մϴ�" << endl;
                return;
            }

            if (subname.find(' ') != string::npos || subname == "") {//���� �ϳ��� ������ ����
                cout << "\n������ ������ �ʽ��ϴ�. �ٽ� �Է����ּ���\n\n";

            }
            else {
                for (int i = 0; i < subsnames.size(); i++) {//�̹� ��ϵ� ���� �� �̸� ���� ���� �ִ��� üũ
                    if (subname == subsnames.at(i)) check = true;
                }
                if (!check) {//���� ���� ���� ��� -> ���� �̸� ���Ϳ� �̸� �߰��ؾ���
                    subsnames.push_back(subname);
                }

                while (true) {

                    cout << "������ �Է��ϼ���(1~12)==> ";
                    cin >> credit;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (credit != INT_MAX && credit != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                    }
                    else {
                        if (!(1 <= credit && credit <= 12)) {
                            cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                        }
                        else {
                            break;
                        }
                    }
                }

                while (true) {
                    cout << "�����Ͽ� �� �� ������� �Է��ϼ���.==> ";
                    cin >> count;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (count != INT_MAX && count != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                    }
                    else {

                        if (!(1 <= count && count <= 5)) {
                            cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                        }
                        else {
                            break;
                        }
                    }
                }


                days = new int[count];
                start = new float[count];
                end = new float[count];

                getchar();
                for (int i = 0; i < count; i++) {
                    stringstream ss;
                    cout << i + 1 << "��° ���ϰ� �ð��� �Է��ϼ���.(��: �� 12~13.5)==> ";
                    string input;
                    getline(cin, input);
                    if (input.find('~') == -1) {//~�� �������� ����
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    ss.str(input);
                    string day;
                    ss >> day;
                    if (dayToInt(day) == -1) {
                        cout << "������ Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    days[i] = dayToInt(day);
                    string temp;

                    getline(ss, temp, '~');
                    if (!(stringstream(temp) >> start[i])) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }

                    getline(ss, temp);
                    if (!(stringstream(temp) >> end[i])) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }

                    if (!(start[i] - (int)start[i] == 0.5 || start[i] - (int)start[i] == 0)) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    if (!(end[i] - (int)end[i] == 0.5 || end[i] - (int)end[i] == 0)) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    if (!(9 <= start[i] && start[i] <= 20 && 9 <= end[i] && end[i] <= 20)) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    else if (start[i] == end[i] || start[i] > end[i]) {
                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                        i--;
                        continue;
                    }
                    else {
                        int check = false;
                        for (int j = 0; j < i; j++) {
                            if (days[j] == days[i]) {
                                if (start[i] < start[j]) {
                                    float temp = start[i];
                                    while (temp < end[i]) {
                                        temp += 0.5;
                                        if (temp == start[j]) {
                                            check = true;
                                            break;
                                        }
                                    }
                                    if (check) break;
                                }
                                else if (start[i] > start[j]) {
                                    float temp = start[j];
                                    while (temp < end[j]) {
                                        temp += 0.5;
                                        if (temp == start[i]) {
                                            check = true;
                                            break;
                                        }
                                    }
                                    if (check) break;
                                }
                                else if (start[i] == start[j]) {
                                    check = true;
                                    break;
                                }
                            }
                        }
                        if (check) {
                            cout << "�ð��� Ȯ�����ּ���.\n\n";
                            i--;
                            continue;
                        }
                    }

                    cout << "\n����:" << day << endl;
                    cout << "�ð�:" << start[i] << "~" << end[i] << "\n\n";

                    if (check) {
                        for (int j = 0; j < subs.size(); j++) {
                            if (subs.at(j)->getName() == subname) {
                                for (int k = 0; k < subs.at(j)->getCount(); k++) {
                                    for (int l = 0; l < count; l++) {
                                        if (subs.at(j)->getDays()[k] == days[l]) {//���� ����
                                            if (subs.at(j)->getStart()[k] == start[l]) {
                                                //���۽ð� ������ �ߺ����� ����
                                                cout << "���۽ð��� ���� " << subname << "������ �̹� �ֽ��ϴ�." << endl;
                                                return;//�������� ���ư�
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }



                }
                subs.push_back(new Subject(subname, days, start, end, count, credit));
                cout << "=====���� ��� �Ϸ�=====\n\n";
            }

        }



    }

    int dayToInt(string day) {
        if (day == "��") {
            return 0;
        }
        else if (day == "ȭ") {
            return 1;
        }
        else if (day == "��") {
            return 2;
        }
        else if (day == "��") {
            return 3;
        }
        else if (day == "��") {
            return 4;
        }
        else {
            return -1;
        }

    }

    string IntToDay(int d) {
        switch (d) {
        case 0:
            return "��";
        case 1:
            return "ȭ";
        case 2:
            return "��";
        case 3:
            return "��";
        case 4:
            return "��";
        default:
            return NULL;
        }

    }

    void table_manage() {
        while (true) {
            int menu;
            cout << "\n[�ð�ǥ ����]\n\n1) �ð�ǥ �����   2) ���� �ð�ǥ ����   3) ���� �ð�ǥ ���� �߰�/����   4) �ð�ǥ ���� ����\n\n";
            cout << "==> ";
            cin >> menu;
            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
            }
            if (1 <= menu && menu <= 4) {
                switch (menu) {
                case 1:
                    choose_option();
                    break;
                case 2:
                    print_table();
                    break;
                case 3:
                {
                    cout << "\n1) ���� �ð�ǥ ���� �߰� 2) ���� �ð�ǥ ���� ����\n==>";
                    int menu;
                    cin >> menu;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (menu != INT_MAX && menu != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                    }
                    else {
                        if (menu == 1) {
                            add_present_table();
                        }
                        else if (menu == 2) {
                            delete_present_table();
                        }
                        else {
                            cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                        }
                    }

                    break;
                }

                case 4:
                default:
                    cout << '\n';
                    return;

                }
            }
            else {
                cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
            }

        }
    }

    void delete_present_table() {
        while (true) {
            if (present_table.size() == 0 && esubs.size() == 0) {
                cout << "�� ���� �ð�ǥ�� �����ϴ�. �ð�ǥ�� ������ּ���. ��\n\n";
                return;
            }
            print_table();
            cout << "1) ���� ���� ���� 2) e���� ���� ���� 3)���� ���� ����\n==>";
            int menu;
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";

            }
            else {
                getchar();
                if (menu == 1) {
                    if (present_table.size() == 0) {
                        cout << "�� ���� �ð�ǥ�� ���� ������ �����ϴ�.  ��\n\n";
                        continue;
                    }
                    int t;
                    cout << "������ ������ ��ȣ�� �Է��ϼ���\n==> ";
                    cin >> t;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (t != INT_MAX && t != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "�߸��� �Է��Դϴ�.\n\n";
                    }
                    else if (!(1 <= t && t <= present_table.size())) {
                        cout << "���� �����ȣ�Դϴ�.\n\n" << endl;
                    }
                    else {
                        t--;
                        cout << "\n" << present_table.at(t)->getName() << " ������ �����Ǿ����ϴ�.\n\n";
                        for (int j = 0; j < present_table.at(t)->getCount(); j++) {
                            int start = (present_table.at(t)->getStart()[j] - 9) / 0.5;
                            int end = (present_table.at(t)->getEnd()[j] - 9) / 0.5;
                            int day = present_table.at(t)->getDays()[j];
                            while (start < end) {
                                present_check[start][day] = -1;
                                start++;
                            }
                        }
                        present_table.erase(present_table.begin() + t);

                    }

                }
                else if (menu == 2) {
                    if (esubs.size() == 0) {
                        cout << "�� ���� �ð�ǥ�� e���� ������ �����ϴ�.  ��\n\n";
                        continue;
                    }
                    int t;
                    cout << "������ ������ ��ȣ�� �Է��ϼ���\n==> ";
                    cin >> t;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (t != INT_MAX && t != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "�߸��� �Է��Դϴ�.\n\n";
                    }
                    else if (!(1 <= t && t <= esubs.size())) {
                        cout << "���� �����ȣ�Դϴ�.\n\n" << endl;
                    }
                    else {
                        t--;
                        cout << "\n" << esubs.at(t)->getName() << " ������ �����Ǿ����ϴ�.\n\n";
                        esubs.erase(esubs.begin() + t);

                    }
                }
                else if (menu == 3) {
                    return;
                }
                else {
                    cout << "\n���� �޴��Դϴ�. �ٽ� �Է����ּ���.\n\n";
                }

            }
        }
    }

    void add_present_table() {

        while (true) {
            if (present_table.size() == 0) {
                cout << "�� ���� �ð�ǥ�� �����ϴ�. �ð�ǥ�� ������ּ���. ��\n\n";
                return;
            }
            print_table();
            cout << "1) ���� ���� �߰� 2) e���� ���� �߰� 3)���� �߰� ����\n==>";
            int menu;
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";

            }
            else {
                if (menu == 1) {
                    string subname;

                    int credit;
                    bool check = false;
                    int count;
                    int* days;
                    float* start;
                    float* end;

                    getchar();
                    cout << "\n���� �̸��� �Է��ϼ���.==> ";
                    getline(cin, subname);

                    if (subname.find(' ') != string::npos || subname == "") {//���� �ϳ��� ������ ����
                        cout << "\n������ ������ �ʽ��ϴ�. �ٽ� �Է����ּ���\n\n";

                    }
                    else {
                        for (int i = 0; i < present_table.size(); i++) {//�̹� ��ϵ� ���� �� �̸� ���� ���� �ִ��� üũ
                            if (subname == present_table.at(i)->getName()) check = true;
                        }
                        if (check) {//���� ���� ���� ��� -> ���� �̸� ���Ϳ� �̸� �߰��ؾ���
                            cout << "\n" << subname << " ������ �̹� �ִ� �����Դϴ�.\n\n";

                        }
                        else {
                            //���� �Է�
                            while (true) {

                                cout << "\n������ �Է��ϼ���(1~12)==> ";
                                cin >> credit;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (credit != INT_MAX && credit != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                }
                                else {
                                    if (!(1 <= credit && credit <= 12)) {
                                        cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            //�����Ͽ� ��� ������� �Է�
                            while (true) {
                                cout << "\n�����Ͽ� �� �� ������� �Է��ϼ���.==> ";
                                cin >> count;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (count != INT_MAX && count != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                }
                                else {
                                    if (!(1 <= count && count <= 5)) {
                                        cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            days = new int[count];
                            start = new float[count];
                            end = new float[count];
                            getchar();
                            for (int i = 0; i < count; i++) {
                                stringstream ss;
                                cout << "\n" << i + 1 << "��° ���ϰ� �ð��� �Է��ϼ���.(��: �� 12~13.5)==> ";
                                string input;
                                getline(cin, input);
                                if (input.find('~') == -1) {//~�� �������� ����
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }
                                ss.str(input);
                                string day;
                                ss >> day;
                                if (dayToInt(day) == -1) {
                                    cout << "������ Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }
                                days[i] = dayToInt(day);
                                string temp;

                                getline(ss, temp, '~');
                                if (!(stringstream(temp) >> start[i])) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }

                                getline(ss, temp);

                                //�ð� ���� üũ
                                if (!(stringstream(temp) >> end[i])) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }

                                if (!(start[i] - (int)start[i] == 0.5 || start[i] - (int)start[i] == 0)) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }
                                if (!(end[i] - (int)end[i] == 0.5 || end[i] - (int)end[i] == 0)) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }

                                if (!(9 <= start[i] && start[i] <= 20 && 9 <= end[i] && end[i] <= 20)) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }
                                else if (start[i] == end[i] || start[i] > end[i]) {
                                    cout << "�ð��� Ȯ�����ּ���.\n\n";
                                    i--;
                                    continue;
                                }
                                else {//�� �� ������ �ִٸ� �� �ð��̶� ��ġ���� üũ

                                    for (int j = 0; j < i; j++) {
                                        if (days[j] == days[i]) {
                                            if (start[i] < start[j]) {
                                                float temp = start[i];
                                                while (temp < end[i]) {
                                                    temp += 0.5;
                                                    if (temp == start[j]) {
                                                        check = true;
                                                        break;
                                                    }
                                                }
                                                if (check) break;
                                            }
                                            else if (start[i] > start[j]) {
                                                float temp = start[j];
                                                while (temp < end[j]) {
                                                    temp += 0.5;
                                                    if (temp == start[i]) {
                                                        check = true;
                                                        break;
                                                    }
                                                }
                                                if (check) break;
                                            }
                                            else if (start[i] == start[j]) {
                                                check = true;
                                                break;
                                            }
                                        }
                                    }
                                    if (check) {
                                        cout << "�ð��� Ȯ�����ּ���.\n\n";
                                        i--;
                                        continue;
                                    }
                                }


                                int s = (start[i] - 9) / 0.5;
                                int e = (end[i] - 9) / 0.5;
                                int d = days[i];
                                while (s < e) {
                                    if (present_check[s][d] == -1) {
                                        s++;
                                    }
                                    else {
                                        break;
                                    }
                                }
                                if (s != e) {
                                    cout << "\n�ð��� �ߺ��Ǵ� ������ �־� �߰��� �� �����ϴ�.\n\n";
                                    check = true;
                                    break;
                                }

                                cout << "\n����:" << day << endl;
                                cout << "�ð�:" << start[i] << "~" << end[i] << "\n\n";

                            }

                            if (!check) {
                                int subnum = present_table.size();
                                for (int i = 0; i < count; i++) {
                                    int s = (start[i] - 9) / 0.5;
                                    int e = (end[i] - 9) / 0.5;
                                    int d = days[i];
                                    while (s < e) {
                                        present_check[s][d] = subnum;
                                        s++;
                                    }
                                }

                                present_table.push_back(new Subject(subname, days, start, end, count, credit));
                                cout << "=====���� ��� �Ϸ�=====\n\n";
                            }

                        }











                    }

                }
                else if (menu == 2) {
                    string subname;
                    int credit;
                    bool check = false;
                    cout << "\n���� �̸��� �Է��ϼ���.==> ";
                    getline(cin, subname);
                    if (subname.find(' ') != string::npos || subname == "") {//���� �ϳ��� ������ ����
                        cout << "\n������ ������ �ʽ��ϴ�. �ٽ� �Է����ּ���\n\n";

                    }
                    else {
                        if (esubs.size() != 0) {
                            for (int i = 0; i < esubs.size(); i++) {
                                if (subname == esubs[i]->getName()) {
                                    check = true;
                                    cout << "\n���� �̸��� " << subname << "�� e���� ������ �̹� �ֽ��ϴ�.\n\n";
                                    break;
                                }
                            }
                        }
                        if (!check) {
                            while (true) {

                                cout << "\n������ �Է��ϼ���(1~12)==> ";
                                cin >> credit;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (credit != INT_MAX && credit != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                }
                                else {
                                    if (!(1 <= credit && credit <= 12)) {
                                        cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            esubs.push_back(new Subject(subname, 0, 0, 0, 0, credit));
                            cout << "\n" << subname << " ������ ��ϵǾ����ϴ�.\n\n";
                        }
                    }
                }
                else if (menu == 3) {
                    return;
                }
                else {
                    cout << "\n���� �޴��Դϴ�. �ٽ� �Է����ּ���.\n\n";
                }

            }
        }

    }

    void choose_option() {
        while (true) {
            if (subsnames.size() == 0) {
                cout << "��ϵ� ������ �����ϴ�.\n\n";
                return;
            }
            cout << "������ �ɼ��� �������ּ���. \n1) ���� ���� 2) �ִ� ���� �ð� 3) �ִ� ���� �� 4) �׷� ����(�ð�ǥ �����) \n==> ";
            int option;
            cin >> option;
            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (option != INT_MAX && option != INT_MIN) {
                    while (getchar() != '\n');
                }
            }
            if (1 <= option && option <= 4) {
                switch (option) {
                case 1:
                {
                    cout << "\n�������� �ϰ� ���� ������ �Է����ּ��� (��: �� ��)\n���� �ɼ��� �����Ͻ÷��� q�� �Է����ּ���.\n==>";
                    getchar();
                    string days;
                    getline(cin, days);
                    stringstream ss(days);
                    string temp;
                    if (days == "q") {
                        emptydays.clear();
                        option_check[0] = false;
                        cout << "\n���� �ɼ��� ���ŵǾ����ϴ�. ��� ������ ������ �ݿ��մϴ�.\n\n";
                        break;
                    }

                    if (emptydays.size() != 0) {
                        emptydays.clear();
                    }
                    while (ss >> temp) {

                        int day = dayToInt(temp);

                        if (day == -1) {
                            cout << "�߸��� �����Դϴ�.\n";
                            break;
                        }
                        else {
                            emptydays.push_back(day);
                        }
                    }
                    if (emptydays.size()) {
                        option_check[0] = true;//�ɼ� ���� �Ǿ�����
                        for (int i = 0; i < emptydays.size(); i++) {
                            cout << IntToDay(emptydays.at(i)) << " ";
                        }
                        cout << "������ �����Ǿ����ϴ�.\n";
                    }
                    else {//������ ����� 0 -> ������ �ƹ��͵� ������������.
                        cout << "���� �����Ͽ����ϴ�. �Է��� Ȯ�����ּ���.\n\n";
                    }

                    break;
                }
                case 2:
                    cout << "\n�ִ� ���� �ð��� �Է����ּ���(1~11)\n�ɼ��� �����Ͻ÷��� 0�� �Է����ּ���.\n==>";
                    cin >> maxempty;
                    if (!maxempty) {
                        option_check[1] = false;
                        cout << "\n�ִ� ���� �ð� �ɼ��� ���ŵǾ����ϴ�.\n\n";
                        break;
                    }
                    if (1 <= maxempty && maxempty <= 11) {
                        option_check[1] = true;
                        cout << "�ִ� ���� �ð� " << maxempty << "�ð��� �����Ǿ����ϴ�.\n\n";
                        maxempty *= 2;
                    }
                    else {
                        cout << "���� �����Ͽ����ϴ�. �Է��� Ȯ�����ּ���.\n\n";
                    }
                    break;
                case 3:
                    cout << "\n�ִ� ���� ���� �Է����ּ���(1~10)\n�ɼ��� �����Ͻ÷��� 0�� �Է����ּ���.\n==>";
                    cin >> maxnum;
                    if (!maxnum) {
                        option_check[2] = false;
                        cout << "\n�ִ� ���� �� �ɼ��� ���ŵǾ����ϴ�.\n\n";
                        break;
                    }
                    if (1 <= maxnum && maxnum <= 10) {
                        option_check[2] = true;
                        cout << "�ִ� ���� �� " << maxnum << "���� �����Ǿ����ϴ�.\n\n";
                    }
                    else {
                        cout << "���� �����Ͽ����ϴ�. �Է��� Ȯ�����ּ���.\n\n";
                    }
                    break;
                case 4:
                    while (getchar() != '\n');
                    group_make();
                    //cout << groupnum << endl;
                    makeTable();
                    return;
                default:
                    break;
                }
            }
            else {
                cout << "�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
            }

        }

    }

    void showSubsNames() {
        cout << "\n[��ϵ� ���� ���]" << endl;
        for (int i = 0; i < subsnames.size(); i++) {
            cout << i + 1 << ". " << subsnames.at(i) << endl;
        }
    }

    int searchSameSubjectName(int index) { // subsnames.at(index) �� ���� �̸��� ������ subs���� ã�� ������ ��ȯ.
        int ret = 0;
        string name = subsnames.at(index);
        for (int i = 0; i < subs.size(); i++) {
            if (name == subs[i]->getName()) { ret++; }
        }
        return ret;
    }

    void delete_subject() {
        if (subsnames.size() == 0) {
            cout << "\n��ϵ� ������ �����ϴ�.\n";
            return;
        }

        int t;
        showSubsNames();
        cout << endl;
        cout << "������ ������ ��ȣ�� �Է��ϼ���\n\n==> ";
        cin >> t;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            if (t != INT_MAX && t != INT_MIN) {
                while (getchar() != '\n');
            }
            cout << "�߸��� �Է��Դϴ�.\n\n";
        }
        else if (!(1 <= t && t <= subsnames.size())) {
            cout << "���� �����ȣ�Դϴ�.\n\n" << endl;
        }
        else {
            string target = subsnames.at(t - 1); // ������ ������ �̸�
            int classes_count = searchSameSubjectName(t - 1);
            if (classes_count == 0) {
                cout << "��ϵ��� ���� ������Դϴ�." << endl;
            }
            else if (classes_count == 1) {
                for (int i = 0; i < subs.size(); i++) {
                    if (subs[i]->getName() == target) { subs.erase(subs.begin() + i); }
                }
                subsnames.erase(subsnames.begin() + (t - 1));
                cout << "���� " << '\"' << target << '\"' << " ���� �Ϸ�" << endl;
            }
            else {
                int count = 0;
                for (int i = 0; i < subs.size(); i++) {
                    if (subs.at(i)->getName() == target) {
                        cout << char('A' + count++) << ". " << endl;
                        for (int k = 0; k < subs.at(i)->getCount(); k++) {
                            cout << IntToDay(subs.at(i)->getDays()[k]) << " / " << subs.at(i)->getStart()[k] << " ~ " << subs.at(i)->getEnd()[k] << endl;
                        }
                        cout << "����: " << subs.at(i)->getCredit() << endl;
                    }
                }

                string temp;
                cout << "������ �й��� �����ϼ���==> ";
                cin >> temp;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    while (getchar() != '\n');
                    cout << "�߸��� �Է��Դϴ�.\n\n";
                }
                else if (temp.size() > 1) {
                    cout << "�߸��� �Է��Դϴ�.\n\n";
                }
                else {
                    char ch = temp.at(0);
                    int temp;
                    if ('A' <= ch && ch < 'A' + classes_count) {
                        temp = ch - 'A';//�� ��° �ִ���
                    }
                    else if ('a' <= ch && ch < 'a' + classes_count) {
                        temp = ch - 'a';
                    }
                    else {
                        cout << "���� �й��Դϴ�.\n\n";
                        return;
                    }
                    int c = 0;
                    for (int i = 0; i < subs.size(); i++) {
                        if (subs.at(i)->getName() == target) {
                            if (c == temp) {
                                subs.erase(subs.begin() + i);
                                cout << target << " - " << ch << "���� �Ϸ�" << endl;
                                break;
                            }
                            else {
                                c++;
                            }
                        }
                    }
                }

            }
        }



    }

    void list_subject() {
        if (subsnames.size() == 0) {
            cout << "\n��ϵ� ������ �����ϴ�.\n";
            return;
        }
        cout << endl;

        cout << "============���� ��Ȳ============\n";
        for (int i = 0; i < subsnames.size(); i++) {
            cout << (i + 1) << ". " << subsnames.at(i) << "\n";
            int max = searchSameSubjectName(i);

            if (max == 1) {//�й� ���� ����
                for (int j = 0; j < subs.size(); j++) {
                    if (subs.at(j)->getName() == subsnames.at(i)) {
                        cout << "\t";
                        int count = subs.at(j)->getCount();
                        for (int k = 0; k < count; k++) {
                            cout << IntToDay(subs.at(j)->getDays()[k]) << " " << subs.at(j)->getStart()[k] << " ~ " << subs.at(j)->getEnd()[k];
                            if (k != count - 1) {
                                cout << " / ";
                            }
                        }
                        cout << "\n";
                        break;
                    }
                }
            }
            else {//�й� �ִ� ����
                int classcount = 0;
                for (int j = 0; j < subs.size(); j++) {
                    if (subs.at(j)->getName() == subsnames.at(i)) {
                        int count = subs.at(j)->getCount();
                        cout << "\t" << char('A' + classcount) << ". ";
                        for (int k = 0; k < count; k++) {
                            cout << IntToDay(subs.at(j)->getDays()[k]) << " " << subs.at(j)->getStart()[k] << " ~ " << subs.at(j)->getEnd()[k];
                            if (k != count - 1) {
                                cout << " / ";
                            }
                        }
                        cout << "\n";
                        classcount++;
                    }
                    if (classcount == max) {

                        break;
                    }
                }
            }

        }


        cout << "=================================\n\n";

        return;
    }

    int get_sub_num(Subject* subject) {
        for (int i = 0; i < subsnames.size(); i++) {
            if (subject->getName() == subsnames.at(i)) {
                return i;
            }
        }
    }

    // ��õ �ð�ǥ ����
    void makeTable() {
        if (sub_tables.size() != 0) {
            sub_tables.clear();
            check_tables.clear();
        }
        if (esubs.size() != 0) {
            esubs.clear();
        }
        int table[22][5];
        for (int i = 0; i < 22; i++) {
            for (int j = 0; j < 5; j++) {
                table[i][j] = -1;
            }
        }

        vector<Subject*> temp;
        search_group(table, 0, temp);

        if (sub_tables.size() == 0) {
            cout << "�� ���� �� �ִ� �ð�ǥ�� �����ϴ�. �׷�, �ɼ�, �Ǵ� ������ ������ �� ����� ������. ��\n\n";
            return;
        }

        cout << "\n================�ð�ǥ ���� �Ϸ�================\n\n";
        for (int i = 0; i < sub_tables.size(); i++) {
            cout << "-------------------" << i + 1 << "�� �ð�ǥ-------------------\n";
            int credit = 0;
            for (int j = 0; j < sub_tables[i].size(); j++) {
                cout << j + 1 << ". " << sub_tables[i][j]->getName() << "\n";
                credit += sub_tables[i][j]->getCredit();
                for (int k = 0; k < sub_tables[i][j]->getCount(); k++) {
                    cout << "\t" << IntToDay(sub_tables[i][j]->getDays()[k]) << " " << sub_tables[i][j]->getStart()[k] << "~" << sub_tables[i][j]->getEnd()[k] << "\n";
                }
                cout << "\n";
            }
            cout << "�� " << credit << " ����\n";

        }
        cout << "================================================\n";
        while (true) {
            cout << "������ ��� �ð�ǥ�� �ϳ� ����ּ���\n==>";
            int num;
            cin >> num;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (num != INT_MAX && num != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
            }
            else {
                if (!(1 <= num && num <= sub_tables.size())) {
                    cout << "\n���� �ð�ǥ�Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                    continue;
                }
                getchar();
                cout << num << "�� �ð�ǥ�� �����߽��ϴ�.\n\n";
                num--;
                if (present_table.size() != 0) {
                    present_table.clear();
                }
                for (int i = 0; i < sub_tables[num].size(); i++) {
                    present_table.push_back(sub_tables[num][i]);
                }
                for (int i = 0; i < 22; i++) {
                    for (int j = 0; j < 5; j++) {
                        present_check[i][j] = check_tables[num][i][j];
                    }
                }

                while (true) {
                    cout << "e���� ������ �߰��Ͻðڽ��ϱ�?(y/n)\n==>";

                    string temp;
                    getline(cin, temp);
                    int count = 0;
                    if (temp == "y") {
                        while (true) {
                            if (count) {
                                getchar();
                            }
                            else {
                                count++;
                            }
                            string subname;
                            int credit;
                            bool check = false;
                            cout << "\n���� �̸��� �Է��ϼ���.(�Է� �����Ϸ��� q�� �Է��ϼ���.)==> ";
                            getline(cin, subname);
                            if (subname == "q") {
                                cout << "\ne���� ���� �Է��� �����մϴ�" << endl;
                                return;
                            }
                            if (subname.find(' ') != string::npos || subname == "") {//���� �ϳ��� ������ ����
                                cout << "\n������ ������ �ʽ��ϴ�. �ٽ� �Է����ּ���\n\n";

                            }
                            else {
                                if (esubs.size() != 0) {
                                    for (int i = 0; i < esubs.size(); i++) {
                                        if (subname == esubs[i]->getName()) {
                                            check = true;
                                            cout << "\n���� �̸��� " << subname << "�� e���� ������ �̹� �ֽ��ϴ�.\n\n";
                                            break;
                                        }
                                    }
                                }
                                if (!check) {
                                    while (true) {

                                        cout << "\n������ �Է��ϼ���(1~12)==> ";
                                        cin >> credit;

                                        if (cin.fail()) {
                                            cin.clear();
                                            cin.ignore();
                                            if (credit != INT_MAX && credit != INT_MIN) {
                                                while (getchar() != '\n');
                                            }
                                            cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                        }
                                        else {
                                            if (!(1 <= credit && credit <= 12)) {
                                                cout << "\n�߸��� �Է��Դϴ�. �ٽ� Ȯ�����ּ���.\n\n";
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                    }

                                    esubs.push_back(new Subject(subname, 0, 0, 0, 0, credit));
                                    cout << "\n" << subname << " ������ ��ϵǾ����ϴ�.\n\n";
                                }
                            }


                        }
                    }
                    else if (temp == "n") {
                        break;
                    }
                    else {
                        cout << "�Է��� Ȯ�����ּ���.\n\n";
                    }
                }


                break;
            }
        }

        /*
        9~9.5   0
        9.5~10   1
        10~10.5   2
        10.5~11   3
        11~11.5   4
        11.5~12   5
        12~12.5   6
        12.5~13   7
        13~13.5   8
        13.5~14   9
        14~14.5   10
        14.5~15   11
        15~15.5   12
        15.5~16   13
        16~16.5   14
        16.5~17   15
        17~17.5   16
        17.5~18   17
        18~18.5   18
        18.5~19   19
        19~19.5   20
        19.5~20   21

        */
    }

    void print_table() {
        if (present_table.size() == 0 && esubs.size() == 0) {
            cout << "�� ���� �ð�ǥ�� �����ϴ�. �ð�ǥ�� ������ּ���. ��\n\n";
            return;
        }
        else {
            cout << "\n==========���� �ð�ǥ==========\n\n";
            int credit = 0;
            if (present_table.size() > 0) {
                for (int j = 0; j < present_table.size(); j++) {
                    credit += present_table[j]->getCredit();
                    cout << j + 1 << ". " << present_table[j]->getName() << "\n";
                    for (int k = 0; k < present_table[j]->getCount(); k++) {
                        cout << "\t" << IntToDay(present_table[j]->getDays()[k]) << " " << present_table[j]->getStart()[k] << "~" << present_table[j]->getEnd()[k] << "\n";
                    }
                    cout << "\n";
                }
                cout << "�� " << credit << " ����\n\n";
            }
            if (esubs.size() > 0) {
                int ecredit = 0;
                cout << "----------�̷��� ���----------\n\n";
                for (int i = 0; i < esubs.size(); i++) {
                    cout << i + 1 << ". " << esubs[i]->getName() << "\n\n";
                    ecredit += esubs[i]->getCredit();
                }
                cout << "\n�� " << ecredit << " ����\n\n";
                cout << "-------------------------------\n";
                cout << "\n���� " << credit + ecredit << "����\n\n";
            }
            cout << "===============================\n\n";

            return;

        }
    }

    void search_group(int table[22][5], int group, vector<Subject*> vec) {


        vector<Subject*> temp;
        int last_table[22][5];

        if (group == groupnum) {
            //�ð�ǥ �ֱ� ���� �ɼ� üũ

            //bool option_check[3] = { false,false,false }
            bool option0 = true;
            bool option1 = true;
            bool option2 = true;
            if (option_check[0]) {//���� ����

                for (int i = 0; i < emptydays.size(); i++) {
                    for (int j = 0; j < vec.size(); j++) {
                        for (int k = 0; k < vec.at(j)->getCount(); k++) {
                            if (emptydays.at(i) == vec.at(j)->getDays()[k]) {
                                option0 = false;
                                break;
                            }
                        }
                        if (!option0) {
                            break;
                        }

                    }
                    if (!option0) {
                        break;
                    }

                }

            }
            if (option0) {
                if (option_check[1]) {//�ִ� �����ð�
                    for (int i = 0; i < 5; i++) {
                        int start = -1;
                        int count = 0;
                        for (int j = 0; j < 22; j++) {
                            if (start == -1 && table[j][i] != -1) {
                                start = j;//���� ���� �ε���
                            }
                            else if (start != -1 && table[j][i] != -1) {

                                if (start + 1 == j) {
                                    start++;
                                }
                                else {//�߰��� ������ �־���
                                    if (count > maxempty) {
                                        option1 = false;
                                        break;
                                    }
                                    else {
                                        count = 0;
                                        start = j;
                                    }

                                }
                            }
                            else if (start != -1 && table[j][i] == -1) {
                                count++;
                            }

                        }
                        if (!option1) {
                            break;
                        }
                    }
                }
                if (option1) {
                    if (option_check[2]) {//�ִ� ���� ��
                        for (int i = 0; i < 5; i++) {
                            int start = -1;
                            int count = 0;
                            for (int j = 0; j < 22; j++) {
                                if (start == -1 && table[j][i] != -1) {
                                    start = table[j][i];//�� ó�� ������ �����ȣ
                                    count++;//1<- ���� 1�� �߰�
                                }
                                else if (start != -1 && table[j][i] != -1) {
                                    if (start != table[j][i]) {//���ǰ� �޶���, �߰��� ���� ������.
                                        count++;
                                        start = table[j][i];
                                        if (count > maxnum) {
                                            option2 = false;
                                            break;
                                        }
                                    }
                                }
                                else if (start != -1 && table[j][i] == -1) {
                                    start = -1;
                                    count = 0;
                                }

                            }
                            if (!option2) {
                                break;
                            }
                        }
                    }
                    if (option2) {
                        sub_tables.push_back(vec);
                        int** check = new int* [22];
                        for (int i = 0; i < 22; i++) {
                            check[i] = new int[5];
                            for (int j = 0; j < 5; j++) {
                                check[i][j] = table[i][j];
                            }
                        }
                        check_tables.push_back(check);
                        return;
                    }


                }

            }
        }
        else {
            for (int i = 0; i < vecarr[group].size(); i++) {
                //��������, �� �� �� �ǵ帮�� �ʱ� ���ؼ�
                temp.clear();
                for (int i = 0; i < vec.size(); i++) {
                    temp.push_back(vec.at(i));
                }
                for (int i = 0; i < 22; i++) {
                    for (int j = 0; j < 5; j++) {
                        last_table[i][j] = table[i][j];
                    }
                }


                bool check = true;//true�� ����
                for (int j = 0; j < vecarr[group].at(i)->getCount(); j++) {
                    //cout << vecarr[group].at(i)->getName() << "\n";
                    int start = (vecarr[group][i]->getStart()[j] - 9) / 0.5;
                    int end = (vecarr[group][i]->getEnd()[j] - 9) / 0.5;
                    int day = vecarr[group][i]->getDays()[j];
                    while (start < end) {
                        if (last_table[start][day] == -1) {
                            start++;
                        }
                        else {
                            break;
                        }
                    }
                    if (start != end) {
                        check = false;//�Ұ����� ����
                    }
                }
                if (check) {
                    for (int j = 0; j < vecarr[group][i]->getCount(); j++) {
                        int start = (vecarr[group][i]->getStart()[j] - 9) / 0.5;
                        int end = (vecarr[group][i]->getEnd()[j] - 9) / 0.5;
                        int day = vecarr[group][i]->getDays()[j];
                        int subnum = get_sub_num(vecarr[group][i]);
                        while (start < end) {
                            last_table[start][day] = subnum;
                            start++;
                        }
                    }
                    temp.push_back(vecarr[group][i]);

                    search_group(last_table, group + 1, temp);


                }

            }

        }

    }

};

int main() {

    Manager manager;
    manager.play();

    return 0;
}