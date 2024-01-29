#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include <queue>

using namespace std;

class Subject {
private:
    string name; // 강의명
    int credit; // 학점
    int* days; // 무슨요일에 들었는가
    float* start; // 시작 시간
    float* end; // 끝 시간
    int count; // 일주일에 몇번 들었는가

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
    vector<Subject*> subs;//과목들 저장
    vector<string> subsnames;//과목이름만 저장, 중복 체크, 과목 번호 부여용
    vector<int>* groups;
    vector<Subject*>* vecarr;

    int groupnum;
    vector<vector<Subject*>> sub_tables;
    vector<int**>check_tables;
    vector<Subject*> esubs;

    bool option_check[3] = { false,false,false };//옵션 세개 순서대로 공강 요일, 최대 공강 시간, 최대 연강 수
    vector<int> emptydays;//옵션1 공강요일
    float maxempty;//옵션2 최대 공강 시간
    int maxnum;//옵션3 최대 연강 수
    vector<Subject*> present_table;
    int present_check[22][5];

public:

    void play() {
        while (true) {
            cout << "1) 과목 관리   2) 시간표 관리   3) 종료\n\n";
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
                    cout << "프로그램 종료합니다." << endl;
                    return;
                }
            }
            else {
                cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
            }

        }
    }

    void group_make() {
        vector<int> vec;
        while (true) {
            int count = 1;
            string input;
            cout << "\n============현재 등록된 과목 현황============" << endl << endl;
            for (int i = 0; i < subsnames.size(); i++) {
                cout << "과목 이름: " << subsnames.at(i) << " / 과목 번호: " << (i + 1) << endl;

            }
            cout << "\n=============================================" << endl << endl;
            cout << "다음의 형식에 맞게 과목들을 그룹을 지어 입력해 주세요 " << endl;
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

            groupnum = count; // group개수
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
                cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
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
            cout << "\n[과목관리]\n\n1) 과목 추가\t2) 과목 삭제\t3) 과목 현황\t4) 과목 관리 종료" << endl;
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
                cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
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

            cout << "과목 이름을 입력하세요.(입력 종료하려면 q를 입력하세요.)==> ";
            getline(cin, subname);


            if (subname == "q") {
                cout << "\n과목 입력을 종료합니다" << endl;
                return;
            }

            if (subname.find(' ') != string::npos || subname == "") {//공백 하나라도 있으면 제거
                cout << "\n공백은 허용되지 않습니다. 다시 입력해주세요\n\n";

            }
            else {
                for (int i = 0; i < subsnames.size(); i++) {//이미 등록된 과목 중 이름 같은 것이 있는지 체크
                    if (subname == subsnames.at(i)) check = true;
                }
                if (!check) {//같은 과목 없는 경우 -> 과목 이름 벡터에 이름 추가해야함
                    subsnames.push_back(subname);
                }

                while (true) {

                    cout << "학점을 입력하세요(1~12)==> ";
                    cin >> credit;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (credit != INT_MAX && credit != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
                    }
                    else {
                        if (!(1 <= credit && credit <= 12)) {
                            cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
                        }
                        else {
                            break;
                        }
                    }
                }

                while (true) {
                    cout << "일주일에 몇 번 들었는지 입력하세요.==> ";
                    cin >> count;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (count != INT_MAX && count != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
                    }
                    else {

                        if (!(1 <= count && count <= 5)) {
                            cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
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
                    cout << i + 1 << "번째 요일과 시간을 입력하세요.(예: 월 12~13.5)==> ";
                    string input;
                    getline(cin, input);
                    if (input.find('~') == -1) {//~이 존재하지 않음
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }
                    ss.str(input);
                    string day;
                    ss >> day;
                    if (dayToInt(day) == -1) {
                        cout << "요일을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }
                    days[i] = dayToInt(day);
                    string temp;

                    getline(ss, temp, '~');
                    if (!(stringstream(temp) >> start[i])) {
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }

                    getline(ss, temp);
                    if (!(stringstream(temp) >> end[i])) {
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }

                    if (!(start[i] - (int)start[i] == 0.5 || start[i] - (int)start[i] == 0)) {
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }
                    if (!(end[i] - (int)end[i] == 0.5 || end[i] - (int)end[i] == 0)) {
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }
                    if (!(9 <= start[i] && start[i] <= 20 && 9 <= end[i] && end[i] <= 20)) {
                        cout << "시간을 확인해주세요.\n\n";
                        i--;
                        continue;
                    }
                    else if (start[i] == end[i] || start[i] > end[i]) {
                        cout << "시간을 확인해주세요.\n\n";
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
                            cout << "시간을 확인해주세요.\n\n";
                            i--;
                            continue;
                        }
                    }

                    cout << "\n요일:" << day << endl;
                    cout << "시간:" << start[i] << "~" << end[i] << "\n\n";

                    if (check) {
                        for (int j = 0; j < subs.size(); j++) {
                            if (subs.at(j)->getName() == subname) {
                                for (int k = 0; k < subs.at(j)->getCount(); k++) {
                                    for (int l = 0; l < count; l++) {
                                        if (subs.at(j)->getDays()[k] == days[l]) {//요일 같음
                                            if (subs.at(j)->getStart()[k] == start[l]) {
                                                //시작시간 같으면 중복으로 간주
                                                cout << "시작시간이 같은 " << subname << "과목이 이미 있습니다." << endl;
                                                return;//메인으로 돌아감
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }



                }
                subs.push_back(new Subject(subname, days, start, end, count, credit));
                cout << "=====과목 등록 완료=====\n\n";
            }

        }



    }

    int dayToInt(string day) {
        if (day == "월") {
            return 0;
        }
        else if (day == "화") {
            return 1;
        }
        else if (day == "수") {
            return 2;
        }
        else if (day == "목") {
            return 3;
        }
        else if (day == "금") {
            return 4;
        }
        else {
            return -1;
        }

    }

    string IntToDay(int d) {
        switch (d) {
        case 0:
            return "월";
        case 1:
            return "화";
        case 2:
            return "수";
        case 3:
            return "목";
        case 4:
            return "금";
        default:
            return NULL;
        }

    }

    void table_manage() {
        while (true) {
            int menu;
            cout << "\n[시간표 관리]\n\n1) 시간표 만들기   2) 현재 시간표 보기   3) 현재 시간표 과목 추가/삭제   4) 시간표 관리 종료\n\n";
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
                    cout << "\n1) 현재 시간표 과목 추가 2) 현재 시간표 과목 삭제\n==>";
                    int menu;
                    cin >> menu;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (menu != INT_MAX && menu != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                    }
                    else {
                        if (menu == 1) {
                            add_present_table();
                        }
                        else if (menu == 2) {
                            delete_present_table();
                        }
                        else {
                            cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
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
                cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
            }

        }
    }

    void delete_present_table() {
        while (true) {
            if (present_table.size() == 0 && esubs.size() == 0) {
                cout << "※ 현재 시간표가 없습니다. 시간표를 만들어주세요. ※\n\n";
                return;
            }
            print_table();
            cout << "1) 현강 과목 삭제 2) e러닝 과목 삭제 3)과목 삭제 종료\n==>";
            int menu;
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";

            }
            else {
                getchar();
                if (menu == 1) {
                    if (present_table.size() == 0) {
                        cout << "※ 현재 시간표에 현강 과목이 없습니다.  ※\n\n";
                        continue;
                    }
                    int t;
                    cout << "삭제할 과목의 번호를 입력하세요\n==> ";
                    cin >> t;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (t != INT_MAX && t != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "잘못된 입력입니다.\n\n";
                    }
                    else if (!(1 <= t && t <= present_table.size())) {
                        cout << "없는 과목번호입니다.\n\n" << endl;
                    }
                    else {
                        t--;
                        cout << "\n" << present_table.at(t)->getName() << " 과목이 삭제되었습니다.\n\n";
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
                        cout << "※ 현재 시간표에 e러닝 과목이 없습니다.  ※\n\n";
                        continue;
                    }
                    int t;
                    cout << "삭제할 과목의 번호를 입력하세요\n==> ";
                    cin >> t;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore();
                        if (t != INT_MAX && t != INT_MIN) {
                            while (getchar() != '\n');
                        }
                        cout << "잘못된 입력입니다.\n\n";
                    }
                    else if (!(1 <= t && t <= esubs.size())) {
                        cout << "없는 과목번호입니다.\n\n" << endl;
                    }
                    else {
                        t--;
                        cout << "\n" << esubs.at(t)->getName() << " 과목이 삭제되었습니다.\n\n";
                        esubs.erase(esubs.begin() + t);

                    }
                }
                else if (menu == 3) {
                    return;
                }
                else {
                    cout << "\n없는 메뉴입니다. 다시 입력해주세요.\n\n";
                }

            }
        }
    }

    void add_present_table() {

        while (true) {
            if (present_table.size() == 0) {
                cout << "※ 현재 시간표가 없습니다. 시간표를 만들어주세요. ※\n\n";
                return;
            }
            print_table();
            cout << "1) 현강 과목 추가 2) e러닝 과목 추가 3)과목 추가 종료\n==>";
            int menu;
            cin >> menu;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (menu != INT_MAX && menu != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";

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
                    cout << "\n과목 이름을 입력하세요.==> ";
                    getline(cin, subname);

                    if (subname.find(' ') != string::npos || subname == "") {//공백 하나라도 있으면 제거
                        cout << "\n공백은 허용되지 않습니다. 다시 입력해주세요\n\n";

                    }
                    else {
                        for (int i = 0; i < present_table.size(); i++) {//이미 등록된 과목 중 이름 같은 것이 있는지 체크
                            if (subname == present_table.at(i)->getName()) check = true;
                        }
                        if (check) {//같은 과목 없는 경우 -> 과목 이름 벡터에 이름 추가해야함
                            cout << "\n" << subname << " 과목은 이미 있는 과목입니다.\n\n";

                        }
                        else {
                            //학점 입력
                            while (true) {

                                cout << "\n학점을 입력하세요(1~12)==> ";
                                cin >> credit;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (credit != INT_MAX && credit != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                }
                                else {
                                    if (!(1 <= credit && credit <= 12)) {
                                        cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            //일주일에 몇번 들었는지 입력
                            while (true) {
                                cout << "\n일주일에 몇 번 들었는지 입력하세요.==> ";
                                cin >> count;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (count != INT_MAX && count != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                }
                                else {
                                    if (!(1 <= count && count <= 5)) {
                                        cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
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
                                cout << "\n" << i + 1 << "번째 요일과 시간을 입력하세요.(예: 월 12~13.5)==> ";
                                string input;
                                getline(cin, input);
                                if (input.find('~') == -1) {//~이 존재하지 않음
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }
                                ss.str(input);
                                string day;
                                ss >> day;
                                if (dayToInt(day) == -1) {
                                    cout << "요일을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }
                                days[i] = dayToInt(day);
                                string temp;

                                getline(ss, temp, '~');
                                if (!(stringstream(temp) >> start[i])) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }

                                getline(ss, temp);

                                //시간 조건 체크
                                if (!(stringstream(temp) >> end[i])) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }

                                if (!(start[i] - (int)start[i] == 0.5 || start[i] - (int)start[i] == 0)) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }
                                if (!(end[i] - (int)end[i] == 0.5 || end[i] - (int)end[i] == 0)) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }

                                if (!(9 <= start[i] && start[i] <= 20 && 9 <= end[i] && end[i] <= 20)) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }
                                else if (start[i] == end[i] || start[i] > end[i]) {
                                    cout << "시간을 확인해주세요.\n\n";
                                    i--;
                                    continue;
                                }
                                else {//그 전 요일이 있다면 그 시간이랑 겹치는지 체크

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
                                        cout << "시간을 확인해주세요.\n\n";
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
                                    cout << "\n시간이 중복되는 과목이 있어 추가할 수 없습니다.\n\n";
                                    check = true;
                                    break;
                                }

                                cout << "\n요일:" << day << endl;
                                cout << "시간:" << start[i] << "~" << end[i] << "\n\n";

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
                                cout << "=====과목 등록 완료=====\n\n";
                            }

                        }











                    }

                }
                else if (menu == 2) {
                    string subname;
                    int credit;
                    bool check = false;
                    cout << "\n과목 이름을 입력하세요.==> ";
                    getline(cin, subname);
                    if (subname.find(' ') != string::npos || subname == "") {//공백 하나라도 있으면 제거
                        cout << "\n공백은 허용되지 않습니다. 다시 입력해주세요\n\n";

                    }
                    else {
                        if (esubs.size() != 0) {
                            for (int i = 0; i < esubs.size(); i++) {
                                if (subname == esubs[i]->getName()) {
                                    check = true;
                                    cout << "\n과목 이름이 " << subname << "인 e러닝 과목이 이미 있습니다.\n\n";
                                    break;
                                }
                            }
                        }
                        if (!check) {
                            while (true) {

                                cout << "\n학점을 입력하세요(1~12)==> ";
                                cin >> credit;

                                if (cin.fail()) {
                                    cin.clear();
                                    cin.ignore();
                                    if (credit != INT_MAX && credit != INT_MIN) {
                                        while (getchar() != '\n');
                                    }
                                    cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                }
                                else {
                                    if (!(1 <= credit && credit <= 12)) {
                                        cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                    }
                                    else {
                                        break;
                                    }
                                }
                            }

                            esubs.push_back(new Subject(subname, 0, 0, 0, 0, credit));
                            cout << "\n" << subname << " 과목이 등록되었습니다.\n\n";
                        }
                    }
                }
                else if (menu == 3) {
                    return;
                }
                else {
                    cout << "\n없는 메뉴입니다. 다시 입력해주세요.\n\n";
                }

            }
        }

    }

    void choose_option() {
        while (true) {
            if (subsnames.size() == 0) {
                cout << "등록된 과목이 없습니다.\n\n";
                return;
            }
            cout << "설정할 옵션을 선택해주세요. \n1) 공강 요일 2) 최대 공강 시간 3) 최대 연강 수 4) 그룹 설정(시간표 만들기) \n==> ";
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
                    cout << "\n공강으로 하고 싶은 요일을 입력해주세요 (예: 월 금)\n공강 옵션을 제거하시려면 q를 입력해주세요.\n==>";
                    getchar();
                    string days;
                    getline(cin, days);
                    stringstream ss(days);
                    string temp;
                    if (days == "q") {
                        emptydays.clear();
                        option_check[0] = false;
                        cout << "\n공강 옵션이 제거되었습니다. 모든 요일의 수업을 반영합니다.\n\n";
                        break;
                    }

                    if (emptydays.size() != 0) {
                        emptydays.clear();
                    }
                    while (ss >> temp) {

                        int day = dayToInt(temp);

                        if (day == -1) {
                            cout << "잘못된 형식입니다.\n";
                            break;
                        }
                        else {
                            emptydays.push_back(day);
                        }
                    }
                    if (emptydays.size()) {
                        option_check[0] = true;//옵션 설정 되어있음
                        for (int i = 0; i < emptydays.size(); i++) {
                            cout << IntToDay(emptydays.at(i)) << " ";
                        }
                        cout << "공강이 설정되었습니다.\n";
                    }
                    else {//벡터의 사이즈가 0 -> 요일이 아무것도 설정되지않음.
                        cout << "설정 실패하였습니다. 입력을 확인해주세요.\n\n";
                    }

                    break;
                }
                case 2:
                    cout << "\n최대 공강 시간을 입력해주세요(1~11)\n옵션을 제거하시려면 0을 입력해주세요.\n==>";
                    cin >> maxempty;
                    if (!maxempty) {
                        option_check[1] = false;
                        cout << "\n최대 공강 시간 옵션이 제거되었습니다.\n\n";
                        break;
                    }
                    if (1 <= maxempty && maxempty <= 11) {
                        option_check[1] = true;
                        cout << "최대 공강 시간 " << maxempty << "시간이 설정되었습니다.\n\n";
                        maxempty *= 2;
                    }
                    else {
                        cout << "설정 실패하였습니다. 입력을 확인해주세요.\n\n";
                    }
                    break;
                case 3:
                    cout << "\n최대 연강 수를 입력해주세요(1~10)\n옵션을 제거하시려면 0을 입력해주세요.\n==>";
                    cin >> maxnum;
                    if (!maxnum) {
                        option_check[2] = false;
                        cout << "\n최대 연강 수 옵션이 제거되었습니다.\n\n";
                        break;
                    }
                    if (1 <= maxnum && maxnum <= 10) {
                        option_check[2] = true;
                        cout << "최대 연강 수 " << maxnum << "개가 설정되었습니다.\n\n";
                    }
                    else {
                        cout << "설정 실패하였습니다. 입력을 확인해주세요.\n\n";
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
                cout << "잘못된 입력입니다. 다시 확인해주세요.\n\n";
            }

        }

    }

    void showSubsNames() {
        cout << "\n[등록된 과목 목록]" << endl;
        for (int i = 0; i < subsnames.size(); i++) {
            cout << i + 1 << ". " << subsnames.at(i) << endl;
        }
    }

    int searchSameSubjectName(int index) { // subsnames.at(index) 와 같은 이름의 과목을 subs에서 찾아 개수를 반환.
        int ret = 0;
        string name = subsnames.at(index);
        for (int i = 0; i < subs.size(); i++) {
            if (name == subs[i]->getName()) { ret++; }
        }
        return ret;
    }

    void delete_subject() {
        if (subsnames.size() == 0) {
            cout << "\n등록된 과목이 없습니다.\n";
            return;
        }

        int t;
        showSubsNames();
        cout << endl;
        cout << "삭제할 과목의 번호를 입력하세요\n\n==> ";
        cin >> t;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            if (t != INT_MAX && t != INT_MIN) {
                while (getchar() != '\n');
            }
            cout << "잘못된 입력입니다.\n\n";
        }
        else if (!(1 <= t && t <= subsnames.size())) {
            cout << "없는 과목번호입니다.\n\n" << endl;
        }
        else {
            string target = subsnames.at(t - 1); // 삭제할 과목의 이름
            int classes_count = searchSameSubjectName(t - 1);
            if (classes_count == 0) {
                cout << "등록되지 않은 과목명입니다." << endl;
            }
            else if (classes_count == 1) {
                for (int i = 0; i < subs.size(); i++) {
                    if (subs[i]->getName() == target) { subs.erase(subs.begin() + i); }
                }
                subsnames.erase(subsnames.begin() + (t - 1));
                cout << "과목 " << '\"' << target << '\"' << " 삭제 완료" << endl;
            }
            else {
                int count = 0;
                for (int i = 0; i < subs.size(); i++) {
                    if (subs.at(i)->getName() == target) {
                        cout << char('A' + count++) << ". " << endl;
                        for (int k = 0; k < subs.at(i)->getCount(); k++) {
                            cout << IntToDay(subs.at(i)->getDays()[k]) << " / " << subs.at(i)->getStart()[k] << " ~ " << subs.at(i)->getEnd()[k] << endl;
                        }
                        cout << "학점: " << subs.at(i)->getCredit() << endl;
                    }
                }

                string temp;
                cout << "삭제할 분반을 선택하세요==> ";
                cin >> temp;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    while (getchar() != '\n');
                    cout << "잘못된 입력입니다.\n\n";
                }
                else if (temp.size() > 1) {
                    cout << "잘못된 입력입니다.\n\n";
                }
                else {
                    char ch = temp.at(0);
                    int temp;
                    if ('A' <= ch && ch < 'A' + classes_count) {
                        temp = ch - 'A';//몇 번째 있는지
                    }
                    else if ('a' <= ch && ch < 'a' + classes_count) {
                        temp = ch - 'a';
                    }
                    else {
                        cout << "없는 분반입니다.\n\n";
                        return;
                    }
                    int c = 0;
                    for (int i = 0; i < subs.size(); i++) {
                        if (subs.at(i)->getName() == target) {
                            if (c == temp) {
                                subs.erase(subs.begin() + i);
                                cout << target << " - " << ch << "삭제 완료" << endl;
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
            cout << "\n등록된 과목이 없습니다.\n";
            return;
        }
        cout << endl;

        cout << "============과목 현황============\n";
        for (int i = 0; i < subsnames.size(); i++) {
            cout << (i + 1) << ". " << subsnames.at(i) << "\n";
            int max = searchSameSubjectName(i);

            if (max == 1) {//분반 없는 과목
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
            else {//분반 있는 과목
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

    // 추천 시간표 생성
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
            cout << "※ 만들 수 있는 시간표가 없습니다. 그룹, 옵션, 또는 과목을 수정한 뒤 만들어 보세요. ※\n\n";
            return;
        }

        cout << "\n================시간표 생성 완료================\n\n";
        for (int i = 0; i < sub_tables.size(); i++) {
            cout << "-------------------" << i + 1 << "번 시간표-------------------\n";
            int credit = 0;
            for (int j = 0; j < sub_tables[i].size(); j++) {
                cout << j + 1 << ". " << sub_tables[i][j]->getName() << "\n";
                credit += sub_tables[i][j]->getCredit();
                for (int k = 0; k < sub_tables[i][j]->getCount(); k++) {
                    cout << "\t" << IntToDay(sub_tables[i][j]->getDays()[k]) << " " << sub_tables[i][j]->getStart()[k] << "~" << sub_tables[i][j]->getEnd()[k] << "\n";
                }
                cout << "\n";
            }
            cout << "▶ " << credit << " 학점\n";

        }
        cout << "================================================\n";
        while (true) {
            cout << "마음에 드는 시간표를 하나 골라주세요\n==>";
            int num;
            cin >> num;

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                if (num != INT_MAX && num != INT_MIN) {
                    while (getchar() != '\n');
                }
                cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
            }
            else {
                if (!(1 <= num && num <= sub_tables.size())) {
                    cout << "\n없는 시간표입니다. 다시 확인해주세요.\n\n";
                    continue;
                }
                getchar();
                cout << num << "번 시간표를 저장했습니다.\n\n";
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
                    cout << "e러닝 과목을 추가하시겠습니까?(y/n)\n==>";

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
                            cout << "\n과목 이름을 입력하세요.(입력 종료하려면 q를 입력하세요.)==> ";
                            getline(cin, subname);
                            if (subname == "q") {
                                cout << "\ne러닝 과목 입력을 종료합니다" << endl;
                                return;
                            }
                            if (subname.find(' ') != string::npos || subname == "") {//공백 하나라도 있으면 제거
                                cout << "\n공백은 허용되지 않습니다. 다시 입력해주세요\n\n";

                            }
                            else {
                                if (esubs.size() != 0) {
                                    for (int i = 0; i < esubs.size(); i++) {
                                        if (subname == esubs[i]->getName()) {
                                            check = true;
                                            cout << "\n과목 이름이 " << subname << "인 e러닝 과목이 이미 있습니다.\n\n";
                                            break;
                                        }
                                    }
                                }
                                if (!check) {
                                    while (true) {

                                        cout << "\n학점을 입력하세요(1~12)==> ";
                                        cin >> credit;

                                        if (cin.fail()) {
                                            cin.clear();
                                            cin.ignore();
                                            if (credit != INT_MAX && credit != INT_MIN) {
                                                while (getchar() != '\n');
                                            }
                                            cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                        }
                                        else {
                                            if (!(1 <= credit && credit <= 12)) {
                                                cout << "\n잘못된 입력입니다. 다시 확인해주세요.\n\n";
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                    }

                                    esubs.push_back(new Subject(subname, 0, 0, 0, 0, credit));
                                    cout << "\n" << subname << " 과목이 등록되었습니다.\n\n";
                                }
                            }


                        }
                    }
                    else if (temp == "n") {
                        break;
                    }
                    else {
                        cout << "입력을 확인해주세요.\n\n";
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
            cout << "※ 현재 시간표가 없습니다. 시간표를 만들어주세요. ※\n\n";
            return;
        }
        else {
            cout << "\n==========현재 시간표==========\n\n";
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
                cout << "▷ " << credit << " 학점\n\n";
            }
            if (esubs.size() > 0) {
                int ecredit = 0;
                cout << "----------이러닝 목록----------\n\n";
                for (int i = 0; i < esubs.size(); i++) {
                    cout << i + 1 << ". " << esubs[i]->getName() << "\n\n";
                    ecredit += esubs[i]->getCredit();
                }
                cout << "\n▷ " << ecredit << " 학점\n\n";
                cout << "-------------------------------\n";
                cout << "\n▶총 " << credit + ecredit << "학점\n\n";
            }
            cout << "===============================\n\n";

            return;

        }
    }

    void search_group(int table[22][5], int group, vector<Subject*> vec) {


        vector<Subject*> temp;
        int last_table[22][5];

        if (group == groupnum) {
            //시간표 넣기 전에 옵션 체크

            //bool option_check[3] = { false,false,false }
            bool option0 = true;
            bool option1 = true;
            bool option2 = true;
            if (option_check[0]) {//공강 요일

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
                if (option_check[1]) {//최대 공강시간
                    for (int i = 0; i < 5; i++) {
                        int start = -1;
                        int count = 0;
                        for (int j = 0; j < 22; j++) {
                            if (start == -1 && table[j][i] != -1) {
                                start = j;//강의 시작 인덱스
                            }
                            else if (start != -1 && table[j][i] != -1) {

                                if (start + 1 == j) {
                                    start++;
                                }
                                else {//중간에 공강이 있었음
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
                    if (option_check[2]) {//최대 연강 수
                        for (int i = 0; i < 5; i++) {
                            int start = -1;
                            int count = 0;
                            for (int j = 0; j < 22; j++) {
                                if (start == -1 && table[j][i] != -1) {
                                    start = table[j][i];//맨 처음 강의의 과목번호
                                    count++;//1<- 강의 1개 발견
                                }
                                else if (start != -1 && table[j][i] != -1) {
                                    if (start != table[j][i]) {//강의가 달라짐, 중간에 공강 없었음.
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
                //복사해줌, 그 전 값 건드리지 않기 위해서
                temp.clear();
                for (int i = 0; i < vec.size(); i++) {
                    temp.push_back(vec.at(i));
                }
                for (int i = 0; i < 22; i++) {
                    for (int j = 0; j < 5; j++) {
                        last_table[i][j] = table[i][j];
                    }
                }


                bool check = true;//true면 가능
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
                        check = false;//불가능한 과목
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